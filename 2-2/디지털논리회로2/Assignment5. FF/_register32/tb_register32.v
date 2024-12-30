`timescale 1ns/100ps

module tb_register32;
	reg tb_clk;
	reg [31:0] tb_d;
	wire [31:0] tb_q;
	
	_register32 U0_register32(.clk(tb_clk), .d(tb_d), .q(tb_q));
	
	always begin
		#5; tb_clk = ~tb_clk;
	end
	
	initial begin
		tb_clk = 0; tb_d = 32'h12345678;
		#3; tb_d = 32'h98765432;
		#3; tb_d = 32'hffeeddcc;
		#6; tb_d = 32'hbbaa9988;
		#2; tb_d = 32'h77665544;
		#4; tb_d = 32'h33221100;
		#4; tb_d = 32'h12345678;
		#7; $stop;
	end

endmodule