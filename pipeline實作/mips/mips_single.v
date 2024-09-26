/**
Title: MIPS Single-Cycle Processor

input :
    clk : Clock signal
    rst : Reset signal
*/
module mips_single( clk, rst );
	input clk, rst;
	
	// instruction bus
	wire[31:0] instr;
	
	// break out important fields from instruction
	wire [5:0] opcode, funct;
    wire [4:0] rs, rt, rd, shamt;
    wire [15:0] immed;
    wire [31:0] extend_immed, b_offset;
    wire [25:0] jumpoffset;
	
	// datapath signals
    wire [4:0] rfile_wn;
    wire [31:0] rfile_rd1, rfile_rd2, rfile_wd, alu_b, alu_out, b_tgt, pc_next,
                pc, pc_incr, dmem_rdata, jump_addr, branch_addr, b_tgt2MEM;

	// control signals
    wire RegWrite, Branch, PCSrc, RegDst, MemtoReg, MemRead, MemWrite, ALUSrc, Zero, Jump;
    wire [1:0] ALUOp;
    wire [2:0] Operation;

	wire[31:0] instrID, pc_incrID;
	wire [5:0] functEX;
	wire [31:0] rfile_rd1EX ,rfile_rd2EX, pc_incrEX, extend_immedEX;
	wire [4:0] rtEX, rdEX, shamtEX;
	wire RegDstEX, ALUSrcEX, BranchEX, MemReadEX, MemWriteEX, RegWriteEX, MemtoRegEX, JumpEX;
	wire [1:0] ALUOpEX;	
	wire [31:0] alu_outMEM, rfile_rd2MEM, rfile_rd1MEM;
	wire[4:0] rfile_wnMEM;
	wire ZeroMEM, BranchMEM, MemReadMEM, MemWriteMEM, RegWriteMEM, MemtoRegMEM;
	wire [31:0] b_tgtMEM, dmem_rdataWB, alu_outWB;
	wire [4:0] rfile_wnWB;
	wire RegWriteWB, MemtoRegWB;

////////////////////////////////////////////////////     IF     //////////////////////////////////////////////////////////////////////////////////////////

    wire nopID, nopEX, en_pc, nop_pc, stop, read, JumpMEM;
    wire [31:0] instrInMEm, jump_addrEX, jump_addrMEM;

	memory InstrMem( .clk(clk), .MemRead(1'b1), .MemWrite(1'b0), .wd(32'd0), .addr(pc), .rd(instr) );  // 取得指令

	reg32 PC( .clk(clk), .rst(rst), .en_reg(1'b1), .d_in(pc_next), .d_out(pc) );

    add32 PCADD( .a(pc), .b(32'd4), .result(pc_incr), .sel(1'b1) );  // pc = pc + 4

	reg_IFID IFID( .clk(clk), .reset(rst), .en_reg(1'b1),
		           .pc_incr(pc_incr), .instr(instr),
		           .pc_incrID(pc_incrID), .instrID(instrID), .nopID(nopID) );

////////////////////////////////////////////////////     ID     //////////////////////////////////////////////////////////////////////////////////////////

  	assign opcode = instrID[31:26];
   	assign rs = instrID[25:21];
   	assign rt = instrID[20:16];
    assign rd = instrID[15:11];
   	assign shamt = instrID[10:6];
   	assign funct = instrID[5:0];
   	assign immed = instrID[15:0];

	unsign_extend SignExt( .immed_in(immed), .ext_immed_out(extend_immed) );  // unsign-extender

	reg_file RegFile( .clk(clk), .RegWrite(RegWriteWB), .RN1(rs), .RN2(rt), .WN(rfile_wnWB), .WD(rfile_wd), .RD1(rfile_rd1), .RD2(rfile_rd2) );

  	control_single CTL(.opcode(opcode), .RegDst(RegDst), .ALUSrc(ALUSrc), .MemtoReg(MemtoReg), 
                       .RegWrite(RegWrite), .MemRead(MemRead), .MemWrite(MemWrite), .Branch(Branch), 
                       .Jump(Jump), .ALUOp(ALUOp));

   assign jumpoffset = instrID[25:0];  // 取得jump offset

 	assign jump_addr = { pc_incr[31:28], jumpoffset <<2 };  // jump offset shifter & concatenation

	reg_IDEX IDEX( .clk(clk), .reset(rst), .en_reg(1'b1),
			       .RegDst(RegDst), .ALUSrc(ALUSrc), .Branch(Branch), .MemRead(MemRead), .MemWrite(MemWrite), .RegWrite(RegWrite), .MemtoReg(MemtoReg), .Jump(Jump),
			       .pc_incrID(pc_incrID), .rfile_rd1(rfile_rd1), .rfile_rd2(rfile_rd2), .extend_immed(extend_immed), .jump_addr(jump_addr), 
                   .ALUOp(ALUOp), 
                   .rt(rt), .rd(rd), .shamt(shamt), 
                   .funct(funct), .nopID(nopID),
			       .RegDstEX(RegDstEX), .ALUSrcEX(ALUSrcEX), .BranchEX(BranchEX), .MemReadEX(MemReadEX), .MemWriteEX(MemWriteEX), .RegWriteEX(RegWriteEX), .MemtoRegEX(MemtoRegEX), .JumpEX(JumpEX),
                   .ALUOpEX(ALUOpEX), 
			       .pc_incrEX(pc_incrEX), .rfile_rd1EX(rfile_rd1EX), .rfile_rd2EX(rfile_rd2EX), .extend_immedEX(extend_immedEX), .jump_addrEX(jump_addrEX), 
                   .rtEX(rtEX), .rdEX(rdEX), .shamtEX(shamtEX), 
                   .functEX(functEX), .nopEX(nopEX) );

////////////////////////////////////////////////////     EX     //////////////////////////////////////////////////////////////////////////////////////////

   	assign b_offset = extend_immedEX << 2;  // branch offset shifter

    add32 BRADD( .a(pc_incrEX), .b(b_offset), .result(b_tgt), .sel(1'b1) );

    mux2 #(32) ALUMUX( .sel(ALUSrcEX), .a(rfile_rd2EX), .b(extend_immedEX), .y(alu_b) );

    TotalALU ALU( .clk(clk), .dataA(rfile_rd1EX), .dataB(alu_b), .shamt(shamtEX), .ALUOp(ALUOpEX), .Signal(functEX), .Output(alu_out), .zero(Zero), .reset(rst), .nop(nopEX) );

    mux2 #(5) RFMUX( .sel(RegDstEX), .a(rtEX), .b(rdEX), .y(rfile_wn) );

	reg_EXMEM EXMEM( .clk(clk), .reset(rst), .en_reg(1'b1),
				     .BranchEX(BranchEX), .MemReadEX(MemReadEX), .MemWriteEX(MemWriteEX), .RegWriteEX(RegWriteEX), .MemtoRegEX(MemtoRegEX), .JumpEX(JumpEX), 
				     .b_tgt(b_tgt), .alu_out(alu_out), .rfile_rd2EX(rfile_rd2EX), .jump_addrEX(jump_addrEX), .rfile_rd1EX(rfile_rd1EX),
                     .rfile_wn(rfile_wn), 
                     .Zero(Zero), 
				     .BranchMEM(BranchMEM), .MemReadMEM(MemReadMEM), .MemWriteMEM(MemWriteMEM), .RegWriteMEM(RegWriteMEM), .MemtoRegMEM(MemtoRegMEM), .JumpMEM(JumpMEM), 
				     .b_tgtMEM(b_tgtMEM), .alu_outMEM(alu_outMEM), .rfile_rd2MEM(rfile_rd2MEM), .jump_addrMEM(jump_addrMEM), .rfile_rd1MEM(rfile_rd1MEM), 
                     .rfile_wnMEM(rfile_wnMEM), 
                     .ZeroMEM(ZeroMEM));

////////////////////////////////////////////////////     MEM     //////////////////////////////////////////////////////////////////////////////////////////

   	and BR_AND(PCSrc, BranchMEM, ZeroMEM);

	mux2 #(32) PCMUX( .sel(PCSrc), .a(pc_incr), .b(b_tgtMEM), .y(branch_addr) );  // 決定要拿pc+4還是分支跳躍後的指令位址

	mux2 #(32) J_pc_MUX( .sel(JumpMEM), .a(branch_addr), .b(jump_addrMEM), .y(pc_next) ); 

	memory DatMem( .clk(clk), .MemRead(MemReadMEM), .MemWrite(MemWriteMEM), .wd(rfile_rd2MEM), 
				   .addr(alu_outMEM), .rd(dmem_rdata) );	

	reg_MEMWB MEMWB( .clk(clk), .reset(rst), .en_reg(1'b1),
				     .RegWriteMEM(RegWriteMEM), .MemtoRegMEM(MemtoRegMEM),
				     .dmem_rdata(dmem_rdata), .alu_outMEM(alu_outMEM), 
                     .rfile_wnMEM(rfile_wnMEM),
				     .RegWriteWB(RegWriteWB), .MemtoRegWB(MemtoRegWB),
				     .dmem_rdataWB(dmem_rdataWB), .alu_outWB(alu_outWB), 
                     .rfile_wnWB(rfile_wnWB) );

////////////////////////////////////////////////////     WB     //////////////////////////////////////////////////////////////////////////////////////////

    mux2 #(32) WRMUX( .sel(MemtoRegWB), .a(alu_outWB), .b(dmem_rdataWB), .y(rfile_wd) );

endmodule
