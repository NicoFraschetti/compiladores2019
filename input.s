	.globl main
main:
	enter	$64, $0
	movq	$2, %rax
	addq	$3, %rax
	movq	%rax, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	imulq	$6, %rax
	movq	%rax, -32(%rbp)
	movq	-32(%rbp), %rax
	movq	%rax, -16(%rbp)
	movq	-16(%rbp), %rax
	cltd
	movq	$2, %rbx
	idivq	%rbx
	movq	%rax, -48(%rbp)
	movq	-8(%rbp), %rax
	addq	-48(%rbp), %rax
	movq	%rax, -40(%rbp)
	movq	-40(%rbp), %rax
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rdi
	call	printi
	leave
	ret
