.type start, %function

.word stack_top
.word start

.global start
start:
	ldr r1, =arm_kernel_start
	bx r1
