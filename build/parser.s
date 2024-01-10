	.file	"parser.c"
	.text
	.p2align 4
	.globl	func_free
	.type	func_free, @function
func_free:
.LFB24:
	.cfi_startproc
	ret
	.cfi_endproc
.LFE24:
	.size	func_free, .-func_free
	.p2align 4
	.globl	init_array
	.type	init_array, @function
init_array:
.LFB25:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movl	$24, %esi
	movq	%rdi, %rbp
	movl	$1, %edi
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	call	calloc@PLT
	movq	%rbp, %rdi
	movl	$8, %esi
	movq	%rbp, 16(%rax)
	movq	%rax, %rbx
	call	calloc@PLT
	movq	%rax, (%rbx)
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	movq	%rbx, %rax
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE25:
	.size	init_array, .-init_array
	.p2align 4
	.globl	array_append
	.type	array_append, @function
array_append:
.LFB26:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsi, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rdi, %rbx
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movq	16(%rdi), %rsi
	movq	8(%rdi), %rdx
	movq	(%rdi), %rdi
	leaq	-2(%rsi), %rax
	cmpq	%rax, %rdx
	jnb	.L8
.L6:
	movq	%rbp, (%rdi,%rdx,8)
	addq	$1, %rdx
	movq	%rdx, 8(%rbx)
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L8:
	.cfi_restore_state
	leaq	(%rsi,%rsi), %rax
	salq	$4, %rsi
	movq	%rax, 16(%rbx)
	call	realloc@PLT
	movq	8(%rbx), %rdx
	movq	%rax, (%rbx)
	movq	%rax, %rdi
	jmp	.L6
	.cfi_endproc
.LFE26:
	.size	array_append, .-array_append
	.p2align 4
	.globl	array_pop
	.type	array_pop, @function
array_pop:
.LFB27:
	.cfi_startproc
	movq	8(%rdi), %rax
	xorl	%edx, %edx
	testq	%rax, %rax
	je	.L9
	movq	(%rdi), %rdx
	movq	-8(%rdx,%rax,8), %rdx
	subq	$1, %rax
	movq	%rax, 8(%rdi)
.L9:
	movq	%rdx, %rax
	ret
	.cfi_endproc
.LFE27:
	.size	array_pop, .-array_pop
	.p2align 4
	.globl	array_extend
	.type	array_extend, @function
array_extend:
.LFB28:
	.cfi_startproc
	cmpq	$0, 8(%rsi)
	je	.L21
	pushq	%r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	movq	%rsi, %r13
	pushq	%r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	pushq	%rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	movq	%rdi, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	xorl	%ebx, %ebx
	subq	$8, %rsp
	.cfi_def_cfa_offset 48
	movq	8(%rdi), %rdx
	movq	(%rdi), %rdi
.L24:
	movq	0(%r13), %rcx
	movq	16(%rbp), %rsi
.L18:
	leaq	-2(%rsi), %rax
	movq	(%rcx,%rbx,8), %r12
	cmpq	%rax, %rdx
	jnb	.L26
	movq	%r12, (%rdi,%rdx,8)
	addq	$1, %rdx
	addq	$1, %rbx
	movq	%rdx, 8(%rbp)
	cmpq	8(%r13), %rbx
	jb	.L18
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 40
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%rbp
	.cfi_def_cfa_offset 24
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L26:
	.cfi_restore_state
	leaq	(%rsi,%rsi), %rax
	salq	$4, %rsi
	addq	$1, %rbx
	movq	%rax, 16(%rbp)
	call	realloc@PLT
	movq	8(%rbp), %rdx
	movq	%rax, 0(%rbp)
	movq	%rax, %rdi
	movq	%r12, (%rax,%rdx,8)
	addq	$1, %rdx
	movq	%rdx, 8(%rbp)
	cmpq	8(%r13), %rbx
	jb	.L24
	addq	$8, %rsp
	.cfi_def_cfa_offset 40
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%rbp
	.cfi_def_cfa_offset 24
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L21:
	.cfi_restore 3
	.cfi_restore 6
	.cfi_restore 12
	.cfi_restore 13
	ret
	.cfi_endproc
.LFE28:
	.size	array_extend, .-array_extend
	.p2align 4
	.globl	init_value
	.type	init_value, @function
init_value:
.LFB31:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movl	$48, %esi
	movl	%edi, %ebx
	movl	$1, %edi
	call	calloc@PLT
	movl	%ebx, (%rax)
	movb	$0, 40(%rax)
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE31:
	.size	init_value, .-init_value
	.section	.text.unlikely,"ax",@progbits
.LCOLDB1:
	.text
.LHOTB1:
	.p2align 4
	.globl	value_copy
	.type	value_copy, @function
value_copy:
.LFB32:
	.cfi_startproc
	pushq	%r14
	.cfi_def_cfa_offset 16
	.cfi_offset 14, -16
	movl	$48, %esi
	pushq	%r13
	.cfi_def_cfa_offset 24
	.cfi_offset 13, -24
	pushq	%r12
	.cfi_def_cfa_offset 32
	.cfi_offset 12, -32
	pushq	%rbp
	.cfi_def_cfa_offset 40
	.cfi_offset 6, -40
	movq	%rdi, %rbp
	movl	$1, %edi
	pushq	%rbx
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
	subq	$16, %rsp
	.cfi_def_cfa_offset 64
	call	calloc@PLT
	movq	%rax, %rbx
	movl	0(%rbp), %eax
	movl	%eax, (%rbx)
	cmpl	$6, %eax
	ja	.L29
	leaq	.L32(%rip), %rdx
	movslq	(%rdx,%rax,4), %rax
	addq	%rdx, %rax
	jmp	*%rax
	.section	.rodata
	.align 4
	.align 4
.L32:
	.long	.L34-.L32
	.long	.L35-.L32
	.long	.L35-.L32
	.long	.L34-.L32
	.long	.L33-.L32
	.long	.L29-.L32
	.long	.L31-.L32
	.text
	.p2align 4,,10
	.p2align 3
.L33:
	movq	16(%rbp), %r12
	movl	$24, %esi
	movl	$1, %edi
	call	calloc@PLT
	movl	$8, %esi
	movdqu	8(%r12), %xmm0
	movq	%rax, %r13
	movhlps	%xmm0, %xmm1
	movups	%xmm0, 8(%rax)
	movq	%xmm1, %rdi
	movaps	%xmm0, (%rsp)
	call	calloc@PLT
	movdqa	(%rsp), %xmm0
	movq	%rax, 0(%r13)
	movq	%xmm0, %rdx
	testq	%rdx, %rdx
	je	.L36
	xorl	%ebp, %ebp
	jmp	.L37
	.p2align 4,,10
	.p2align 3
.L53:
	movq	0(%r13), %rax
.L37:
	leaq	(%rax,%rbp,8), %r14
	movq	(%r12), %rax
	movq	(%rax,%rbp,8), %rdi
	addq	$1, %rbp
	call	value_copy
	movq	%rax, (%r14)
	cmpq	8(%r12), %rbp
	jb	.L53
.L36:
	movq	%r13, 16(%rbx)
.L29:
	addq	$16, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 48
	movq	%rbx, %rax
	popq	%rbx
	.cfi_def_cfa_offset 40
	popq	%rbp
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r13
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L35:
	.cfi_restore_state
	fldt	16(%rbp)
	movq	%rbx, %rax
	fstpt	16(%rbx)
	addq	$16, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 48
	popq	%rbx
	.cfi_def_cfa_offset 40
	popq	%rbp
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r13
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L34:
	.cfi_restore_state
	movq	16(%rbp), %rdi
	call	string_copy@PLT
	movq	%rax, 16(%rbx)
	movzbl	40(%rbp), %eax
	movb	%al, 40(%rbx)
	addq	$16, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 48
	movq	%rbx, %rax
	popq	%rbx
	.cfi_def_cfa_offset 40
	popq	%rbp
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r13
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L31:
	.cfi_restore_state
	movq	16(%rbx), %rax
	movq	OBJ_TABLE(%rip), %rdi
	movq	16(%rax), %r12
	movq	(%rdi), %r8
	movl	$5381, %eax
	movsbq	(%r12), %rdx
	leaq	1(%r12), %rcx
	testb	%dl, %dl
	je	.L38
	.p2align 4,,10
	.p2align 3
.L39:
	movq	%rax, %rsi
	addq	$1, %rcx
	salq	$5, %rsi
	addq	%rsi, %rax
	addq	%rdx, %rax
	movsbq	-1(%rcx), %rdx
	testb	%dl, %dl
	jne	.L39
.L38:
	xorl	%edx, %edx
	divq	8(%rdi)
	movq	(%r8,%rdx,8), %rax
	movq	(%rax), %r13
	testq	%r13, %r13
	jne	.L42
	jmp	.L40
	.p2align 4,,10
	.p2align 3
.L41:
	movq	16(%r13), %r13
	testq	%r13, %r13
	je	.L40
.L42:
	movq	0(%r13), %rax
	movq	%r12, %rdi
	movq	16(%rax), %rsi
	call	strcmp@PLT
	testl	%eax, %eax
	jne	.L41
	movq	8(%r13), %rax
	movq	32(%rbp), %rdi
	call	*8(%rax)
	movq	16(%rbp), %rdi
	movq	%rax, 32(%rbx)
	call	string_copy@PLT
	movq	%rax, 16(%rbx)
	jmp	.L29
	.cfi_endproc
	.section	.text.unlikely
	.cfi_startproc
	.type	value_copy.cold, @function
value_copy.cold:
.LFSB32:
.L40:
	.cfi_def_cfa_offset 64
	.cfi_offset 3, -48
	.cfi_offset 6, -40
	.cfi_offset 12, -32
	.cfi_offset 13, -24
	.cfi_offset 14, -16
	movq	8, %rax
	ud2
	.cfi_endproc
.LFE32:
	.text
	.size	value_copy, .-value_copy
	.section	.text.unlikely
	.size	value_copy.cold, .-value_copy.cold
.LCOLDE1:
	.text
.LHOTE1:
	.p2align 4
	.globl	array_copy
	.type	array_copy, @function
array_copy:
.LFB30:
	.cfi_startproc
	pushq	%r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	movl	$24, %esi
	pushq	%r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	pushq	%rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	movq	%rdi, %rbp
	movl	$1, %edi
	pushq	%rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	subq	$24, %rsp
	.cfi_def_cfa_offset 64
	call	calloc@PLT
	movdqu	8(%rbp), %xmm0
	movl	$8, %esi
	movq	%rax, %r13
	movhlps	%xmm0, %xmm1
	movups	%xmm0, 8(%rax)
	movq	%xmm1, %rdi
	movaps	%xmm0, (%rsp)
	call	calloc@PLT
	movdqa	(%rsp), %xmm0
	movq	%rax, 0(%r13)
	movq	%xmm0, %rdx
	testq	%rdx, %rdx
	je	.L54
	xorl	%ebx, %ebx
	jmp	.L56
	.p2align 4,,10
	.p2align 3
.L61:
	movq	0(%r13), %rax
.L56:
	leaq	(%rax,%rbx,8), %r12
	movq	0(%rbp), %rax
	movq	(%rax,%rbx,8), %rdi
	addq	$1, %rbx
	call	value_copy
	movq	%rax, (%r12)
	cmpq	8(%rbp), %rbx
	jb	.L61
.L54:
	addq	$24, %rsp
	.cfi_def_cfa_offset 40
	movq	%r13, %rax
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%rbp
	.cfi_def_cfa_offset 24
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE30:
	.size	array_copy, .-array_copy
	.p2align 4
	.globl	value_free
	.type	value_free, @function
value_free:
.LFB33:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	movl	(%rdi), %eax
	movq	%rdi, %rbx
	cmpl	$3, %eax
	je	.L77
	testl	%eax, %eax
	je	.L77
	cmpl	$4, %eax
	je	.L86
.L65:
	cmpl	$6, %eax
	je	.L87
.L68:
	movq	%rbx, %rdi
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	jmp	free@PLT
	.p2align 4,,10
	.p2align 3
.L77:
	.cfi_restore_state
	movq	16(%rbx), %rdi
	call	string_free@PLT
	movl	(%rbx), %eax
	cmpl	$4, %eax
	jne	.L65
.L86:
	movq	16(%rbx), %r12
	xorl	%ebp, %ebp
	cmpq	$0, 8(%r12)
	je	.L67
	.p2align 4,,10
	.p2align 3
.L66:
	movq	(%r12), %rax
	movq	(%rax,%rbp,8), %rdi
	addq	$1, %rbp
	call	value_free
	cmpq	8(%r12), %rbp
	jb	.L66
.L67:
	movq	(%r12), %rdi
	call	free@PLT
	movq	%r12, %rdi
	call	free@PLT
	movl	(%rbx), %eax
	cmpl	$6, %eax
	jne	.L68
.L87:
	movq	16(%rbx), %rax
	movq	OBJ_TABLE(%rip), %rdi
	movq	16(%rax), %rbp
	movq	(%rdi), %r8
	movl	$5381, %eax
	movsbq	0(%rbp), %rdx
	leaq	1(%rbp), %rcx
	testb	%dl, %dl
	je	.L69
	.p2align 4,,10
	.p2align 3
.L70:
	movq	%rax, %rsi
	addq	$1, %rcx
	salq	$5, %rsi
	addq	%rsi, %rax
	addq	%rdx, %rax
	movsbq	-1(%rcx), %rdx
	testb	%dl, %dl
	jne	.L70
.L69:
	xorl	%edx, %edx
	divq	8(%rdi)
	movq	(%r8,%rdx,8), %rax
	movq	(%rax), %r12
	testq	%r12, %r12
	jne	.L73
	.p2align 4,,10
	.p2align 3
.L76:
	xorl	%eax, %eax
	jmp	.L71
	.p2align 4,,10
	.p2align 3
.L72:
	movq	16(%r12), %r12
	testq	%r12, %r12
	je	.L76
.L73:
	movq	(%r12), %rax
	movq	%rbp, %rdi
	movq	16(%rax), %rsi
	call	strcmp@PLT
	testl	%eax, %eax
	jne	.L72
	movq	8(%r12), %rax
.L71:
	movq	32(%rbx), %rdi
	call	*%rax
	movq	%rbx, %rdi
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	jmp	free@PLT
	.cfi_endproc
.LFE33:
	.size	value_free, .-value_free
	.p2align 4
	.globl	array_free
	.type	array_free, @function
array_free:
.LFB29:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rdi, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	cmpq	$0, 8(%rdi)
	je	.L89
	xorl	%ebx, %ebx
	.p2align 4,,10
	.p2align 3
.L90:
	movq	0(%rbp), %rax
	movq	(%rax,%rbx,8), %rdi
	addq	$1, %rbx
	call	value_free
	cmpq	8(%rbp), %rbx
	jb	.L90
.L89:
	movq	0(%rbp), %rdi
	call	free@PLT
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	movq	%rbp, %rdi
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	free@PLT
	.cfi_endproc
.LFE29:
	.size	array_free, .-array_free
	.p2align 4
	.globl	init_custom
	.type	init_custom, @function
init_custom:
.LFB34:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	movq	%rdi, %r12
	movl	$1, %edi
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	movq	%rsi, %rbp
	movl	$24, %esi
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	movq	%rdx, %rbx
	call	calloc@PLT
	movq	%r12, (%rax)
	movq	%rbp, 16(%rax)
	movq	%rbx, 8(%rax)
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE34:
	.size	init_custom, .-init_custom
	.p2align 4
	.globl	custom_free
	.type	custom_free, @function
custom_free:
.LFB35:
	.cfi_startproc
	jmp	free@PLT
	.cfi_endproc
.LFE35:
	.size	custom_free, .-custom_free
	.p2align 4
	.globl	init_parser
	.type	init_parser, @function
init_parser:
.LFB36:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movl	$16, %esi
	movq	%rdi, %rbx
	movl	$1, %edi
	call	calloc@PLT
	movzbl	(%rbx), %edx
	movq	%rbx, (%rax)
	movb	%dl, 12(%rax)
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE36:
	.size	init_parser, .-init_parser
	.p2align 4
	.globl	parser_reset
	.type	parser_reset, @function
parser_reset:
.LFB37:
	.cfi_startproc
	movq	%rsi, (%rdi)
	movl	$0, 8(%rdi)
	movzbl	(%rsi), %eax
	movb	%al, 12(%rdi)
	ret
	.cfi_endproc
.LFE37:
	.size	parser_reset, .-parser_reset
	.p2align 4
	.globl	parser_move
	.type	parser_move, @function
parser_move:
.LFB38:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	movq	(%rdi), %r12
	movq	%rdi, %rbx
	movl	8(%rdi), %ebp
	movq	%r12, %rdi
	call	strlen@PLT
	movslq	%ebp, %rdx
	cmpq	%rax, %rdx
	jnb	.L99
	cmpb	$0, 12(%rbx)
	je	.L99
	addl	$1, %ebp
	movl	%ebp, 8(%rbx)
	movslq	%ebp, %rbp
	movzbl	(%r12,%rbp), %eax
	movb	%al, 12(%rbx)
.L99:
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE38:
	.size	parser_move, .-parser_move
	.p2align 4
	.globl	parser_skip_whitespace
	.type	parser_skip_whitespace, @function
parser_skip_whitespace:
.LFB39:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	movq	%rdi, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$8, %rsp
	.cfi_def_cfa_offset 64
	call	__ctype_b_loc@PLT
	movq	(%rax), %r13
	movsbq	12(%rbp), %rax
	testb	$32, 1(%r13,%rax,2)
	je	.L102
	movq	0(%rbp), %r12
	movq	%rax, %r14
.L106:
	movslq	8(%rbp), %r15
	movq	%r15, %rbx
.L105:
	movq	%r12, %rdi
	call	strlen@PLT
.L108:
	cmpq	%rax, %r15
	jnb	.L108
	testb	%r14b, %r14b
	je	.L105
	addl	$1, %ebx
	movl	%ebx, 8(%rbp)
	movslq	%ebx, %rbx
	movsbq	(%r12,%rbx), %rax
	movb	%al, 12(%rbp)
	movq	%rax, %r14
	testb	$32, 1(%r13,%rax,2)
	jne	.L106
.L102:
	addq	$8, %rsp
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE39:
	.size	parser_skip_whitespace, .-parser_skip_whitespace
	.p2align 4
	.globl	parse_string
	.type	parse_string, @function
parse_string:
.LFB40:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	movl	$48, %esi
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	movq	%rdi, %rbx
	movl	$1, %edi
	subq	$8, %rsp
	.cfi_def_cfa_offset 64
	call	calloc@PLT
	movq	(%rbx), %r13
	movl	8(%rbx), %ebp
	movl	$3, (%rax)
	movq	%rax, %r12
	movb	$0, 40(%rax)
	movq	%r13, %rdi
	call	strlen@PLT
	movslq	%ebp, %rdx
	cmpq	%rax, %rdx
	jnb	.L114
	cmpb	$0, 12(%rbx)
	jne	.L152
.L114:
	xorl	%edi, %edi
	call	init_string@PLT
	movsbl	12(%rbx), %esi
	movq	%rax, %rbp
	.p2align 4,,10
	.p2align 3
.L115:
	testb	%sil, %sil
	setne	%r13b
	cmpb	$34, %sil
	je	.L133
.L154:
	testb	%r13b, %r13b
	je	.L133
	cmpb	$92, %sil
	je	.L153
	movq	%rbp, %rdi
	call	string_append@PLT
	movq	(%rbx), %r13
	movl	8(%rbx), %r14d
	movq	%r13, %rdi
	call	strlen@PLT
	movslq	%r14d, %rdx
	cmpq	%rax, %rdx
	jb	.L127
	movsbl	12(%rbx), %esi
	testb	%sil, %sil
	setne	%r13b
	cmpb	$34, %sil
	jne	.L154
	.p2align 4,,10
	.p2align 3
.L133:
	movq	(%rbx), %r15
	movl	8(%rbx), %r14d
	movq	%r15, %rdi
	call	strlen@PLT
	movslq	%r14d, %rdx
	cmpq	%rax, %rdx
	jnb	.L132
	testb	%r13b, %r13b
	jne	.L155
.L132:
	movq	%rbp, 16(%r12)
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	movq	%r12, %rax
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L127:
	.cfi_restore_state
	xorl	%esi, %esi
	cmpb	$0, 12(%rbx)
	je	.L115
.L151:
	addl	$1, %r14d
	movl	%r14d, 8(%rbx)
	movslq	%r14d, %r14
	movsbl	0(%r13,%r14), %esi
	movb	%sil, 12(%rbx)
	jmp	.L115
	.p2align 4,,10
	.p2align 3
.L153:
	movq	(%rbx), %r13
.L128:
	movl	8(%rbx), %r14d
	movq	%r13, %rdi
	call	strlen@PLT
	movslq	%r14d, %rdx
	cmpq	%rax, %rdx
	jnb	.L128
	addl	$1, %r14d
	movl	%r14d, 8(%rbx)
	movslq	%r14d, %r14
	movsbl	0(%r13,%r14), %esi
	movb	%sil, 12(%rbx)
	cmpb	$92, %sil
	je	.L128
	cmpb	$114, %sil
	je	.L118
	jg	.L119
	cmpb	$34, %sil
	je	.L120
	cmpb	$110, %sil
	jne	.L122
	movl	$10, %esi
	movq	%rbp, %rdi
	call	string_append@PLT
.L124:
	movq	(%rbx), %r13
	movl	8(%rbx), %r14d
	movq	%r13, %rdi
	call	strlen@PLT
	movslq	%r14d, %rdx
	movsbl	12(%rbx), %esi
	cmpq	%rax, %rdx
	jnb	.L115
	testb	%sil, %sil
	je	.L115
	jmp	.L151
	.p2align 4,,10
	.p2align 3
.L119:
	cmpb	$116, %sil
	jne	.L122
	movl	$9, %esi
	movq	%rbp, %rdi
	call	string_append@PLT
	jmp	.L124
	.p2align 4,,10
	.p2align 3
.L118:
	movl	$13, %esi
	movq	%rbp, %rdi
	call	string_append@PLT
	jmp	.L124
	.p2align 4,,10
	.p2align 3
.L122:
	movq	%rbp, %rdi
	call	string_append@PLT
	jmp	.L124
	.p2align 4,,10
	.p2align 3
.L120:
	movl	$34, %esi
	movq	%rbp, %rdi
	call	string_append@PLT
	jmp	.L124
	.p2align 4,,10
	.p2align 3
.L155:
	addl	$1, %r14d
	movl	%r14d, 8(%rbx)
	movslq	%r14d, %r14
	movzbl	(%r15,%r14), %eax
	movb	%al, 12(%rbx)
	jmp	.L132
	.p2align 4,,10
	.p2align 3
.L152:
	addl	$1, %ebp
	movl	%ebp, 8(%rbx)
	movslq	%ebp, %rbp
	movzbl	0(%r13,%rbp), %eax
	movb	%al, 12(%rbx)
	jmp	.L114
	.cfi_endproc
.LFE40:
	.size	parse_string, .-parse_string
	.p2align 4
	.globl	parser_error
	.type	parser_error, @function
parser_error:
.LFB42:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movl	$1, %edi
	call	exit@PLT
	.cfi_endproc
.LFE42:
	.size	parser_error, .-parser_error
	.p2align 4
	.globl	parse_word
	.type	parse_word, @function
parse_word:
.LFB43:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	movl	$48, %esi
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	movq	%rdi, %rbp
	movl	$1, %edi
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$8, %rsp
	.cfi_def_cfa_offset 64
	call	calloc@PLT
	xorl	%edi, %edi
	movq	%rax, %r13
	call	init_string@PLT
	movzbl	12(%rbp), %ebx
	movq	%rax, %r12
	cmpb	$92, %bl
	je	.L159
	call	__ctype_b_loc@PLT
	movq	%rax, %r15
	movsbq	%bl, %rax
	movq	(%r15), %rdx
	testb	$32, 1(%rdx,%rax,2)
	je	.L181
	jmp	.L162
	.p2align 4,,10
	.p2align 3
.L182:
	movzbl	12(%rbp), %ebx
.L166:
	movq	(%r15), %rdx
	movsbq	%bl, %rax
	testb	$32, 1(%rdx,%rax,2)
	jne	.L162
.L181:
	testb	%bl, %bl
	je	.L162
.L167:
	movsbl	%bl, %esi
	movq	%r12, %rdi
	call	string_append@PLT
	movq	0(%rbp), %r14
	movl	8(%rbp), %ebx
	movq	%r14, %rdi
	call	strlen@PLT
	movslq	%ebx, %rdx
	cmpq	%rax, %rdx
	jnb	.L182
	cmpb	$0, 12(%rbp)
	jne	.L183
.L162:
	movq	%r12, 16(%r13)
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	movq	%r13, %rax
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L183:
	.cfi_restore_state
	addl	$1, %ebx
	movl	%ebx, 8(%rbp)
	movslq	%ebx, %rbx
	movzbl	(%r14,%rbx), %ebx
	movb	%bl, 12(%rbp)
	jmp	.L166
	.p2align 4,,10
	.p2align 3
.L159:
	movq	0(%rbp), %r15
	movl	8(%rbp), %r14d
	movb	$1, 40(%r13)
	movq	%r15, %rdi
	call	strlen@PLT
	movslq	%r14d, %rdx
	cmpq	%rax, %rdx
	jb	.L184
.L163:
	call	__ctype_b_loc@PLT
	movq	%rax, %r15
	movsbq	%bl, %rax
	movq	(%r15), %rdx
	testb	$32, 1(%rdx,%rax,2)
	jne	.L169
	testb	%bl, %bl
	jne	.L167
.L169:
	movl	$1, %edi
	call	exit@PLT
	.p2align 4,,10
	.p2align 3
.L184:
	addl	$1, %r14d
	movl	%r14d, 8(%rbp)
	movslq	%r14d, %r14
	movzbl	(%r15,%r14), %ebx
	movb	%bl, 12(%rbp)
	jmp	.L163
	.cfi_endproc
.LFE43:
	.size	parse_word, .-parse_word
	.p2align 4
	.globl	parse_num
	.type	parse_num, @function
parse_num:
.LFB44:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	xorl	%r15d, %r15d
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	movq	%rdi, %rbx
	xorl	%edi, %edi
	subq	$24, %rsp
	.cfi_def_cfa_offset 80
	call	init_string@PLT
	movq	%rax, %r12
	call	__ctype_b_loc@PLT
	movq	%rax, %r13
	.p2align 4,,10
	.p2align 3
.L186:
	movsbl	12(%rbx), %esi
	movq	0(%r13), %rax
	movsbq	%sil, %rcx
	testb	$8, 1(%rax,%rcx,2)
	jne	.L189
	cmpb	$46, %sil
	jne	.L190
	testb	%r15b, %r15b
	jne	.L191
	movl	$1, %r15d
.L187:
	movq	%r12, %rdi
	call	string_append@PLT
	movq	(%rbx), %r14
	movl	8(%rbx), %ebp
	movq	%r14, %rdi
	call	strlen@PLT
	movslq	%ebp, %rcx
	cmpq	%rax, %rcx
	jnb	.L186
	cmpb	$0, 12(%rbx)
	je	.L186
	addl	$1, %ebp
	movl	%ebp, 8(%rbx)
	movslq	%ebp, %rbp
	movzbl	(%r14,%rbp), %eax
	movb	%al, 12(%rbx)
	jmp	.L186
	.p2align 4,,10
	.p2align 3
.L189:
	cmpb	$46, %sil
	movl	$1, %eax
	cmove	%eax, %r15d
	jmp	.L187
	.p2align 4,,10
	.p2align 3
.L190:
	testb	%r15b, %r15b
	jne	.L191
	movl	$48, %esi
	movl	$1, %edi
	call	calloc@PLT
	movl	$1, (%rax)
	movq	%rax, %rbx
	movb	$0, 40(%rax)
.L193:
	movq	16(%r12), %rdi
	xorl	%esi, %esi
	call	strtod@PLT
	movq	%r12, %rdi
	movsd	%xmm0, 8(%rsp)
	fldl	8(%rsp)
	fstpt	16(%rbx)
	call	string_free@PLT
	addq	$24, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	movq	%rbx, %rax
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L191:
	.cfi_restore_state
	movl	$48, %esi
	movl	$1, %edi
	call	calloc@PLT
	movl	$2, (%rax)
	movq	%rax, %rbx
	movb	$0, 40(%rax)
	jmp	.L193
	.cfi_endproc
.LFE44:
	.size	parse_num, .-parse_num
	.p2align 4
	.globl	parser_get_next
	.type	parser_get_next, @function
parser_get_next:
.LFB45:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	movq	%rdi, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$8, %rsp
	.cfi_def_cfa_offset 64
	call	__ctype_b_loc@PLT
	movq	(%rax), %r13
	movsbq	12(%rbp), %rax
	movq	%rax, %r14
	movzwl	0(%r13,%rax,2), %eax
	testb	$32, %ah
	je	.L198
	movq	0(%rbp), %r12
.L200:
	movslq	8(%rbp), %r15
	movq	%r15, %rbx
.L199:
	movq	%r12, %rdi
	call	strlen@PLT
.L204:
	cmpq	%rax, %r15
	jnb	.L204
	testb	%r14b, %r14b
	je	.L199
	addl	$1, %ebx
	movl	%ebx, 8(%rbp)
	movslq	%ebx, %rbx
	movsbq	(%r12,%rbx), %rax
	movb	%al, 12(%rbp)
	movq	%rax, %r14
	movzwl	0(%r13,%rax,2), %eax
	testb	$32, %ah
	jne	.L200
.L198:
	testb	$8, %ah
	jne	.L215
	cmpb	$34, %r14b
	je	.L205
	cmpb	$91, %r14b
	je	.L206
	testb	%r14b, %r14b
	je	.L207
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	movq	%rbp, %rdi
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	jmp	parse_word
	.p2align 4,,10
	.p2align 3
.L215:
	.cfi_restore_state
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	movq	%rbp, %rdi
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	jmp	parse_num
	.p2align 4,,10
	.p2align 3
.L207:
	.cfi_restore_state
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L206:
	.cfi_restore_state
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	movq	%rbp, %rdi
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	jmp	parse_quote
	.p2align 4,,10
	.p2align 3
.L205:
	.cfi_restore_state
	addq	$8, %rsp
	.cfi_def_cfa_offset 56
	movq	%rbp, %rdi
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	jmp	parse_string
	.cfi_endproc
.LFE45:
	.size	parser_get_next, .-parser_get_next
	.p2align 4
	.globl	parse_quote
	.type	parse_quote, @function
parse_quote:
.LFB41:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	movl	$48, %esi
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	movq	%rdi, %rbp
	movl	$1, %edi
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$24, %rsp
	.cfi_def_cfa_offset 80
	call	calloc@PLT
	movl	$24, %esi
	movl	$1, %edi
	movl	$4, (%rax)
	movq	%rax, %r14
	movb	$0, 40(%rax)
	movq	%rax, (%rsp)
	call	calloc@PLT
	movl	$8, %esi
	movl	$10, %edi
	movq	$10, 16(%rax)
	movq	%rax, %rbx
	call	calloc@PLT
	movq	%rbx, 16(%r14)
	movq	0(%rbp), %r12
	movq	%rax, (%rbx)
	movl	8(%rbp), %ebx
	movq	%r12, %rdi
	call	strlen@PLT
	movslq	%ebx, %rdx
	movzbl	12(%rbp), %r13d
	cmpq	%rax, %rdx
	jnb	.L218
	testb	%r13b, %r13b
	je	.L219
	addl	$1, %ebx
	movl	%ebx, 8(%rbp)
	movslq	%ebx, %rbx
	movzbl	(%r12,%rbx), %r13d
	movb	%r13b, 12(%rbp)
.L218:
	call	__ctype_b_loc@PLT
	movq	(%rax), %r15
	movq	%rax, 8(%rsp)
	movsbq	%r13b, %rax
	testb	$32, 1(%r15,%rax,2)
	je	.L221
	movl	8(%rbp), %ebx
.L223:
	movslq	%ebx, %r14
.L222:
	movq	%r12, %rdi
	call	strlen@PLT
.L227:
	cmpq	%rax, %r14
	jnb	.L227
	testb	%r13b, %r13b
	je	.L222
	addl	$1, %ebx
	movslq	%ebx, %rax
	movl	%ebx, 8(%rbp)
	movsbq	(%r12,%rax), %rax
	movb	%al, 12(%rbp)
	movq	%rax, %r13
	testb	$32, 1(%r15,%rax,2)
	jne	.L223
.L221:
	cmpb	$93, %r13b
	je	.L225
	.p2align 4,,10
	.p2align 3
.L234:
	movq	%rbp, %rdi
	call	parser_get_next
	movq	%rax, %r12
	movq	(%rsp), %rax
	movq	16(%rax), %rbx
	movq	16(%rbx), %rsi
	movq	8(%rbx), %rdx
	leaq	-2(%rsi), %rax
	cmpq	%rax, %rdx
	jnb	.L228
	movq	(%rbx), %rax
.L229:
	movq	%r12, (%rax,%rdx,8)
	movq	8(%rsp), %rax
	addq	$1, %rdx
	movq	%rdx, 8(%rbx)
	movq	(%rax), %r13
	movsbq	12(%rbp), %rax
	movq	%rax, %r15
	testb	$32, 1(%r13,%rax,2)
	je	.L231
	movq	0(%rbp), %r12
.L233:
	movslq	8(%rbp), %r14
	movq	%r14, %rbx
.L232:
	movq	%r12, %rdi
	call	strlen@PLT
.L237:
	cmpq	%rax, %r14
	jnb	.L237
	testb	%r15b, %r15b
	je	.L232
	addl	$1, %ebx
	movl	%ebx, 8(%rbp)
	movslq	%ebx, %rbx
	movsbq	(%r12,%rbx), %rax
	movb	%al, 12(%rbp)
	movq	%rax, %r15
	testb	$32, 1(%r13,%rax,2)
	jne	.L233
.L231:
	cmpb	$93, %r15b
	jne	.L234
	movq	0(%rbp), %r12
.L225:
	movl	8(%rbp), %ebx
	movq	%r12, %rdi
	call	strlen@PLT
	movslq	%ebx, %rdx
	cmpq	%rax, %rdx
	jnb	.L216
	addl	$1, %ebx
	movl	%ebx, 8(%rbp)
	movslq	%ebx, %rbx
	movzbl	(%r12,%rbx), %eax
	movb	%al, 12(%rbp)
.L216:
	movq	(%rsp), %rax
	addq	$24, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L228:
	.cfi_restore_state
	leaq	(%rsi,%rsi), %rax
	movq	(%rbx), %rdi
	salq	$4, %rsi
	movq	%rax, 16(%rbx)
	call	realloc@PLT
	movq	8(%rbx), %rdx
	movq	%rax, (%rbx)
	jmp	.L229
.L219:
	call	__ctype_b_loc@PLT
	movq	(%rax), %r15
	movq	%rax, 8(%rsp)
	testw	$8192, (%r15)
	jne	.L223
	jmp	.L234
	.cfi_endproc
.LFE41:
	.size	parse_quote, .-parse_quote
	.p2align 4
	.globl	init_node
	.type	init_node, @function
init_node:
.LFB46:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rdi, %rbp
	movl	$1, %edi
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rsi, %rbx
	movl	$24, %esi
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	call	calloc@PLT
	movq	%rbp, (%rax)
	movq	%rbx, 8(%rax)
	movq	$0, 16(%rax)
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE46:
	.size	init_node, .-init_node
	.p2align 4
	.globl	node_free
	.type	node_free, @function
node_free:
.LFB47:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsi, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rdi, %rbx
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movq	(%rdi), %rdi
	call	string_free@PLT
	movq	8(%rbx), %rdi
	call	*%rbp
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	movq	%rbx, %rdi
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	free@PLT
	.cfi_endproc
.LFE47:
	.size	node_free, .-node_free
	.p2align 4
	.globl	init_sll
	.type	init_sll, @function
init_sll:
.LFB48:
	.cfi_startproc
	movl	$16, %esi
	movl	$1, %edi
	jmp	calloc@PLT
	.cfi_endproc
.LFE48:
	.size	init_sll, .-init_sll
	.p2align 4
	.globl	sll_add
	.type	sll_add, @function
sll_add:
.LFB49:
	.cfi_startproc
	pushq	%r14
	.cfi_def_cfa_offset 16
	.cfi_offset 14, -16
	pushq	%r13
	.cfi_def_cfa_offset 24
	.cfi_offset 13, -24
	movq	%rdx, %r13
	pushq	%r12
	.cfi_def_cfa_offset 32
	.cfi_offset 12, -32
	movq	%rsi, %r12
	pushq	%rbp
	.cfi_def_cfa_offset 40
	.cfi_offset 6, -40
	pushq	%rbx
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
	movq	(%rdi), %rbx
	testq	%rbx, %rbx
	je	.L263
	movq	16(%rsi), %rbp
	jmp	.L257
	.p2align 4,,10
	.p2align 3
.L258:
	testl	%eax, %eax
	je	.L262
.L257:
	movq	%rbx, %r14
	movq	16(%rbx), %rbx
	movq	%rbp, %rdi
	movq	(%r14), %rax
	movq	16(%rax), %rsi
	call	strcmp@PLT
	testq	%rbx, %rbx
	jne	.L258
	testl	%eax, %eax
	je	.L262
	movl	$24, %esi
	movl	$1, %edi
	call	calloc@PLT
	movq	%r12, (%rax)
	movq	%r13, 8(%rax)
	movq	$0, 16(%rax)
	movq	%rax, 16(%r14)
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 40
	popq	%rbp
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r13
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L262:
	.cfi_restore_state
	movq	8(%r14), %rdi
	call	value_free
	movq	%r12, %rdi
	call	string_free@PLT
	movq	%r13, 8(%r14)
.L254:
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 40
	popq	%rbp
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r13
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L263:
	.cfi_restore_state
	movq	%rdi, %rbp
	movl	$24, %esi
	movl	$1, %edi
	call	calloc@PLT
	addq	$1, 8(%rbp)
	movq	%r12, (%rax)
	movq	%r13, 8(%rax)
	movq	$0, 16(%rax)
	movq	%rax, 0(%rbp)
	jmp	.L254
	.cfi_endproc
.LFE49:
	.size	sll_add, .-sll_add
	.p2align 4
	.globl	sll_add_func
	.type	sll_add_func, @function
sll_add_func:
.LFB50:
	.cfi_startproc
	pushq	%r14
	.cfi_def_cfa_offset 16
	.cfi_offset 14, -16
	pushq	%r13
	.cfi_def_cfa_offset 24
	.cfi_offset 13, -24
	movq	%rdx, %r13
	pushq	%r12
	.cfi_def_cfa_offset 32
	.cfi_offset 12, -32
	movq	%rsi, %r12
	pushq	%rbp
	.cfi_def_cfa_offset 40
	.cfi_offset 6, -40
	pushq	%rbx
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
	movq	(%rdi), %rbx
	testq	%rbx, %rbx
	je	.L273
	movq	16(%rsi), %rbp
	jmp	.L267
	.p2align 4,,10
	.p2align 3
.L268:
	testl	%eax, %eax
	je	.L272
.L267:
	movq	%rbx, %r14
	movq	16(%rbx), %rbx
	movq	%rbp, %rdi
	movq	(%r14), %rax
	movq	16(%rax), %rsi
	call	strcmp@PLT
	testq	%rbx, %rbx
	jne	.L268
	testl	%eax, %eax
	je	.L272
	movl	$24, %esi
	movl	$1, %edi
	call	calloc@PLT
	movq	%r12, (%rax)
	movq	%r13, 8(%rax)
	movq	$0, 16(%rax)
	movq	%rax, 16(%r14)
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 40
	popq	%rbp
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r13
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L272:
	.cfi_restore_state
	movq	%r12, %rdi
	call	string_free@PLT
	movq	%r13, 8(%r14)
.L264:
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 40
	popq	%rbp
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r13
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L273:
	.cfi_restore_state
	movq	%rdi, %rbp
	movl	$24, %esi
	movl	$1, %edi
	call	calloc@PLT
	addq	$1, 8(%rbp)
	movq	%r12, (%rax)
	movq	%r13, 8(%rax)
	movq	$0, 16(%rax)
	movq	%rax, 0(%rbp)
	jmp	.L264
	.cfi_endproc
.LFE50:
	.size	sll_add_func, .-sll_add_func
	.p2align 4
	.globl	sll_get
	.type	sll_get, @function
sll_get:
.LFB51:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movq	(%rdi), %rbx
	testq	%rbx, %rbx
	je	.L274
	movq	16(%rsi), %rbp
	jmp	.L277
	.p2align 4,,10
	.p2align 3
.L276:
	movq	16(%rbx), %rbx
	testq	%rbx, %rbx
	je	.L274
.L277:
	movq	(%rbx), %rax
	movq	%rbp, %rdi
	movq	16(%rax), %rsi
	call	strcmp@PLT
	testl	%eax, %eax
	jne	.L276
	movq	8(%rbx), %rbx
.L274:
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	movq	%rbx, %rax
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE51:
	.size	sll_get, .-sll_get
	.p2align 4
	.globl	sll_free
	.type	sll_free, @function
sll_free:
.LFB52:
	.cfi_startproc
	pushq	%r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	movq	%rdi, %r13
	pushq	%r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	pushq	%rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	pushq	%rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	subq	$8, %rsp
	.cfi_def_cfa_offset 48
	movq	(%rdi), %rbx
	testq	%rbx, %rbx
	je	.L284
	movq	%rsi, %r12
	.p2align 4,,10
	.p2align 3
.L285:
	movq	%rbx, %rbp
	movq	16(%rbx), %rbx
	movq	0(%rbp), %rdi
	call	string_free@PLT
	movq	8(%rbp), %rdi
	call	*%r12
	movq	%rbp, %rdi
	call	free@PLT
	testq	%rbx, %rbx
	jne	.L285
.L284:
	addq	$8, %rsp
	.cfi_def_cfa_offset 40
	movq	%r13, %rdi
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%rbp
	.cfi_def_cfa_offset 24
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	free@PLT
	.cfi_endproc
.LFE52:
	.size	sll_free, .-sll_free
	.p2align 4
	.globl	init_ht
	.type	init_ht, @function
init_ht:
.LFB53:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	movl	$16, %esi
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	movq	%rdi, %rbp
	movl	$1, %edi
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	call	calloc@PLT
	movl	$8, %esi
	movq	%rbp, %rdi
	movq	%rbp, 8(%rax)
	movq	%rax, %r12
	call	calloc@PLT
	movq	%rax, (%r12)
	testq	%rbp, %rbp
	je	.L291
	movq	%rax, %rbx
	leaq	(%rax,%rbp,8), %rbp
	.p2align 4,,10
	.p2align 3
.L293:
	movl	$16, %esi
	movl	$1, %edi
	addq	$8, %rbx
	call	calloc@PLT
	movq	%rax, -8(%rbx)
	cmpq	%rbp, %rbx
	jne	.L293
.L291:
	movq	%r12, %rax
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE53:
	.size	init_ht, .-init_ht
	.p2align 4
	.globl	ht_add
	.type	ht_add, @function
ht_add:
.LFB54:
	.cfi_startproc
	movq	16(%rsi), %rax
	movq	%rdx, %r9
	movq	(%rdi), %r10
	movq	%rsi, %r8
	movsbq	(%rax), %rdx
	leaq	1(%rax), %rcx
	movl	$5381, %eax
	testb	%dl, %dl
	je	.L300
	.p2align 4,,10
	.p2align 3
.L301:
	movq	%rax, %rsi
	addq	$1, %rcx
	salq	$5, %rsi
	addq	%rsi, %rax
	addq	%rdx, %rax
	movsbq	-1(%rcx), %rdx
	testb	%dl, %dl
	jne	.L301
.L300:
	xorl	%edx, %edx
	movq	%r8, %rsi
	divq	8(%rdi)
	movq	(%r10,%rdx,8), %rdi
	movq	%r9, %rdx
	jmp	sll_add
	.cfi_endproc
.LFE54:
	.size	ht_add, .-ht_add
	.p2align 4
	.globl	ht_add_func
	.type	ht_add_func, @function
ht_add_func:
.LFB55:
	.cfi_startproc
	movq	16(%rsi), %rax
	movq	%rdx, %r9
	movq	(%rdi), %r10
	movq	%rsi, %r8
	movsbq	(%rax), %rdx
	leaq	1(%rax), %rcx
	movl	$5381, %eax
	testb	%dl, %dl
	je	.L305
	.p2align 4,,10
	.p2align 3
.L306:
	movq	%rax, %rsi
	addq	$1, %rcx
	salq	$5, %rsi
	addq	%rsi, %rax
	addq	%rdx, %rax
	movsbq	-1(%rcx), %rdx
	testb	%dl, %dl
	jne	.L306
.L305:
	xorl	%edx, %edx
	movq	%r8, %rsi
	divq	8(%rdi)
	movq	(%r10,%rdx,8), %rdi
	movq	%r9, %rdx
	jmp	sll_add_func
	.cfi_endproc
.LFE55:
	.size	ht_add_func, .-ht_add_func
	.p2align 4
	.globl	ht_get
	.type	ht_get, @function
ht_get:
.LFB56:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movq	16(%rsi), %rbx
	movq	(%rdi), %r8
	movsbq	(%rbx), %rdx
	testb	%dl, %dl
	je	.L315
	leaq	1(%rbx), %rcx
	movl	$5381, %eax
	.p2align 4,,10
	.p2align 3
.L311:
	movq	%rax, %rsi
	addq	$1, %rcx
	salq	$5, %rsi
	addq	%rsi, %rax
	addq	%rdx, %rax
	movsbq	-1(%rcx), %rdx
	testb	%dl, %dl
	jne	.L311
.L310:
	xorl	%edx, %edx
	divq	8(%rdi)
	movq	(%r8,%rdx,8), %rax
	movq	(%rax), %rbp
	testq	%rbp, %rbp
	jne	.L314
	jmp	.L309
	.p2align 4,,10
	.p2align 3
.L313:
	movq	16(%rbp), %rbp
	testq	%rbp, %rbp
	je	.L309
.L314:
	movq	0(%rbp), %rax
	movq	%rbx, %rdi
	movq	16(%rax), %rsi
	call	strcmp@PLT
	testl	%eax, %eax
	jne	.L313
	movq	8(%rbp), %rbp
.L309:
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%rbp, %rax
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L315:
	.cfi_restore_state
	movl	$5381, %eax
	jmp	.L310
	.cfi_endproc
.LFE56:
	.size	ht_get, .-ht_get
	.p2align 4
	.globl	ht_exists
	.type	ht_exists, @function
ht_exists:
.LFB57:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movq	16(%rsi), %rbx
	movq	(%rdi), %r8
	movsbq	(%rbx), %rdx
	testb	%dl, %dl
	je	.L328
	leaq	1(%rbx), %rcx
	movl	$5381, %eax
	.p2align 4,,10
	.p2align 3
.L324:
	movq	%rax, %rsi
	addq	$1, %rcx
	salq	$5, %rsi
	addq	%rsi, %rax
	addq	%rdx, %rax
	movsbq	-1(%rcx), %rdx
	testb	%dl, %dl
	jne	.L324
.L323:
	xorl	%edx, %edx
	divq	8(%rdi)
	movq	(%r8,%rdx,8), %rax
	movq	(%rax), %rbp
	testq	%rbp, %rbp
	jne	.L327
	jmp	.L329
	.p2align 4,,10
	.p2align 3
.L326:
	movq	16(%rbp), %rbp
	testq	%rbp, %rbp
	je	.L329
.L327:
	movq	0(%rbp), %rax
	movq	%rbx, %rdi
	movq	16(%rax), %rsi
	call	strcmp@PLT
	testl	%eax, %eax
	jne	.L326
	cmpq	$0, 8(%rbp)
	setne	%al
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L329:
	.cfi_restore_state
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L328:
	.cfi_restore_state
	movl	$5381, %eax
	jmp	.L323
	.cfi_endproc
.LFE57:
	.size	ht_exists, .-ht_exists
	.p2align 4
	.globl	ht_free
	.type	ht_free, @function
ht_free:
.LFB58:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	movq	%rdi, %r15
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$8, %rsp
	.cfi_def_cfa_offset 64
	cmpq	$0, 8(%rdi)
	je	.L334
	movq	%rsi, %r12
	xorl	%r13d, %r13d
	.p2align 4,,10
	.p2align 3
.L337:
	movq	(%r15), %rax
	movq	(%rax,%r13,8), %r14
	movq	(%r14), %rbx
	testq	%rbx, %rbx
	je	.L335
	.p2align 4,,10
	.p2align 3
.L336:
	movq	%rbx, %rbp
	movq	16(%rbx), %rbx
	movq	0(%rbp), %rdi
	call	string_free@PLT
	movq	8(%rbp), %rdi
	call	*%r12
	movq	%rbp, %rdi
	call	free@PLT
	testq	%rbx, %rbx
	jne	.L336
.L335:
	movq	%r14, %rdi
	addq	$1, %r13
	call	free@PLT
	cmpq	8(%r15), %r13
	jb	.L337
.L334:
	movq	(%r15), %rdi
	call	free@PLT
	addq	$8, %rsp
	.cfi_def_cfa_offset 56
	movq	%r15, %rdi
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	jmp	free@PLT
	.cfi_endproc
.LFE58:
	.size	ht_free, .-ht_free
	.p2align 4
	.globl	hash
	.type	hash, @function
hash:
.LFB59:
	.cfi_startproc
	movsbq	(%rsi), %rdx
	leaq	1(%rsi), %rcx
	movl	$5381, %eax
	testb	%dl, %dl
	je	.L345
	.p2align 4,,10
	.p2align 3
.L346:
	movq	%rax, %rsi
	addq	$1, %rcx
	salq	$5, %rsi
	addq	%rsi, %rax
	addq	%rdx, %rax
	movsbq	-1(%rcx), %rdx
	testb	%dl, %dl
	jne	.L346
.L345:
	xorl	%edx, %edx
	divq	8(%rdi)
	movq	%rdx, %rax
	ret
	.cfi_endproc
.LFE59:
	.size	hash, .-hash
	.p2align 4
	.globl	eval_builtins
	.type	eval_builtins, @function
eval_builtins:
.LFB61:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	movq	%rdi, %rbp
	movq	FLIT(%rip), %rdi
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	movq	16(%rbp), %rax
	movq	(%rdi), %r8
	movq	16(%rax), %rbx
	movsbq	(%rbx), %rdx
	testb	%dl, %dl
	je	.L358
	leaq	1(%rbx), %rcx
	movl	$5381, %eax
	.p2align 4,,10
	.p2align 3
.L351:
	movq	%rax, %rsi
	addq	$1, %rcx
	salq	$5, %rsi
	addq	%rsi, %rax
	addq	%rdx, %rax
	movsbq	-1(%rcx), %rdx
	testb	%dl, %dl
	jne	.L351
.L350:
	xorl	%edx, %edx
	divq	8(%rdi)
	movq	(%r8,%rdx,8), %rax
	movq	(%rax), %r12
	testq	%r12, %r12
	jne	.L355
	jmp	.L360
	.p2align 4,,10
	.p2align 3
.L353:
	movq	16(%r12), %r12
	testq	%r12, %r12
	je	.L360
.L355:
	movq	(%r12), %rax
	movq	%rbx, %rdi
	movq	16(%rax), %rsi
	call	strcmp@PLT
	testl	%eax, %eax
	jne	.L353
	movq	8(%r12), %r12
	testq	%r12, %r12
	je	.L360
	movq	EVAL_STACK(%rip), %rbx
	movq	16(%rbx), %rsi
	movq	8(%rbx), %rdx
	movq	(%rbx), %rdi
	leaq	-2(%rsi), %rax
	cmpq	%rax, %rdx
	jnb	.L368
.L356:
	movq	%rbp, (%rdi,%rdx,8)
	addq	$1, %rdx
	movq	%rbp, %rdi
	movq	%rdx, 8(%rbx)
	call	*%r12
	movq	EVAL_STACK(%rip), %rdx
	movq	8(%rdx), %rax
	testq	%rax, %rax
	je	.L357
	subq	$1, %rax
	movq	%rax, 8(%rdx)
.L357:
	movq	%rbp, %rdi
	call	value_free
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movl	$1, %eax
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L360:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	xorl	%eax, %eax
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L368:
	.cfi_restore_state
	leaq	(%rsi,%rsi), %rax
	salq	$4, %rsi
	movq	%rax, 16(%rbx)
	call	realloc@PLT
	movq	8(%rbx), %rdx
	movq	%rax, (%rbx)
	movq	%rax, %rdi
	jmp	.L356
	.p2align 4,,10
	.p2align 3
.L358:
	movl	$5381, %eax
	jmp	.L350
	.cfi_endproc
.LFE61:
	.size	eval_builtins, .-eval_builtins
	.p2align 4
	.globl	eval
	.type	eval, @function
eval:
.LFB62:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	movq	%rdi, %r12
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	movl	(%rdi), %eax
	testl	%eax, %eax
	je	.L370
	subl	$1, %eax
	cmpl	$5, %eax
	ja	.L369
.L400:
	movq	STACK(%rip), %rbx
	movq	16(%rbx), %rsi
	movq	8(%rbx), %rdx
	movq	(%rbx), %rdi
	leaq	-2(%rsi), %rax
	cmpq	%rax, %rdx
	jnb	.L402
.L383:
	movq	%r12, (%rdi,%rdx,8)
	addq	$1, %rdx
	movq	%rdx, 8(%rbx)
.L369:
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L370:
	.cfi_restore_state
	cmpb	$0, 40(%rdi)
	jne	.L403
	movq	16(%r12), %rax
	movq	FLIT(%rip), %rdi
	movq	16(%rax), %rbx
	movq	(%rdi), %r8
	movl	$5381, %eax
	movsbq	(%rbx), %rdx
	leaq	1(%rbx), %rcx
	testb	%dl, %dl
	je	.L375
	.p2align 4,,10
	.p2align 3
.L376:
	movq	%rax, %rsi
	addq	$1, %rcx
	salq	$5, %rsi
	addq	%rsi, %rax
	addq	%rdx, %rax
	movsbq	-1(%rcx), %rdx
	testb	%dl, %dl
	jne	.L376
.L375:
	xorl	%edx, %edx
	divq	8(%rdi)
	movq	(%r8,%rdx,8), %rax
	movq	(%rax), %rbp
	testq	%rbp, %rbp
	jne	.L380
	jmp	.L377
	.p2align 4,,10
	.p2align 3
.L378:
	movq	16(%rbp), %rbp
	testq	%rbp, %rbp
	je	.L377
.L380:
	movq	0(%rbp), %rax
	movq	%rbx, %rdi
	movq	16(%rax), %rsi
	call	strcmp@PLT
	testl	%eax, %eax
	jne	.L378
	movq	8(%rbp), %rbp
	testq	%rbp, %rbp
	je	.L377
	movq	EVAL_STACK(%rip), %rbx
	movq	16(%rbx), %rsi
	movq	8(%rbx), %rdx
	movq	(%rbx), %rdi
	leaq	-2(%rsi), %rax
	cmpq	%rax, %rdx
	jnb	.L404
.L381:
	movq	%r12, (%rdi,%rdx,8)
	addq	$1, %rdx
	movq	%r12, %rdi
	movq	%rdx, 8(%rbx)
	call	*%rbp
	movq	EVAL_STACK(%rip), %rdx
	movq	8(%rdx), %rax
	testq	%rax, %rax
	je	.L382
	subq	$1, %rax
	movq	%rax, 8(%rdx)
.L382:
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r12, %rdi
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	jmp	value_free
	.p2align 4,,10
	.p2align 3
.L402:
	.cfi_restore_state
	leaq	(%rsi,%rsi), %rax
	salq	$4, %rsi
	movq	%rax, 16(%rbx)
	call	realloc@PLT
	movq	8(%rbx), %rdx
	movq	%rax, (%rbx)
	movq	%rax, %rdi
	jmp	.L383
	.p2align 4,,10
	.p2align 3
.L403:
	movb	$0, 40(%rdi)
	jmp	.L400
	.p2align 4,,10
	.p2align 3
.L377:
	movq	%r12, %rdi
	call	eval_ht
	testb	%al, %al
	jne	.L369
	jmp	.L400
	.p2align 4,,10
	.p2align 3
.L404:
	leaq	(%rsi,%rsi), %rax
	salq	$4, %rsi
	movq	%rax, 16(%rbx)
	call	realloc@PLT
	movq	8(%rbx), %rdx
	movq	%rax, (%rbx)
	movq	%rax, %rdi
	jmp	.L381
	.cfi_endproc
.LFE62:
	.size	eval, .-eval
	.p2align 4
	.globl	eval_ht
	.type	eval_ht, @function
eval_ht:
.LFB60:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	movq	%rdi, %rbp
	movq	WORD_TABLE(%rip), %rdi
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	movq	16(%rbp), %rax
	movq	(%rdi), %r8
	movq	16(%rax), %rbx
	movsbq	(%rbx), %rdx
	testb	%dl, %dl
	je	.L416
	leaq	1(%rbx), %rcx
	movl	$5381, %eax
	.p2align 4,,10
	.p2align 3
.L407:
	movq	%rax, %rsi
	addq	$1, %rcx
	salq	$5, %rsi
	addq	%rsi, %rax
	addq	%rdx, %rax
	movsbq	-1(%rcx), %rdx
	testb	%dl, %dl
	jne	.L407
.L406:
	xorl	%edx, %edx
	divq	8(%rdi)
	movq	(%r8,%rdx,8), %rax
	movq	(%rax), %r12
	testq	%r12, %r12
	jne	.L411
	jmp	.L418
	.p2align 4,,10
	.p2align 3
.L409:
	movq	16(%r12), %r12
	testq	%r12, %r12
	je	.L418
.L411:
	movq	(%r12), %rax
	movq	%rbx, %rdi
	movq	16(%rax), %rsi
	call	strcmp@PLT
	testl	%eax, %eax
	jne	.L409
	movq	8(%r12), %r12
	testq	%r12, %r12
	je	.L418
	movq	%rbp, %rdi
	call	value_free
	cmpl	$4, (%r12)
	jne	.L412
	movq	16(%r12), %rax
	xorl	%ebx, %ebx
	cmpq	$0, 8(%rax)
	je	.L415
	.p2align 4,,10
	.p2align 3
.L413:
	movq	(%rax), %rax
	movq	(%rax,%rbx,8), %rdi
	addq	$1, %rbx
	call	value_copy
	movq	%rax, %rdi
	call	eval
	movq	16(%r12), %rax
	cmpq	8(%rax), %rbx
	jb	.L413
	jmp	.L415
	.p2align 4,,10
	.p2align 3
.L418:
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	xorl	%eax, %eax
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L412:
	.cfi_restore_state
	movq	%r12, %rdi
	call	value_copy
	movq	%rax, %rdi
	call	eval
.L415:
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movl	$1, %eax
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L416:
	.cfi_restore_state
	movl	$5381, %eax
	jmp	.L406
	.cfi_endproc
.LFE60:
	.size	eval_ht, .-eval_ht
	.globl	OBJ_TABLE
	.bss
	.align 8
	.type	OBJ_TABLE, @object
	.size	OBJ_TABLE, 8
OBJ_TABLE:
	.zero	8
	.globl	FLIT
	.align 8
	.type	FLIT, @object
	.size	FLIT, 8
FLIT:
	.zero	8
	.globl	PARSER
	.align 8
	.type	PARSER, @object
	.size	PARSER, 8
PARSER:
	.zero	8
	.globl	INBUF
	.align 8
	.type	INBUF, @object
	.size	INBUF, 8
INBUF:
	.zero	8
	.globl	WORD_TABLE
	.align 8
	.type	WORD_TABLE, @object
	.size	WORD_TABLE, 8
WORD_TABLE:
	.zero	8
	.globl	EVAL_STACK
	.align 8
	.type	EVAL_STACK, @object
	.size	EVAL_STACK, 8
EVAL_STACK:
	.zero	8
	.globl	STACK
	.align 8
	.type	STACK, @object
	.size	STACK, 8
STACK:
	.zero	8
	.ident	"GCC: (GNU) 13.2.1 20230801"
	.section	.note.GNU-stack,"",@progbits
