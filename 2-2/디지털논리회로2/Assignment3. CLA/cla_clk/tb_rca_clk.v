`timescale 1ns/100ps
module tb_rca_clk;
	reg clk; // set reg
	reg [31:0] tb_a, tb_b; 
	reg tb_ci;
	wire [31:0] tb_s; // set wire
	wire tb_co;
	
	parameter STEP = 10; // clock period is 10ns
	
	rca_clk U0_rca_clk(.clk(clk), .a(tb_a), .b(tb_b), .ci(tb_ci), .s(tb_s), .co(tb_co));

	always # (STEP/2) clk = ~clk; // clk switch per 5ns
	
	initial
	begin
		// expected co_s (hexadecimal)
		clk = 1'b1; tb_a = 32'h0; tb_b = 32'h0; tb_ci = 1'b0; // 
		#(STEP-2); tb_a = 32'hFFFF_FFFF; tb_b = 32'h0; tb_ci = 1'b1;
		#(STEP); tb_a = 32'h0000_FFFF; tb_b = 32'hFFFF_0000; tb_ci = 1'b0;
		#(STEP); tb_a = 32'h135f_a562; tb_b = 32'h3561_4642; tb_ci = 1'b0;
		#(STEP*2); $stop;
	end
endmodule