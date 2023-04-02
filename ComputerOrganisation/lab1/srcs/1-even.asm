	.data
n:
	8
l:
	2
	-1
	7
	5
	3
	4
	-6
	2
	.text
main:
	load %x0, $n, %x1
	addi %x0, -1, %x5
	addi %x0, 0, %x6
loop:
	beq %x1, %x6, exit
	subi %x1, 1, %x1
	load %x1, $l, %x2
	bgt %x2, %x5, positive
	beq %x1, %x6, exit
	jmp loop
positive:
	divi %x2, 2, %x3
	muli %x3, 2, %x3
	beq %x2, %x3, even
	jmp loop
even:
	addi %x4, 1, %x4
	jmp loop
exit:
	addi %x4, 0, %x10
	end
