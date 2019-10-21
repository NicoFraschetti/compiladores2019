	.globl main
main:
	enter	$32, $0
	movq	$3, -8(%rbp)
	leave
	ret
