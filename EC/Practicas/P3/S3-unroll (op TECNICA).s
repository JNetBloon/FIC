.data
  N: .word 10
  v1: .float 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
  v2: .float 1, 2, 3, 1, 2, 3, 1, 2, 3, 1
  
.text
.globl main

main:
  la $t0, N
  lw $t0, 0($t0)
  la $a1, v1
  la $a2, v2
  
Loop:
  lwc1 $f2, 0($a1)     # load v1[i]
  lwc1 $f4, 0($a2)     # load v2[i]
  lwc1 $f6, 4($a1)     
  lwc1 $f8, 4($a2)     

  swc1 $f2, 0($a2)     # v2[i] = v1[i] x v2[i]
  swc1 $f6, 4($a2)
  addi $a2, $a2, 8

  addi $t0, $t0, -2
 
  mul.s $f2, $f2, $f4  # v1[i] x v2[i]
  mul.s $f6, $f6, $f8  

  addi $a1, $a1, 8


  bne $t0, $0, Loop
  
end:

  addi $v0, $0, 10
  syscall
