###############################################################################
# File         : srlv.S
# Project      : EE180 Lab 2: MIPS
#
# Standards/Formatting:
#   MIPS gas, soft tab, 80 column
#
# Description:
#   Test the functionality of the 'srlv' instruction.
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
    ori     $t0, $0, 0
    nop
    ori     $t1, $0, 0
    nop
    ori     $t2, $0, 0
    nop
    ori     $t3, $0, 0
    nop
    nop
    nop
    nop
    lui     $t0, 0xdeaf         # A = 0xdeafbeef
    nop
    nop
    ori     $t0, $t0, 0xbeef
    nop
    nop
    ori     $t1, $0, 4
    nop
    nop
    srlv    $t2, $t0, $t1       # B = 0xdeafbeef >> 4 = 0x0deafbee
    nop
    nop
    lui     $t3, 0x0dea         # C = 0x0deafbee
    nop
    nop
    ori     $t3, $t3, 0xfbee
    nop
    nop
    subu    $t4, $t2, $t3       # D = B - C = 0
    nop
    nop
    sltiu   $v0, $t4, 1
    nop
    nop
    sw      $v0, 4($s0)         # Set the test result
    sw      $s1, 0($s0)         # Set 'done'

$done:
    j       $done
    nop

    .end boot
