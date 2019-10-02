	.globl main
main:
	enter	$48, $0
	movq	$9, -8(%rbp)
	movq	-8(%rbp), %rax
	cltd
	movq	$2, %rbx
	idivq	%rbx
	movq	%rax, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, -16(%rbp)
	movq	-8(%rbp), %rax
	cltd
	idivq	-16(%rbp)
	movq	%rdx, -32(%rbp)
	movq	-32(%rbp), %rdi
	call	printi
	leave
	ret
