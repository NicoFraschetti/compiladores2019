	.globl main
main:
	enter	$48, $0
	movq	$1, -8(%rbp)
	movq	$10, -16(%rbp)
	jmp		.L0
.L1:
	movq	-8(%rbp), %rdi
	call	printi
	movq	-8(%rbp), %rax
	addq	$1, %rax
	movq	%rax, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, -8(%rbp)
.L0:
	movq	-8(%rbp), %r10
	movq	-16(%rbp), %r11
	cmpq	%r11, %r10
	setl	%al
	movzbl	%al, %eax
	movq	%rax, -32(%rbp)
	movq	-32(%rbp), %r10
	cmpq	$1, %r10
	je 		.L1
	leave
	ret
