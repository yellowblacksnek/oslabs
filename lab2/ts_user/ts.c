#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdlib.h>

#include <linux/fs.h>


#include <dirent.h> 
#include <stdio.h> 

#define u16 __u16
#define u32 __u32

#include "exported_ts.h"

#define PRINT_TLS 0

#define PRINT_FIELD(field) printf(#field": %lu\n", (unsigned long) ts->field);
#define PRINT_FIELD_HEX(field) printf(#field": %lx\n", (unsigned long) ts->field);

int main( int argc, char *argv[] )
{
    if( argc != 2 ) {
        printf("Wrong format\nUsage: ts <PID>\n");
        return -1;
    }
    int pid = atoi(argv[1]);
    struct exp_thread_struct* ts = malloc(sizeof(struct exp_thread_struct));

    long int res = syscall(548, pid, ts);
    if(res == -1) {
        printf("No task found");
        return -1;
    }

    if(ts) {
        
        if(ts->is32) {
            printf("sp0: %lu\n", ts->sp0);
            printf("sysenter_cs: %lu\n", ts->sysenter_cs);
            printf("fs: %lu\n", ts->fs);
            printf("gs: %lu\n", ts->gs);
        } else {
            PRINT_FIELD_HEX(es);
            PRINT_FIELD_HEX(ds);
            PRINT_FIELD_HEX(fsindex);
            PRINT_FIELD_HEX(gsindex);
            PRINT_FIELD_HEX(fsbase);
            PRINT_FIELD_HEX(gsbase);
        }

        PRINT_FIELD_HEX(sp);
        PRINT_FIELD(virtual_dr6);
        PRINT_FIELD(ptrace_dr7);
        PRINT_FIELD(cr2);
        PRINT_FIELD(trap_nr);
        PRINT_FIELD(error_code);
        PRINT_FIELD(iopl_emul);
        PRINT_FIELD(iopl_warn);
        PRINT_FIELD(sig_on_uaccess_err);

        // printf("sp: %lu\n", ts->sp);
        // printf("virtual_dr6: %lu\n", ts->virtual_dr6);
        // printf("ptrace_dr7: %lu\n", ts->ptrace_dr7);
        // printf("virtual_dr6: %lu\n", ts->virtual_dr6);
        // printf("cr2: %lu\n", ts->cr2);
        // printf("trap_nr: %lu\n", ts->trap_nr);
        // printf("error_code: %lu\n", ts->error_code);
        // printf("iopl_emul: %lu\n", ts->iopl_emul);
        // printf("iopl_warn: %lu\n", ts->iopl_warn);
        // printf("sig_on_uaccess_err: %lu\n", ts->sig_on_uaccess_err);

        if(PRINT_TLS) {
            for(int i = 0; i < 3; ++i) {
                printf("[%d] %6s:%u\n", i, "limit0", ts->tls_array[i].limit0);
                printf("[%d] %6s:%u\n", i, "base0",  ts->tls_array[i].base0);
                printf("[%d] %6s:%u\n", i, "base1",  ts->tls_array[i].base1);
                printf("[%d] %6s:%u\n", i, "type",   ts->tls_array[i].type);
                printf("[%d] %6s:%u\n", i, "s",      ts->tls_array[i].s);
                printf("[%d] %6s:%u\n", i, "dpl",    ts->tls_array[i].dpl);
                printf("[%d] %6s:%u\n", i, "p",      ts->tls_array[i].p);
                printf("[%d] %6s:%u\n", i, "limit1", ts->tls_array[i].limit1);
                printf("[%d] %6s:%u\n", i, "avl",    ts->tls_array[i].avl);
                printf("[%d] %6s:%u\n", i, "l",      ts->tls_array[i].l);
                printf("[%d] %6s:%u\n", i, "d",      ts->tls_array[i].d);
                printf("[%d] %6s:%u\n", i, "g",      ts->tls_array[i].g);
                printf("[%d] %6s:%u\n", i, "base2",  ts->tls_array[i].base2);
            }
        }
    }
    else {
        printf("Unable to get");
        return -1;
    }
    return 0;
}
