//src of sample code:
//http://pete.akeo.ie/2011/08/writing-linux-device-driver-for-kernels.html
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/types.h>
#include <linux/mutex.h>
#include <linux/kfifo.h>
#include "parrot_driver.h"

MODULE_AUTHOR(AUTHOR);
MODULE_DESCRIPTION(DESCRIPTION);
MODULE_VERSION(VERSION);
MODULE_LICENSE("GPL");

static struct class* parrot_class = NULL;
static struct device* parrot_device = NULL:
static int parrot_major;

static bool message_read;

static DEFINE_MUTEX(parrot_device_mutex);

static DECLARE_KFIFO(parrot_msg_fifo, char, PARROT_MSG_FIFO_SIZE);
static unsigned int parrot_msg_len[PARROT_MSG_FIFO_MAX];

static int parrot_msg_idx_rd, parrot_msg_idx_wr;
