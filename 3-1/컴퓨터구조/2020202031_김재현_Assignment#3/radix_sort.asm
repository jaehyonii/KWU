    .text
    .globl main

main:
    nop
    ori   $a0, $zero, 0x2000  
    ori   $a1, $zero, 0x2280  
    ori   $a2, $zero, 0x2200  
    ori   $a3, $zero, 0x0010  
    ori   $s0, $zero, 0x0005  
    and   $t5, $zero, $zero   
    and   $t0, $zero, $zero   
L0:                           
    and   $t1, $zero, $zero   
    add   $t8, $a2, $zero     
    nop
    nop
    nop
L1:                           
    sw    $zero, 0($t8)       # eliminate data dependency with L0 add by inserting 3 nop stall
    addi  $t8, $t8, 0x04      
    addi  $t1, $t1, 0x01      
    nop
    nop
    nop
    slti  $at, $t1, 0x08      # eliminate data dependency with L1 addi by inserting 3 nop stall
    nop
    nop
    nop
    bne   $at, $zero, L1      # eliminate data dependency with L1 slti by inserting 3 nop stall
    nop
    add   $t6, $a0, $zero     
    and   $t1, $zero, $zero   
    nop
    nop
L2:                           
    lw    $v0, 0($t6)         # eliminate data dependency with L2 add by inserting 2 nop stall
    nop
    nop
    nop
    srlv  $v1, $v0, $t5       # eliminate data dependency with L2 lw by inserting 3 nop stall
    nop
    nop
    nop
    andi  $t4, $v1, 0x7       # eliminate data dependency with srlv andi by inserting 3 nop stall
    nop
    nop
    nop
    sll   $v1, $t4, 0x02     # eliminate data dependency with L2 andi by inserting 3 nop stall
    nop
    nop
    nop 
    mult  $v1, $a3            # eliminate data dependency with L2 sll by inserting 3 nop stall
    mflo  $t9         
    nop
    nop
    nop        
    add   $t9, $a1, $t9        # eliminate data dependency with L2 mflo by inserting 3 nop stall
    add   $t8, $a2, $v1       
    nop
    nop
    nop
    lw    $v1, 0($t8)       # eliminate data dependency with L2 add by inserting 3 nop stall
    nop
    nop
    nop  
    sll   $t7, $v1, 0x02      # eliminate data dependency with L2 lw by inserting 3 nop stall
    nop
    nop
    nop
    add   $t7, $t9, $t7       # eliminate data dependency with L2 sll by inserting 3 nop stall
    nop
    nop
    nop
    sw    $v0, 0($t7)         # eliminate data dependency with L2 add by inserting 3 nop stall
    addi  $v1, $v1, 0x01     
    nop
    nop
    nop 
    sw    $v1, 0($t8)         # eliminate data dependency with L2 addi by inserting 3 nop stall
    addi  $t6, $t6, 0x04      
    addi  $t1, $t1, 0x01      
    nop
    nop
    nop
    slt   $at, $t1, $a3       # eliminate data dependency with L2 addi by inserting 3 nop stall
    nop
    nop
    nop
    bne   $at, $zero, L2      # eliminate data dependency with L2 slt by inserting 3 nop stall
    nop
    add   $t6, $a0, $zero     
    and   $t1, $zero, $zero   
L3:                           
    and   $t2, $zero, $zero   
    nop
    nop
    sll   $t8, $t1, 0x02      # eliminate data dependency with L2 and by inserting 2 nop stall
    nop
    nop
    nop
    mult  $t8, $a3            # eliminate data dependency with L3 sll by inserting 3 nop stall
    mflo  $t7                 
    add   $t8, $a2, $t8      
    nop
    nop
    nop 
    lw    $v1, 0($t8)         # eliminate data dependency with L3 add by inserting 3 nop stall
    nop
    nop
    nop
    beq   $v1, $zero, L5      # eliminate data dependency with L3 lw by inserting 3 nop stall
    nop
    add   $t7, $a1, $t7       
    nop
    nop
    nop
L4:                           
    lw    $v0, 0($t7)         # eliminate data dependency with L3 add by inserting 3 nop stall
    nop
    nop
    nop
    sw    $v0, 0($t6)         # eliminate data dependency with L4 lw by inserting 3 nop stall
    addi  $t6, $t6, 0x04      
    addi  $t7, $t7, 0x04      
    addi  $t2, $t2, 0x01      
    nop
    nop
    nop
    slt   $at, $t2, $v1       # eliminate data dependency with L4 addi by inserting 3 nop stall
    nop
    nop
    nop
    bne   $at, $zero, L4      # eliminate data dependency with L4 slt by inserting 3 nop stall
    nop
L5:                           
    add   $t1, $t1, 0x01     
    nop
    nop
    nop 
    slti  $at, $t1, 0x08      # eliminate data dependency with L5 add by inserting 3 nop stall
    nop
    nop
    nop
    bne   $at, $zero, L3      # eliminate data dependency with L5 slti by inserting 3 nop stall
    nop
    add   $t5, $t5, 0x03      
    add   $t0, $t0, 0x01      
    nop
    nop
    nop
    slt   $at, $t0, $s0       # eliminate data dependency with L5 add by inserting 3 nop stall
    nop
    nop
    nop
    bne   $at, $zero, L0      # eliminate data dependency with L5 slt by inserting 3 nop stall
    nop
Done:                         
    break                     

    .data
LC0:
       .word   31028
       .word   16610
       .word   12937
       .word   7525
       .word   25005
       .word   17956
       .word   23964
       .word   13951
       .word   3084
       .word   23696
       .word   3881
       .word   11872
       .word   24903
       .word   16843
       .word   25957
       .word   25086
