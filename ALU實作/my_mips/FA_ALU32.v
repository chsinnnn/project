`timescale 1ns/1ns
// 用FA_ALU組成32-bit的ALU
module FA_ALU32( input [31:0] a, input [31:0] b, input cin, output [31:0] sum, output [31:0] andNum, output [31:0] orNum, output [31:0] sltNum ) ;

	wire  [31:0] c ; // to record the number of carry bits
    // 控制cin知道要做+-法 課本3-1 p.16
	// 把數字變成二進位然後是32bit一個位置一個位置做
	// control是從ALU.v給的
    // FA_ALU( input a, input b, input cin, input control, output cout, output sum, output andNum, output orNum, input less, output sltNum )
	FA_ALU t0( .a(a[0]), .b(b[0]), .cin(cin), .control(cin), .cout(c[0]), .sum(sum[0]), .andNum(andNum[0]), .orNum(orNum[0]), .less(sum[31]), .sltNum(sltNum[0]));
	FA_ALU t1( .a(a[1]), .b(b[1]), .cin(c[0]), .control(cin), .cout(c[1]), .sum(sum[1]), .andNum(andNum[1]), .orNum(orNum[1]), .less(1'b0), .sltNum(sltNum[1]));
	FA_ALU t2( .a(a[2]), .b(b[2]), .cin(c[1]), .control(cin), .cout(c[2]), .sum(sum[2]), .andNum(andNum[2]), .orNum(orNum[2]), .less(1'b0), .sltNum(sltNum[2]));
	FA_ALU t3( .a(a[3]), .b(b[3]), .cin(c[2]), .control(cin), .cout(c[3]), .sum(sum[3]), .andNum(andNum[3]), .orNum(orNum[3]), .less(1'b0), .sltNum(sltNum[3]));
	FA_ALU t4( .a(a[4]), .b(b[4]), .cin(c[3]), .control(cin), .cout(c[4]), .sum(sum[4]), .andNum(andNum[4]), .orNum(orNum[4]), .less(1'b0), .sltNum(sltNum[4]));
	FA_ALU t5( .a(a[5]), .b(b[5]), .cin(c[4]), .control(cin), .cout(c[5]), .sum(sum[5]), .andNum(andNum[5]), .orNum(orNum[5]), .less(1'b0), .sltNum(sltNum[5]));
	FA_ALU t6( .a(a[6]), .b(b[6]), .cin(c[5]), .control(cin), .cout(c[6]), .sum(sum[6]), .andNum(andNum[6]), .orNum(orNum[6]), .less(1'b0), .sltNum(sltNum[6]));
	FA_ALU t7( .a(a[7]), .b(b[7]), .cin(c[6]), .control(cin), .cout(c[7]), .sum(sum[7]), .andNum(andNum[7]), .orNum(orNum[7]), .less(1'b0), .sltNum(sltNum[7]));
	FA_ALU t8( .a(a[8]), .b(b[8]), .cin(c[7]), .control(cin), .cout(c[8]), .sum(sum[8]), .andNum(andNum[8]), .orNum(orNum[8]), .less(1'b0), .sltNum(sltNum[8]));
	FA_ALU t9( .a(a[9]), .b(b[9]), .cin(c[8]), .control(cin), .cout(c[9]), .sum(sum[9]), .andNum(andNum[9]), .orNum(orNum[9]), .less(1'b0), .sltNum(sltNum[9]));
	FA_ALU t10( .a(a[10]), .b(b[10]), .cin(c[9]), .control(cin), .cout(c[10]), .sum(sum[10]), .andNum(andNum[10]), .orNum(orNum[10]), .less(1'b0), .sltNum(sltNum[10]));
	FA_ALU t11( .a(a[11]), .b(b[11]), .cin(c[10]), .control(cin), .cout(c[11]), .sum(sum[11]), .andNum(andNum[11]), .orNum(orNum[11]), .less(1'b0), .sltNum(sltNum[11]));
	FA_ALU t12( .a(a[12]), .b(b[12]), .cin(c[11]), .control(cin), .cout(c[12]), .sum(sum[12]), .andNum(andNum[12]), .orNum(orNum[12]), .less(1'b0), .sltNum(sltNum[12]));
	FA_ALU t13( .a(a[13]), .b(b[13]), .cin(c[12]), .control(cin), .cout(c[13]), .sum(sum[13]), .andNum(andNum[13]), .orNum(orNum[13]), .less(1'b0), .sltNum(sltNum[13]));
	FA_ALU t14( .a(a[14]), .b(b[14]), .cin(c[13]), .control(cin), .cout(c[14]), .sum(sum[14]), .andNum(andNum[14]), .orNum(orNum[14]), .less(1'b0), .sltNum(sltNum[14]));
	FA_ALU t15( .a(a[15]), .b(b[15]), .cin(c[14]), .control(cin), .cout(c[15]), .sum(sum[15]), .andNum(andNum[15]), .orNum(orNum[15]), .less(1'b0), .sltNum(sltNum[15]));
	FA_ALU t16( .a(a[16]), .b(b[16]), .cin(c[15]), .control(cin), .cout(c[16]), .sum(sum[16]), .andNum(andNum[16]), .orNum(orNum[16]), .less(1'b0), .sltNum(sltNum[16]));
	FA_ALU t17( .a(a[17]), .b(b[17]), .cin(c[16]), .control(cin), .cout(c[17]), .sum(sum[17]), .andNum(andNum[17]), .orNum(orNum[17]), .less(1'b0), .sltNum(sltNum[17]));
	FA_ALU t18( .a(a[18]), .b(b[18]), .cin(c[17]), .control(cin), .cout(c[18]), .sum(sum[18]), .andNum(andNum[18]), .orNum(orNum[18]), .less(1'b0), .sltNum(sltNum[18]));
	FA_ALU t19( .a(a[19]), .b(b[19]), .cin(c[18]), .control(cin), .cout(c[19]), .sum(sum[19]), .andNum(andNum[19]), .orNum(orNum[19]), .less(1'b0), .sltNum(sltNum[19]));
	FA_ALU t20( .a(a[20]), .b(b[20]), .cin(c[19]), .control(cin), .cout(c[20]), .sum(sum[20]), .andNum(andNum[20]), .orNum(orNum[20]), .less(1'b0), .sltNum(sltNum[20]));
	FA_ALU t21( .a(a[21]), .b(b[21]), .cin(c[20]), .control(cin), .cout(c[21]), .sum(sum[21]), .andNum(andNum[21]), .orNum(orNum[21]), .less(1'b0), .sltNum(sltNum[21]));
	FA_ALU t22( .a(a[22]), .b(b[22]), .cin(c[21]), .control(cin), .cout(c[22]), .sum(sum[22]), .andNum(andNum[22]), .orNum(orNum[22]), .less(1'b0), .sltNum(sltNum[22]));
	FA_ALU t23( .a(a[23]), .b(b[23]), .cin(c[22]), .control(cin), .cout(c[23]), .sum(sum[23]), .andNum(andNum[23]), .orNum(orNum[23]), .less(1'b0), .sltNum(sltNum[23]));
	FA_ALU t24( .a(a[24]), .b(b[24]), .cin(c[23]), .control(cin), .cout(c[24]), .sum(sum[24]), .andNum(andNum[24]), .orNum(orNum[24]), .less(1'b0), .sltNum(sltNum[24]));
	FA_ALU t25( .a(a[25]), .b(b[25]), .cin(c[24]), .control(cin), .cout(c[25]), .sum(sum[25]), .andNum(andNum[25]), .orNum(orNum[25]), .less(1'b0), .sltNum(sltNum[25]));
	FA_ALU t26( .a(a[26]), .b(b[26]), .cin(c[25]), .control(cin), .cout(c[26]), .sum(sum[26]), .andNum(andNum[26]), .orNum(orNum[26]), .less(1'b0), .sltNum(sltNum[26]));
	FA_ALU t27( .a(a[27]), .b(b[27]), .cin(c[26]), .control(cin), .cout(c[27]), .sum(sum[27]), .andNum(andNum[27]), .orNum(orNum[27]), .less(1'b0), .sltNum(sltNum[27]));
	FA_ALU t28( .a(a[28]), .b(b[28]), .cin(c[27]), .control(cin), .cout(c[28]), .sum(sum[28]), .andNum(andNum[28]), .orNum(orNum[28]), .less(1'b0), .sltNum(sltNum[28]));
	FA_ALU t29( .a(a[29]), .b(b[29]), .cin(c[28]), .control(cin), .cout(c[29]), .sum(sum[29]), .andNum(andNum[29]), .orNum(orNum[29]), .less(1'b0), .sltNum(sltNum[29]));
	FA_ALU t30( .a(a[30]), .b(b[30]), .cin(c[29]), .control(cin), .cout(c[30]), .sum(sum[30]), .andNum(andNum[30]), .orNum(orNum[30]), .less(1'b0), .sltNum(sltNum[30]));
	FA_ALU t31( .a(a[31]), .b(b[31]), .cin(c[30]), .control(cin), .cout(c[31]), .sum(sum[31]), .andNum(andNum[31]), .orNum(orNum[31]), .less(1'b0), .sltNum(sltNum[31]));

endmodule



