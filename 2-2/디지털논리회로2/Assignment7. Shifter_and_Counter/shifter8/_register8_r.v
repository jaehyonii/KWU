module _register8_r(clk, reset_n, d, q);
	input clk, reset_n;
	input [7:0] d;
	output [7:0] q;
	
	// make 8bits register using 8 resettable d flipflop
	_dff_r U0(.clk(clk), .reset_n(reset_n), .d(d[0]), .q(q[0]));
	_dff_r U1(.clk(clk), .reset_n(reset_n), .d(d[1]), .q(q[1]));
	_dff_r U2(.clk(clk), .reset_n(reset_n), .d(d[2]), .q(q[2]));
	_dff_r U3(.clk(clk), .reset_n(reset_n), .d(d[3]), .q(q[3]));
	_dff_r U4(.clk(clk), .reset_n(reset_n), .d(d[4]), .q(q[4]));
	_dff_r U5(.clk(clk), .reset_n(reset_n), .d(d[5]), .q(q[5]));
	_dff_r U6(.clk(clk), .reset_n(reset_n), .d(d[6]), .q(q[6]));
	_dff_r U7(.clk(clk), .reset_n(reset_n), .d(d[7]), .q(q[7]));
	
endmodule