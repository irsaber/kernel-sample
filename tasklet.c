#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/jiffies.h>
#include <linux/timer.h>

MODULE_LICENSE("GPL");
unsigned long ja, jb, jc;

char my_tasklet_data[] = "my_tasklet_function was called";

void my_tasklet_function (unsigned long data) {
	ja = jiffies;
	printk("%s\n", (char *) data);
	printk(KERN_INFO "jiffies in my_tasklet_function schedule: %lu \n", ja);
	return;
}

DECLARE_TASKLET(my_tasklet, my_tasklet_function, (unsigned long)
		&my_tasklet_data);


int init_module(void)
{
	printk("===============\n");
	printk("module started\n");
	jb = jiffies;
	printk("\n jiffies before schedule: %lu", jb);
	tasklet_schedule(&my_tasklet);
	int i;
	unsigned long c = 0;
//	for (i = 0; i < 10000; i++)
//		printk("i = %d", i);

	jc = jiffies;
	printk("\n jiffies after schedule: %lu", jc);
	return 0;
}

void cleanup_module(void)
{
	tasklet_kill(&my_tasklet);
	return;
}

