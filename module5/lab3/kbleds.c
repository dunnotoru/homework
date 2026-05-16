#include <linux/module.h>
#include <linux/configfs.h>
#include <linux/init.h>
#include <linux/tty.h>
#include <linux/kd.h>   
#include <linux/vt.h>
#include <linux/console_struct.h>
#include <linux/vt_kern.h>
#include <linux/timer.h>

MODULE_DESCRIPTION("Example module illustrating the use of Keyboard LEDs.");
MODULE_LICENSE("GPL");

#define LED_DIRECTORY "led_blink"

#define BLINK_DELAY HZ / 5
#define RESTORE_LEDS 0xFF

static struct timer_list my_timer;
static struct tty_driver *my_driver;

static struct kobject *led_kobject;
static int led_config = 0x07;
static int led_status = RESTORE_LEDS;

static ssize_t led_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf);
static ssize_t led_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count);
static void my_timer_func(struct timer_list *ptr);
static int __init kbleds_init(void);
static void __exit kbleds_cleanup(void);

static ssize_t led_show(struct kobject *kobj, struct kobj_attribute *attr,
                        char *buf) {
  return sprintf(buf, "%d\n", led_config);
}

static ssize_t led_store(struct kobject *kobj, struct kobj_attribute *attr,
                         const char *buf, size_t count) {
  int status = sscanf(buf, "%du", &led_config);
  if (status == 0) {
    return -EINVAL;
  }

  return count;
}

static void my_timer_func(struct timer_list *ptr) {
  if (led_status == RESTORE_LEDS) {
    led_status = led_config;
  } else {
    led_status = RESTORE_LEDS;
  }

  (my_driver->ops->ioctl)(vc_cons[fg_console].d->port.tty, KDSETLED,
                          led_status);
  my_timer.expires = jiffies + BLINK_DELAY;
  add_timer(&my_timer);
}

static struct kobj_attribute led_attribute = __ATTR(led_config, 0660, led_show, led_store);

static int __init kbleds_init(void) {
  int i;
  pr_info("kbleds: loading\n");

  led_kobject = kobject_create_and_add(LED_DIRECTORY, kernel_kobj);
  if (!led_kobject) {
    return -ENOMEM;
  }

  int error = sysfs_create_file(led_kobject, &led_attribute.attr);
  if (error) {
    pr_debug("failed to create the foo file in /sys/kernel/%s\n", LED_DIRECTORY);
  }

  pr_info("kbleds: fgconsole is %x\n", fg_console);
  for (i = 0; i < MAX_NR_CONSOLES; i++) {
    if (!vc_cons[i].d) {
      break;
    }
    pr_info("poet_atkm: console[%i/%i] #%i, tty %lx\n", i,
           MAX_NR_CONSOLES, vc_cons[i].d->vc_num,
           (unsigned long)vc_cons[i].d->port.tty);

  }

  pr_info("kbleds: finished scanning consoles\n");
  my_driver = vc_cons[fg_console].d->port.tty->driver;
  pr_info("kbleds: tty driver name %s\n", my_driver->driver_name);

  timer_setup(&my_timer, my_timer_func, 0);
  my_timer.expires = jiffies + BLINK_DELAY;
  add_timer(&my_timer);
  return 0;
}

static void __exit kbleds_cleanup(void) {
  pr_info("kbleds: unloading...\n");
  timer_delete_sync(&my_timer);
  (my_driver->ops->ioctl)(vc_cons[fg_console].d->port.tty, KDSETLED,
                          RESTORE_LEDS);
  kobject_put(led_kobject);
}

module_init(kbleds_init);
module_exit(kbleds_cleanup);