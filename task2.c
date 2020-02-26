// asmlinkage int sys_mycall(int number) 
// { 
// 　　return number; 

#include <linux/unistd.h>
#include <sys/syscall.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
void sys_mycall_copy(char * input, char * output){
        struct file* fp1 = filp_open(input, O_RDONLY,0);
        struct file* fp2 = filp_open(output,O_WRONLY | O_CREAT, S_IRUSR|S_IWUSR);
        if (IS_ERR(fp1)) {  
        printk("create file1 error\n");  
        return ;  
    } 
     if (IS_ERR(fp2)) {  
        printk("create file2 error\n");  
        return ;  
    } 

        int bitn = 1;
        char a[100];
        mm_segment_t fs;  
        
        fs = get_fs();
        set_fs(KERNEL_DS);
        loff_t pos1, pos2; 
        
        pos1 = 0;
         pos2 = 0;
        while(bitn > 0){
            
            bitn = vfs_read(fp1, a, 50, &pos1);
            pos1 = pos1 + bitn;
            if (bitn == -1) printk("read func error!\n");
            else if (bitn == 0) break;
           
            vfs_write(fp2, a, bitn, &pos2);
            pos2 = pos2 + bitn;
        }
        set_fs(fs);
        filp_close(fp1, NULL);
        filp_close(fp2, NULL);
}



int main(){
    sys_mycll_copy("./input", "./output");
    return 0;
}