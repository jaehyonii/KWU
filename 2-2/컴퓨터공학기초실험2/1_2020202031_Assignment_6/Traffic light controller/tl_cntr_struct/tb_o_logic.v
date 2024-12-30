`timescale 1ns/100ps

module tb_o_logic;
	reg [1:0] tb_Q;
	wire [1:0]tb_La, tb_Lb;
	
	o_logic U0_o_logic(.Q(tb_Q), .La(tb_La), .Lb(tb_Lb));
	
	initial begin
		tb_Q = 2'b00; #5; // state S0, La: green, Lb: red
		tb_Q = 2'b01; #5; // state S1, La: orange, Lb: red
		tb_Q = 2'b10; #5; // state S2, La: red, Lb: green
		tb_Q = 2'b11; #5; // state S3, La: red, Lb: orange
		$stop;
	end
	
endmodule