.text
.globl main

main:
  la $t0, array1
  la $t2, array2
  la $t1, count
  lw $t1, 0($t1)

 Loop:
  lw $t3, 0($t0)
  lw $t4, 0($t2)
  add $t3, $t3, $t4
  sw $t3, 0($t0)
  addi $t0, $t0, 4
  addi $t2, $t2, 4
  addi $t1, $t1, -1
  bne $t1, $0, Loop


fin:
  addi $v0,$0,10      # la llamada para salir del programa
  syscall

# segmento de datos
.data
array1: .word 3, 4, 9, 8, 5, 1
array2: .word 3, 4, 9, 8, 5, 1
count: .word 6


# fin