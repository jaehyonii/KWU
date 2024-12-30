`timescale 1ns/100ps

module tb_multiplier;
	reg	tb_clk, tb_reset_n;
	reg	[63:0] tb_multiplier, tb_multiplicand;
	reg	tb_op_start, tb_op_clear;
	wire	tb_op_done;
	wire	[127:0] tb_result;
	
	parameter INIT = 2'b00;
	parameter MUL  = 2'b01;
	parameter DONE = 2'b10;
	
	multiplier U0(.clk(tb_clk), .reset_n(tb_reset_n), .multiplier(tb_multiplier), .multiplicand(tb_multiplicand),
					.op_start(tb_op_start), .op_clear(tb_op_clear), .op_done(tb_op_done), .result(tb_result));
	
	always begin
		#5; tb_clk = ~tb_clk;
	end
	
	initial begin
		tb_clk = 0; tb_reset_n = 0;
		tb_multiplier = 64'h1239857abdabcdef; tb_multiplicand = 64'hafffabcdefaaaadd;
		tb_op_start = 1'b0; tb_op_clear = 1'b0; #3;
		tb_reset_n = 1; #2;
		tb_op_start = 1'b1; tb_op_clear = 1'b0; #5;
		tb_op_start = 1'b0; tb_op_clear = 1'b0; #5;
		
		tb_op_start = 1'b1; tb_op_clear = 1'b0; #100;
		
		tb_multiplier = 64'h986128497; tb_multiplicand = 64'habcdefabcdefabcd; #100; // change operands while calculation
		tb_op_start = 1'b0; #50;
		tb_op_start = 1'b1; #50;
		tb_op_start = 1'b0; #50;
		
		tb_op_start = 1'b0; tb_op_clear = 1'b1; #10; // initializing
		tb_op_start = 1'b1; tb_op_clear = 1'b0; #10; // state SET
		#20; // state MUL
		tb_op_start = 1'b0; tb_op_clear = 1'b1; #30; // initializing
		
		$stop;
	end
	
endmodule
