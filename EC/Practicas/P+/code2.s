# Salto retardado
# Unidad de suma en punto flotante segmentada con latencia 2
# Unidad de multiplicación en punto flotante segmentada con latencia 4
# Unidad de división en punto flotante no segmentada con latencia 7



.data
n: .word 10
a: .word 1, 2, 3, 4, 5, 4, 3, 2, 1, 10
b: .word 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
c: .space 4

.text
.globl main

main:
  la $t0, n
  lw $t0, 0($t0)
  la $t1, a
  la $t2, b
  la $t3, c
  addi $t4, $0, 0

loop:

  lw $t5, 0($t1)
  lw $t6, 0($t2)
  addi $t1, $t1, 4
  addi $t0, $t0, -1
  beq $t6, $t5, skip
  addi $t2, $t2, 4
  addi $t4, $t4, 1
skip:

  bne $t0, $0, loop
  sw $t4, 0($t3)


end:

  addi $v0, $0, 10
  syscall
