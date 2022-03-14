.data 

arrayx:	.float 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25
arrayy:	.float 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27
a:	.float 3 ; Constante a . Además sirve de dirección donde acaba el vector y.

.text
	addi 	r1,r0, arrayx    ;Puntero a Arrayx
	addi 	r2,r0, arrayy    ;Puntero a Arrayy
	addi 	r3,r0, a    ;Puntero a variable a
	lf  f11, 0(r3)		; carga el valor de a en f11
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop  ; instrucciones solo para evitar dependencias con las instr. anteriores al bucle
	nop  ; y que no salgan algunos ciclos de espera en las primeras iteraciones
bucle:	lf  f5, 0(r1)		; carga elem del vector x
	lf  f6, 0(r2)		; carga elem del vector y
	multf f5, f11, f5
	addf  f5, f6, f5
	sf  0(r2), f5
	addi r1, r1, 4
	addi r2, r2, 4
	slti r3, r2, a
	bnez r3, bucle  
	xor 	r3, r3, r3 ;instr posterior al bucle, para que se vea más claramente donde acaba 
	; (y como se aborta esta instruccion cuando el bucle está iterando)
	nop
	nop
	nop
	trap 0
