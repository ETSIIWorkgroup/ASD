* P0 One challenge for architects is that the design created today will require several years of implementation, verification,
and testing before appearing on the market. This means that the architect must project what the technology will be like several years
in advance. Sometimes, this is difficult to do.

  - a) According to the trend in device scaling historically observed by Moore’s Law, the number of transistors on a chip in 2025
should be how many times the number in 2015?
  
    20 times
  
  - b) The increase in performance once mirrored this trend. Had performance continued to climb at the same rate as in the 1990s,
approximately what performance would chips have over the VAX-11/780 in 2025?

    5Mhz * (1.35)^47 = a really big number
  
  - c) At the current rate of increase of the mid-2000s, what is a more updated projection of performance in 2025?
  
    Ej. 3.2Gh * (1.01)^12 = 3.6Ghz
  
  - d) What has limited the rate of growth of the clock rate, and what are architects doing with the extra transistors now to increase
performance?
  
    Smaller CPUs
  
* P1 You are designing a system for a real-time application in which specific deadlines must be met. Finishing the computation
faster gains nothing. You find that your system can execute the necessary code, in the worst case, twice as fast as necessary.

  - a) How much energy do you save if you execute at the current speed and turn off the system when the computation is
complete?
  
    E/2
  
  - b) How much energy do you save if you set the voltage and frequency to be half as much?
  
    If E ∝ V^2 => E ∝ 1/2V^2 => 1/4 of E
  
* P2 Using the instruction mix of Figure A.27, compute the effective CPI for integer instructions of MIPS. Assume that 60%
of the conditional branches are taken. Also assume that we have made the following measurements of average CPI for instruction
types:

    - Load/store => 26 + 10 = 36
    - ALU => 19 + 3 + 5 + 2 + 1 + 2 + 4 + 9 + 3 = 48
    - Branch => 12
    - Jump => 3
    - TOTAL = 99%
    
    CPI = (36 * 1.4 + 48 * 1 + 12 * (2 * 0.6 + 1.5 * 0.4) + 3 * 1.2) / 99 = 1,2
    
* P3 Using the instruction mix of Figure A.28, compute the effective CPI for Floating Points instructions of MIPS. Assume
the same measurements of average CPI for instruction types of exercise P2.

  - Load/Store => 17
  - ALU => 37
  - Branch => 4
  - Jump => 0
  - Load/Store FP => 22
  - Add/sub FP => 10
  - Mul/div FP => 8
  - Others FP => 1
  - TOTAL = 99
  
  CPI = (37 * 1 + 17 * 1.4 + 4 * (0.6 * 2 + 0.4 * 1.5) + 22 * 1.5 + 10 * 4 + 8 * 6 + 1 * 2) / 99 = 1.92
  
* P4 Hallar la aceleración entre una máquina M1 y otra M2 si: M1 ejecuta el doble de instrucciones que M2, M1 tiene la cuarta parte
 de CPI que M2 y la frecuencia de reloj de M2 es el doble que la de M1.
 
|         |  M1     | M2  |   
|---------|---------|-----|         
| Nº inst | 2n      | n   |       
| CPI     | 1/4 CPI | CPI |         
| f       | f       | 2f  |

  (2 * 1/4 * 1) / (1 * 1 * 1/2) = 1 => They are equal!
  
* P5 Suponga que un compilador consigue paralelizar para N procesadores la mitad de un determinado código. Usando la ley de
Amdahl, calcule la aceleración total conseguida. Hallar el límite cuando N tiende a infinito.

  A = 1 / ((1 - σ) * (σ / N)) => If N tends towards ∞ and σ is 0.5 ===> A = 1 / ((1-0.5) * (0.5/∞)) => A = 2 times faster.

* P6 Suponga que se desea conseguir una aceleración de 80 en un sistema con 100 procesadores. Usando la ley de Amdahl, calcule
la fracción del código que debe ser paralela.

  If A = 80 and N = 100 => 80 = 1 / ((1 - σ) * (σ / 100)) ===> σ = 0.9975 equals 99.75%

* P7 Dado el siguiente código, y suponiendo que en el bucle desenrollado se consigue una aceleración de 10 (respecto del no
desenrollado), calcule la aceleración total conseguida si se compara el tiempo de ejecución de Daxpy_no_opt con el de
Daxpy_opt_10. Idem entre Daxpy_no_opt y Daxpy_opt_90.

  A = 1 / ((1-0.1) * (0.1/10)) = 1.09 => 9% better.
   
  A = 1 / ((1-0.9) * (0.9/10)) = 5.2613 => 426.13% better.
  
* P8 Calcule el MTTR, MTTF y MTBF de un servidor que ha sufrido 5 paradas inesperadas en los últimos 3 meses. Las 3
primeros se resolvieron en 5 minutos, pero las 2 últimas requirieron 30 y 40 minutos, respectivamente. ¿Cuál es la disponibilidad
(Availability) de este servidor? ¿Cuánto tiempo puede estar parado un servidor en un mes para tener una disponibilidad de “5
nueves”?

  Availability:<br>
  MTTR = t. to repair / nº repairs<br>
  MTTF = h. of work / nº servers<br>
  MTBF = MTTR + MTTF<br><br>
  A = MTTF / MTTF + MTTR<br>
  
  3 * 5 + 1 * 30 + 1 * 40 = 85 mins stopped<br>
  MTTR = 85 / 5 = 17 mins<br>
  MTTF = (30 * 3 * 24 * 60) / 1 = 129600 mins <= 30 days, 24 hours<br>
  MTBF = 17 + 129600 = 129617 mins<br>
  A = 129600 / 129617 = 0.99<br>
  
  Max. time stopped for a five nines availability:<br>
  0.99999 = 43200 / (43200 + x) ===> x = 0.42 mins max.<br>
  (where 43200 is 30 * 1 * 24 * 60)<br>
  
  
