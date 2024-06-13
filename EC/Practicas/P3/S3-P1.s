.text
.globl main

main:
  la $t0, array
  la $t4, count
  lw $t1, 0($t4)
  addi $t2, $0, 0
  addi $t0, $t0, 20

Loop:
  lw $t3, 0($t0)
  add $t2, $t3, $t2
  addi $t1, $t1, -1
  addi $t0, $t0, -4
  bne $t1, $0, Loop
  sw $t2, 0($t4)
  
end:
  addi $v0, $0, 10
  syscall

.data
  array: .word 3, 4, 9, 8, 5, 1, 3
  count: .word 6
