#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/debugfs.h>
#include <linux/slab.h>

#include "queue.h"

int init_module(void);
void cleanup_module(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);

#define DEVICE_NAME "netdev"

char *data;
static int Major;

void mmap_open(struct vm_area_struct *vma)
{

}

void mmap_close(struct vm_area_struct *vma)
{

}

struct page *mmap_nopage (struct vm_area_struct *vma, struct vm_fault *vmf)
{
    struct page *page;
    page = virt_to_page(data);
    get_page(page);
    vmf->page = page;
    return 0;
}

struct vm_operations_struct mmap_vm_ops = {
    .open  =    mmap_open,
    .close =    mmap_close,
    .fault =    mmap_nopage
};

int my_close(struct inode *inode, struct file *filp)
{
    free_pages((unsigned long)data, 10);
    return 0;
}

node *driver_in_user_out;
node *user_in_driver_out;

int device_open(struct inode *inode, struct file *filp)
{
    data	 =	__get_free_pages(GFP_KERNEL, 10);
    driver_in_user_out = (node *) data;
    user_in_driver_out = (node *) data + 1024 * 5;
    strcpy(data,"rahul sings\n");
    return 0;
}

int device_mmap(struct file *filp, struct vm_area_struct *vma)
{
    vma->vm_ops = &mmap_vm_ops;
    vma->vm_flags |= VM_RESERVED;
    vma->vm_flags |= VM_SHARED;
    vma->vm_flags |= VM_WRITE;
    return 0;
}

int device_close(struct inode *inode, struct file *filp)
{
    free_page((unsigned long)data);
    return 0;
}
static struct file_operations fops = {
    .open = device_open,
    .release= device_close,
    .mmap = device_mmap
};


void cleanup_bsetter();
void intialize_bsetter(void);
int init_net_hook(void);
void cleanup_net_hook(void);

int init_module(void)
{
    Major = register_chrdev(0, DEVICE_NAME, &fops);

    if (Major < 0) {
        printk ("Failed to register device %d\n", Major);
        return Major;
    }
    printk("'mknod /dev/test c %d 0'.\n", Major);
    intialize_bsetter();
    init_net_hook();
    return 0;
}


void cleanup_module(void)
{
    cleanup_bsetter();
    cleanup_net_hook();
    unregister_chrdev(Major, DEVICE_NAME);
}
MODULE_LICENSE("GPL");
