`timescale 1ns/100ps
module tb_cla32;
	reg [31:0] tb_a, tb_b; // wet reg
	reg tb_ci;
	
	wire [31:0] tb_s; // set wire
	wire tb_co;
	wire [32:0] tb_result;
	
	assign tb_result = {tb_co, tb_s};

	cla32 U0_cla32(.a(tb_a), .b(tb_b), .ci(tb_ci), .s(tb_s), .co(tb_co));
	
	initial
	begin
		// expected co_s (hexadecimal)
		tb_a = 32'h0; tb_b = 32'h0; tb_ci = 0; // 0_00000000
		#10; tb_a = 32'hffff_ffff; tb_b = 32'h0; tb_ci = 1; // 1_00000000
		#10; tb_a = 32'hffff_0000; tb_b = 32'h0000_ffff; tb_ci = 0; // 0_ffffffff
		#10; tb_a = 32'h1234_5678; tb_b = 32'h8765_4321; tb_ci = 1; // 0_9999999a
		#10; tb_a = 32'h2828_9583; tb_b = 32'h88ff_aabb; tb_ci = 0; // 0_b128403e
		#10; $stop;
	end
endmodule