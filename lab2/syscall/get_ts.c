#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <asm/processor.h>
#include <linux/uaccess.h>
#include <asm/io_bitmap.h>

#include <linux/mount.h>
#include <linux/path.h>
#include <linux/namei.h>
#include <linux/fs.h>

#include <linux/time64.h>

#include "exported_ts.h"
#include "exported_inode.h"

SYSCALL_DEFINE0(get_ts0)
{
    printk(KERN_INFO "get_ts0 called\\n");
    return 0;
}

SYSCALL_DEFINE2(get_inode, char*, path_name, struct exp_inode*, inode) 
{
    struct path path;
    struct inode *oinode;

    if(kern_path(path_name, LOOKUP_FOLLOW, &path)) {
        //printk(KERN_INFO "all bad");
        return -1;
    } 


    #define COPY_FIELD_TO_USER(field) inode->field=oinode->field


    oinode = path.dentry->d_inode;

    COPY_FIELD_TO_USER(i_mode);
    COPY_FIELD_TO_USER(i_opflags);
    COPY_FIELD_TO_USER(i_flags);
    COPY_FIELD_TO_USER(i_ino);
    COPY_FIELD_TO_USER(i_nlink);
    COPY_FIELD_TO_USER(i_rdev);
    COPY_FIELD_TO_USER(i_size);
    COPY_FIELD_TO_USER(i_bytes);
    COPY_FIELD_TO_USER(i_blkbits);
    COPY_FIELD_TO_USER(i_write_hint);
    COPY_FIELD_TO_USER(i_blocks);

    inode->i_atime = timespec64_to_ns(&oinode->i_atime);
    inode->i_mtime = timespec64_to_ns(&oinode->i_mtime);
    inode->i_ctime = timespec64_to_ns(&oinode->i_ctime);

    #undef COPY_FIELD_TO_USER
    return 0;
}

SYSCALL_DEFINE2(get_ts, int, pid, struct exp_thread_struct*, ts)
{
    printk(KERN_INFO "get_ts called\n");
    struct task_struct *task_s = find_task_by_vpid(pid);
    //ts = &task_s->thread;
    struct thread_struct *ots = &task_s->thread;
    
    if(task_s == NULL) {
        printk(KERN_INFO "no such task");
        return -1;
    }

    // struct exp_thread_struct *ts = kmalloc(sizeof(struct exp_thread_struct),GFP_USER);

#ifdef CONFIG_X86_32
    ts->is32 = 1;
#else
    ts->is32 = 0;   
#endif


    if(ots != NULL) {

        #define COPY_FIELD_TO_USER(field) ts->field= ots->field;\
                printk(#field": %lu\n", (unsigned long) ots->field);\
                printk(#field": %lu\n", (unsigned long) ts->field)

    #ifdef CONFIG_X86_32
        COPY_FIELD_TO_USER(sp0);
        COPY_FIELD_TO_USER(sysenter_cs);
        COPY_FIELD_TO_USER(fs);
        COPY_FIELD_TO_USER(gs);
    #else
        COPY_FIELD_TO_USER(es);
        COPY_FIELD_TO_USER(ds);
        COPY_FIELD_TO_USER(fsindex);
        COPY_FIELD_TO_USER(gsindex);

        COPY_FIELD_TO_USER(fsbase);
        COPY_FIELD_TO_USER(gsbase);
        // ts->gsbase = 987;
    #endif 

        COPY_FIELD_TO_USER(sp);
        COPY_FIELD_TO_USER(virtual_dr6);
        COPY_FIELD_TO_USER(ptrace_dr7);

        COPY_FIELD_TO_USER(cr2);
        COPY_FIELD_TO_USER(trap_nr);
        COPY_FIELD_TO_USER(error_code);

        COPY_FIELD_TO_USER(iopl_emul);
        COPY_FIELD_TO_USER(iopl_warn);
        COPY_FIELD_TO_USER(sig_on_uaccess_err);

        COPY_FIELD_TO_USER(pkru);

        unsigned long res = copy_to_user(ts->tls_array, ots->tls_array, 24);
        if(res) printk(KERN_INFO "copy_to_user failed\n");
        
        #undef COPY_FIELD_TO_USER
    }
    else {
        printk(KERN_INFO "ots is NULL");
    }
    return 0;
}