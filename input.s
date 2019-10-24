	.globl main
main:
	enter	$80, $0
	movq	$0, -8(%rbp)
	movq	$1, -16(%rbp)
	movq	$1, -24(%rbp)
	movq	$30, -32(%rbp)
	jmp		.L0
.L1:
	movq	-8(%rbp), %rax
	addq	-16(%rbp), %rax
	movq	%rax, -48(%rbp)
	movq	-48(%rbp), %rax
	movq	%rax, -40(%rbp)
	movq	-16(%rbp), %rax
	movq	%rax, -8(%rbp)
	movq	-40(%rbp), %rax
	movq	%rax, -16(%rbp)
	movq	-24(%rbp), %rax
	addq	$1, %rax
	movq	%rax, -56(%rbp)
	movq	-56(%rbp), %rax
	movq	%rax, -24(%rbp)
	movq	-8(%rbp), %rdi
	call	printi
.L0:
	movq	-24(%rbp), %r10
	movq	-32(%rbp), %r11
	cmpq	%r11, %r10
	setl	%al
	movzbl	%al, %eax
	movq	%rax, -64(%rbp)
	movq	-64(%rbp), %r10
	cmpq	$1, %r10
	je 		.L1
	leave
	ret
