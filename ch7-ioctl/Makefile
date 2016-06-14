#obj-m += tasklet.o
#obj-m += jiffie.o
#obj-m += workqueue_test.o
obj-m += workqueue_global_test.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
