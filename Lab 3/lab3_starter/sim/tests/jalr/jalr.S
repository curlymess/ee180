###############################################################################
# File         : jalr.S
# Project      : EE180 Lab 2: MIPS
#
# Standards/Formatting:
#   MIPS gas, soft tab, 80 column
#
# Description:
#   Test the functionality of the 'jalr' instruction.
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
    ori     $v0, $0, 0          # The test result starts as a failure
    nop
    ori     $t0, $0, 0
    nop
    addiu    $t0, $t0, $target  # jalr can't jump to $target directly, needs a register
    nop
    jalr    $t0

$finish:
    nop
    nop
    sw      $v0, 4($s0)
    sw      $s1, 0($s0)

$done:
    j       $done
    nop

$target:
    j       $finish
    ori     $v0, $0, 1          # Set the result to pass

    .end boot
