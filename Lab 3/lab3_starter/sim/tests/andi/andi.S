###############################################################################
# File         : andi.S
# Project      : EE180 Lab 2: MIPS
#
# Standards/Formatting:
#   MIPS gas, soft tab, 80 column
#
# Description:
#   Test the functionality of the 'andi' instruction.
#
###############################################################################

    .section .boot, "x"
    .balign 4
    .set    noreorder
    .global boot
    .ent    boot
boot:
    lui     $s0, 0x8002         # Load the base address of the status/test registers
    nop
    nop
    ori     $s1, $0, 1          # Prepare the 'done' status
    nop
    nop
    ori     $t0, $0, 0xdeaf     # A = 0xdeaf
    nop
    nop
    andi    $t1, $t0, 0x3141    # B = and(A, 0x3141) = 0x1001
    nop
    nop
    andi    $t2, $t1, 0xefff    # C = and(B, 0xefee) = 1
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
