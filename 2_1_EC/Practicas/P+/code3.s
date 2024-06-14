# Salto retardado
# Unidad de suma en punto flotante segmentada con latencia 2
# Unidad de multiplicación en punto flotante segmentada con latencia 4
# Unidad de división en punto flotante no segmentada con latencia 7

.data
n: .word 10
a: .float 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
x: .space 4

.text
.globl main

main:
  la $t0, n
  lw $t0, 0($t0)
  la $t1, x
  add $t2, $0, $0
loop:
  lwc1 $f0, -4($t1)
  swc1 $f0, 0($t1)
  addi $t1, $t1, -4
  addi $t2, $t2, 1
  addi $t0, $t0, -1
  bne $t0, $0, loop
  nop
end:
  sw $t2, 0($t1)
  addi $v0, $0, 10
  syscall
