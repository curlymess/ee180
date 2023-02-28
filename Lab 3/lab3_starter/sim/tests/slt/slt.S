###############################################################################
# File         : slt.S
# Project      : EE180 Lab 2: MIPS
#
# Standards/Formatting:
#   MIPS gas, soft tab, 80 column
#
# Description:
#   Test the functionality of the 'slti' instruction.
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
	ori $t1, $0, 1
	nop
	nop
	slt $t0, $zero, $t1     #t0 = (0 < 1) = 1
	nop
	nop
	slt $s2, $t1, $zero 	#s2 = (1 < 0) = 0
	nop
	nop
        slt $s2, $s2, $t1       #s2 = (0 < 1) = 1  
	nop
	nop
	and $t0, $t0, $s2       # expecting 1
	
    sw      $t0, 4($s0)         # Set the test result
    sw      $s1, 0($s0)         # Set 'done'

$done:
    j       $done
    nop

    .end boot
