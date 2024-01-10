	.file	"macros.c"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"ERROR: %s\n"
	.text
	.p2align 4
	.globl	die
	.type	die, @function
die:
.LFB22:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movq	%rdi, %rdx
	movq	stderr(%rip), %rdi
	xorl	%eax, %eax
	leaq	.LC0(%rip), %rsi
	call	fprintf@PLT
	movl	$1, %edi
	call	exit@PLT
	.cfi_endproc
.LFE22:
	.size	die, .-die
	.ident	"GCC: (GNU) 13.2.1 20230801"
	.section	.note.GNU-stack,"",@progbits
