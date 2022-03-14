.data 

arrayx:	.float 1 2 3 4 5 6 7 8 9 10 11 12
fin:	.byte	33        ;solo para marcar el fin del vector.

.text
	addi 	r1,r0, arrayx    ;Puntero a Arrayx
	lf  f11, 0(r1)		; carga el valor 1.0 en f11
	addi r1, r1, 4
	nop
	nop
	nop
	nop
	nop
	nop
	nop   ;; solo para que al comenzar el bucle no haya RAW con las instr anteriores
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
bucle:	lf  f5, 0(r1)		; carga elem del vector
	addf f5, f11, f5
	sf  -4(r1), f5
	addi r1, r1, 4
	slti r3, r1, fin
	bnez r3, bucle  
	xor 	r3, r3, r3 ;instr inutil solo para que se vea bien que es la rama sig. al salto
	nop
	nop
	nop
	trap 0
