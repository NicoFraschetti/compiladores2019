	.globl main
main:
	enter	$40, $0
	movq	$1, -8(%rbp)
	movq	$10, -16(%rbp)
	jmp		.L0
.L1:
.L0:
	movq	-8(%rbp), %r10
	movq	-16(%rbp), %r11
	cmpq	%r11, %r10
	setl	%al
	movzbl	%al, %eax
	movq	%rax, -24(%rbp)
	movq	-24(%rbp), %r10
	cmpq	$1, %r10
	je 		.L1
	leave
	ret
