//=============================================================================
// EE108B Lab 2
//
// Instruction fetch module. Maintains PC and updates it. Reads from the
// instruction ROM.
//=============================================================================

module instruction_fetch (
    input clk,
    input rst,
    input en,
    input jump_target,
    input [31:0] pc_id,
    input [25:0] instr_id,  // Lower 26 bits of the instruction
    input jump_branch,
    input [31:0] branch_addr_id,
    input jump_reg,
    input [31:0] jr_pc_id,
    output [31:0] pc
);


    wire [31:0] pc_id_p4 = pc_id + 3'h4;
    wire [31:0] j_addr = jump_reg ? jr_pc_id : {pc_id_p4[31:28], instr_id[25:0], 2'b0};

    wire [31:0] pc_next = (jump_target | jump_reg) ? j_addr : ((jump_branch) ? branch_addr_id : (pc + 3'h4));

    dffare #(32) pc_reg (.clk(clk), .r(rst), .en(en), .d(pc_next), .q(pc));

endmodule
