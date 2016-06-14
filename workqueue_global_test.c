#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/jiffies.h>
#include <linux/slab.h>
#include <linux/timer.h>

MODULE_LICENSE("GPL");


typedef struct {
	struct work_struct my_work;
	int x;
} my_work_t;

my_work_t *work, *work2;

static void my_wq_function( struct work_struct *work) {
	my_work_t *my_work = (my_work_t *) work;
	printk("my_work.x: %d\n", my_work->x);
	kfree((void *) work);

	return;
}


int init_module(void)
{
	//warning: ISO C90 forbids mixed declarations and code
	//because of that, we have to move the variable declaration to top of
	//block.
	int ret;

	printk("===============\n");
	printk("module started\n");



		work = (my_work_t *) kmalloc(sizeof(my_work_t), GFP_KERNEL);
		if (work) {
			INIT_WORK( (struct work_struct *) work, my_wq_function);
			work->x = 1;

			ret = schedule_work((struct work_struct *) work);
		}

		work2 = (my_work_t *) kmalloc(sizeof(my_work_t), GFP_KERNEL);

		if (work2) {
			INIT_WORK( (struct work_struct *)work2, my_wq_function);

			work2->x = 2;

			ret = schedule_work((struct work_struct *)work2);
		}

	return 0;
}

void cleanup_module(void)
{

	flush_work((struct work_struct *) work);
	flush_work((struct work_struct *) work2);

	return;
}

