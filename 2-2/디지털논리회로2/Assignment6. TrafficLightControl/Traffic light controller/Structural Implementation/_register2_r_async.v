module _register2_r_async(clk, reset_n, D, Q);
	input clk, reset_n;
	input [1:0]D;
	output [1:0]Q;
	
	// put 2 bits data to two Dff
	_dff_r_async U0_dff_r_async(.clk(clk), .reset_n(reset_n), .d(D[1]), .q(Q[1]));
	_dff_r_async U1_dff_r_async(.clk(clk), .reset_n(reset_n), .d(D[0]), .q(Q[0]));
endmodule