#ifndef _EXPORTED_TS_H
#define _EXPORTED_TS_H

#include <linux/types.h>

#define GDT_ENTRY_TLS_ENTRIES		3

// #define IO_BITMAP_BITS			65536
// #define IO_BITMAP_BYTES			(IO_BITMAP_BITS / BITS_PER_BYTE)
// #define IO_BITMAP_LONGS			(IO_BITMAP_BYTES / sizeof(long))


struct exp_desc_struct {
	u16	limit0;
	u16	base0;
	u16	base1: 8, type: 4, s: 1, dpl: 2, p: 1;
	u16	limit1: 4, avl: 1, l: 1, d: 1, g: 1, base2: 8;
} __attribute__((packed));

struct exp_thread_struct {

	unsigned short is32;
	struct exp_desc_struct	tls_array[GDT_ENTRY_TLS_ENTRIES];

	/*  32  */
	unsigned long		sp0;
	unsigned long		sysenter_cs;
	unsigned long 		fs;
	unsigned long 		gs;
	/*		*/

	/*  64  */
	unsigned short		es;
	unsigned short		ds;
	unsigned short		fsindex;
	unsigned short		gsindex;
	unsigned long		fsbase;
	unsigned long		gsbase;
	/*		*/


	unsigned long		sp;

	unsigned long       virtual_dr6;
	unsigned long       ptrace_dr7;
	unsigned long		cr2;
	unsigned long		trap_nr;
	unsigned long		error_code;
	
	
	unsigned long		iopl_emul;
	unsigned int		iopl_warn:1;
	unsigned int		sig_on_uaccess_err:1;
	u32			pkru;
};

#endif