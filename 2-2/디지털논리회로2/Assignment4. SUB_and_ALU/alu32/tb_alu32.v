`timescale 1ns/100ps

module tb_alu32;
	reg clk, reset;
	reg [31:0] tb_a, tb_b;
	reg [2:0] tb_op;
	reg useless; // just to make op to be be read as 4bits
	reg [31:0] r_expected; // result
	reg [3:0] f_expected; // flags
	wire [31:0] tb_result;
	wire tb_c, tb_n, tb_z, tb_v;
	
	reg [31:0] vectornum, errors; // bookkeeping variables
	reg [103:0] testvectors[10000:0]; // array of testvectors (ex: a_b_op_result_flags)
	
	// instantiate device under test
	alu32 U0_alu32(.a(tb_a), .b(tb_b), .op(tb_op), .result(tb_result), .c(tb_c), .n(tb_n), .z(tb_z), .v(tb_v));
	
	// generate clock
	always begin
		clk = 1; #5; clk = 0; #5;
	end
	
	// at start of test, load vectors
	// and pulse reset
	initial begin
		$readmemh("example.tv", testvectors);
		vectornum = 0; errors = 0;
		reset = 1; #27; reset = 0;
	end
	
	// apply test vectors on rising dege of clk
	always @(posedge clk) begin
		#1; {tb_a, tb_b, useless, tb_op, r_expected, f_expected} = testvectors[vectornum];
	end
		
	// check results on falling dege of clk
	always @(negedge clk) begin
		if(~reset) begin // skip during reset
			if({tb_result, tb_c, tb_n, tb_z, tb_v} !== {r_expected, f_expected}) begin
				$display("Error: inputs = %h, %h, %b", tb_a, tb_b, tb_op);
				$display("	output = %h, %b (%h, %b expected)", tb_result, {tb_c, tb_n, tb_z, tb_v}, r_expected, f_expected);
				errors = errors + 1;
			end
		
			//increment array index and read next testvector
			vectornum = vectornum + 1;
			if(testvectors[vectornum] === 104'bx) begin
				$display("%d tests completed with %d errors", vectornum, errors);
				$finish;
			end
		end
	end
endmodule
