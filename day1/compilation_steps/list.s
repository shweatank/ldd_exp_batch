	.file	"list.c"
	.text
	.section	.rodata
.LC0:
	.string	"Enter the length list: "
.LC1:
	.string	"%d"
.LC2:
	.string	"enter data: "
.LC3:
	.string	"Printing the list : "
.LC4:
	.string	"%d "
.LC5:
	.string	"%d\n"
.LC6:
	.string	"c %d\n"
.LC7:
	.string	"Printing the reversed list : "
	.text
	.globl	main
	.type	main, @function
main:
.LFB6:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$96, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movl	$0, -84(%rbp)
	movl	$0, -80(%rbp)
	movq	$0, -56(%rbp)
	movq	$0, -48(%rbp)
	movq	$0, -40(%rbp)
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	-84(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC1(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	__isoc99_scanf@PLT
	movl	$0, -76(%rbp)
	jmp	.L2
.L5:
	movl	$16, %edi
	call	malloc@PLT
	movq	%rax, -48(%rbp)
	leaq	.LC2(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	-80(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC1(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	__isoc99_scanf@PLT
	movl	-80(%rbp), %edx
	movq	-48(%rbp), %rax
	movl	%edx, (%rax)
	cmpl	$0, -76(%rbp)
	jne	.L3
	movq	-48(%rbp), %rax
	movq	%rax, -56(%rbp)
	movq	-48(%rbp), %rax
	movq	%rax, -40(%rbp)
	jmp	.L4
.L3:
	movq	-40(%rbp), %rax
	movq	-48(%rbp), %rdx
	movq	%rdx, 8(%rax)
	movq	-48(%rbp), %rax
	movq	%rax, -40(%rbp)
.L4:
	addl	$1, -76(%rbp)
.L2:
	movl	-84(%rbp), %eax
	cmpl	%eax, -76(%rbp)
	jl	.L5
	movq	-48(%rbp), %rax
	movq	$0, 8(%rax)
	movq	-56(%rbp), %rax
	movq	%rax, -48(%rbp)
	leaq	.LC3(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
.L6:
	movq	-48(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, %esi
	leaq	.LC4(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	-48(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, -48(%rbp)
	movq	-48(%rbp), %rax
	movq	8(%rax), %rax
	testq	%rax, %rax
	jne	.L6
	movq	-48(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, %esi
	leaq	.LC5(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	-56(%rbp), %rax
	movq	%rax, -48(%rbp)
	movl	$0, -72(%rbp)
	jmp	.L7
.L8:
	movq	-48(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, -48(%rbp)
	addl	$1, -72(%rbp)
.L7:
	cmpq	$0, -48(%rbp)
	jne	.L8
	movl	-72(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC6(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	-56(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, -48(%rbp)
	movq	-56(%rbp), %rax
	movq	%rax, -40(%rbp)
	movq	$0, -16(%rbp)
	movq	$0, -32(%rbp)
	movq	$0, -24(%rbp)
	movq	-56(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, -32(%rbp)
	movl	$1, -68(%rbp)
	movl	$0, -64(%rbp)
	jmp	.L9
.L19:
	cmpq	$0, -32(%rbp)
	je	.L23
	movl	$1, -60(%rbp)
	jmp	.L12
.L18:
	cmpq	$0, -24(%rbp)
	je	.L24
	movq	-24(%rbp), %rax
	movl	(%rax), %edx
	movq	-32(%rbp), %rax
	movl	(%rax), %eax
	cmpl	%eax, %edx
	jle	.L15
	movq	-48(%rbp), %rax
	movq	8(%rax), %rdx
	movq	-40(%rbp), %rax
	movq	%rdx, 8(%rax)
	movq	-48(%rbp), %rax
	movq	-40(%rbp), %rdx
	movq	%rdx, 8(%rax)
	movq	-48(%rbp), %rax
	movq	%rax, -24(%rbp)
	movl	$0, -68(%rbp)
.L15:
	cmpl	$1, -68(%rbp)
	jne	.L16
	movq	-40(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, -40(%rbp)
	jmp	.L17
.L16:
	movq	-24(%rbp), %rax
	movq	%rax, -40(%rbp)
.L17:
	addl	$1, -60(%rbp)
.L12:
	movl	-84(%rbp), %eax
	cmpl	%eax, -60(%rbp)
	jl	.L18
	jmp	.L14
.L24:
	nop
.L14:
	movl	$1, -68(%rbp)
	movq	-56(%rbp), %rax
	movq	%rax, -40(%rbp)
	movq	-32(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, -32(%rbp)
	movq	-32(%rbp), %rax
	movq	%rax, -48(%rbp)
	addl	$1, -64(%rbp)
.L9:
	movl	-84(%rbp), %eax
	cmpl	%eax, -64(%rbp)
	jl	.L19
	jmp	.L11
.L23:
	nop
.L11:
	leaq	.LC7(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	-56(%rbp), %rax
	movq	%rax, -48(%rbp)
.L20:
	movq	-48(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, %esi
	leaq	.LC4(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	-48(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, -48(%rbp)
	movq	-48(%rbp), %rax
	movq	8(%rax), %rax
	testq	%rax, %rax
	jne	.L20
	movq	-48(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, %esi
	leaq	.LC5(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, %eax
	movq	-8(%rbp), %rdx
	subq	%fs:40, %rdx
	je	.L22
	call	__stack_chk_fail@PLT
.L22:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
