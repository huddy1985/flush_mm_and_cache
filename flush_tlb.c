/*
 * flush all tables lookside buffer, for performance tunnint
 */
#include <linux/sched.h>
#include <asm-generic/cacheflush.h>
#include <asm/tlbflush.h>
#include <asm/tlb.h>
#include <linux/device.h>
#include <linux/miscdevice.h>

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

#include <linux/mm.h>
#include <linux/mm_types.h>
#include <linux/fs.h>

#include <linux/errno.h>
#include <linux/ioctl.h>

#include <linux/moduleparam.h>
#include <asm/uaccess.h>
#include <asm/atomic.h>

#include <linux/kallsyms.h>

#include "fls.h"

#define FLS_TLB "1.0.0"
#define DEVICE_NAME "fls"

MODULE_VERSION(FLS_TLB);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("xuguo wang");

ssize_t fls_write(struct file *file, unsigned int cmd,unsigned long arg);
ssize_t fls_write(struct file *file, unsigned int cmd,unsigned long arg)
{
    struct task_struct *t;
	struct vm_area_struct *p = NULL;
	struct mm_struct *mm = NULL;
	
	static void (*flush_tlb_mm_range)(struct mm_struct*, unsigned long, unsigned end, unsigned long);

	if ((flush_tlb_mm_range = (void*)kallsyms_lookup_name("flush_tlb_mm_range")) == NULL) {
		pr_err("flush tlb api not found\n");
		return -1;	
	}

    pr_info("flush pid is %ld\n", arg);

    if (_IOC_TYPE(cmd) != FLS_TYPE_PROCESS &&
        _IOC_TYPE(cmd) != FLS_TYPE_THREAD) {
        pr_err("please check you input cmd type: %d\n", cmd);
        return -1;
    }

    switch(cmd) {
    case 0:
        t = NULL;

        t = pid_task(find_vpid(arg), PIDTYPE_PID);
        if (!t) {
            pr_err("get pid %d of task_struct failed\n", (int)arg);
            pr_err("please make sure you input pid\n");
            return -1;
        }

		mm = t->mm;

		if (!mm)
			return -1;

		down_read(&t->mm->mmap_sem);
		flush_cache_mm(t->mm);
		up_read(&t->mm->mmap_sem);

		p = mm->mmap;
		while (p) {
			down_read(&t->mm->mmap_sem);
			flush_tlb_mm_range(mm, p->vm_start, p->vm_end, 0L);
			pr_info("flush %p -> %p\n", p->vm_start, p->vm_end);
			up_read(&t->mm->mmap_sem);
			p = p->vm_next;
		}

        break;
    case 1:
        break;
    }
    return 0;
}

static const struct file_operations fls_fops = {
    .owner = THIS_MODULE,
    .unlocked_ioctl = fls_write,
};

static struct miscdevice misc = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = DEVICE_NAME,
    .fops = &fls_fops,
};

static int __init fls_init(void);
static int __init fls_init(void)
{
    int err;

    pr_info("register a misc device: fls_tlb\n");
    pr_info("\tapi:\n");
    pr_info("\tflush mem\n");
    pr_info("\tflush cache\n");
    err = misc_register(&misc);

    return err;
}

static void __exit fls_exit(void)
{
    pr_info("unregister misc flush tlb and cache\n");
    misc_deregister(&misc);
}

module_init(fls_init);
module_exit(fls_exit);
