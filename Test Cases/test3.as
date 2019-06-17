	lw 0 0 base
	lw 0 1 init
	lw 0 2 nine
	lw 0 3 numcount
	lw 0 4 one
start	beq 3 2 done
	add 4 3 3
	add 1 1 1
	beq 1 1 start
done	halt
base	.fill 0
init	.fill 111111
numcount .fill 0
one	.fill 1
nine	.fill 9
