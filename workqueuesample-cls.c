//We need this header in all kernel modules
#include <linux/module.h>
//Absolutely because we are doing kernel job
#include <linux/kernel.h>
//And this is needed for the macros
#include <linux/init.h>
//For create and register a procfs entry
#include <linux/proc_fs.h>
//For providing read function of the entry with ease
#include <linux/seq_file.h>
//For jiffies, which will give us timelapce of the system
#include <linux/jiffies.h>
//For finding the parent process ID of the module
#include <asm/current.h>
//For "struct task_struct"
#include <linux/sched.h>
//For catching CPU number
#include <linux/smp.h>
//Ofcourse for using workqueues
#include <linux/workqueue.h>


//It is always good to have a meaningful constant as a return code
#define SUCCESS 0
//This will be our module name
#define DEVICE_NAME "workqueuesample"

//These are some useful information that could reveald with modinfo command
//Set module license to get rid of tainted kernel warnings
MODULE_LICENSE("GPL");
//Introduce the module's developer and it's functionality
MODULE_AUTHOR("Aliireeza Teymoorian");
MODULE_DESCRIPTION("This is just a simple ProcFS module which workqueues for educationa purpose");

//Creating a proc directory entry structure
static struct proc_dir_entry *our_proc_file;
// there is no need to be declared, BUT if declared as static
//we should define static DECLARE_WORK(...);
static struct work_struct my_our_work;

//Here are some useful variables
static unsigned long before_delay, after_delay;



//This is our work function which we wanted to queue
static void our_work_function(struct work_struct *samvoid){
	after_delay = jiffies;
	printk(KERN_INFO "WORKQUEUESAMPLE: work function is running on CPU %d \n", smp_processor_id());
}


//here we there's no need to define my_our_work, BUT if defined NO PROBLEM.
//important thins is that our_work_function should have a specific prototypes.
// static void our_work_function(struct work_struct *samvoid)
static DECLARE_WORK(my_our_work, our_work_function);
//This function calls on demand of read request from seq_files
static int proc_show(struct seq_file *m, void *v){
	//Our work function will assign jiffies to our after_delay variables
	//some while in future, So there is nothing else to do here
	//Now we create a sharp simple output
	seq_printf(m, "%lu\t %lu\n", before_delay, after_delay);
	return SUCCESS;
}


//This is where system functionallity triggers every time some process try to read from our proc entry
static int proc_open(struct inode *inode, struct file *file){
	printk(KERN_INFO "WORKQUEUESAMPLE: Seqfile Read, Process \"%s:%d\" \n", current->comm, current->pid);
	return single_open(file, proc_show, NULL);
}


//Struct file_operations is the key to the functionality of the module
//functions that defined here are going to add to the kernel functionallity
//in order to respond to userspace access demand to the correspond /proc entry
static const struct file_operations fops = {
	.open = proc_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};


//Your module's entry point
static int __init workqueue_module_init(void){
	//These mesages will not show to the user but instead they will apear in kernel log
	//You can observe the kernel log in /dev/kmsg or with using dmsg command
	printk(KERN_INFO "WORKQUEUESAMPLE: Initialization.\n");
	printk(KERN_INFO "WORKQUEUESAMPLE: Init Module, Process \"%s:%d\" \n", current->comm, current->pid);
	our_proc_file = proc_create(DEVICE_NAME, 0644 , NULL, &fops);
	//Put an error message in kernel log if cannot create proc entry
	if(!our_proc_file){
		printk(KERN_ALERT "WORKQUEUESAMPLE: Registration of Proc Entry Failure %s .\n", DEVICE_NAME);
		//Because of this fact that procfs is a ram filesystem, this error means the lack of enough memory
		return -ENOMEM;
	}

	printk(KERN_INFO "WORKQUEUESAMPLE: ou work function and workqueue initiated on CPU %d \n", smp_processor_id());
	before_delay = jiffies;
	//Then we have to initiate a work

	//Then we are going to queue our work
	schedule_work(&my_our_work);

	printk(KERN_INFO "WORKQUEUESAMPLE: /proc/%s has been created.\n", DEVICE_NAME);
	//The init_module should return a value to the rest of kernel that asure
	//them to its successfully registration of its functionality
	return SUCCESS;
}


//You sould clean up the mess before exiting the module
static void __exit workqueue_module_exit(void){
	//Remove proc filesystem entry from system
	printk(KERN_INFO "WORKQUEUESAMPLE: Cleanup Module, Process \"%s:%d\" \n", current->comm, current->pid);
	remove_proc_entry(DEVICE_NAME, NULL);
	printk(KERN_INFO "WORKQUEUESAMPLE: /proc/%s has been removed.\n", DEVICE_NAME);

  //Now we have to get rid of our workqueue, so first we cancel the work,
//  cancel_delayed_work(&our_work);

	printk(KERN_INFO "WORKQUEUESAMPLE: GoodBye.\n");
	//The cleanup_module function doesn't need to return any value to the rest of the kernel
}


//Now we need to define init-module and cleanup_module aliases
module_init(workqueue_module_init);
module_exit(workqueue_module_exit);
