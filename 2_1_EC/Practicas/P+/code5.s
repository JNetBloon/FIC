# Salto retardado
# Unidad de suma en punto flotante segmentada con latencia 3
# Unidad de multiplicación en punto flotante segmentada con latencia 4

.data

a: .float 1, 2, 3, 4, 5, 6
b: .float 6, 5, 4, 3, 2, 1
c: .float 0, 2, 4, 6, 8, 10
count: .word 6

.text
.globl main

main:
la $t0, count
lw $t0, 0($t0)
addi $t1, $0, 0
la $t2, a
la $t3, b
la $t4, c

Loop:
lwc1 $f0, 0($t2)
lwc1 $f2, 0($t3)
lwc1 $f4, 0($t4)
mul.s $f0, $f0, $f2
add.s $f0, $f0, $f4
swc1 $f0, 0($t4)
addi $t2, $t2, 4
addi $t3, $t3, 4
addi $t4, $t4, 4
addi $t1, $t1, 1
bne $t1, $t0, Loop
nop

End:
addi $v0, $0, 10
syscall
