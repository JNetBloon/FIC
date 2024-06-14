# Salto retardado
# Unidad de suma en punto flotante segmentada con latencia 2
# Unidad de multiplicación en punto flotante segmentada con latencia 4
# Unidad de división en punto flotante no segmentada con latencia 7

.data
n: .word 10
a: .float 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
b: .float 1, 2, 3, 1, 2, 3, 1, 2, 3, 1
c: .space 40

.text
.globl main

main:
  la $t0, n
  lw $t0, 0($t0)
  la $t1, a
  la $t2, b
  la $t3, c
loop:
  lwc1 $f0, 0($t1)
  lwc1 $f2, 0($t2)
  sub.s $f4, $f0, $f2
  swc1 $f4, 0($t3)
  addi $t1, $t1, 4
  addi $t2, $t2, 4
  addi $t3, $t3, 4
  addi $t0, $t0, -1
  bne $t0, $0, loop
end:
  addi $v0, $0, 10
  syscall
