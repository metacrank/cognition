	.file	"builtins.c"
	.text
	.p2align 4
	.globl	nop
	.type	nop, @function
nop:
.LFB32:
	.cfi_startproc
	ret
	.cfi_endproc
.LFE32:
	.size	nop, .-nop
	.p2align 4
	.globl	ssize
	.type	ssize, @function
ssize:
.LFB71:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movl	$1, %edi
	call	init_value@PLT
	movq	STACK(%rip), %rdi
	movq	%rax, %rsi
	fildq	8(%rdi)
	cmpq	$0, 8(%rdi)
	jns	.L4
	fadds	.LC0(%rip)
.L4:
	fstpt	16(%rsi)
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	jmp	array_append@PLT
	.cfi_endproc
.LFE71:
	.size	ssize, .-ssize
	.p2align 4
	.globl	clear
	.type	clear, @function
clear:
.LFB58:
	.cfi_startproc
	movq	STACK(%rip), %rdi
	cmpq	$0, 8(%rdi)
	je	.L11
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	.p2align 4,,10
	.p2align 3
.L8:
	call	array_pop@PLT
	movq	%rax, %rdi
	call	value_free@PLT
	movq	STACK(%rip), %rdi
	cmpq	$0, 8(%rdi)
	jne	.L8
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L11:
	ret
	.cfi_endproc
.LFE58:
	.size	clear, .-clear
	.p2align 4
	.globl	stemexit
	.type	stemexit, @function
stemexit:
.LFB45:
	.cfi_startproc
	movq	value_free@GOTPCREL(%rip), %rsi
	movq	WORD_TABLE(%rip), %rdi
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	call	ht_free@PLT
	movq	func_free@GOTPCREL(%rip), %rbx
	movq	FLIT(%rip), %rdi
	movq	%rbx, %rsi
	call	ht_free@PLT
	movq	STACK(%rip), %rdi
	call	array_free@PLT
	movq	INBUF(%rip), %rdi
	call	free@PLT
	movq	PARSER(%rip), %rdi
	call	free@PLT
	movq	EVAL_STACK(%rip), %rdi
	call	array_free@PLT
	movq	OBJ_TABLE(%rip), %rdi
	movq	%rbx, %rsi
	call	ht_free@PLT
	xorl	%edi, %edi
	call	exit@PLT
	.cfi_endproc
.LFE45:
	.size	stemexit, .-stemexit
	.p2align 4
	.globl	qstack
	.type	qstack, @function
qstack:
.LFB72:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movl	$4, %edi
	call	init_value@PLT
	movq	STACK(%rip), %rdi
	movq	%rax, %rbx
	call	array_copy@PLT
	movq	STACK(%rip), %rdi
	movq	%rbx, %rsi
	movq	%rax, 16(%rbx)
	popq	%rbx
	.cfi_def_cfa_offset 8
	jmp	array_append@PLT
	.cfi_endproc
.LFE72:
	.size	qstack, .-qstack
	.p2align 4
	.globl	dsc
	.type	dsc, @function
dsc:
.LFB48:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movq	STACK(%rip), %rdi
	call	array_pop@PLT
	testq	%rax, %rax
	je	.L20
	movq	%rax, %rdi
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	jmp	value_free@PLT
	.p2align 4,,10
	.p2align 3
.L20:
	.cfi_restore_state
	movl	$5, %edi
	call	init_value@PLT
	movq	STACK(%rip), %rdi
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	movq	%rax, %rsi
	jmp	array_append@PLT
	.cfi_endproc
.LFE48:
	.size	dsc, .-dsc
	.p2align 4
	.globl	stemdup
	.type	stemdup, @function
stemdup:
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
	movq	STACK(%rip), %rdi
	call	array_pop@PLT
	testq	%rax, %rax
	je	.L25
	movq	%rax, %rdi
	movq	%rax, %rbx
	call	value_copy@PLT
	movq	STACK(%rip), %rdi
	movq	%rbx, %rsi
	movq	%rax, %rbp
	call	array_append@PLT
	movq	%rbp, %rsi
.L24:
	movq	STACK(%rip), %rdi
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	array_append@PLT
	.p2align 4,,10
	.p2align 3
.L25:
	.cfi_restore_state
	movl	$5, %edi
	call	init_value@PLT
	movq	%rax, %rsi
	jmp	.L24
	.cfi_endproc
.LFE51:
	.size	stemdup, .-stemdup
	.p2align 4
	.globl	stemceil
	.type	stemceil, @function
stemceil:
.LFB39:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	subq	$16, %rsp
	.cfi_def_cfa_offset 32
	movq	STACK(%rip), %rdi
	call	array_pop@PLT
	testq	%rax, %rax
	je	.L30
	movq	%rax, %rbx
	movl	$2, %edi
	call	init_value@PLT
	fldt	16(%rbx)
	movsd	.LC5(%rip), %xmm2
	movsd	.LC2(%rip), %xmm3
	movq	%rax, %rsi
	fstpl	8(%rsp)
	movsd	8(%rsp), %xmm0
	movapd	%xmm0, %xmm1
	andpd	%xmm2, %xmm1
	ucomisd	%xmm1, %xmm3
	ja	.L31
.L28:
	movsd	%xmm0, 8(%rsp)
	fldl	8(%rsp)
	movq	STACK(%rip), %rdi
	fstpt	16(%rsi)
	call	array_append@PLT
	addq	$16, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 16
	movq	%rbx, %rdi
	popq	%rbx
	.cfi_def_cfa_offset 8
	jmp	value_free@PLT
	.p2align 4,,10
	.p2align 3
.L31:
	.cfi_restore_state
	cvttsd2siq	%xmm0, %rax
	pxor	%xmm1, %xmm1
	movapd	%xmm0, %xmm3
	movsd	.LC4(%rip), %xmm4
	andnpd	%xmm0, %xmm2
	cvtsi2sdq	%rax, %xmm1
	cmpnlesd	%xmm1, %xmm3
	andpd	%xmm4, %xmm3
	addsd	%xmm3, %xmm1
	movapd	%xmm1, %xmm0
	orpd	%xmm2, %xmm0
	jmp	.L28
	.p2align 4,,10
	.p2align 3
.L30:
	movl	$5, %edi
	call	init_value@PLT
	movq	STACK(%rip), %rdi
	addq	$16, %rsp
	.cfi_def_cfa_offset 16
	movq	%rax, %rsi
	popq	%rbx
	.cfi_def_cfa_offset 8
	jmp	array_append@PLT
	.cfi_endproc
.LFE39:
	.size	stemceil, .-stemceil
	.p2align 4
	.globl	stemfloor
	.type	stemfloor, @function
stemfloor:
.LFB40:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	subq	$16, %rsp
	.cfi_def_cfa_offset 32
	movq	STACK(%rip), %rdi
	call	array_pop@PLT
	testq	%rax, %rax
	je	.L36
	movq	%rax, %rbx
	movl	$2, %edi
	call	init_value@PLT
	fldt	16(%rbx)
	movsd	.LC5(%rip), %xmm2
	movsd	.LC2(%rip), %xmm3
	movq	%rax, %rsi
	fstpl	8(%rsp)
	movsd	8(%rsp), %xmm0
	movapd	%xmm0, %xmm1
	andpd	%xmm2, %xmm1
	ucomisd	%xmm1, %xmm3
	ja	.L37
.L34:
	movsd	%xmm0, 8(%rsp)
	fldl	8(%rsp)
	movq	STACK(%rip), %rdi
	fstpt	16(%rsi)
	call	array_append@PLT
	addq	$16, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 16
	movq	%rbx, %rdi
	popq	%rbx
	.cfi_def_cfa_offset 8
	jmp	value_free@PLT
	.p2align 4,,10
	.p2align 3
.L37:
	.cfi_restore_state
	cvttsd2siq	%xmm0, %rax
	pxor	%xmm1, %xmm1
	movsd	.LC4(%rip), %xmm4
	andnpd	%xmm0, %xmm2
	cvtsi2sdq	%rax, %xmm1
	movapd	%xmm1, %xmm3
	cmpnlesd	%xmm0, %xmm3
	andpd	%xmm4, %xmm3
	subsd	%xmm3, %xmm1
	movapd	%xmm1, %xmm0
	orpd	%xmm2, %xmm0
	jmp	.L34
	.p2align 4,,10
	.p2align 3
.L36:
	movl	$5, %edi
	call	init_value@PLT
	movq	STACK(%rip), %rdi
	addq	$16, %rsp
	.cfi_def_cfa_offset 16
	movq	%rax, %rsi
	popq	%rbx
	.cfi_def_cfa_offset 8
	jmp	array_append@PLT
	.cfi_endproc
.LFE40:
	.size	stemfloor, .-stemfloor
	.p2align 4
	.globl	stemln
	.type	stemln, @function
stemln:
.LFB38:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movq	STACK(%rip), %rdi
	call	array_pop@PLT
	testq	%rax, %rax
	je	.L41
	movq	%rax, %rbx
	movl	$2, %edi
	call	init_value@PLT
	pushq	24(%rbx)
	.cfi_def_cfa_offset 40
	pushq	16(%rbx)
	.cfi_def_cfa_offset 48
	movq	%rax, %rbp
	call	logl@PLT
	movq	STACK(%rip), %rdi
	movq	%rbp, %rsi
	fstpt	16(%rbp)
	call	array_append@PLT
	addq	$24, %rsp
	.cfi_def_cfa_offset 24
	movq	%rbx, %rdi
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	value_free@PLT
	.p2align 4,,10
	.p2align 3
.L41:
	.cfi_def_cfa_offset 32
	movl	$5, %edi
	call	init_value@PLT
	movq	STACK(%rip), %rdi
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	movq	%rax, %rsi
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	array_append@PLT
	.cfi_endproc
.LFE38:
	.size	stemln, .-stemln
	.p2align 4
	.globl	quote
	.type	quote, @function
quote:
.LFB46:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movq	STACK(%rip), %rdi
	call	array_pop@PLT
	testq	%rax, %rax
	je	.L46
	movl	$4, %edi
	movq	%rax, %rbx
	call	init_value@PLT
	movl	$10, %edi
	movq	%rax, %rbp
	call	init_array@PLT
	movq	%rbx, %rsi
	movq	%rax, 16(%rbp)
	movq	%rax, %rdi
	call	array_append@PLT
	movq	%rbp, %rsi
.L45:
	movq	STACK(%rip), %rdi
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	array_append@PLT
	.p2align 4,,10
	.p2align 3
.L46:
	.cfi_restore_state
	movl	$5, %edi
	call	init_value@PLT
	movq	%rax, %rsi
	jmp	.L45
	.cfi_endproc
.LFE46:
	.size	quote, .-quote
	.p2align 4
	.globl	stemtype
	.type	stemtype, @function
stemtype:
.LFB47:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$24, %rsp
	.cfi_def_cfa_offset 48
	movq	STACK(%rip), %rdi
	call	array_pop@PLT
	testq	%rax, %rax
	je	.L51
	movl	$1, %edi
	movq	%rax, %rbx
	call	init_value@PLT
	movq	STACK(%rip), %rdi
	movq	%rbx, %rsi
	movq	%rax, %rbp
	movl	(%rbx), %eax
	movq	%rax, 8(%rsp)
	fildq	8(%rsp)
	fstpt	16(%rbp)
	call	array_append@PLT
	movq	%rbp, %rsi
.L50:
	movq	STACK(%rip), %rdi
	addq	$24, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	array_append@PLT
	.p2align 4,,10
	.p2align 3
.L51:
	.cfi_restore_state
	movl	$5, %edi
	call	init_value@PLT
	movq	%rax, %rsi
	jmp	.L50
	.cfi_endproc
.LFE47:
	.size	stemtype, .-stemtype
	.p2align 4
	.globl	stemsin
	.type	stemsin, @function
stemsin:
.LFB34:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movq	STACK(%rip), %rdi
	call	array_pop@PLT
	testq	%rax, %rax
	je	.L55
	movq	%rax, %rbx
	movl	$2, %edi
	call	init_value@PLT
	pushq	24(%rbx)
	.cfi_def_cfa_offset 40
	pushq	16(%rbx)
	.cfi_def_cfa_offset 48
	movq	%rax, %rbp
	call	sinhl@PLT
	movq	STACK(%rip), %rdi
	movq	%rbp, %rsi
	fstpt	16(%rbp)
	call	array_append@PLT
	addq	$24, %rsp
	.cfi_def_cfa_offset 24
	movq	%rbx, %rdi
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	value_free@PLT
	.p2align 4,,10
	.p2align 3
.L55:
	.cfi_def_cfa_offset 32
	movl	$5, %edi
	call	init_value@PLT
	movq	STACK(%rip), %rdi
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	movq	%rax, %rsi
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	array_append@PLT
	.cfi_endproc
.LFE34:
	.size	stemsin, .-stemsin
	.p2align 4
	.globl	stemcos
	.type	stemcos, @function
stemcos:
.LFB35:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movq	STACK(%rip), %rdi
	call	array_pop@PLT
	testq	%rax, %rax
	je	.L59
	movq	%rax, %rbx
	movl	$2, %edi
	call	init_value@PLT
	pushq	24(%rbx)
	.cfi_def_cfa_offset 40
	pushq	16(%rbx)
	.cfi_def_cfa_offset 48
	movq	%rax, %rbp
	call	coshl@PLT
	movq	STACK(%rip), %rdi
	movq	%rbp, %rsi
	fstpt	16(%rbp)
	call	array_append@PLT
	addq	$24, %rsp
	.cfi_def_cfa_offset 24
	movq	%rbx, %rdi
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	value_free@PLT
	.p2align 4,,10
	.p2align 3
.L59:
	.cfi_def_cfa_offset 32
	movl	$5, %edi
	call	init_value@PLT
	movq	STACK(%rip), %rdi
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	movq	%rax, %rsi
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	array_append@PLT
	.cfi_endproc
.LFE35:
	.size	stemcos, .-stemcos
	.p2align 4
	.globl	stemexp
	.type	stemexp, @function
stemexp:
.LFB36:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movq	STACK(%rip), %rdi
	call	array_pop@PLT
	testq	%rax, %rax
	je	.L63
	movq	%rax, %rbx
	movl	$2, %edi
	call	init_value@PLT
	pushq	24(%rbx)
	.cfi_def_cfa_offset 40
	pushq	16(%rbx)
	.cfi_def_cfa_offset 48
	movq	%rax, %rbp
	call	expl@PLT
	movq	STACK(%rip), %rdi
	movq	%rbp, %rsi
	fstpt	16(%rbp)
	call	array_append@PLT
	addq	$24, %rsp
	.cfi_def_cfa_offset 24
	movq	%rbx, %rdi
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	value_free@PLT
	.p2align 4,,10
	.p2align 3
.L63:
	.cfi_def_cfa_offset 32
	movl	$5, %edi
	call	init_value@PLT
	movq	STACK(%rip), %rdi
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	movq	%rax, %rsi
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	array_append@PLT
	.cfi_endproc
.LFE36:
	.size	stemexp, .-stemexp
	.p2align 4
	.globl	stoi
	.type	stoi, @function
stoi:
.LFB70:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$24, %rsp
	.cfi_def_cfa_offset 48
	movq	STACK(%rip), %rdi
	call	array_pop@PLT
	testq	%rax, %rax
	je	.L67
	movl	$1, %edi
	movq	%rax, %rbx
	call	init_value@PLT
	movl	$10, %edx
	xorl	%esi, %esi
	movq	%rax, %rbp
	movq	16(%rbx), %rax
	movq	16(%rax), %rdi
	call	strtol@PLT
	movq	STACK(%rip), %rdi
	movq	%rbp, %rsi
	movl	%eax, 12(%rsp)
	fildl	12(%rsp)
	fstpt	16(%rbp)
	call	array_append@PLT
	addq	$24, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%rbx, %rdi
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	value_free@PLT
	.p2align 4,,10
	.p2align 3
.L67:
	.cfi_restore_state
	movl	$5, %edi
	call	init_value@PLT
	movq	STACK(%rip), %rdi
	addq	$24, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	movq	%rax, %rsi
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	array_append@PLT
	.cfi_endproc
.LFE70:
	.size	stoi, .-stoi
	.p2align 4
	.globl	isdef
	.type	isdef, @function
isdef:
.LFB50:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$24, %rsp
	.cfi_def_cfa_offset 48
	movq	STACK(%rip), %rdi
	call	array_pop@PLT
	testq	%rax, %rax
	je	.L74
	movl	$1, %edi
	movq	%rax, %rbx
	call	init_value@PLT
	movq	%rax, %rbp
	movl	(%rbx), %eax
	testl	%eax, %eax
	je	.L75
	fldz
.L70:
	fstpt	16(%rbp)
	movq	STACK(%rip), %rdi
	movq	%rbx, %rsi
	call	array_append@PLT
	movq	%rbp, %rsi
.L73:
	movq	STACK(%rip), %rdi
	addq	$24, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	array_append@PLT
	.p2align 4,,10
	.p2align 3
.L75:
	.cfi_restore_state
	movq	16(%rbx), %rsi
	movq	WORD_TABLE(%rip), %rdi
	call	ht_exists@PLT
	movzbl	%al, %eax
	movw	%ax, 14(%rsp)
	filds	14(%rsp)
	jmp	.L70
	.p2align 4,,10
	.p2align 3
.L74:
	movl	$5, %edi
	call	init_value@PLT
	movq	%rax, %rsi
	jmp	.L73
	.cfi_endproc
.LFE50:
	.size	isdef, .-isdef
	.p2align 4
	.globl	isnum
	.type	isnum, @function
isnum:
.LFB69:
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
	subq	$8, %rsp
	.cfi_def_cfa_offset 48
	movq	STACK(%rip), %rdi
	call	array_pop@PLT
	testq	%rax, %rax
	je	.L85
	movq	%rax, %r12
	movl	$1, %edi
	call	init_value@PLT
	movq	16(%r12), %r13
	movq	%rax, %rbp
	movq	0(%r13), %rbx
	testq	%rbx, %rbx
	je	.L80
	call	__ctype_b_loc@PLT
	movq	(%rax), %rdi
	movq	16(%r13), %rax
	leaq	(%rbx,%rax), %rcx
	.p2align 4,,10
	.p2align 3
.L79:
	movsbq	(%rax), %rdx
	testw	$10240, (%rdi,%rdx,2)
	je	.L81
	addq	$1, %rax
	cmpq	%rax, %rcx
	jne	.L79
.L80:
	fld1
.L78:
	fstpt	16(%rbp)
	movq	STACK(%rip), %rdi
	movq	%r12, %rsi
	call	array_append@PLT
	movq	%rbp, %rsi
.L84:
	movq	STACK(%rip), %rdi
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
	jmp	array_append@PLT
	.p2align 4,,10
	.p2align 3
.L81:
	.cfi_restore_state
	fldz
	jmp	.L78
	.p2align 4,,10
	.p2align 3
.L85:
	movl	$5, %edi
	call	init_value@PLT
	movq	%rax, %rsi
	jmp	.L84
	.cfi_endproc
.LFE69:
	.size	isnum, .-isnum
	.p2align 4
	.globl	stemlen
	.type	stemlen, @function
stemlen:
.LFB54:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$24, %rsp
	.cfi_def_cfa_offset 48
	movq	STACK(%rip), %rdi
	call	array_pop@PLT
	testq	%rax, %rax
	je	.L96
	movl	$1, %edi
	movq	%rax, %rbx
	call	init_value@PLT
	movq	%rax, %rbp
	movl	(%rbx), %eax
	leal	-1(%rax), %edx
	cmpl	$1, %edx
	jbe	.L97
	cmpl	$3, %eax
	je	.L93
	testl	%eax, %eax
	je	.L93
	cmpl	$4, %eax
	jne	.L89
	movq	16(%rbx), %rax
	fildq	8(%rax)
	cmpq	$0, 8(%rax)
	js	.L98
.L92:
	fstpt	16(%rbp)
	jmp	.L89
	.p2align 4,,10
	.p2align 3
.L97:
	fld1
	fstpt	16(%rbp)
.L89:
	movq	STACK(%rip), %rdi
	movq	%rbx, %rsi
	call	array_append@PLT
	movq	%rbp, %rsi
.L95:
	movq	STACK(%rip), %rdi
	addq	$24, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	array_append@PLT
	.p2align 4,,10
	.p2align 3
.L93:
	.cfi_restore_state
	movq	16(%rbx), %rax
	movq	16(%rax), %rdi
	call	strlen@PLT
	movq	%rax, 8(%rsp)
	fildq	8(%rsp)
	fstpt	16(%rbp)
	jmp	.L89
	.p2align 4,,10
	.p2align 3
.L96:
	movl	$5, %edi
	call	init_value@PLT
	movq	%rax, %rsi
	jmp	.L95
	.p2align 4,,10
	.p2align 3
.L98:
	fadds	.LC0(%rip)
	jmp	.L92
	.cfi_endproc
.LFE54:
	.size	stemlen, .-stemlen
	.p2align 4
	.globl	swap
	.type	swap, @function
swap:
.LFB49:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movq	STACK(%rip), %rdi
	call	array_pop@PLT
	testq	%rax, %rax
	je	.L104
	movq	STACK(%rip), %rdi
	movq	%rax, %rbx
	call	array_pop@PLT
	movq	STACK(%rip), %rdi
	movq	%rbx, %rsi
	movq	%rax, %rbp
	testq	%rax, %rax
	je	.L105
	call	array_append@PLT
	movq	%rbp, %rsi
.L103:
	movq	STACK(%rip), %rdi
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	array_append@PLT
	.p2align 4,,10
	.p2align 3
.L105:
	.cfi_restore_state
	call	array_append@PLT
.L104:
	movl	$5, %edi
	call	init_value@PLT
	movq	%rax, %rsi
	jmp	.L103
	.cfi_endproc
.LFE49:
	.size	swap, .-swap
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC9:
	.string	"add_funcs"
.LC10:
	.string	"add_objs"
.LC11:
	.string	"%s\n"
	.text
	.p2align 4
	.globl	clib
	.type	clib, @function
clib:
.LFB62:
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
	subq	$8, %rsp
	.cfi_def_cfa_offset 48
	movq	STACK(%rip), %rdi
	call	array_pop@PLT
	movl	$1, %esi
	movq	%rax, %rbp
	movq	16(%rax), %rax
	movq	16(%rax), %rdi
	call	dlopen@PLT
	testq	%rax, %rax
	je	.L111
	movq	%rax, %rbx
	call	dlerror@PLT
	movq	%rbx, %rdi
	leaq	.LC9(%rip), %rsi
	call	dlsym@PLT
	movq	%rbx, %rdi
	leaq	.LC10(%rip), %rsi
	movq	%rax, %r12
	call	dlsym@PLT
	movq	%rax, %r13
	call	dlerror@PLT
	movq	%rax, %rbx
	testq	%rax, %rax
	jne	.L112
	call	*%r12
	call	*%r13
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 40
	movq	%rbp, %rdi
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%rbp
	.cfi_def_cfa_offset 24
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	value_free@PLT
	.p2align 4,,10
	.p2align 3
.L112:
	.cfi_restore_state
	movq	%rbp, %rdi
	call	value_free@PLT
	movq	stderr(%rip), %rdi
	movq	%rbx, %rdx
	xorl	%eax, %eax
	leaq	.LC11(%rip), %rsi
	call	fprintf@PLT
.L110:
	movl	$5, %edi
	call	init_value@PLT
	movq	STACK(%rip), %rdi
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 40
	popq	%rbx
	.cfi_def_cfa_offset 32
	movq	%rax, %rsi
	popq	%rbp
	.cfi_def_cfa_offset 24
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	array_append@PLT
	.p2align 4,,10
	.p2align 3
.L111:
	.cfi_restore_state
	movq	STACK(%rip), %rdi
	movq	%rbp, %rsi
	call	array_append@PLT
	jmp	.L110
	.cfi_endproc
.LFE62:
	.size	clib, .-clib
	.p2align 4
	.globl	stemmul
	.type	stemmul, @function
stemmul:
.LFB29:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movq	STACK(%rip), %rdi
	call	array_pop@PLT
	testq	%rax, %rax
	je	.L118
	movq	STACK(%rip), %rdi
	movq	%rax, %rbx
	call	array_pop@PLT
	movq	%rax, %rbp
	testq	%rax, %rax
	je	.L119
	movl	$2, %edi
	call	init_value@PLT
	cmpl	$1, 0(%rbp)
	movq	%rax, %rsi
	je	.L120
.L116:
	fldt	16(%rbp)
	movq	STACK(%rip), %rdi
	fldt	16(%rbx)
	fmulp	%st, %st(1)
	fstpt	16(%rsi)
	call	array_append@PLT
	movq	%rbp, %rdi
	call	value_free@PLT
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%rbx, %rdi
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	value_free@PLT
	.p2align 4,,10
	.p2align 3
.L120:
	.cfi_restore_state
	cmpl	$1, (%rbx)
	jne	.L116
	movl	$1, (%rax)
	jmp	.L116
	.p2align 4,,10
	.p2align 3
.L119:
	movq	STACK(%rip), %rdi
	movq	%rbx, %rsi
	call	array_append@PLT
.L118:
	movl	$5, %edi
	call	init_value@PLT
	movq	STACK(%rip), %rdi
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	movq	%rax, %rsi
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	array_append@PLT
	.cfi_endproc
.LFE29:
	.size	stemmul, .-stemmul
	.p2align 4
	.globl	stemsub
	.type	stemsub, @function
stemsub:
.LFB28:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movq	STACK(%rip), %rdi
	call	array_pop@PLT
	testq	%rax, %rax
	je	.L126
	movq	STACK(%rip), %rdi
	movq	%rax, %rbx
	call	array_pop@PLT
	movq	%rax, %rbp
	testq	%rax, %rax
	je	.L127
	movl	$2, %edi
	call	init_value@PLT
	cmpl	$1, 0(%rbp)
	movq	%rax, %rsi
	je	.L128
.L124:
	fldt	16(%rbp)
	movq	STACK(%rip), %rdi
	fldt	16(%rbx)
	fsubrp	%st, %st(1)
	fstpt	16(%rsi)
	call	array_append@PLT
	movq	%rbp, %rdi
	call	value_free@PLT
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%rbx, %rdi
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	value_free@PLT
	.p2align 4,,10
	.p2align 3
.L128:
	.cfi_restore_state
	cmpl	$1, (%rbx)
	jne	.L124
	movl	$1, (%rax)
	jmp	.L124
	.p2align 4,,10
	.p2align 3
.L127:
	movq	STACK(%rip), %rdi
	movq	%rbx, %rsi
	call	array_append@PLT
.L126:
	movl	$5, %edi
	call	init_value@PLT
	movq	STACK(%rip), %rdi
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	movq	%rax, %rsi
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	array_append@PLT
	.cfi_endproc
.LFE28:
	.size	stemsub, .-stemsub
	.p2align 4
	.globl	stemdiv
	.type	stemdiv, @function
stemdiv:
.LFB30:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movq	STACK(%rip), %rdi
	call	array_pop@PLT
	testq	%rax, %rax
	je	.L134
	movq	STACK(%rip), %rdi
	movq	%rax, %rbx
	call	array_pop@PLT
	movq	%rax, %rbp
	testq	%rax, %rax
	je	.L135
	movl	$2, %edi
	call	init_value@PLT
	cmpl	$1, 0(%rbp)
	movq	%rax, %rsi
	je	.L136
.L132:
	fldt	16(%rbp)
	movq	STACK(%rip), %rdi
	fldt	16(%rbx)
	fdivrp	%st, %st(1)
	fstpt	16(%rsi)
	call	array_append@PLT
	movq	%rbp, %rdi
	call	value_free@PLT
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%rbx, %rdi
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	value_free@PLT
	.p2align 4,,10
	.p2align 3
.L136:
	.cfi_restore_state
	cmpl	$1, (%rbx)
	jne	.L132
	movl	$1, (%rax)
	jmp	.L132
	.p2align 4,,10
	.p2align 3
.L135:
	movq	STACK(%rip), %rdi
	movq	%rbx, %rsi
	call	array_append@PLT
.L134:
	movl	$5, %edi
	call	init_value@PLT
	movq	STACK(%rip), %rdi
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	movq	%rax, %rsi
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	array_append@PLT
	.cfi_endproc
.LFE30:
	.size	stemdiv, .-stemdiv
	.p2align 4
	.globl	stempow
	.type	stempow, @function
stempow:
.LFB33:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	movq	STACK(%rip), %rdi
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	call	array_pop@PLT
	testq	%rax, %rax
	je	.L142
	movq	STACK(%rip), %rdi
	movq	%rax, %rbx
	call	array_pop@PLT
	movq	%rax, %rbp
	testq	%rax, %rax
	je	.L143
	movl	$2, %edi
	call	init_value@PLT
	cmpl	$1, 0(%rbp)
	movq	%rax, %r12
	je	.L144
.L140:
	pushq	24(%rbx)
	.cfi_def_cfa_offset 40
	pushq	16(%rbx)
	.cfi_def_cfa_offset 48
	pushq	24(%rbp)
	.cfi_def_cfa_offset 56
	pushq	16(%rbp)
	.cfi_def_cfa_offset 64
	call	powl@PLT
	movq	STACK(%rip), %rdi
	addq	$32, %rsp
	.cfi_def_cfa_offset 32
	movq	%r12, %rsi
	fstpt	16(%r12)
	call	array_append@PLT
	movq	%rbp, %rdi
	call	value_free@PLT
	movq	%rbx, %rdi
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	jmp	value_free@PLT
	.p2align 4,,10
	.p2align 3
.L144:
	.cfi_restore_state
	cmpl	$1, (%rbx)
	jne	.L140
	movl	$1, (%rax)
	jmp	.L140
	.p2align 4,,10
	.p2align 3
.L143:
	movq	STACK(%rip), %rdi
	movq	%rbx, %rsi
	call	array_append@PLT
.L142:
	movl	$5, %edi
	call	init_value@PLT
	movq	STACK(%rip), %rdi
	popq	%rbx
	.cfi_def_cfa_offset 24
	movq	%rax, %rsi
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	jmp	array_append@PLT
	.cfi_endproc
.LFE33:
	.size	stempow, .-stempow
	.p2align 4
	.globl	keep
	.type	keep, @function
keep:
.LFB56:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	movq	STACK(%rip), %rdi
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	call	array_pop@PLT
	testq	%rax, %rax
	je	.L155
	movq	STACK(%rip), %rdi
	movq	%rax, %rbp
	call	array_pop@PLT
	movq	%rax, %r12
	testq	%rax, %rax
	je	.L156
	movq	%rax, %rdi
	call	value_copy@PLT
	movq	STACK(%rip), %rdi
	movq	%rax, %rsi
	call	array_append@PLT
	cmpl	$4, 0(%rbp)
	je	.L157
	movq	%r12, %rdi
	call	eval@PLT
.L151:
	movq	STACK(%rip), %rdi
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r12, %rsi
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	jmp	array_append@PLT
	.p2align 4,,10
	.p2align 3
.L157:
	.cfi_restore_state
	movq	EVAL_STACK(%rip), %rdi
	movq	%rbp, %rsi
	call	array_append@PLT
	movq	16(%rbp), %rax
	cmpq	$0, 8(%rax)
	je	.L149
	xorl	%ebx, %ebx
	.p2align 4,,10
	.p2align 3
.L150:
	movq	(%rax), %rax
	movq	(%rax,%rbx,8), %rdi
	addq	$1, %rbx
	call	value_copy@PLT
	movq	%rax, %rdi
	call	eval@PLT
	movq	16(%rbp), %rax
	cmpq	8(%rax), %rbx
	jb	.L150
.L149:
	movq	EVAL_STACK(%rip), %rdi
	call	array_pop@PLT
	movq	%rax, %rdi
	call	value_free@PLT
	jmp	.L151
	.p2align 4,,10
	.p2align 3
.L156:
	movq	STACK(%rip), %rdi
	movq	%rbp, %rsi
	call	array_append@PLT
.L155:
	movl	$5, %edi
	call	init_value@PLT
	movq	STACK(%rip), %rdi
	popq	%rbx
	.cfi_def_cfa_offset 24
	movq	%rax, %rsi
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	jmp	array_append@PLT
	.cfi_endproc
.LFE56:
	.size	keep, .-keep
	.p2align 4
	.globl	stemeval
	.type	stemeval, @function
stemeval:
.LFB37:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movq	STACK(%rip), %rdi
	call	array_pop@PLT
	testq	%rax, %rax
	je	.L166
	cmpl	$4, (%rax)
	movq	%rax, %rbp
	je	.L167
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%rax, %rdi
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	eval@PLT
	.p2align 4,,10
	.p2align 3
.L167:
	.cfi_restore_state
	movq	EVAL_STACK(%rip), %rdi
	movq	%rax, %rsi
	call	array_append@PLT
	movq	16(%rbp), %rax
	cmpq	$0, 8(%rax)
	je	.L161
	xorl	%ebx, %ebx
	.p2align 4,,10
	.p2align 3
.L162:
	movq	(%rax), %rax
	movq	(%rax,%rbx,8), %rdi
	addq	$1, %rbx
	call	value_copy@PLT
	movq	%rax, %rdi
	call	eval@PLT
	movq	16(%rbp), %rax
	cmpq	8(%rax), %rbx
	jb	.L162
.L161:
	movq	EVAL_STACK(%rip), %rdi
	call	array_pop@PLT
	testq	%rax, %rax
	je	.L158
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%rax, %rdi
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	value_free@PLT
	.p2align 4,,10
	.p2align 3
.L158:
	.cfi_restore_state
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
.L166:
	.cfi_restore_state
	movq	STACK(%rip), %rdi
	xorl	%esi, %esi
	call	array_append@PLT
	movl	$5, %edi
	call	init_value@PLT
	movq	STACK(%rip), %rdi
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	movq	%rax, %rsi
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	array_append@PLT
	.cfi_endproc
.LFE37:
	.size	stemeval, .-stemeval
	.p2align 4
	.globl	stemadd
	.type	stemadd, @function
stemadd:
.LFB27:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movq	STACK(%rip), %rdi
	call	array_pop@PLT
	testq	%rax, %rax
	je	.L176
	movq	STACK(%rip), %rdi
	movq	%rax, %rbx
	call	array_pop@PLT
	movq	%rax, %rbp
	testq	%rax, %rax
	je	.L177
	movl	$2, %edi
	call	init_value@PLT
	movl	0(%rbp), %edx
	cmpl	$1, %edx
	je	.L178
.L171:
	subl	$1, %edx
	cmpl	$1, %edx
	ja	.L179
	movl	(%rbx), %edx
.L172:
	subl	$1, %edx
	movq	STACK(%rip), %rdi
	cmpl	$1, %edx
	jbe	.L180
.L174:
	movq	%rbp, %rsi
	call	array_append@PLT
.L177:
	movq	STACK(%rip), %rdi
	movq	%rbx, %rsi
	call	array_append@PLT
.L176:
	movl	$5, %edi
	call	init_value@PLT
	movq	STACK(%rip), %rdi
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	movq	%rax, %rsi
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	array_append@PLT
	.p2align 4,,10
	.p2align 3
.L178:
	.cfi_restore_state
	movl	(%rbx), %edx
	cmpl	$1, %edx
	jne	.L172
	movl	$1, (%rax)
	movl	0(%rbp), %edx
	jmp	.L171
	.p2align 4,,10
	.p2align 3
.L179:
	movq	STACK(%rip), %rdi
	jmp	.L174
	.p2align 4,,10
	.p2align 3
.L180:
	fldt	16(%rbp)
	movq	%rax, %rsi
	fldt	16(%rbx)
	faddp	%st, %st(1)
	fstpt	16(%rax)
	call	array_append@PLT
	movq	%rbp, %rdi
	call	value_free@PLT
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	movq	%rbx, %rdi
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	value_free@PLT
	.cfi_endproc
.LFE27:
	.size	stemadd, .-stemadd
	.p2align 4
	.globl	ltequals
	.type	ltequals, @function
ltequals:
.LFB61:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	movq	STACK(%rip), %rdi
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	call	array_pop@PLT
	testq	%rax, %rax
	je	.L198
	movq	STACK(%rip), %rdi
	movq	%rax, %rbx
	call	array_pop@PLT
	movq	%rax, %rbp
	testq	%rax, %rax
	je	.L199
	movl	$1, %edi
	call	init_value@PLT
	movq	%rax, %r12
	movl	0(%rbp), %eax
	cmpl	$3, %eax
	je	.L200
	testl	%eax, %eax
	jne	.L187
	movl	(%rbx), %eax
	testl	%eax, %eax
	je	.L185
.L186:
	movq	STACK(%rip), %rdi
	movq	%rbp, %rsi
	call	array_append@PLT
.L199:
	movq	STACK(%rip), %rdi
	movq	%rbx, %rsi
	call	array_append@PLT
.L198:
	movl	$5, %edi
	call	init_value@PLT
	movq	STACK(%rip), %rdi
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%rax, %rsi
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	jmp	array_append@PLT
	.p2align 4,,10
	.p2align 3
.L200:
	.cfi_restore_state
	cmpl	$3, (%rbx)
	jne	.L186
.L185:
	movq	16(%rbx), %rax
	movq	16(%rax), %rsi
	movq	16(%rbp), %rax
	movq	16(%rax), %rdi
	call	strcmp@PLT
	testl	%eax, %eax
	jle	.L197
.L195:
	fldz
.L189:
	fstpt	16(%r12)
	movq	STACK(%rip), %rdi
	movq	%r12, %rsi
	call	array_append@PLT
	movq	%rbp, %rdi
	call	value_free@PLT
	movq	%rbx, %rdi
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	jmp	value_free@PLT
	.p2align 4,,10
	.p2align 3
.L187:
	.cfi_restore_state
	subl	$1, %eax
	cmpl	$1, %eax
	ja	.L186
	movl	(%rbx), %eax
	subl	$1, %eax
	cmpl	$1, %eax
	ja	.L186
	fldt	16(%rbp)
	fldt	16(%rbx)
	fcomip	%st(1), %st
	fstp	%st(0)
	jb	.L195
.L197:
	fld1
	jmp	.L189
	.cfi_endproc
.LFE61:
	.size	ltequals, .-ltequals
	.p2align 4
	.globl	lthan
	.type	lthan, @function
lthan:
.LFB64:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	movq	STACK(%rip), %rdi
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	call	array_pop@PLT
	testq	%rax, %rax
	je	.L218
	movq	STACK(%rip), %rdi
	movq	%rax, %rbx
	call	array_pop@PLT
	movq	%rax, %rbp
	testq	%rax, %rax
	je	.L219
	movl	$1, %edi
	call	init_value@PLT
	movq	%rax, %r12
	movl	0(%rbp), %eax
	cmpl	$3, %eax
	je	.L220
	testl	%eax, %eax
	jne	.L207
	movl	(%rbx), %eax
	testl	%eax, %eax
	je	.L205
.L206:
	movq	STACK(%rip), %rdi
	movq	%rbp, %rsi
	call	array_append@PLT
.L219:
	movq	STACK(%rip), %rdi
	movq	%rbx, %rsi
	call	array_append@PLT
.L218:
	movl	$5, %edi
	call	init_value@PLT
	movq	STACK(%rip), %rdi
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%rax, %rsi
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	jmp	array_append@PLT
	.p2align 4,,10
	.p2align 3
.L220:
	.cfi_restore_state
	cmpl	$3, (%rbx)
	jne	.L206
.L205:
	movq	16(%rbx), %rax
	movq	16(%rax), %rsi
	movq	16(%rbp), %rax
	movq	16(%rax), %rdi
	call	strcmp@PLT
	testl	%eax, %eax
	js	.L217
.L215:
	fldz
.L209:
	fstpt	16(%r12)
	movq	STACK(%rip), %rdi
	movq	%r12, %rsi
	call	array_append@PLT
	movq	%rbp, %rdi
	call	value_free@PLT
	movq	%rbx, %rdi
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	jmp	value_free@PLT
	.p2align 4,,10
	.p2align 3
.L207:
	.cfi_restore_state
	subl	$1, %eax
	cmpl	$1, %eax
	ja	.L206
	movl	(%rbx), %eax
	subl	$1, %eax
	cmpl	$1, %eax
	ja	.L206
	fldt	16(%rbp)
	fldt	16(%rbx)
	fcomip	%st(1), %st
	fstp	%st(0)
	jbe	.L215
.L217:
	fld1
	jmp	.L209
	.cfi_endproc
.LFE64:
	.size	lthan, .-lthan
	.p2align 4
	.globl	gtequals
	.type	gtequals, @function
gtequals:
.LFB60:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	movq	STACK(%rip), %rdi
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	call	array_pop@PLT
	testq	%rax, %rax
	je	.L238
	movq	STACK(%rip), %rdi
	movq	%rax, %rbx
	call	array_pop@PLT
	movq	%rax, %rbp
	testq	%rax, %rax
	je	.L239
	movl	$1, %edi
	call	init_value@PLT
	movq	%rax, %r12
	movl	0(%rbp), %eax
	cmpl	$3, %eax
	je	.L240
	testl	%eax, %eax
	jne	.L227
	movl	(%rbx), %eax
	testl	%eax, %eax
	je	.L225
.L226:
	movq	STACK(%rip), %rdi
	movq	%rbp, %rsi
	call	array_append@PLT
.L239:
	movq	STACK(%rip), %rdi
	movq	%rbx, %rsi
	call	array_append@PLT
.L238:
	movl	$5, %edi
	call	init_value@PLT
	movq	STACK(%rip), %rdi
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%rax, %rsi
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	jmp	array_append@PLT
	.p2align 4,,10
	.p2align 3
.L240:
	.cfi_restore_state
	cmpl	$3, (%rbx)
	jne	.L226
.L225:
	movq	16(%rbx), %rax
	movq	16(%rax), %rsi
	movq	16(%rbp), %rax
	movq	16(%rax), %rdi
	call	strcmp@PLT
	testl	%eax, %eax
	jle	.L237
.L235:
	fldz
.L229:
	fstpt	16(%r12)
	movq	STACK(%rip), %rdi
	movq	%r12, %rsi
	call	array_append@PLT
	movq	%rbp, %rdi
	call	value_free@PLT
	movq	%rbx, %rdi
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	jmp	value_free@PLT
	.p2align 4,,10
	.p2align 3
.L227:
	.cfi_restore_state
	subl	$1, %eax
	cmpl	$1, %eax
	ja	.L226
	movl	(%rbx), %eax
	subl	$1, %eax
	cmpl	$1, %eax
	ja	.L226
	fldt	16(%rbx)
	fldt	16(%rbp)
	fcomip	%st(1), %st
	fstp	%st(0)
	jb	.L235
.L237:
	fld1
	jmp	.L229
	.cfi_endproc
.LFE60:
	.size	gtequals, .-gtequals
	.p2align 4
	.globl	gthan
	.type	gthan, @function
gthan:
.LFB63:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	movq	STACK(%rip), %rdi
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	call	array_pop@PLT
	testq	%rax, %rax
	je	.L258
	movq	STACK(%rip), %rdi
	movq	%rax, %rbx
	call	array_pop@PLT
	movq	%rax, %rbp
	testq	%rax, %rax
	je	.L259
	movl	$1, %edi
	call	init_value@PLT
	movq	%rax, %r12
	movl	0(%rbp), %eax
	cmpl	$3, %eax
	je	.L260
	testl	%eax, %eax
	jne	.L247
	movl	(%rbx), %eax
	testl	%eax, %eax
	je	.L245
.L246:
	movq	STACK(%rip), %rdi
	movq	%rbp, %rsi
	call	array_append@PLT
.L259:
	movq	STACK(%rip), %rdi
	movq	%rbx, %rsi
	call	array_append@PLT
.L258:
	movl	$5, %edi
	call	init_value@PLT
	movq	STACK(%rip), %rdi
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%rax, %rsi
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	jmp	array_append@PLT
	.p2align 4,,10
	.p2align 3
.L260:
	.cfi_restore_state
	cmpl	$3, (%rbx)
	jne	.L246
.L245:
	movq	16(%rbx), %rax
	movq	16(%rax), %rsi
	movq	16(%rbp), %rax
	movq	16(%rax), %rdi
	call	strcmp@PLT
	testl	%eax, %eax
	jle	.L255
.L257:
	fld1
.L249:
	fstpt	16(%r12)
	movq	STACK(%rip), %rdi
	movq	%r12, %rsi
	call	array_append@PLT
	movq	%rbp, %rdi
	call	value_free@PLT
	movq	%rbx, %rdi
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	jmp	value_free@PLT
	.p2align 4,,10
	.p2align 3
.L247:
	.cfi_restore_state
	subl	$1, %eax
	cmpl	$1, %eax
	ja	.L246
	movl	(%rbx), %eax
	subl	$1, %eax
	cmpl	$1, %eax
	ja	.L246
	fldt	16(%rbx)
	fldt	16(%rbp)
	fcomip	%st(1), %st
	fstp	%st(0)
	ja	.L257
.L255:
	fldz
	jmp	.L249
	.cfi_endproc
.LFE63:
	.size	gthan, .-gthan
	.p2align 4
	.globl	nequals
	.type	nequals, @function
nequals:
.LFB66:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	movq	STACK(%rip), %rdi
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	call	array_pop@PLT
	testq	%rax, %rax
	je	.L276
	movq	STACK(%rip), %rdi
	movq	%rax, %rbx
	call	array_pop@PLT
	movq	%rax, %rbp
	testq	%rax, %rax
	je	.L277
	movl	$1, %edi
	call	init_value@PLT
	movq	%rax, %r12
	movl	0(%rbp), %eax
	cmpl	$3, %eax
	je	.L278
	testl	%eax, %eax
	jne	.L267
	movl	(%rbx), %eax
	testl	%eax, %eax
	je	.L265
.L266:
	movq	STACK(%rip), %rdi
	movq	%rbp, %rsi
	call	array_append@PLT
.L277:
	movq	STACK(%rip), %rdi
	movq	%rbx, %rsi
	call	array_append@PLT
.L276:
	movl	$5, %edi
	call	init_value@PLT
	movq	STACK(%rip), %rdi
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%rax, %rsi
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	jmp	array_append@PLT
	.p2align 4,,10
	.p2align 3
.L278:
	.cfi_restore_state
	cmpl	$3, (%rbx)
	jne	.L266
.L265:
	movq	16(%rbx), %rax
	movq	16(%rax), %rsi
	movq	16(%rbp), %rax
	movq	16(%rax), %rdi
	call	strcmp@PLT
	testl	%eax, %eax
	jne	.L274
.L273:
	fldz
.L269:
	fstpt	16(%r12)
	movq	STACK(%rip), %rdi
	movq	%r12, %rsi
	call	array_append@PLT
	movq	%rbp, %rdi
	call	value_free@PLT
	movq	%rbx, %rdi
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	jmp	value_free@PLT
	.p2align 4,,10
	.p2align 3
.L267:
	.cfi_restore_state
	subl	$1, %eax
	cmpl	$1, %eax
	ja	.L266
	movl	(%rbx), %eax
	subl	$1, %eax
	cmpl	$1, %eax
	ja	.L266
	fldt	16(%rbx)
	fldt	16(%rbp)
	fucomip	%st(1), %st
	fstp	%st(0)
	jp	.L274
	je	.L273
.L274:
	fld1
	jmp	.L269
	.cfi_endproc
.LFE66:
	.size	nequals, .-nequals
	.p2align 4
	.globl	equals
	.type	equals, @function
equals:
.LFB65:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	movq	STACK(%rip), %rdi
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	call	array_pop@PLT
	testq	%rax, %rax
	je	.L295
	movq	STACK(%rip), %rdi
	movq	%rax, %rbx
	call	array_pop@PLT
	movq	%rax, %rbp
	testq	%rax, %rax
	je	.L296
	movl	$1, %edi
	call	init_value@PLT
	movq	%rax, %r12
	movl	0(%rbp), %eax
	cmpl	$3, %eax
	je	.L297
	testl	%eax, %eax
	jne	.L285
	movl	(%rbx), %eax
	testl	%eax, %eax
	je	.L283
.L284:
	movq	STACK(%rip), %rdi
	movq	%rbp, %rsi
	call	array_append@PLT
.L296:
	movq	STACK(%rip), %rdi
	movq	%rbx, %rsi
	call	array_append@PLT
.L295:
	movl	$5, %edi
	call	init_value@PLT
	movq	STACK(%rip), %rdi
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%rax, %rsi
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	jmp	array_append@PLT
	.p2align 4,,10
	.p2align 3
.L297:
	.cfi_restore_state
	cmpl	$3, (%rbx)
	jne	.L284
.L283:
	movq	16(%rbx), %rax
	movq	16(%rax), %rsi
	movq	16(%rbp), %rax
	movq	16(%rax), %rdi
	call	strcmp@PLT
	testl	%eax, %eax
	je	.L294
.L291:
	fldz
.L287:
	fstpt	16(%r12)
	movq	STACK(%rip), %rdi
	movq	%r12, %rsi
	call	array_append@PLT
	movq	%rbp, %rdi
	call	value_free@PLT
	movq	%rbx, %rdi
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	jmp	value_free@PLT
	.p2align 4,,10
	.p2align 3
.L285:
	.cfi_restore_state
	subl	$1, %eax
	cmpl	$1, %eax
	ja	.L284
	movl	(%rbx), %eax
	subl	$1, %eax
	cmpl	$1, %eax
	ja	.L284
	fldt	16(%rbx)
	fldt	16(%rbp)
	fucomip	%st(1), %st
	fstp	%st(0)
	jp	.L291
	jne	.L291
.L294:
	fld1
	jmp	.L287
	.cfi_endproc
.LFE65:
	.size	equals, .-equals
	.p2align 4
	.globl	stemfunc
	.type	stemfunc, @function
stemfunc:
.LFB31:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movq	STACK(%rip), %rdi
	call	array_pop@PLT
	testq	%rax, %rax
	je	.L303
	movq	STACK(%rip), %rdi
	movq	%rax, %rbp
	call	array_pop@PLT
	movq	%rax, %rbx
	testq	%rax, %rax
	je	.L304
	movl	(%rax), %eax
	testl	%eax, %eax
	jne	.L303
	movq	16(%rbx), %rdi
	call	string_copy@PLT
	movq	WORD_TABLE(%rip), %rdi
	movq	%rbp, %rdx
	movq	%rax, %rsi
	call	ht_add@PLT
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%rbx, %rdi
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	value_free@PLT
	.p2align 4,,10
	.p2align 3
.L304:
	.cfi_restore_state
	movq	STACK(%rip), %rdi
	movq	%rbp, %rsi
	call	array_append@PLT
.L303:
	movl	$5, %edi
	call	init_value@PLT
	movq	STACK(%rip), %rdi
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	movq	%rax, %rsi
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	array_append@PLT
	.cfi_endproc
.LFE31:
	.size	stemfunc, .-stemfunc
	.p2align 4
	.globl	strquote
	.type	strquote, @function
strquote:
.LFB41:
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
	subq	$8, %rsp
	.cfi_def_cfa_offset 48
	movq	STACK(%rip), %rdi
	call	array_pop@PLT
	testq	%rax, %rax
	je	.L311
	cmpl	$3, (%rax)
	movq	%rax, %rbp
	jne	.L312
	movl	$4, %edi
	call	init_value@PLT
	movl	$10, %edi
	movq	%rax, %r12
	call	init_array@PLT
	movq	%rax, 16(%r12)
	movq	16(%rbp), %rax
	movq	16(%rax), %r13
	movq	%r13, %rdi
	call	strlen@PLT
	leaq	1(%rax), %rbx
	movq	%rbx, %rdi
	call	malloc@PLT
	movq	%rbx, %rdx
	movq	%r13, %rsi
	movq	%rax, %rdi
	call	memcpy@PLT
	movq	%rax, %rdi
	call	init_parser@PLT
	movq	%rax, %rbx
	jmp	.L309
	.p2align 4,,10
	.p2align 3
.L313:
	movq	16(%r12), %rdi
	call	array_append@PLT
.L309:
	movq	%rbx, %rdi
	call	parser_get_next@PLT
	movq	%rax, %rsi
	testq	%rax, %rax
	jne	.L313
	movq	STACK(%rip), %rdi
	movq	%r12, %rsi
	call	array_append@PLT
	movq	%rbp, %rdi
	call	value_free@PLT
	movq	(%rbx), %rdi
	call	free@PLT
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 40
	movq	%rbx, %rdi
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%rbp
	.cfi_def_cfa_offset 24
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	free@PLT
	.p2align 4,,10
	.p2align 3
.L312:
	.cfi_restore_state
	movq	STACK(%rip), %rdi
	movq	%rax, %rsi
	call	array_append@PLT
.L311:
	movl	$5, %edi
	call	init_value@PLT
	movq	STACK(%rip), %rdi
	addq	$8, %rsp
	.cfi_def_cfa_offset 40
	popq	%rbx
	.cfi_def_cfa_offset 32
	movq	%rax, %rsi
	popq	%rbp
	.cfi_def_cfa_offset 24
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	array_append@PLT
	.cfi_endproc
.LFE41:
	.size	strquote, .-strquote
	.p2align 4
	.globl	curry
	.type	curry, @function
curry:
.LFB42:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	STACK(%rip), %rdi
	call	array_pop@PLT
	testq	%rax, %rax
	je	.L319
	movq	STACK(%rip), %rdi
	movq	%rax, %rbx
	call	array_pop@PLT
	movq	%rax, %rsi
	testq	%rax, %rax
	je	.L320
	cmpl	$4, (%rbx)
	je	.L317
	movq	STACK(%rip), %rdi
	call	array_append@PLT
.L320:
	movq	STACK(%rip), %rdi
	movq	%rbx, %rsi
	call	array_append@PLT
.L319:
	movl	$5, %edi
	call	init_value@PLT
	movq	STACK(%rip), %rdi
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	movq	%rax, %rsi
	jmp	array_append@PLT
	.p2align 4,,10
	.p2align 3
.L317:
	.cfi_restore_state
	movq	16(%rbx), %rdi
	call	array_append@PLT
	movq	STACK(%rip), %rdi
	movq	%rbx, %rsi
	popq	%rbx
	.cfi_def_cfa_offset 8
	jmp	array_append@PLT
	.cfi_endproc
.LFE42:
	.size	curry, .-curry
	.p2align 4
	.globl	dip
	.type	dip, @function
dip:
.LFB55:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	movq	STACK(%rip), %rdi
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	call	array_pop@PLT
	testq	%rax, %rax
	je	.L331
	movq	STACK(%rip), %rdi
	movq	%rax, %rbp
	call	array_pop@PLT
	movq	%rax, %r12
	testq	%rax, %rax
	je	.L332
	cmpl	$4, 0(%rbp)
	je	.L333
	movq	%rax, %rdi
	call	eval@PLT
.L327:
	movq	STACK(%rip), %rdi
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r12, %rsi
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	jmp	array_append@PLT
	.p2align 4,,10
	.p2align 3
.L333:
	.cfi_restore_state
	movq	EVAL_STACK(%rip), %rdi
	movq	%rax, %rsi
	call	array_append@PLT
	movq	EVAL_STACK(%rip), %rdi
	movq	%rbp, %rsi
	call	array_append@PLT
	movq	16(%rbp), %rax
	cmpq	$0, 8(%rax)
	je	.L325
	xorl	%ebx, %ebx
	.p2align 4,,10
	.p2align 3
.L326:
	movq	(%rax), %rax
	movq	(%rax,%rbx,8), %rdi
	addq	$1, %rbx
	call	value_copy@PLT
	movq	%rax, %rdi
	call	eval@PLT
	movq	16(%rbp), %rax
	cmpq	8(%rax), %rbx
	jb	.L326
.L325:
	movq	EVAL_STACK(%rip), %rdi
	call	array_pop@PLT
	movq	%rax, %rdi
	call	value_free@PLT
	movq	EVAL_STACK(%rip), %rdi
	call	array_pop@PLT
	movq	%rax, %rdi
	call	value_free@PLT
	jmp	.L327
	.p2align 4,,10
	.p2align 3
.L332:
	movq	STACK(%rip), %rdi
	movq	%rbp, %rsi
	call	array_append@PLT
.L331:
	movl	$5, %edi
	call	init_value@PLT
	movq	STACK(%rip), %rdi
	popq	%rbx
	.cfi_def_cfa_offset 24
	movq	%rax, %rsi
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	jmp	array_append@PLT
	.cfi_endproc
.LFE55:
	.size	dip, .-dip
	.p2align 4
	.globl	compose
	.type	compose, @function
compose:
.LFB68:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	movq	STACK(%rip), %rdi
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	call	array_pop@PLT
	testq	%rax, %rax
	je	.L344
	movq	STACK(%rip), %rdi
	movq	%rax, %rbx
	call	array_pop@PLT
	movq	%rax, %rbp
	testq	%rax, %rax
	je	.L345
	movl	(%rbx), %eax
	cmpl	$3, %eax
	je	.L346
	testl	%eax, %eax
	jne	.L340
	movl	0(%rbp), %eax
	testl	%eax, %eax
	je	.L347
.L338:
	movq	STACK(%rip), %rdi
	movq	%rbp, %rsi
	call	array_append@PLT
.L345:
	movq	STACK(%rip), %rdi
	movq	%rbx, %rsi
	call	array_append@PLT
.L344:
	movl	$5, %edi
	call	init_value@PLT
	movq	STACK(%rip), %rdi
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%rax, %rsi
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	jmp	array_append@PLT
	.p2align 4,,10
	.p2align 3
.L346:
	.cfi_restore_state
	cmpl	$3, 0(%rbp)
	movl	$3, %edi
	jne	.L338
.L342:
	call	init_value@PLT
	movq	16(%rbx), %rsi
	movq	16(%rbp), %rdi
	movq	%rax, %r12
	call	string_concat@PLT
	movq	16(%rbp), %rdi
	call	string_copy@PLT
	movq	%rbp, %rdi
	movq	%r12, %rbp
	movq	%rax, 16(%r12)
	call	value_free@PLT
	movq	%rbx, %rdi
	call	value_free@PLT
.L339:
	movq	STACK(%rip), %rdi
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%rbp, %rsi
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	jmp	array_append@PLT
	.p2align 4,,10
	.p2align 3
.L340:
	.cfi_restore_state
	cmpl	$4, %eax
	jne	.L338
	cmpl	$4, 0(%rbp)
	jne	.L338
	movq	16(%rbx), %rsi
	movq	16(%rbp), %rdi
	call	array_extend@PLT
	movq	16(%rbx), %rdi
	call	free@PLT
	movq	%rbx, %rdi
	call	free@PLT
	jmp	.L339
	.p2align 4,,10
	.p2align 3
.L347:
	xorl	%edi, %edi
	jmp	.L342
	.cfi_endproc
.LFE68:
	.size	compose, .-compose
	.p2align 4
	.globl	stemif
	.type	stemif, @function
stemif:
.LFB59:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	movq	STACK(%rip), %rdi
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	call	array_pop@PLT
	testq	%rax, %rax
	je	.L371
	movq	STACK(%rip), %rdi
	movq	%rax, %rbx
	call	array_pop@PLT
	movq	%rax, %rbp
	testq	%rax, %rax
	je	.L372
	movq	STACK(%rip), %rdi
	call	array_pop@PLT
	movq	%rax, %r12
	testq	%rax, %rax
	je	.L374
	cmpl	$1, (%rax)
	jne	.L376
	fldz
	fldt	16(%rax)
	fucomip	%st(1), %st
	fstp	%st(0)
	jp	.L364
	je	.L353
.L364:
	movq	%rbx, %rdi
	call	value_free@PLT
	movq	%r12, %rdi
	call	value_free@PLT
	cmpl	$4, 0(%rbp)
	je	.L377
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%rbp, %rdi
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	jmp	eval@PLT
	.p2align 4,,10
	.p2align 3
.L376:
	.cfi_restore_state
	movq	STACK(%rip), %rdi
	movq	%rax, %rsi
	call	array_append@PLT
.L374:
	movq	STACK(%rip), %rdi
	movq	%rbp, %rsi
	call	array_append@PLT
.L372:
	movq	STACK(%rip), %rdi
	movq	%rbx, %rsi
	call	array_append@PLT
.L371:
	movl	$5, %edi
	call	init_value@PLT
	movq	STACK(%rip), %rdi
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%rax, %rsi
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	jmp	array_append@PLT
	.p2align 4,,10
	.p2align 3
.L353:
	.cfi_restore_state
	movq	%rbp, %rdi
	call	value_free@PLT
	movq	%r12, %rdi
	call	value_free@PLT
	cmpl	$4, (%rbx)
	je	.L378
	movq	%rbx, %rdi
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	jmp	eval@PLT
	.p2align 4,,10
	.p2align 3
.L377:
	.cfi_restore_state
	movq	EVAL_STACK(%rip), %rdi
	movq	%rbp, %rsi
	call	array_append@PLT
	movq	16(%rbp), %rax
	cmpq	$0, 8(%rax)
	je	.L361
	xorl	%ebx, %ebx
	.p2align 4,,10
	.p2align 3
.L357:
	movq	(%rax), %rax
	movq	(%rax,%rbx,8), %rdi
	addq	$1, %rbx
	call	value_copy@PLT
	movq	%rax, %rdi
	call	eval@PLT
	movq	16(%rbp), %rax
	cmpq	8(%rax), %rbx
	jb	.L357
.L361:
	movq	EVAL_STACK(%rip), %rdi
	call	array_pop@PLT
	testq	%rax, %rax
	je	.L348
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%rax, %rdi
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	jmp	value_free@PLT
	.p2align 4,,10
	.p2align 3
.L378:
	.cfi_restore_state
	movq	EVAL_STACK(%rip), %rdi
	movq	%rbx, %rsi
	call	array_append@PLT
	movq	16(%rbx), %rax
	cmpq	$0, 8(%rax)
	je	.L361
	xorl	%ebp, %ebp
	.p2align 4,,10
	.p2align 3
.L362:
	movq	(%rax), %rax
	movq	(%rax,%rbp,8), %rdi
	addq	$1, %rbp
	call	value_copy@PLT
	movq	%rax, %rdi
	call	eval@PLT
	movq	16(%rbx), %rax
	cmpq	8(%rax), %rbp
	jb	.L362
	jmp	.L361
	.p2align 4,,10
	.p2align 3
.L348:
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE59:
	.size	stemif, .-stemif
	.p2align 4
	.globl	vat
	.type	vat, @function
vat:
.LFB73:
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
	subq	$40, %rsp
	.cfi_def_cfa_offset 80
	movq	STACK(%rip), %rdi
	movq	%fs:40, %rax
	movq	%rax, 24(%rsp)
	xorl	%eax, %eax
	call	array_pop@PLT
	testq	%rax, %rax
	je	.L407
	movq	STACK(%rip), %rdi
	movq	%rax, %rbx
	call	array_pop@PLT
	movq	%rax, %rbp
	testq	%rax, %rax
	je	.L409
	cmpl	$1, (%rax)
	jne	.L392
	movl	(%rbx), %eax
	cmpl	$4, %eax
	je	.L410
	cmpl	$3, %eax
	jne	.L392
	movq	16(%rbx), %rax
	fldt	16(%rbp)
	fildq	(%rax)
	cmpq	$0, (%rax)
	js	.L411
	fxch	%st(1)
.L394:
	fcomi	%st(1), %st
	fstp	%st(1)
	jnb	.L412
	fnstcw	14(%rsp)
	leaq	22(%rsp), %rdi
	movzwl	14(%rsp), %edx
	orb	$12, %dh
	movw	%dx, 12(%rsp)
	movq	16(%rax), %rax
	fldcw	12(%rsp)
	fistpl	8(%rsp)
	fldcw	14(%rsp)
	movslq	8(%rsp), %rdx
	movzbl	(%rax,%rdx), %eax
	movb	$0, 23(%rsp)
	movb	%al, 22(%rsp)
	call	init_string@PLT
	movl	$1, %edi
	movq	%rax, %r13
	call	init_value@PLT
	movq	STACK(%rip), %rdi
	movq	%rbx, %rsi
	movq	%r13, 16(%rax)
	movq	%rax, %r12
	call	array_append@PLT
	movq	STACK(%rip), %rdi
	movq	%r12, %rsi
	call	array_append@PLT
	movq	%rbp, %rdi
	call	value_free@PLT
	movq	24(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L404
	addq	$40, %rsp
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
.L412:
	.cfi_restore_state
	fstp	%st(0)
	.p2align 4,,10
	.p2align 3
.L392:
	movq	STACK(%rip), %rdi
	movq	%rbp, %rsi
.L408:
	call	array_append@PLT
.L409:
	movq	STACK(%rip), %rdi
	movq	%rbx, %rsi
	call	array_append@PLT
.L407:
	movl	$5, %edi
	call	init_value@PLT
	movq	%rax, %rsi
	movq	24(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L404
	movq	STACK(%rip), %rdi
	addq	$40, %rsp
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
	jmp	array_append@PLT
	.p2align 4,,10
	.p2align 3
.L411:
	.cfi_restore_state
	fadds	.LC0(%rip)
	fxch	%st(1)
	jmp	.L394
	.p2align 4,,10
	.p2align 3
.L410:
	movq	16(%rbx), %rax
	movq	STACK(%rip), %rdi
	fildq	8(%rax)
	cmpq	$0, 8(%rax)
	jns	.L388
	fadds	.LC0(%rip)
.L388:
	fldt	16(%rbp)
	movq	%rbp, %rsi
	fcomip	%st(1), %st
	fstp	%st(0)
	jnb	.L408
	movq	%rbx, %rsi
	call	array_append@PLT
	movq	16(%rbx), %rdx
	fldt	16(%rbp)
	fnstcw	14(%rsp)
	movzwl	14(%rsp), %eax
	orb	$12, %ah
	movw	%ax, 12(%rsp)
	movq	(%rdx), %rdx
	fldcw	12(%rsp)
	fistpl	8(%rsp)
	fldcw	14(%rsp)
	movslq	8(%rsp), %rax
	movq	(%rdx,%rax,8), %rdi
	call	value_copy@PLT
	movq	STACK(%rip), %rdi
	movq	%rax, %rsi
	call	array_append@PLT
	movq	24(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L404
	addq	$40, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 40
	movq	%rbp, %rdi
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%rbp
	.cfi_def_cfa_offset 24
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	value_free@PLT
.L404:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE73:
	.size	vat, .-vat
	.section	.rodata.str1.1
.LC12:
	.string	"ret = calloc(MAX, 1)"
.LC13:
	.string	"ret = realloc(ret, len *= 2)"
	.section	.text.unlikely,"ax",@progbits
.LCOLDB14:
	.text
.LHOTB14:
	.p2align 4
	.globl	get_line
	.type	get_line, @function
get_line:
.LFB24:
	.cfi_startproc
	pushq	%r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	movl	$1, %esi
	movq	%rdi, %r13
	movl	$1000, %edi
	pushq	%r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	pushq	%rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	pushq	%rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	subq	$1016, %rsp
	.cfi_def_cfa_offset 1056
	movq	%fs:40, %rax
	movq	%rax, 1000(%rsp)
	xorl	%eax, %eax
	call	calloc@PLT
	testq	%rax, %rax
	je	.L421
	movq	%rax, %rbx
	movl	$1000, %ebp
	movq	%rsp, %r12
	jmp	.L414
	.p2align 4,,10
	.p2align 3
.L417:
	movq	%rbx, %rdi
	call	strlen@PLT
	movq	%rax, %rdx
	movslq	%ebp, %rax
	subq	%rdx, %rax
	cmpq	$999, %rax
	ja	.L415
	addl	%ebp, %ebp
	movq	%rbx, %rdi
	movslq	%ebp, %rsi
	call	realloc@PLT
	movq	%rax, %rbx
	testq	%rax, %rax
	je	.L422
.L415:
	movq	%r12, %rsi
	movq	%rbx, %rdi
	call	strcat@PLT
	movl	$10, %esi
	movq	%rbx, %rdi
	call	strrchr@PLT
	testq	%rax, %rax
	jne	.L424
.L414:
	movq	%r13, %rdx
	movl	$1000, %esi
	movq	%r12, %rdi
	call	fgets@PLT
	testq	%rax, %rax
	jne	.L417
.L413:
	movq	1000(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L425
	addq	$1016, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 40
	movq	%rbx, %rax
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
.L424:
	.cfi_restore_state
	movb	$0, (%rax)
	jmp	.L413
.L425:
	call	__stack_chk_fail@PLT
	.cfi_endproc
	.section	.text.unlikely
	.cfi_startproc
	.type	get_line.cold, @function
get_line.cold:
.LFSB24:
.L422:
	.cfi_def_cfa_offset 1056
	.cfi_offset 3, -40
	.cfi_offset 6, -32
	.cfi_offset 12, -24
	.cfi_offset 13, -16
	leaq	.LC13(%rip), %rdi
	call	perror@PLT
	movl	$1, %edi
	call	exit@PLT
.L421:
	leaq	.LC12(%rip), %rdi
	call	perror@PLT
	movl	$1, %edi
	call	exit@PLT
	.cfi_endproc
.LFE24:
	.text
	.size	get_line, .-get_line
	.section	.text.unlikely
	.size	get_line.cold, .-get_line.cold
.LCOLDE14:
	.text
.LHOTE14:
	.p2align 4
	.globl	stemread
	.type	stemread, @function
stemread:
.LFB44:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movl	$3, %edi
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	call	init_value@PLT
	movq	stdin(%rip), %rdi
	movq	%rax, %rbp
	call	get_line
	movq	%rax, %rdi
	movq	%rax, %rbx
	call	init_string@PLT
	movq	STACK(%rip), %rdi
	movq	%rbp, %rsi
	movq	%rax, 16(%rbp)
	call	array_append@PLT
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	movq	%rbx, %rdi
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	free@PLT
	.cfi_endproc
.LFE44:
	.size	stemread, .-stemread
	.section	.rodata.str1.1
.LC15:
	.string	"%.0Lf\n"
.LC16:
	.string	"%Lf\n"
.LC17:
	.string	"%s"
.LC18:
	.string	"W: %s\n"
.LC19:
	.string	"Q:"
.LC20:
	.string	"STACK ERR"
	.text
	.p2align 4
	.globl	print_value
	.type	print_value, @function
print_value:
.LFB25:
	.cfi_startproc
	cmpl	$6, (%rdi)
	ja	.L444
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	leaq	.L431(%rip), %rdx
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rdi, %rbx
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movl	(%rdi), %eax
	movslq	(%rdx,%rax,4), %rax
	addq	%rdx, %rax
	jmp	*%rax
	.section	.rodata
	.align 4
	.align 4
.L431:
	.long	.L437-.L431
	.long	.L436-.L431
	.long	.L435-.L431
	.long	.L434-.L431
	.long	.L433-.L431
	.long	.L432-.L431
	.long	.L430-.L431
	.text
	.p2align 4,,10
	.p2align 3
.L432:
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	leaq	.LC20(%rip), %rdi
	popq	%rbx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	jmp	puts@PLT
	.p2align 4,,10
	.p2align 3
.L430:
	.cfi_restore_state
	movq	16(%rdi), %rsi
	movq	OBJ_TABLE(%rip), %rdi
	call	ht_get@PLT
	movq	32(%rbx), %rdi
	movq	(%rax), %rax
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	jmp	*%rax
	.p2align 4,,10
	.p2align 3
.L437:
	.cfi_restore_state
	movq	16(%rdi), %rax
	leaq	.LC18(%rip), %rdi
	movq	16(%rax), %rsi
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	xorl	%eax, %eax
	popq	%rbx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
	.p2align 4,,10
	.p2align 3
.L436:
	.cfi_restore_state
	pushq	24(%rdi)
	.cfi_def_cfa_offset 40
	xorl	%eax, %eax
	pushq	16(%rdi)
	.cfi_def_cfa_offset 48
	leaq	.LC15(%rip), %rdi
	call	printf@PLT
	popq	%rcx
	.cfi_def_cfa_offset 40
	popq	%rsi
	.cfi_def_cfa_offset 32
.L428:
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
.L435:
	.cfi_restore_state
	pushq	24(%rdi)
	.cfi_def_cfa_offset 40
	xorl	%eax, %eax
	pushq	16(%rdi)
	.cfi_def_cfa_offset 48
	leaq	.LC16(%rip), %rdi
	call	printf@PLT
	popq	%rax
	.cfi_def_cfa_offset 40
	popq	%rdx
	.cfi_def_cfa_offset 32
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
.L434:
	.cfi_restore_state
	movq	16(%rdi), %rax
	leaq	.LC17(%rip), %rdi
	movq	16(%rax), %rsi
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	xorl	%eax, %eax
	popq	%rbx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
	.p2align 4,,10
	.p2align 3
.L433:
	.cfi_restore_state
	leaq	.LC19(%rip), %rdi
	xorl	%ebp, %ebp
	call	puts@PLT
	movq	16(%rbx), %rax
	cmpq	$0, 8(%rax)
	je	.L428
.L438:
	movq	(%rax), %rax
	movq	(%rax,%rbp,8), %rdi
	addq	$1, %rbp
	call	print_value
	movq	16(%rbx), %rax
	cmpq	8(%rax), %rbp
	jb	.L438
	jmp	.L428
.L444:
	.cfi_def_cfa_offset 8
	.cfi_restore 3
	.cfi_restore 6
	ret
	.cfi_endproc
.LFE25:
	.size	print_value, .-print_value
	.p2align 4
	.globl	questionmark
	.type	questionmark, @function
questionmark:
.LFB52:
	.cfi_startproc
	movq	STACK(%rip), %rax
	movq	8(%rax), %rdx
	testq	%rdx, %rdx
	je	.L468
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	leaq	.LC20(%rip), %r15
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	leaq	.LC19(%rip), %r14
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	leaq	.L452(%rip), %r12
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	xorl	%ebp, %ebp
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$8, %rsp
	.cfi_def_cfa_offset 64
	.p2align 4,,10
	.p2align 3
.L449:
	movq	(%rax), %rcx
	movq	(%rcx,%rbp,8), %rbx
	cmpl	$6, (%rbx)
	ja	.L450
	movl	(%rbx), %eax
	movslq	(%r12,%rax,4), %rax
	addq	%r12, %rax
	jmp	*%rax
	.section	.rodata
	.align 4
	.align 4
.L452:
	.long	.L458-.L452
	.long	.L457-.L452
	.long	.L456-.L452
	.long	.L455-.L452
	.long	.L454-.L452
	.long	.L453-.L452
	.long	.L451-.L452
	.text
	.p2align 4,,10
	.p2align 3
.L453:
	movq	%r15, %rdi
	call	puts@PLT
	movq	STACK(%rip), %rax
	movq	8(%rax), %rdx
	.p2align 4,,10
	.p2align 3
.L450:
	addq	$1, %rbp
	cmpq	%rdx, %rbp
	jb	.L449
	addq	$8, %rsp
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
.L454:
	.cfi_restore_state
	movq	%r14, %rdi
	xorl	%r13d, %r13d
	call	puts@PLT
	movq	16(%rbx), %rdx
	cmpq	$0, 8(%rdx)
	je	.L472
.L459:
	movq	(%rdx), %rdx
	movq	(%rdx,%r13,8), %rdi
	addq	$1, %r13
	call	print_value
	movq	16(%rbx), %rdx
	cmpq	8(%rdx), %r13
	jb	.L459
	jmp	.L472
	.p2align 4,,10
	.p2align 3
.L455:
	movq	16(%rbx), %rax
	leaq	.LC17(%rip), %rdi
	movq	16(%rax), %rsi
	xorl	%eax, %eax
	call	printf@PLT
	movq	STACK(%rip), %rax
	movq	8(%rax), %rdx
	jmp	.L450
	.p2align 4,,10
	.p2align 3
.L456:
	pushq	24(%rbx)
	.cfi_def_cfa_offset 72
	leaq	.LC16(%rip), %rdi
	pushq	16(%rbx)
	.cfi_def_cfa_offset 80
.L471:
	xorl	%eax, %eax
	call	printf@PLT
	movq	STACK(%rip), %rax
	popq	%rcx
	.cfi_def_cfa_offset 72
	popq	%rsi
	.cfi_def_cfa_offset 64
	movq	8(%rax), %rdx
	jmp	.L450
	.p2align 4,,10
	.p2align 3
.L457:
	pushq	24(%rbx)
	.cfi_remember_state
	.cfi_def_cfa_offset 72
	leaq	.LC15(%rip), %rdi
	pushq	16(%rbx)
	.cfi_def_cfa_offset 80
	jmp	.L471
	.p2align 4,,10
	.p2align 3
.L451:
	.cfi_restore_state
	movq	OBJ_TABLE(%rip), %rdi
	movq	16(%rbx), %rsi
	call	ht_get@PLT
	movq	32(%rbx), %rdi
	call	*(%rax)
.L472:
	movq	STACK(%rip), %rax
	movq	8(%rax), %rdx
	jmp	.L450
	.p2align 4,,10
	.p2align 3
.L458:
	movq	16(%rbx), %rax
	leaq	.LC18(%rip), %rdi
	movq	16(%rax), %rsi
	xorl	%eax, %eax
	call	printf@PLT
	movq	STACK(%rip), %rax
	movq	8(%rax), %rdx
	jmp	.L450
	.p2align 4,,10
	.p2align 3
.L468:
	.cfi_def_cfa_offset 8
	.cfi_restore 3
	.cfi_restore 6
	.cfi_restore 12
	.cfi_restore 13
	.cfi_restore 14
	.cfi_restore 15
	ret
	.cfi_endproc
.LFE52:
	.size	questionmark, .-questionmark
	.p2align 4
	.globl	period
	.type	period, @function
period:
.LFB53:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movq	STACK(%rip), %rdi
	call	array_pop@PLT
	testq	%rax, %rax
	je	.L490
	cmpl	$6, (%rax)
	movq	%rax, %rbx
	ja	.L475
	movl	(%rax), %eax
	leaq	.L477(%rip), %rdx
	movslq	(%rdx,%rax,4), %rax
	addq	%rdx, %rax
	jmp	*%rax
	.section	.rodata
	.align 4
	.align 4
.L477:
	.long	.L483-.L477
	.long	.L482-.L477
	.long	.L481-.L477
	.long	.L480-.L477
	.long	.L479-.L477
	.long	.L478-.L477
	.long	.L476-.L477
	.text
	.p2align 4,,10
	.p2align 3
.L476:
	movq	OBJ_TABLE(%rip), %rdi
	movq	16(%rbx), %rsi
	call	ht_get@PLT
	movq	32(%rbx), %rdi
	call	*(%rax)
.L475:
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%rbx, %rdi
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	value_free@PLT
	.p2align 4,,10
	.p2align 3
.L478:
	.cfi_restore_state
	leaq	.LC20(%rip), %rdi
	call	puts@PLT
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%rbx, %rdi
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	value_free@PLT
	.p2align 4,,10
	.p2align 3
.L483:
	.cfi_restore_state
	movq	16(%rbx), %rax
	leaq	.LC18(%rip), %rdi
	movq	16(%rax), %rsi
	xorl	%eax, %eax
	call	printf@PLT
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%rbx, %rdi
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	value_free@PLT
	.p2align 4,,10
	.p2align 3
.L482:
	.cfi_restore_state
	pushq	24(%rbx)
	.cfi_def_cfa_offset 40
	leaq	.LC15(%rip), %rdi
	xorl	%eax, %eax
	pushq	16(%rbx)
	.cfi_def_cfa_offset 48
	call	printf@PLT
	popq	%rcx
	.cfi_def_cfa_offset 40
	popq	%rsi
	.cfi_def_cfa_offset 32
	movq	%rbx, %rdi
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	value_free@PLT
	.p2align 4,,10
	.p2align 3
.L481:
	.cfi_restore_state
	pushq	24(%rbx)
	.cfi_def_cfa_offset 40
	leaq	.LC16(%rip), %rdi
	xorl	%eax, %eax
	pushq	16(%rbx)
	.cfi_def_cfa_offset 48
	call	printf@PLT
	popq	%rax
	.cfi_def_cfa_offset 40
	popq	%rdx
	.cfi_def_cfa_offset 32
	movq	%rbx, %rdi
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	value_free@PLT
	.p2align 4,,10
	.p2align 3
.L480:
	.cfi_restore_state
	movq	16(%rbx), %rax
	leaq	.LC17(%rip), %rdi
	movq	16(%rax), %rsi
	xorl	%eax, %eax
	call	printf@PLT
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%rbx, %rdi
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	value_free@PLT
	.p2align 4,,10
	.p2align 3
.L479:
	.cfi_restore_state
	leaq	.LC19(%rip), %rdi
	xorl	%ebp, %ebp
	call	puts@PLT
	movq	16(%rbx), %rax
	cmpq	$0, 8(%rax)
	je	.L475
.L484:
	movq	(%rax), %rax
	movq	(%rax,%rbp,8), %rdi
	addq	$1, %rbp
	call	print_value
	movq	16(%rbx), %rax
	cmpq	8(%rax), %rbp
	jb	.L484
	jmp	.L475
	.p2align 4,,10
	.p2align 3
.L490:
	movl	$5, %edi
	call	init_value@PLT
	movq	STACK(%rip), %rdi
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	movq	%rax, %rsi
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	array_append@PLT
	.cfi_endproc
.LFE53:
	.size	period, .-period
	.p2align 4
	.globl	eval_error
	.type	eval_error, @function
eval_error:
.LFB26:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movl	$5, %edi
	call	init_value@PLT
	movq	STACK(%rip), %rdi
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	movq	%rax, %rsi
	jmp	array_append@PLT
	.cfi_endproc
.LFE26:
	.size	eval_error, .-eval_error
	.section	.rodata.str1.1
.LC21:
	.string	"rb"
	.text
	.p2align 4
	.globl	stemfread
	.type	stemfread, @function
stemfread:
.LFB43:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$40, %rsp
	.cfi_def_cfa_offset 64
	movq	STACK(%rip), %rdi
	movq	%fs:40, %rax
	movq	%rax, 24(%rsp)
	xorl	%eax, %eax
	call	array_pop@PLT
	testq	%rax, %rax
	je	.L504
	cmpl	$3, (%rax)
	movq	%rax, %rbx
	jne	.L505
	movq	16(%rax), %rax
	leaq	.LC21(%rip), %rsi
	movq	$0, 8(%rsp)
	movq	16(%rax), %rdi
	call	fopen@PLT
	movq	%rax, %rbp
	testq	%rax, %rax
	je	.L506
	movq	%rax, %rcx
	leaq	16(%rsp), %rsi
	leaq	8(%rsp), %rdi
	xorl	%edx, %edx
	call	getdelim@PLT
	movq	%rbp, %rdi
	call	fclose@PLT
	movl	$3, %edi
	call	init_value@PLT
	movq	8(%rsp), %rdi
	movq	%rax, %rbp
	call	init_string@PLT
	movq	STACK(%rip), %rdi
	movq	%rbp, %rsi
	movq	%rax, 16(%rbp)
	call	array_append@PLT
	movq	%rbx, %rdi
	call	value_free@PLT
	movq	8(%rsp), %rdi
	call	free@PLT
.L493:
	movq	24(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L503
	addq	$40, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L505:
	.cfi_restore_state
	movq	STACK(%rip), %rdi
	movq	%rax, %rsi
	call	array_append@PLT
.L504:
	movl	$5, %edi
	call	init_value@PLT
	movq	%rax, %rsi
	movq	24(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L503
	movq	STACK(%rip), %rdi
	addq	$40, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	array_append@PLT
.L503:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
.L506:
	movq	STACK(%rip), %rdi
	movq	%rbx, %rsi
	call	array_append@PLT
	xorl	%eax, %eax
	call	eval_error
	jmp	.L493
	.cfi_endproc
.LFE43:
	.size	stemfread, .-stemfread
	.section	.rodata.str1.1
.LC22:
	.string	"w+"
	.text
	.p2align 4
	.globl	stemfwrite
	.type	stemfwrite, @function
stemfwrite:
.LFB74:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movq	STACK(%rip), %rdi
	call	array_pop@PLT
	testq	%rax, %rax
	je	.L512
	cmpl	$3, (%rax)
	movq	%rax, %rbx
	jne	.L513
	movq	16(%rax), %rax
	leaq	.LC22(%rip), %rsi
	movq	16(%rax), %rdi
	call	fopen@PLT
	movq	%rax, %rbp
	testq	%rax, %rax
	je	.L514
	movq	16(%rbx), %rax
	movq	%rbp, %rsi
	movq	16(%rax), %rdi
	call	fputs@PLT
	movq	%rbx, %rdi
	call	value_free@PLT
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%rbp, %rdi
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	fclose@PLT
	.p2align 4,,10
	.p2align 3
.L513:
	.cfi_restore_state
	movq	STACK(%rip), %rdi
	movq	%rax, %rsi
	call	array_append@PLT
.L512:
	movl	$5, %edi
	call	init_value@PLT
	movq	STACK(%rip), %rdi
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	movq	%rax, %rsi
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	array_append@PLT
.L514:
	.cfi_restore_state
	movq	STACK(%rip), %rdi
	movq	%rbx, %rsi
	call	array_append@PLT
	popq	%rdx
	.cfi_def_cfa_offset 24
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	eval_error
	.cfi_endproc
.LFE74:
	.size	stemfwrite, .-stemfwrite
	.p2align 4
	.globl	del
	.type	del, @function
del:
.LFB57:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	STACK(%rip), %rdi
	call	array_pop@PLT
	testq	%rax, %rax
	je	.L524
	movq	STACK(%rip), %rdi
	movq	%rax, %rbx
	call	array_pop@PLT
	movq	%rax, %rsi
	testq	%rax, %rax
	je	.L525
	cmpl	$1, (%rbx)
	jne	.L526
	movl	(%rax), %eax
	testl	%eax, %eax
	je	.L515
	subl	$3, %eax
	cmpl	$1, %eax
	ja	.L527
.L515:
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L526:
	.cfi_restore_state
	movq	STACK(%rip), %rdi
	call	array_append@PLT
.L525:
	movq	STACK(%rip), %rdi
	movq	%rbx, %rsi
	call	array_append@PLT
.L524:
	movl	$5, %edi
	call	init_value@PLT
	movq	STACK(%rip), %rdi
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	movq	%rax, %rsi
	jmp	array_append@PLT
	.p2align 4,,10
	.p2align 3
.L527:
	.cfi_restore_state
	movq	%rsi, %rdi
	popq	%rbx
	.cfi_def_cfa_offset 8
	jmp	value_free@PLT
	.cfi_endproc
.LFE57:
	.size	del, .-del
	.p2align 4
	.globl	wtostr
	.type	wtostr, @function
wtostr:
.LFB67:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movq	STACK(%rip), %rdi
	call	array_pop@PLT
	testq	%rax, %rax
	je	.L532
	movq	%rax, %rsi
	movl	(%rax), %eax
	movq	STACK(%rip), %rdi
	testl	%eax, %eax
	jne	.L533
	movl	$3, (%rsi)
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	jmp	array_append@PLT
	.p2align 4,,10
	.p2align 3
.L533:
	.cfi_restore_state
	call	array_append@PLT
.L532:
	movl	$5, %edi
	call	init_value@PLT
	movq	STACK(%rip), %rdi
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	movq	%rax, %rsi
	jmp	array_append@PLT
	.cfi_endproc
.LFE67:
	.size	wtostr, .-wtostr
	.p2align 4
	.globl	add_func
	.type	add_func, @function
add_func:
.LFB75:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsi, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rdi, %rbx
	movq	%rdx, %rdi
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	call	init_string@PLT
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	movq	%rbp, %rdx
	movq	%rbx, %rdi
	movq	%rax, %rsi
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	ht_add@PLT
	.cfi_endproc
.LFE75:
	.size	add_func, .-add_func
	.p2align 4
	.globl	add_obj
	.type	add_obj, @function
add_obj:
.LFB76:
	.cfi_startproc
	pushq	%r14
	.cfi_def_cfa_offset 16
	.cfi_offset 14, -16
	pushq	%r13
	.cfi_def_cfa_offset 24
	.cfi_offset 13, -24
	pushq	%r12
	.cfi_def_cfa_offset 32
	.cfi_offset 12, -32
	movq	%rdi, %r12
	movq	%rdx, %rdi
	movq	%r8, %rdx
	pushq	%rbp
	.cfi_def_cfa_offset 40
	.cfi_offset 6, -40
	movq	%r9, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
	movq	48(%rsp), %r13
	movq	%rsi, %rbx
	movq	%rcx, %rsi
	call	init_custom@PLT
	movq	%r13, %rdi
	movq	%rax, %r14
	call	init_string@PLT
	movq	%r14, %rdx
	movq	%r12, %rdi
	movq	%rax, %rsi
	call	ht_add@PLT
	movq	%r13, %rdi
	call	init_string@PLT
	movq	%rbp, %rdx
	movq	%rbx, %rdi
	popq	%rbx
	.cfi_def_cfa_offset 40
	movq	%rax, %rsi
	popq	%rbp
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r13
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	jmp	ht_add@PLT
	.cfi_endproc
.LFE76:
	.size	add_obj, .-add_obj
	.p2align 4
	.globl	add_objs
	.type	add_objs, @function
add_objs:
.LFB77:
	.cfi_startproc
	ret
	.cfi_endproc
.LFE77:
	.size	add_objs, .-add_objs
	.section	.rodata.str1.1
.LC23:
	.string	"."
.LC24:
	.string	"?"
.LC25:
	.string	"+"
.LC26:
	.string	"-"
.LC27:
	.string	"/"
.LC28:
	.string	"*"
.LC29:
	.string	"pow"
.LC30:
	.string	"sin"
.LC31:
	.string	"cos"
.LC32:
	.string	"exp"
.LC33:
	.string	"strquote"
.LC34:
	.string	"eval"
.LC35:
	.string	"func"
.LC36:
	.string	"nop"
.LC37:
	.string	"ln"
.LC38:
	.string	"floor"
.LC39:
	.string	"ceil"
.LC40:
	.string	"fread"
.LC41:
	.string	"read"
.LC42:
	.string	"vat"
.LC43:
	.string	"qstack"
.LC44:
	.string	"ssize"
.LC45:
	.string	"stoi"
.LC46:
	.string	"isnum"
.LC47:
	.string	"compose"
.LC48:
	.string	"wtostr"
.LC49:
	.string	"<"
.LC50:
	.string	"<="
.LC51:
	.string	">"
.LC52:
	.string	">="
.LC53:
	.string	"!="
.LC54:
	.string	"="
.LC55:
	.string	"if"
.LC56:
	.string	"clear"
.LC57:
	.string	"keep"
.LC58:
	.string	"dip"
.LC59:
	.string	"len"
.LC60:
	.string	"quote"
.LC61:
	.string	"exit"
.LC62:
	.string	"type"
.LC63:
	.string	"dup"
.LC64:
	.string	"curry"
.LC65:
	.string	"swap"
.LC66:
	.string	"isdef"
.LC67:
	.string	"dsc"
.LC68:
	.string	"clib"
	.text
	.p2align 4
	.globl	add_funcs
	.type	add_funcs, @function
add_funcs:
.LFB78:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	leaq	.LC23(%rip), %rdi
	movq	FLIT(%rip), %rbx
	call	init_string@PLT
	leaq	period(%rip), %rdx
	movq	%rbx, %rdi
	movq	%rax, %rsi
	call	ht_add@PLT
	movq	FLIT(%rip), %rbx
	leaq	.LC24(%rip), %rdi
	call	init_string@PLT
	leaq	questionmark(%rip), %rdx
	movq	%rax, %rsi
	movq	%rbx, %rdi
	call	ht_add@PLT
	movq	FLIT(%rip), %rbx
	leaq	.LC25(%rip), %rdi
	call	init_string@PLT
	leaq	stemadd(%rip), %rdx
	movq	%rax, %rsi
	movq	%rbx, %rdi
	call	ht_add@PLT
	movq	FLIT(%rip), %rbx
	leaq	.LC26(%rip), %rdi
	call	init_string@PLT
	leaq	stemsub(%rip), %rdx
	movq	%rax, %rsi
	movq	%rbx, %rdi
	call	ht_add@PLT
	movq	FLIT(%rip), %rbx
	leaq	.LC27(%rip), %rdi
	call	init_string@PLT
	leaq	stemdiv(%rip), %rdx
	movq	%rax, %rsi
	movq	%rbx, %rdi
	call	ht_add@PLT
	movq	FLIT(%rip), %rbx
	leaq	.LC28(%rip), %rdi
	call	init_string@PLT
	leaq	stemmul(%rip), %rdx
	movq	%rax, %rsi
	movq	%rbx, %rdi
	call	ht_add@PLT
	movq	FLIT(%rip), %rbx
	leaq	.LC29(%rip), %rdi
	call	init_string@PLT
	leaq	stempow(%rip), %rdx
	movq	%rax, %rsi
	movq	%rbx, %rdi
	call	ht_add@PLT
	movq	FLIT(%rip), %rbx
	leaq	.LC30(%rip), %rdi
	call	init_string@PLT
	leaq	stemsin(%rip), %rdx
	movq	%rax, %rsi
	movq	%rbx, %rdi
	call	ht_add@PLT
	movq	FLIT(%rip), %rbx
	leaq	.LC31(%rip), %rdi
	call	init_string@PLT
	leaq	stemcos(%rip), %rdx
	movq	%rax, %rsi
	movq	%rbx, %rdi
	call	ht_add@PLT
	movq	FLIT(%rip), %rbx
	leaq	.LC32(%rip), %rdi
	call	init_string@PLT
	leaq	stemexp(%rip), %rdx
	movq	%rax, %rsi
	movq	%rbx, %rdi
	call	ht_add@PLT
	movq	FLIT(%rip), %rbx
	leaq	.LC33(%rip), %rdi
	call	init_string@PLT
	leaq	strquote(%rip), %rdx
	movq	%rax, %rsi
	movq	%rbx, %rdi
	call	ht_add@PLT
	movq	FLIT(%rip), %rbx
	leaq	.LC34(%rip), %rdi
	call	init_string@PLT
	leaq	stemeval(%rip), %rdx
	movq	%rax, %rsi
	movq	%rbx, %rdi
	call	ht_add@PLT
	movq	FLIT(%rip), %rbx
	leaq	.LC35(%rip), %rdi
	call	init_string@PLT
	leaq	stemfunc(%rip), %rdx
	movq	%rax, %rsi
	movq	%rbx, %rdi
	call	ht_add@PLT
	movq	FLIT(%rip), %rbx
	leaq	.LC36(%rip), %rdi
	call	init_string@PLT
	leaq	nop(%rip), %rdx
	movq	%rax, %rsi
	movq	%rbx, %rdi
	call	ht_add@PLT
	movq	FLIT(%rip), %rbx
	leaq	.LC37(%rip), %rdi
	call	init_string@PLT
	leaq	stemln(%rip), %rdx
	movq	%rax, %rsi
	movq	%rbx, %rdi
	call	ht_add@PLT
	movq	FLIT(%rip), %rbx
	leaq	.LC38(%rip), %rdi
	call	init_string@PLT
	leaq	stemfloor(%rip), %rdx
	movq	%rax, %rsi
	movq	%rbx, %rdi
	call	ht_add@PLT
	movq	FLIT(%rip), %rbx
	leaq	.LC39(%rip), %rdi
	call	init_string@PLT
	leaq	stemceil(%rip), %rdx
	movq	%rax, %rsi
	movq	%rbx, %rdi
	call	ht_add@PLT
	movq	FLIT(%rip), %rbx
	leaq	.LC40(%rip), %rdi
	call	init_string@PLT
	leaq	stemfread(%rip), %rdx
	movq	%rax, %rsi
	movq	%rbx, %rdi
	call	ht_add@PLT
	movq	FLIT(%rip), %rbx
	leaq	.LC41(%rip), %rdi
	call	init_string@PLT
	leaq	stemread(%rip), %rdx
	movq	%rax, %rsi
	movq	%rbx, %rdi
	call	ht_add@PLT
	movq	FLIT(%rip), %rbx
	leaq	.LC42(%rip), %rdi
	call	init_string@PLT
	leaq	vat(%rip), %rdx
	movq	%rax, %rsi
	movq	%rbx, %rdi
	call	ht_add@PLT
	movq	FLIT(%rip), %rbx
	leaq	.LC43(%rip), %rdi
	call	init_string@PLT
	leaq	qstack(%rip), %rdx
	movq	%rax, %rsi
	movq	%rbx, %rdi
	call	ht_add@PLT
	movq	FLIT(%rip), %rbx
	leaq	.LC44(%rip), %rdi
	call	init_string@PLT
	leaq	ssize(%rip), %rdx
	movq	%rax, %rsi
	movq	%rbx, %rdi
	call	ht_add@PLT
	movq	FLIT(%rip), %rbx
	leaq	.LC45(%rip), %rdi
	call	init_string@PLT
	leaq	stoi(%rip), %rdx
	movq	%rax, %rsi
	movq	%rbx, %rdi
	call	ht_add@PLT
	movq	FLIT(%rip), %rbx
	leaq	.LC46(%rip), %rdi
	call	init_string@PLT
	leaq	isnum(%rip), %rdx
	movq	%rax, %rsi
	movq	%rbx, %rdi
	call	ht_add@PLT
	movq	FLIT(%rip), %rbx
	leaq	.LC47(%rip), %rdi
	call	init_string@PLT
	leaq	compose(%rip), %rdx
	movq	%rax, %rsi
	movq	%rbx, %rdi
	call	ht_add@PLT
	movq	FLIT(%rip), %rbx
	leaq	.LC48(%rip), %rdi
	call	init_string@PLT
	leaq	stemfwrite(%rip), %rdx
	movq	%rax, %rsi
	movq	%rbx, %rdi
	call	ht_add@PLT
	movq	FLIT(%rip), %rbx
	leaq	.LC49(%rip), %rdi
	call	init_string@PLT
	leaq	lthan(%rip), %rdx
	movq	%rax, %rsi
	movq	%rbx, %rdi
	call	ht_add@PLT
	movq	FLIT(%rip), %rbx
	leaq	.LC50(%rip), %rdi
	call	init_string@PLT
	leaq	ltequals(%rip), %rdx
	movq	%rax, %rsi
	movq	%rbx, %rdi
	call	ht_add@PLT
	movq	FLIT(%rip), %rbx
	leaq	.LC51(%rip), %rdi
	call	init_string@PLT
	leaq	gthan(%rip), %rdx
	movq	%rax, %rsi
	movq	%rbx, %rdi
	call	ht_add@PLT
	movq	FLIT(%rip), %rbx
	leaq	.LC52(%rip), %rdi
	call	init_string@PLT
	leaq	gtequals(%rip), %rdx
	movq	%rax, %rsi
	movq	%rbx, %rdi
	call	ht_add@PLT
	movq	FLIT(%rip), %rbx
	leaq	.LC53(%rip), %rdi
	call	init_string@PLT
	leaq	nequals(%rip), %rdx
	movq	%rax, %rsi
	movq	%rbx, %rdi
	call	ht_add@PLT
	movq	FLIT(%rip), %rbx
	leaq	.LC54(%rip), %rdi
	call	init_string@PLT
	leaq	equals(%rip), %rdx
	movq	%rax, %rsi
	movq	%rbx, %rdi
	call	ht_add@PLT
	movq	FLIT(%rip), %rbx
	leaq	.LC55(%rip), %rdi
	call	init_string@PLT
	leaq	stemif(%rip), %rdx
	movq	%rax, %rsi
	movq	%rbx, %rdi
	call	ht_add@PLT
	movq	FLIT(%rip), %rbx
	leaq	.LC56(%rip), %rdi
	call	init_string@PLT
	leaq	clear(%rip), %rdx
	movq	%rax, %rsi
	movq	%rbx, %rdi
	call	ht_add@PLT
	movq	FLIT(%rip), %rbx
	leaq	.LC57(%rip), %rdi
	call	init_string@PLT
	leaq	keep(%rip), %rdx
	movq	%rax, %rsi
	movq	%rbx, %rdi
	call	ht_add@PLT
	movq	FLIT(%rip), %rbx
	leaq	.LC58(%rip), %rdi
	call	init_string@PLT
	leaq	dip(%rip), %rdx
	movq	%rax, %rsi
	movq	%rbx, %rdi
	call	ht_add@PLT
	movq	FLIT(%rip), %rbx
	leaq	.LC59(%rip), %rdi
	call	init_string@PLT
	leaq	stemlen(%rip), %rdx
	movq	%rax, %rsi
	movq	%rbx, %rdi
	call	ht_add@PLT
	movq	FLIT(%rip), %rbx
	leaq	.LC60(%rip), %rdi
	call	init_string@PLT
	leaq	quote(%rip), %rdx
	movq	%rax, %rsi
	movq	%rbx, %rdi
	call	ht_add@PLT
	movq	FLIT(%rip), %rbx
	leaq	.LC61(%rip), %rdi
	call	init_string@PLT
	leaq	stemexit(%rip), %rdx
	movq	%rax, %rsi
	movq	%rbx, %rdi
	call	ht_add@PLT
	movq	FLIT(%rip), %rbx
	leaq	.LC62(%rip), %rdi
	call	init_string@PLT
	leaq	stemtype(%rip), %rdx
	movq	%rax, %rsi
	movq	%rbx, %rdi
	call	ht_add@PLT
	movq	FLIT(%rip), %rbx
	leaq	.LC63(%rip), %rdi
	call	init_string@PLT
	leaq	stemdup(%rip), %rdx
	movq	%rax, %rsi
	movq	%rbx, %rdi
	call	ht_add@PLT
	movq	FLIT(%rip), %rbx
	leaq	.LC64(%rip), %rdi
	call	init_string@PLT
	leaq	curry(%rip), %rdx
	movq	%rax, %rsi
	movq	%rbx, %rdi
	call	ht_add@PLT
	movq	FLIT(%rip), %rbx
	leaq	.LC65(%rip), %rdi
	call	init_string@PLT
	leaq	swap(%rip), %rdx
	movq	%rax, %rsi
	movq	%rbx, %rdi
	call	ht_add@PLT
	movq	FLIT(%rip), %rbx
	leaq	.LC66(%rip), %rdi
	call	init_string@PLT
	leaq	isdef(%rip), %rdx
	movq	%rax, %rsi
	movq	%rbx, %rdi
	call	ht_add@PLT
	movq	FLIT(%rip), %rbx
	leaq	.LC67(%rip), %rdi
	call	init_string@PLT
	leaq	dsc(%rip), %rdx
	movq	%rax, %rsi
	movq	%rbx, %rdi
	call	ht_add@PLT
	movq	FLIT(%rip), %rbx
	leaq	.LC68(%rip), %rdi
	call	init_string@PLT
	leaq	clib(%rip), %rdx
	movq	%rbx, %rdi
	movq	%rax, %rsi
	popq	%rbx
	.cfi_def_cfa_offset 8
	jmp	ht_add@PLT
	.cfi_endproc
.LFE78:
	.size	add_funcs, .-add_funcs
	.section	.rodata.cst4,"aM",@progbits,4
	.align 4
.LC0:
	.long	1602224128
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC2:
	.long	0
	.long	1127219200
	.align 8
.LC4:
	.long	0
	.long	1072693248
	.align 8
.LC5:
	.long	-1
	.long	2147483647
	.ident	"GCC: (GNU) 13.2.1 20230801"
	.section	.note.GNU-stack,"",@progbits
