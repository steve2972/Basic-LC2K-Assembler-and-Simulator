	lw 0 1 x
	lw 0 2 ncount
	lw 0 3 one
	lw 0 4 hundred
start	beq 2 4 done
	add 2 3 2
	add 1 2 1
	beq 1 1 start
done	halt
x	.fill 4
ncount	.fill 0
one	.fill 1
hundred	.fill 100
