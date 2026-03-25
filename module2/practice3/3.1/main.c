#include <regex.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

uint16_t get_mask(char ch) {
  switch (ch) {
    case 'a':
      return S_IRWXU | S_IRWXG | S_IRWXO;
    case 'u':
      return S_IRWXU;
    case 'g':
      return S_IRWXG;
    case 'o':
      return S_IRWXO;
    case 'r':
      return S_IRUSR | S_IRGRP | S_IROTH;
    case 'w':
      return S_IWUSR | S_IWGRP | S_IWOTH;
    case 'x':
      return S_IXUSR | S_IXGRP | S_IXOTH;
    default:
      return 0;
  }
}

bool calculate_from_str(const char* str, uint16_t original, uint16_t* result) {
  char op = '\0';
  uint16_t ugo = 0;
  uint16_t rwx = 0;
  size_t i = 0;
  *result = 0;

  while (str[i] != '\0') {
    if (strchr("augo", str[i])) {
      ugo |= get_mask(str[i]);
    } else {
      break;
    }
    i++;
  }

  if (!strchr("-+=", str[i])) {
    return false;
  }

  while (str[i] != '\0') {
    if (strchr("-+=", str[i])) {
      op = str[i];
    } else {
      break;
    }
    i++;
  }

  if (!strchr("rwx", str[i])) {
    return false;
  }

  while (str[i] != '\0') {
    if (strchr("rwx", str[i])) {
      rwx |= get_mask(str[i]);
    } else {
      break;
    }
    i++;
  }

  if (op == 0 || rwx == 0) {
    return false;
  }

  switch (op) {
    case '+':
      *result = original | (ugo & rwx);
      return true;
    case '-':
      *result = original & ~(ugo & rwx);
      return true;
    case '=':
      *result = (original & ~ugo) | (ugo & rwx);
      return true;
    default:
      return false;
  }
}

bool try_parse_str(const char* str) {
  size_t i = 0;
  uint16_t perms = 0;
  
  while (str[i] != '\0') {
    if (i % 3 == 0 && str[i] == 'r') {
      perms |= 1 << (8 - i);
    } else if (i % 3 == 1 && str[i] == 'w') {
      perms |= 1 << (8 - i);
    } else if (i % 3 == 2 && str[i] == 'x') {
      perms |= 1 << (8 - i); 
    } else if (str[i] != '-') {
      return false;
    }

    i++;
  }

  if (i > 9) {
    return false;
  }

  printf("%09b\n", perms);
  return true;
}

bool try_parse_oct(const char* str) {
  uint16_t perms;
  if (sscanf(str, "%ho", &perms) != 1) {
    return false;
  }

  printf("%09b\n", perms);
  return true;
}

bool print_bits(const char* str) {
  printf("Ввод: %s\n", str);
  printf("Биты: ");
  if (try_parse_str(str)) {
    return true;
  }

  if (try_parse_oct(str)) {
    return true;
  }

  printf("Ошибка\n");
  return false;
}

bool bitmask_to_str(uint16_t bitmask, char str[11]) {
  strcpy(str, "drwxrwxrwx");

  const uint16_t masks[] = {S_IRUSR, S_IWUSR, S_IXUSR, S_IRGRP, S_IWGRP,
                            S_IXGRP, S_IROTH, S_IWOTH, S_IXOTH};
  for (size_t i = 0; i < 9; i++) {
    if ((bitmask & masks[i]) == 0) {
      str[i + 1] = '-';
    }
  }

  str[0] = S_ISDIR(bitmask) ? 'd' : '-';

  if (bitmask & S_ISUID) {
    str[3] = (bitmask & S_IXUSR) ? 's' : 'S';
  }

  if (bitmask & S_ISGID) {
    str[6] = (bitmask & S_IXGRP) ? 's' : 'S';
  }

  if (bitmask & __S_ISVTX) {
    str[9] = (bitmask & S_IXOTH) ? 't' : 'T';
  }

  return true;
}

bool print_file_stat(const char* filename) {
  struct stat buf;
  if (stat(filename, &buf)) {
    return false;
  }

  char str[11];
  bitmask_to_str(buf.st_mode, str);

  uint16_t oct = buf.st_mode & 0777;
  printf("file: %s\n", filename);
  printf("norm: %9s\t%16s\n", str, "    ugtrwxrwxrwx");
  printf("bin:  %10b\t%016b\n", oct, buf.st_mode);
  printf("oct:  %10o\n", oct);
  printf("----------------");
  printf("\n");

  return true;
}

bool dry_run_chmod(const char* permissions, const char* filename) {
  struct stat buf;
  if (stat(filename, &buf)) {
    fprintf(stderr, "Произошла ошибка при получении данных файла %s.\n",
            filename);
    return false;
  }

  uint16_t result = 0;
  if (!calculate_from_str(permissions, buf.st_mode, &result)) {
    fprintf(stderr, "Неверный формат: %s.\n", permissions);
    return false;
  }

  char str[11];
  bitmask_to_str(buf.st_mode, str);
  printf("Объект: %s\n", filename);
  printf("Было:  %s\n", str);
  bitmask_to_str(result, str);
  printf("Стало: %s\n", str);

  return true;
}

int main(int argc, char const* argv[]) {
  if (argc == 2) {
    if (print_file_stat(argv[1])) {
      return 0;
    }
    if (print_bits(argv[1])) {
      return 0;
    }
  } else if (argc == 3) {
    if (dry_run_chmod(argv[1], argv[2])) {
      return 0;
    }
  } else {
    printf("Варианты использования:\n");
    printf("1. fake-chmod <права в текстовом|числовом виде>\n");
    printf("2. fake-chmod <имя файла>\n");
    printf("3. fake-chmod <режим> <имя файла>\n");
  }

  return 0;
}
