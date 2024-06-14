.data

Num: .word 4
vector: .float 5, 4, 3, 7, 6

.text 
.globl main

main:
  la $s4, Num
  la $s5, vector
  lw $s0, 0($s4)
  lwc1 $f12, 0($s5)
  addi $s1, $0, 1
  addi $s5, $s5, 4

Loop:
  slt $t1, $s1, $s0
  beq $t1, $0, fin
  lwc1 $f0, 0($s5)
  c.le.s $f0, $f12
  bc1t mayor
  mov.s $f12, $f0

mayor:
  addi $s1, $s1, 1
  addi $s5, $s5, 4
  j Loop

fin:
  swc1 $f12, 0($s5)
  addi $v0, $0, 2
  syscall
  
  addi $v0, $0, 10
  syscall