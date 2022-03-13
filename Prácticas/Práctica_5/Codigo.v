;Cabecera con definición de variables e inicialización de registros
;Las siguientes etiquetas sirven para obtener la dirección de comienzo de vectores o variables:

.data 
arrayx: .float 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25
arrayy: .float 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27
a: .float 3 ;Constante a. Además sirve de dirección donde acaba el vector y.

.text
addi r1,r0, arrayx ;Puntero a Arrayx
addi r2,r0, arrayy ;Puntero a Arrayy
addi r3,r0, a ;Puntero a variable a
lf f11, 0(r3) ;Carga el valor de a en f11

;Bucle
bucle: 
lf f5, 0(r1) ;Carga elem del vector x
lf f6, 0(r2) ;Carga elem del vector y
multf f5, f11, f5
addf f5, f6, f5
sf 0(r2), f5
addi r1, r1, 4
addi r2, r2, 4
slti r3, r2, a
bnez r3, bucle 