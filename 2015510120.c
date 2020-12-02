#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h> //file_operations
#include <asm/uaccess.h> //memory access

MODULE_LICENSE("GPL");
MODULE_AUTHOR("DEUCENG");
MODULE_DESCRIPTION("An example Linux kernel module.");
MODULE_VERSION("0.01");

//the path to the requested information must be given to the filePath as a parameter
//if no parameter is entered, cpu information is displayed
static char *filePath = "/proc/cpuinfo";
module_param(filePath, charp, S_IRUGO | S_IWUSR); 
MODULE_PARM_DESC(filePath, "A char string");



static int __init homework_init(void) {
   ////////////////////////////////////////////////////////////////////////////////////
// Create variables
 // Create variables
    struct file *f;
    char buf[120];
    mm_segment_t fs;
    int i;
    // Init the buffer with 0
    for(i=0;i<120;i++)
        buf[i] = 0;
    // To see in /var/log/messages that the module is operating
    printk(KERN_INFO "path to the requested information file %s \n" ,filePath);
    // I am using Fedora and for the test I have chosen following file

    f = filp_open(filePath, O_RDONLY, 0);
    if(f == NULL)
        printk(KERN_ALERT "filp_open error!!.\n");
    else{
        // Get current segment descriptor
        fs = get_fs();
        // Set segment descriptor associated to kernel space
        set_fs(get_ds());
        // Read the file
        f->f_op->read(f, buf, 120, &f->f_pos);
        // Restore segment descriptor
        set_fs(fs);
        // See what we read from file
        printk(KERN_INFO "buf:%s\n",buf);
    }
    filp_close(f,NULL);
////////////////////////////////////////////////////////////////////////////////////
 return 0;
}

static void __exit homework_exit(void) {
 printk(KERN_INFO "Goodbye, World!\n");
}
module_init(homework_init);
module_exit(homework_exit);