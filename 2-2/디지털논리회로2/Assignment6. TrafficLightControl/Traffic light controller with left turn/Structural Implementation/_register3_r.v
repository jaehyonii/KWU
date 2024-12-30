module _register3_r(clk, reset_n, D, Q);
	input clk, reset_n;
	input [2:0] D;
	output [2:0] Q;
	
	// instances of _dff_r_async
	_dff_r_async U0_dff_r_async(.clk(clk), .reset_n(reset_n), .d(D[0]), .q(Q[0])); // ind0 of d, q
	_dff_r_async U1_dff_r_async(.clk(clk), .reset_n(reset_n), .d(D[1]), .q(Q[1])); // ind1 of d, q
	_dff_r_async U2_dff_r_async(.clk(clk), .reset_n(reset_n), .d(D[2]), .q(Q[2])); // ind2 of d, q
	
endmodule