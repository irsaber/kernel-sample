#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/jiffies.h>
#include <linux/timer.h>
unsigned long js, je, tet;
int netblock_init_module(void){
	    js = jiffies;
	    printk("\n[Jiffies start Time : %lu] \nModule Started.\n", js);
	    js = jiffies;
	    printk("\n[Jiffies start Time 2 : %lu] \nModule Started.\n", js);
	    return 0;
}

void netblock_cleanup_module(void) {
	je = jiffies;
	printk("\n[Jiffies End Time : %lu] \nModule Removed.\n", je);
	tet = je - js;
	printk("\nEnd Time [%lu] - Start Time [%lu]: \n Totlal elapsed Time \
	       [%lu]\n", js, je, tet);

}

module_init(netblock_init_module);
module_exit(netblock_cleanup_module);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("JIFFIES EXAMPLE");
MODULE_AUTHOR("RAHEEL");
