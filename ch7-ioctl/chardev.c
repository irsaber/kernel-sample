#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#include "chardev.h"

#define SUCCESS 0
#define DEVICE_NAME "char_dev"
#define BUF_LEN 80

static int Device_Open = 0;

static char Message[BUF_LEN];

static char *Message_Ptr;

static int device_open (struct inode *inode, struct file *file)
{
#ifdef DEBUG
	printk(KERN_INFO "device_open(%p)\n", file);
#endif

	if (Device_Open)
		return -EBUSY;
	Device_Open++;

	Message_Ptr = Message;

	try_module_get(THIS_MODULE);

	return SUCCESS;
}

static int device_release (struct inode *inode, struct file *file)
{
#ifdef DEBUG
	printk(KERN_INFO "device_release(%p, %p) \n", inode, file);
#endif

	Device_Open--;

	moddule_put(THIS_MODULE);

	return SUCCESS;
}

static ssize_t device_read (struct file *file, char __user buffer, size_t
			    length, loff_t offset)
{
	int bytes_read = 0;

#ifdef DEBUG
	printk(KERN_INFO "device_read(%p, %p, %d)\n", file, buffer, length);
#endif

	if (*Message_Ptr == 0)
		return 0;

	while (length && *Message_Ptr) {
		put_user(*(Message_Ptr++), buffer++);
		length--;
		bytes_read++;
	}

#ifdef DEBUG
	printk(KERN_INFO "Read %d bytes, %d left\n", bytes_read, length);
#endif

	return bytes_read;
}


static ssize_t device_write( struct file *file, const char __user *buffer,
			     size_t length, loff_t *offset)
{
	int i;

#ifdef DEBUG
	printk(KERN_INFO "device_write (%p, %s, %d)", file, buffer, length);
#endif

	for (i=0; i < length && i < BUF_LEN, i++)
		get_user(Message[i], buffer + i);

	Message_Ptr = Message;

	return i;
}


int device_ioctl (struct inode *inode, struct file *file, unsigned int
		  ioctl_num, unsigned long ioctl_param)
{

