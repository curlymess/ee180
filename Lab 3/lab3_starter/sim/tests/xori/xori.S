###############################################################################
# File         : xori.S
# Project      : EE180 Lab 2: MIPS
#
# Standards/Formatting:
#   MIPS gas, soft tab, 80 column
#
# Description:
#   Test the functionality of the 'xori' instruction.
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
    ori     $t0, $zero, 0
    nop
    nop
    ori     $t2, $zero, 0
    nop
    nop
    ori     $t0, $t0, 0xbeef    # A = 0xbeef
    nop
    nop
    xori     $t1, $t0, 0x5926       # B = xor(A,5926) = 0xe7c9
    nop
    nop
    xori     $t2, $t1, 0xe7c8       # C = xor(B,e7c8) = 0x1
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
