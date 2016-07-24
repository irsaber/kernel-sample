//http://www.linuxjournal.com/article/6872?page=0,0
//Driving Me Nuts - Device Classes Aug 01, 2003  By Greg Kroah-Hartman

#include <linux/module.h> //We need this header in all kernel modules
#include <linux/kernel.h> //Absolutely because we are doing kernel job
#include <linux/init.h> //And this is needed for the macros
#include <asm/current.h> //For finding the parent process ID of the module
#include <linux/sched.h> //For "struct task_struct"
#include <linux/device.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Saber Rezvani");
MODULE_DESCRIPTION("device model");


static struct class i2c_adapter_class = {
	.name = "saber_adapter"
};


static int __init hello_world_init(void){
	if (class_register(&i2c_adapter_class) != 0)
		printk(KERN_ERR "i2c adapter class failed"
		       "to register properly\n");
	return 0;
}


static void __exit hello_world_exit(void){
	class_unregister(&i2c_adapter_class);
}


module_init(hello_world_init);
module_exit(hello_world_exit);
