###############################################################################
# File         : addi.S
# Project      : EE180 Lab 2: MIPS
#
# Standards/Formatting:
#   MIPS gas, soft tab, 80 column
#
# Description:
#   Test the functionality of the 'addi' instruction.
#
###############################################################################


    .section .boot, "x"
    .balign 4
    .set    noreorder
    .global boot
    .ent    boot
boot:
    lui     $s0, 0x8002         # Load the base address of the status/test registers
    ori     $s1, $0, 1          # Prepare the 'done' status
    nop
    nop

# positive + positive overflow
    lui     $t0, 0x7fff # A = 0x7fffffff
    nop
    nop
    ori     $t0, 0xffff
    nop
    nop
    addi    $t1, $t0, 2
    nop
    nop

# negative + positive 
    lui     $t0, 0xffff         # A = 0xfffffffd (-3)
    nop
    nop
    ori     $t0, $t0, 0xfffd
    nop
    nop
 
    addi     $t2, $t0, 4       # C = A+B = 1
    nop
    nop
 
    sw      $t2, 4($s0)         # Set the test result
    nop
    nop
    sw      $s1, 0($s0)         # Set 'done'

$done:
    j       $done
    nop

    .end boot
