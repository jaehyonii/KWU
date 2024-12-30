`timescale 1ns/100ps

module tb_dff;
	reg tb_clk, tb_d;
	wire tb_q, tb_q_bar;
	
	_dff U0_dff(.clk(tb_clk), .d(tb_d), .q(tb_q), .q_bar(tb_q_bar)); // put tb values to instance of _dff
	
	always begin
		#5; tb_clk = ~tb_clk; // clock period is 10ns
	end
		
	initial begin
		tb_clk = 0; tb_d = 0; // set initial value of registers
		#3; tb_d = 1;
		#3; tb_d = 0;
		#6; tb_d = 1;
		#2; tb_d = 0;
		#4; tb_d = 1;
		#4; tb_d = 0;
		#7; $stop;
	end

endmodule