#include <linux/kern_levels.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#define PROC_ENTRY_NAME "hello_entry"
#define N 10

static int len, temp;
static char *msg;

static ssize_t read_proc(struct file *filp, char *buf, size_t count,
                         loff_t *offp);
static ssize_t write_proc(struct file *filp, const char *buf, size_t count,
                          loff_t *offp);
static void create_new_proc_entry(void);
static int proc_init(void);
static void proc_cleanup(void);

static ssize_t read_proc(struct file *filp, char *buf, size_t count,
                         loff_t *offp) {
  if (count > temp) {
    count = temp;
  }

  temp = temp - count;
  unsigned long bytes_copied = copy_to_user(buf, msg, count);
  if (bytes_copied != 0) {
    printk(KERN_WARNING "Failed to copy %lu bytes to user\n", bytes_copied);
    return -EFAULT;
  }

  if (count == 0) {
    temp = len;
  }

  return count;
}

static ssize_t write_proc(struct file *filp, const char *buf, size_t count,
                          loff_t *offp) {
  if (count >= N) {
    return -EINVAL;
  }
  
  unsigned long bytes_copied = copy_from_user(msg, buf, count);
  if (bytes_copied != 0) {
    printk(KERN_WARNING "Failed to copy %lu bytes from user\n", bytes_copied);
    return -EFAULT;
  }

  len = count;
  temp = len;
  return count;
}

static const struct proc_ops proc_fops = {
    .proc_read = read_proc,
    .proc_write = write_proc,
};

static void create_new_proc_entry(void) {
  // use of void for no arguments is compulsory now
  proc_create(PROC_ENTRY_NAME, 0, NULL, &proc_fops);
  msg = kmalloc(N * sizeof(char), GFP_KERNEL);
}

static int proc_init(void) {
  create_new_proc_entry();
  return 0;
}

static void proc_cleanup(void) {
  remove_proc_entry(PROC_ENTRY_NAME, NULL);
  kfree(msg);
}

MODULE_LICENSE("Danil License :D");
MODULE_AUTHOR("Danil Matveev");

module_init(proc_init);
module_exit(proc_cleanup);