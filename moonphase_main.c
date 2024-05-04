#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/time.h>
#include <linux/timekeeping.h>
#include <linux/types.h>
#include <linux/version.h>

#include <asm/errno.h>

#include "moon.h"

#define DEVICE_NAME "moonphase"
#define SUCCESS 0
#define BUF_LEN 10240

static int major;
// A class is a higher-level view of a device
static struct class *cls;
static char msg[BUF_LEN + 1];

#define MOON_ASCII                                                             \
  "            .----------.            \r\n        .--\'   o    .   `--.     " \
  "   \r\n      .\'@  @@@@@@ O   .   . `.      \r\n    .\'@@  @@@@@@@@   "     \
  "@@@@   . `.    \r\n  .\'    . @@@@@@@@  @@@@@@    . `.  \r\n / @@ o    "    \
  "@@@@@@.   @@@@    O   @\\ \r\n |@@@@               @@@@@@     @@| \r\n/ "   \
  "@@@@@   `.-.    . @@@@@@@@  .  @@\\\r\n| @@@@   --`-\'  .  o  @@@@@@@     " \
  " |\r\n|@ @@                 @@@@@@ @@@   |\r\n\\      @@    @   . ()  @@  " \
  " @@@@@  /\r\n |   @      @@@         @@@  @@@  | \r\n \\  .   @@  @\\  .  " \
  "    .  @@    o / \r\n  `.   @@@@  _\\ /     .      o  .\'  \r\n    `.  @@ " \
  "   ()---           .\'    \r\n      `.     / |  .    o    .\'      \r\n   " \
  "     `--./   .       .--\'        \r\n            `----------\'           " \
  " "

void get_moon_output(char **text, char **emoji, struct tm tm_now) {
  int curr_moonphase =
      moon_phase(tm_now.tm_year + 1900, tm_now.tm_mon + 1, tm_now.tm_mday);

  pr_info("%ld %d %d got %d\n", tm_now.tm_year + 1900, tm_now.tm_mon + 1,
          tm_now.tm_mday, curr_moonphase);
  switch (curr_moonphase) {
  case 0:
    *text = "New Moon";
    *emoji = "\xf0\x9f\x8c\x91";
    break;

  case 1:
    *text = "Waxing Crescent Moon";
    *emoji = "\xf0\x9f\x8c\x92";
    break;

  case 2:
    *text = "First Quarter Moon";
    *emoji = "\xf0\x9f\x8c\x93";
    break;

  case 3:
    *text = "Waxing Gibbous Moon";
    *emoji = "\xf0\x9f\x8c\x95";
    break;

  case 4:
    *text = "Full Moon";
    *emoji = "\xf0\x9f\x8c\x96";
    break;

  case 5:
    *text = "Waning Gibbous Moon";
    *emoji = "\xf0\x9f\x8c\x94";
    break;

  case 6:
    *text = "Waning Gibbous Moon";
    *emoji = "\xf0\x9f\x8c\x97";
    break;

  case 7:
    *text = "Waning Crescent Moon";
    *emoji = "\xf0\x9f\x8c\x98";
    break;

    // default:
    //   break;
  }
}

static char *is_devnode(const struct device *dev, umode_t *mode) {
  if (mode)
    *mode = 0666;
  return NULL;
}

static ssize_t device_read(struct file *filp, char __user *buffer,
                           size_t length, loff_t *offset) {
  pr_debug("device_read starting\n");

  struct timespec64 now;
  struct tm tm_now;
  ktime_get_real_ts64(&now);
  time64_to_tm(now.tv_sec, 0, &tm_now);

  char *moonphase_text = "";
  char *moonphase_emoji = "";
  get_moon_output(&moonphase_text, &moonphase_emoji, tm_now);

  int bytes_read = 0;
  const char *msg_ptr = msg;
  sprintf(msg, "%s %s\n%s\n", moonphase_text, moonphase_emoji, MOON_ASCII);

  if (!*(msg_ptr + *offset)) {
    *offset = 0;
    pr_debug("device_read end of message\n");
    return 0;
  }

  msg_ptr += *offset;

  while (length && *msg_ptr) {
    put_user(*(msg_ptr++), buffer++);
    length--;
    bytes_read++;
  }

  *offset += bytes_read;

  pr_debug("device_read got %d\n", bytes_read);

  return bytes_read;
}

static ssize_t device_write(struct file *filp, const char __user *buff,
                            size_t len, loff_t *off) {
  pr_debug("device_write\n");

  pr_alert("Sorry, this operation is not supported.\n");
  return -EINVAL; // Invalid argument
}

static int device_open(struct inode *inode, struct file *file) {
  pr_debug("device_open\n");
  return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file) {
  pr_debug("device_release\n");
  return SUCCESS;
}
static struct file_operations moon_phase_file_ops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release,
};

static int __init moonphase_init(void) {
  major = register_chrdev(0, DEVICE_NAME, &moon_phase_file_ops);

  if (major < 0) {
    pr_alert("Registering char device failed with %d\n", major);
    return major;
  }

  pr_debug("I was assigned major number %d.\n", major);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 4, 0)
  cls = class_create(DEVICE_NAME);
#else
  cls = class_create(THIS_MODULE, DEVICE_NAME);
#endif
  cls->devnode = is_devnode;

  device_create(cls, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);

  pr_debug("Device created on /dev/%s\n", DEVICE_NAME);

  return SUCCESS;
}

static void __exit moonphase_exit(void) {
  device_destroy(cls, MKDEV(major, 0));
  class_destroy(cls);
  unregister_chrdev(major, DEVICE_NAME);
  pr_debug("Shutting down.");
}

module_init(moonphase_init);
module_exit(moonphase_exit);
MODULE_LICENSE("Dual MIT/GPL");
MODULE_AUTHOR("stephentt");
MODULE_DESCRIPTION(
    "Simple module for moon phase character device at /dev/moonphase");
