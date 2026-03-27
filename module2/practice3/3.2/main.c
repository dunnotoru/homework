#include <math.h>
#include <regex.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

regex_t ip_regex;

uint32_t parse_address(const char* ip_str) {
  char* token;
  uint32_t ip = 0;
  char buffer[16];
  strncpy(buffer, ip_str, 16);
  char* tmp;

  int status = regexec(&ip_regex, buffer, 0, NULL, 0);
  if (status != 0) {
    fprintf(stderr, "Ошибка при парсинге адреса: %s\n", ip_str);
    exit(1);
  }

  token = strtok(buffer, ".");
  uint8_t byte_count = 0;
  while (token != NULL) {
    byte_count++;
    int byte = strtol(token, &tmp, 10);
    if (tmp == buffer || *tmp != '\0') {
      fprintf(stderr, "Ошибка при парсинге адреса: %s\n", ip_str);
      exit(1);
    }

    if (byte < 0 || byte > 255) {
      fprintf(stderr,
              "Ошибка при парсинге адреса. Диапазон значений байта [0-255].\n");
      exit(1);
    }

    ip = (ip << 8) | byte;
    token = strtok(NULL, ".");

    if (byte_count > 4) {
      fprintf(stderr, "Ошибка при парсинге адреса. Превышена длина. \n");
      exit(1);
    }
  }

  return ip;
}

uint8_t is_in_subnet(uint32_t ip, uint32_t gateway, uint32_t mask) {
  return (ip & mask) == (gateway & mask);
}

void run(uint32_t n, uint32_t gateway, uint32_t mask) {
  uint32_t in_subnet = 0;
  for (uint32_t i = 0; i < n; i++) {
    //????????? rand generates like 2^31 not 2^15
    uint32_t r = rand() % UINT16_MAX;
    uint32_t tmp = rand() % UINT16_MAX;
    r = (r << 16) + tmp;
    if (is_in_subnet(r, gateway, mask)) {
      in_subnet++;
    }
  }

  double percent = (double)in_subnet / n * 100;

  printf("Всего:\t\t %u\n", n);
  printf("Входят:\t\t %u [%.4f %%]\n", in_subnet, percent);
  printf("Не входят:\t %u [%.4f %%]\n", n - in_subnet, 100.0 - percent);
}

int main(int argc, char const* argv[]) {
  if (argc != 4) {
    fprintf(stderr, "Ожидается: <ip4_шлюза> <ip4_маска> <N>\n");
    return 1;
  }

  srand(time(NULL));

  regcomp(&ip_regex, "^[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}$",
          REG_EXTENDED);

  uint16_t gw = 0;
  uint16_t mask = 0;

  gw = parse_address(argv[1]);
  mask = parse_address(argv[2]);

  char* tmp;
  uint32_t n = strtol(argv[3], &tmp, 10);
  if (*tmp != '\0') {
    fprintf(stderr, "N должно быть числом.\n");
    return 1;
  }

  printf("gateway: %.10u\t%.32b\n", gw, gw);
  printf("mask:\t %.10u\t%.32b\n", mask, mask);
  uint32_t subnet = gw & mask;
  printf("subnet:\t %.10u\t%.32b\n", subnet, subnet);

  run(n, gw, mask);

  return 0;
}
