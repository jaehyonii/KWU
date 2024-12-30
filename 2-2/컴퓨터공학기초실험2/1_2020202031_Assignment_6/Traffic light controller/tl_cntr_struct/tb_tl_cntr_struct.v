`timescale 1ns/100ps

module tb_tl_cntr_struct;
	reg clk, reset_n;
	reg Ta, Tb;
	wire [1:0] La, Lb;
	
	// instance of tl_cntr_struct
	tl_cntr_struct U0(.clk(clk), .reset_n(reset_n), .Ta(Ta), .Tb(Tb), .La(La), .Lb(Lb));

	always begin
		#5 clk = ~clk; // clock period 10ns
	end
	
	initial begin
		clk = 0; reset_n = 0; Ta = 1; Tb = 1; #8; // set initial value
		reset_n = 1; Tb = 0; #10;
		Ta = 0; #10;
		Tb = 1; #10;
		Ta = 1; #10;
		Tb = 0; #10;
		Ta = 0; #10;
		Ta = 1; #10;
		$stop;
	end
endmodule