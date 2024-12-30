`timescale 1ns/100ps
module tb_cla4;
	reg [3:0] tb_a, tb_b; //set reg
	reg tb_ci;
	
	wire [3:0] tb_s; // set wire
	wire tb_co;
	wire [4:0] tb_result;
	
	assign tb_result = {tb_co, tb_s};

	cla4 U0_cla4(.a(tb_a), .b(tb_b), .ci(tb_ci), .s(tb_s), .co(tb_co));
	
	initial
	begin
		// expected co_s (hexadecimal)
		tb_a = 0; tb_b = 0; tb_ci = 0; // 0_0
		#10; tb_a = 4'h0; tb_b = 4'hf; tb_ci = 1; // 1_0
		#10; tb_a = 4'hf; tb_b = 4'h0; tb_ci = 0; // 0_f
		#10; tb_a = 4'ha; tb_b = 4'he; tb_ci = 1; // 1_9
		#10; tb_a = 4'h9; tb_b = 4'hc; tb_ci = 0; // 1_5
		#10; $stop;
	end
endmodule