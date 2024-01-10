	.file	"better_string.c"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"malloc in init_string"
.LC1:
	.string	"calloc in init_string"
	.text
	.p2align 4
	.globl	init_string
	.type	init_string, @function
init_string:
.LFB11:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rdi, %rbp
	movl	$24, %edi
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	call	malloc@PLT
	movq	%rax, %rbx
	testq	%rax, %rax
	je	.L12
.L2:
	testq	%rbp, %rbp
	je	.L3
	cmpb	$0, 0(%rbp)
	jne	.L4
.L3:
	movq	$10, 8(%rbx)
	movl	$1, %esi
	movl	$10, %edi
	call	calloc@PLT
	movq	$0, (%rbx)
	movq	%rax, 16(%rbx)
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%rbx, %rax
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L4:
	.cfi_restore_state
	movq	%rbp, %rdi
	call	strlen@PLT
	movl	$1, %esi
	leaq	(%rax,%rax), %rdi
	movq	%rax, (%rbx)
	movq	%rdi, 8(%rbx)
	call	calloc@PLT
	movq	%rax, 16(%rbx)
	movq	%rax, %rdi
	testq	%rax, %rax
	je	.L13
.L6:
	movq	%rbp, %rsi
	call	strcpy@PLT
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%rbx, %rax
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
.L12:
	.cfi_restore_state
	leaq	.LC0(%rip), %rdi
	call	die@PLT
	jmp	.L2
.L13:
	leaq	.LC1(%rip), %rdi
	call	die@PLT
	movq	16(%rbx), %rdi
	jmp	.L6
	.cfi_endproc
.LFE11:
	.size	init_string, .-init_string
	.p2align 4
	.globl	string_copy
	.type	string_copy, @function
string_copy:
.LFB12:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movq	16(%rdi), %rbp
	movl	$24, %edi
	call	malloc@PLT
	movq	%rax, %rbx
	testq	%rax, %rax
	je	.L24
.L15:
	testq	%rbp, %rbp
	je	.L16
	cmpb	$0, 0(%rbp)
	jne	.L17
.L16:
	movq	$10, 8(%rbx)
	movl	$1, %esi
	movl	$10, %edi
	call	calloc@PLT
	movq	$0, (%rbx)
	movq	%rax, 16(%rbx)
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%rbx, %rax
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L17:
	.cfi_restore_state
	movq	%rbp, %rdi
	call	strlen@PLT
	movq	%rbp, %rdi
	movq	%rax, (%rbx)
	call	strlen@PLT
	movl	$1, %esi
	leaq	(%rax,%rax), %rdi
	movq	%rdi, 8(%rbx)
	call	calloc@PLT
	movq	%rax, 16(%rbx)
	movq	%rax, %rdi
	testq	%rax, %rax
	je	.L25
.L19:
	movq	%rbp, %rsi
	call	strcpy@PLT
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%rbx, %rax
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
.L24:
	.cfi_restore_state
	leaq	.LC0(%rip), %rdi
	call	die@PLT
	jmp	.L15
.L25:
	leaq	.LC1(%rip), %rdi
	call	die@PLT
	movq	16(%rbx), %rdi
	jmp	.L19
	.cfi_endproc
.LFE12:
	.size	string_copy, .-string_copy
	.section	.rodata.str1.1
.LC2:
	.string	"realloc on realloc_zero"
	.text
	.p2align 4
	.globl	realloc_zero
	.type	realloc_zero, @function
realloc_zero:
.LFB13:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	movq	%rsi, %r12
	movq	%rdx, %rsi
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	movq	%rdx, %rbx
	call	realloc@PLT
	movq	%rax, %rbp
	testq	%rax, %rax
	je	.L30
	cmpq	%rbx, %r12
	jb	.L31
.L26:
	movq	%rbp, %rax
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
.L31:
	.cfi_restore_state
	subq	%r12, %rbx
	leaq	(%rax,%r12), %rdi
	xorl	%esi, %esi
	movq	%rbx, %rdx
	call	memset@PLT
	movq	%rbp, %rax
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
.L30:
	.cfi_restore_state
	leaq	.LC2(%rip), %rdi
	call	die@PLT
	jmp	.L26
	.cfi_endproc
.LFE13:
	.size	realloc_zero, .-realloc_zero
	.p2align 4
	.globl	string_concat
	.type	string_concat, @function
string_concat:
.LFB14:
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
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$40, %rsp
	.cfi_def_cfa_offset 96
	movq	%fs:40, %rax
	movq	%rax, 24(%rsp)
	xorl	%eax, %eax
	cmpq	$0, (%rsi)
	je	.L32
	movq	%rdi, %rbx
	movq	%rsi, %r15
	leaq	22(%rsp), %r13
	xorl	%ebp, %ebp
	jmp	.L33
	.p2align 4,,10
	.p2align 3
.L34:
	addq	$1, %rax
	movq	%r13, %rsi
	movq	%r12, %rdi
	addq	$1, %rbp
	movq	%rax, (%rbx)
	call	strcat@PLT
	cmpq	(%r15), %rbp
	jnb	.L32
.L33:
	movq	16(%r15), %rax
	movq	8(%rbx), %rdi
	movb	$0, 23(%rsp)
	movq	16(%rbx), %r12
	movzbl	(%rax,%rbp), %eax
	leaq	-1(%rdi), %rdx
	movb	%al, 22(%rsp)
	movq	(%rbx), %rax
	cmpq	%rdx, %rax
	jb	.L34
	leaq	(%rdi,%rdi), %rdx
	movslq	%edi, %r14
	movq	%r12, %rdi
	movq	%rdx, 8(%rbx)
	movq	%rdx, %rsi
	movq	%rdx, 8(%rsp)
	call	realloc@PLT
	movq	8(%rsp), %rdx
	testq	%rax, %rax
	movq	%rax, %r12
	je	.L43
	cmpq	%rdx, %r14
	jb	.L44
.L36:
	movq	%r12, 16(%rbx)
	movq	(%rbx), %rax
	jmp	.L34
	.p2align 4,,10
	.p2align 3
.L32:
	movq	24(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L45
	addq	$40, %rsp
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
.L44:
	.cfi_restore_state
	subq	%r14, %rdx
	leaq	(%rax,%r14), %rdi
	xorl	%esi, %esi
	call	memset@PLT
	jmp	.L36
.L43:
	leaq	.LC2(%rip), %rdi
	call	die@PLT
	jmp	.L36
.L45:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE14:
	.size	string_concat, .-string_concat
	.p2align 4
	.globl	string_append
	.type	string_append, @function
string_append:
.LFB15:
	.cfi_startproc
	pushq	%r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	pushq	%r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	pushq	%rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	pushq	%rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	movq	%rdi, %rbx
	subq	$24, %rsp
	.cfi_def_cfa_offset 64
	movq	8(%rdi), %rdi
	movq	16(%rbx), %rbp
	movq	%fs:40, %rax
	movq	%rax, 8(%rsp)
	xorl	%eax, %eax
	movq	(%rbx), %rax
	movb	%sil, 6(%rsp)
	leaq	-1(%rdi), %rdx
	movb	$0, 7(%rsp)
	cmpq	%rdx, %rax
	jnb	.L52
.L47:
	addq	$1, %rax
	leaq	6(%rsp), %rsi
	movq	%rbp, %rdi
	movq	%rax, (%rbx)
	call	strcat@PLT
	movq	8(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L53
	addq	$24, %rsp
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
.L52:
	.cfi_restore_state
	leaq	(%rdi,%rdi), %r13
	movslq	%edi, %r12
	movq	%rbp, %rdi
	movq	%r13, 8(%rbx)
	movq	%r13, %rsi
	call	realloc@PLT
	movq	%rax, %rbp
	testq	%rax, %rax
	je	.L54
	cmpq	%r13, %r12
	jb	.L55
.L49:
	movq	%rbp, 16(%rbx)
	movq	(%rbx), %rax
	jmp	.L47
	.p2align 4,,10
	.p2align 3
.L55:
	movq	%r13, %rdx
	leaq	(%rax,%r12), %rdi
	xorl	%esi, %esi
	subq	%r12, %rdx
	call	memset@PLT
	jmp	.L49
.L53:
	call	__stack_chk_fail@PLT
.L54:
	leaq	.LC2(%rip), %rdi
	call	die@PLT
	jmp	.L49
	.cfi_endproc
.LFE15:
	.size	string_append, .-string_append
	.p2align 4
	.globl	string_free
	.type	string_free, @function
string_free:
.LFB16:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	%rdi, %rbx
	movq	16(%rdi), %rdi
	call	free@PLT
	movq	%rbx, %rdi
	popq	%rbx
	.cfi_def_cfa_offset 8
	jmp	free@PLT
	.cfi_endproc
.LFE16:
	.size	string_free, .-string_free
	.ident	"GCC: (GNU) 13.2.1 20230801"
	.section	.note.GNU-stack,"",@progbits
