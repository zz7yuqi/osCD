#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/string.h>

#define BUFN_MAX 4096

static int my_major = 0;
static char * bufn;

static int my_open(struct inode * inode, struct file * file){
    static int counter=0;
    counter++;
    return 0;

}

static int my_release(struct inode * inode, struct file * file){
    return 0;
}

static ssize_t my_read (struct file *filp, char __user * buf,  size_t size, loff_t *offset){
    int len = strlen(bufn);
    if (size <= len) copy_to_user(buf, bufn, size);
    else  copy_to_user(buf, bufn, len);
    return len;
    
}

static ssize_t my_write (struct file *filp,const char __user * buf,  size_t size, loff_t *offset){
    if (size > BUFN_MAX) return -1;
    memset(bufn, 0, BUFN_MAX);
    copy_from_user(bufn, buf, size); //将用户空间中的数据通过buf传递给bufn
    return size;
}

struct file_operations my_fops={
        .owner = THIS_MODULE,  //在此模块运行时不能被卸载
       .read=my_read,
       .write=my_write,
       .open=my_open,
       .release=my_release,
};


static int __init my_init(void){
   int result;
   result=register_chrdev(0,"my_driver",&my_fops);
   if(result<0){
       printk("error:can not register the device\n");
       return -1;
   }
   if(my_major==0){
       my_major=result;
       bufn = kzalloc(BUFN_MAX,GFP_KERNEL);
       printk("<1>hehe,the device has been registered!\n");
       printk("<1>the virtual device was assigned major number %d.\n",my_major);
       printk("<1>To talk to the driver,create a dev file with\n");
       printk("<1>'mknod/dev/my c %d 0'\n",my_major);
       printk("<1>Remove the dev and the file when done\n");
   }
  return 0;
}

static void __exit my_exit(void){
    unregister_chrdev(my_major,"my_driver");
    printk("<1>unloading the device\n");
}

module_init(my_init);
module_exit(my_exit);
MODULE_LICENSE("GPL");