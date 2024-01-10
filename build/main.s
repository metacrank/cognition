	.file	"main.c"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"Usage: stem [-hv] [file]"
	.text
	.p2align 4
	.globl	usage
	.type	usage, @function
usage:
.LFB22:
	.cfi_startproc
	leaq	.LC0(%rip), %rdi
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	call	puts@PLT
	movl	$1, %edi
	call	exit@PLT
	.cfi_endproc
.LFE22:
	.size	usage, .-usage
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC1:
	.string	"Author: Preston Pan, MIT License 2023"
	.section	.rodata.str1.1
.LC2:
	.string	"stem, version 1.2 alpha"
	.text
	.p2align 4
	.globl	version
	.type	version, @function
version:
.LFB23:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	leaq	.LC1(%rip), %rdi
	call	puts@PLT
	leaq	.LC2(%rip), %rdi
	call	puts@PLT
	xorl	%edi, %edi
	call	exit@PLT
	.cfi_endproc
.LFE23:
	.size	version, .-version
	.section	.rodata.str1.1
.LC3:
	.string	"--help"
.LC4:
	.string	"--version"
.LC5:
	.string	"rb"
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB24:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$24, %rsp
	.cfi_def_cfa_offset 48
	movq	%fs:40, %rax
	movq	%rax, 8(%rsp)
	xorl	%eax, %eax
	cmpl	$1, %edi
	jle	.L9
	movq	8(%rsi), %rbx
	movzbl	(%rbx), %ebp
	cmpl	$45, %ebp
	je	.L31
.L16:
	leaq	.LC3(%rip), %rsi
	movq	%rbx, %rdi
	call	strcmp@PLT
	testl	%eax, %eax
	je	.L9
	cmpl	$45, %ebp
	je	.L32
.L17:
	leaq	.LC4(%rip), %rsi
	movq	%rbx, %rdi
	call	strcmp@PLT
	testl	%eax, %eax
	je	.L11
	movq	%rbx, %rdi
	leaq	.LC5(%rip), %rsi
	call	fopen@PLT
	movq	%rax, %rbx
	testq	%rax, %rax
	je	.L9
	movq	%rsp, %rsi
	movq	%rax, %rcx
	leaq	INBUF(%rip), %rdi
	xorl	%edx, %edx
	call	getdelim@PLT
	movq	%rbx, %rdi
	call	fclose@PLT
	movq	INBUF(%rip), %rdi
	call	init_parser@PLT
	movl	$10, %edi
	movq	%rax, PARSER(%rip)
	call	init_array@PLT
	movl	$500, %edi
	movq	%rax, STACK(%rip)
	call	init_ht@PLT
	movl	$10, %edi
	movq	%rax, WORD_TABLE(%rip)
	call	init_array@PLT
	movl	$500, %edi
	movq	%rax, EVAL_STACK(%rip)
	call	init_ht@PLT
	movl	$500, %edi
	movq	%rax, FLIT(%rip)
	call	init_ht@PLT
	movq	%rax, OBJ_TABLE(%rip)
	xorl	%eax, %eax
	call	add_funcs@PLT
	jmp	.L14
	.p2align 4,,10
	.p2align 3
.L33:
	call	eval@PLT
.L14:
	movq	PARSER(%rip), %rdi
	call	parser_get_next@PLT
	movq	%rax, %rdi
	testq	%rax, %rax
	jne	.L33
	movq	INBUF(%rip), %rdi
	call	free@PLT
	movq	value_free@GOTPCREL(%rip), %rsi
	movq	WORD_TABLE(%rip), %rdi
	call	ht_free@PLT
	movq	func_free@GOTPCREL(%rip), %rsi
	movq	FLIT(%rip), %rdi
	call	ht_free@PLT
	movq	custom_free@GOTPCREL(%rip), %rsi
	movq	OBJ_TABLE(%rip), %rdi
	call	ht_free@PLT
	movq	STACK(%rip), %rdi
	call	array_free@PLT
	movq	PARSER(%rip), %rdi
	call	free@PLT
	movq	EVAL_STACK(%rip), %rdi
	call	array_free@PLT
	movq	8(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L34
	addq	$24, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
.L32:
	.cfi_restore_state
	cmpb	$118, 1(%rbx)
	jne	.L17
	cmpb	$0, 2(%rbx)
	jne	.L17
.L11:
	xorl	%eax, %eax
	call	version
.L31:
	cmpb	$104, 1(%rbx)
	jne	.L16
	cmpb	$0, 2(%rbx)
	jne	.L16
.L9:
	xorl	%eax, %eax
	call	usage
.L34:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE24:
	.size	main, .-main
	.ident	"GCC: (GNU) 13.2.1 20230801"
	.section	.note.GNU-stack,"",@progbits
