	.globl main
main:
	enter	$88, $0
	movq	$1, %rax
	addq	$4, %rax
	movq	%rax, -40(%rbp)
	movq	-40(%rbp), %rax
	addq	$5, %rax
	movq	%rax, -32(%rbp)
	movq	-32(%rbp), %r10
	movq	$5, %r11
	cmpq	%r11, %r10
	setl	%al
	movzbl	%al, %eax
	movq	%rax, -24(%rbp)
	movq	$100, %r10
	movq	$500, %r11
	cmpq	%r11, %r10
	setl	%al
	movzbl	%al, %eax
	movq	%rax, -48(%rbp)
	movq	$1, %rax
	orq		-48(%rbp), %rax
	orq		-24(%rbp), %rax
	movq	%rax, -16(%rbp)
	movq	-16(%rbp), %rax
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %r10
	cmpq	$1, %r10
	setne	%al
	movzbl	%al, %eax
	movq	%rax, -72(%rbp)
	movq	$1, %rax
	andq	-8(%rbp), %rax
	andq	-72(%rbp), %rax
	movq	%rax, -64(%rbp)
	movq	$1, %rax
	orq		-8(%rbp), %rax
	orq		-64(%rbp), %rax
	movq	%rax, -56(%rbp)
	movq	-56(%rbp), %rdi
	call	printi
	leave
	ret
