`timescale 1ns/100ps

module tb_dff_en;
	reg tb_clk, tb_en, tb_d;
	wire q;
	
	_dff_en U0_dff_en(.clk(tb_clk), .en(tb_en), .d(tb_d), .q(tb_q)); // put tb values to instance to _dff_en
	
	always begin
		#5; tb_clk = ~tb_clk; // clock period is 10ns
	end

	initial begin
		tb_clk = 0; tb_d = 0; tb_en = 1; // set initial value of registers
		#3; tb_d = 1;
		#10; tb_d = 0;
		#10; tb_d = 1;
		#10; tb_d = 0;
		#10; tb_d = 1;
		#10; tb_d = 0; tb_en = 0;
		#10; tb_d = 1;
		#10; tb_d = 0;
		#10; tb_d = 1;
		#10; tb_d = 0;
		#10; tb_d = 1;
		#10; tb_d = 0;
		#10; $stop;
	end
	
endmodule