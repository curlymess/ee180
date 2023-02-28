###############################################################################
# File         : nor.S
# Project      : EE180 Lab 2: MIPS
#
# Standards/Formatting:
#   MIPS gas, soft tab, 80 column
#
# Description:
#   Test the functionality of the 'ori' instruction.
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
    add $t0, $zero, $zero
    add $t1, $zero, $zero
    nop
    nop
    nor $t2, $t0, $t1		# 0 nor 0 = expected = 1
    nop
    nop
    ori $t0,$zero, 1
    nop
    nop
    nor $t2, $t0, $t1		# 1 nor 0 = expected = 0 
    nop
    nop
    ori $t1, $zero, 1
    nop
    nop
    nor $t2, $t0, $t1		# 1 nor 1 = expected = 0 
    nop
    nop
    nor $t0, $t2, $t2		# 0 nor 0 = expected = 1



    nop
    nop
    sw      $t0, 4($s0)         # Set the test result
    sw      $s1, 0($s0)         # Set 'done'

$done:
    j       $done
    nop

    .end boot
