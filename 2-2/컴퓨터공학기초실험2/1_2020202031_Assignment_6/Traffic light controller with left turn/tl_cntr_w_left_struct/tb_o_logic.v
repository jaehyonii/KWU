`timescale 1ns/100ps

module tb_o_logic;
	reg [2:0] tb_Q;
	wire [1:0] tb_La, tb_Lb;
	
	o_logic U0_o_logic(.Q(tb_Q), .La(tb_La), .Lb(tb_Lb));
	
	initial begin
		tb_Q = 3'b000; #5;
		tb_Q = 3'b001; #5;
		tb_Q = 3'b010; #5;
		tb_Q = 3'b011; #5;
		tb_Q = 3'b100; #5;
		tb_Q = 3'b101; #5;
		tb_Q = 3'b110; #5;
		tb_Q = 3'b111; #5;
		$stop;
	end
	
endmodule