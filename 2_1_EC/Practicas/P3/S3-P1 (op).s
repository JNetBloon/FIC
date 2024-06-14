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

  addi $t1, $t1, -1 #Desde destino de salto
  addi $t0, $t0, -4
  bne $t1, $0, Loop
  add $t2, $t3, $t2 #Desde antes del salto
  sw $t2, 0($t4) #Desde instrucciones siguientes al salto
  
end:
  addi $v0, $0, 10
  syscall

.data
  array: .word 3, 4, 9, 8, 5, 1, 3
  count: .word 6
