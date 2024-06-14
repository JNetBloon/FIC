# Salto fijo no efectivo
# Unidad de suma en punto flotante segmentada con latencia 2
# Unidad de multiplicación en punto flotante segmentada con latencia 4

.data
n: .word 10
a: .word 1, 2
ax: .space 32
f: .word 1, 1
fx: .space 32

.text
.globl main

main:
  la $t0, n
  lw $t0, 0($t0)
  la $t1, ax
  lw $t2, -8($t1) 
  lw $t3, -4($t1)
  la $t4, fx
  lw $t5, -8($t4)
  lw $t6, -4($t4)
  addi $t0, $t0, -2
loop:
  add $t3, $t3, $t2
  sw $t3, 0($t1)
  add $t7, $t6, $t5
  sw $t7, 0($t4)
  add $t5, $t6, $0
  add $t6, $t7, $0
  addi $t1, $t1, 4
  addi $t4, $t4, 4
  addi $t0, $t0, -1
  bne $t0, $0, loop
end:
  addi $v0, $0, 10
  syscall
