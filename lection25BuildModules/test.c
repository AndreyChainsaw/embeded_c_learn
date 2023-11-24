#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/kobject.h>
#include <linux/rwlock.h>

//TODO: Problems with rwlock in kernel 6.1.0-13
//bad address...
static int major = 0;
static rwlock_t lock;
static char test_string[15] = "Hello!\n";

ssize_t test_read(struct file *fd, char __user *buff, size_t size, loff_t *off) {
    size_t rc;

    read_lock(&lock);
    rc = simple_read_from_buffer(buff, size, off, test_string, 15);
    read_unlock(&lock);
    return rc;
}

ssize_t test_write(struct file *fd, const char __user *buff, size_t size, loff_t *off) {
    size_t rc;

    if (size > 15)
        return -EINVAL;

    write_lock(&lock);
    rc = simple_write_to_buffer(test_string, 15, off, buff, size);
    write_unlock(&lock);

    return rc;
}


static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = test_read,
    .write = test_write
};

int init_module(void) {
    pr_info("Test module loaded\n");

    rwlock_init(&lock);
    //pr_info("lock: %d", lock);
    major = register_chrdev(major, "test", &fops);

    if (major < 0)
        return major;
    
    pr_info("Major = %d\n", major);

    return 0;
}

void cleanup_module(void) {
    unregister_chrdev(major, "test");
    pr_info("module unloaded\n");
}

MODULE_LICENSE("GPL");
