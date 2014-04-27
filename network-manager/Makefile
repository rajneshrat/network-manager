obj-m += module.o  
module-objs :=  kernels.o timer.o net_hook.o
 
#bandwidthsetter.o

#bandwidthsetter.o queue.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
