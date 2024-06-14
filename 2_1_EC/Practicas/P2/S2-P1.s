.text
.globl main

main:
  la $t0, array
  la $t1, count
  lw $t1, 0($t1)
  addi $t2, $0, 1
  addi $t0, $t0, 20

Loop:
  lw $t3, 0($t0)

  add $t3, $t3, $t2
  sw $t3, 0($t0)
  addi $t1, $t1, -1
  addi $t0, $t0, -4
  bne $t1, $0, Loop
  
  
fin:
  addi $v0,$0,10      # la llamada para salir del programa
  syscall

# segmento de datos
.data
array: .word 3, 4, 9, 8, 5, 1
count: .word 6

# fin
