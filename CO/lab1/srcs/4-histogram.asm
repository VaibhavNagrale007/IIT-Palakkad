	.data
count:
	0
	0
	0
	0
	0
	0
	0
	0
	0
	0
	0
marks:
	2
	3
	0
	5
	10
	7
	1
	10
	10
	8
	9
	6
	7
	8
	2
	4
	5
	0
	9
	1
n:
	20
	.text
main:
	load %x0, $n, %x1
loop:
	load %x2, $marks, %x4
	load %x4, $count, %x5
	addi %x5, 1, %x5
	store %x5, $count, %x4
	addi %x2, 1, %x2
	beq %x2, %x1, exit
	jmp loop
exit:
	end
