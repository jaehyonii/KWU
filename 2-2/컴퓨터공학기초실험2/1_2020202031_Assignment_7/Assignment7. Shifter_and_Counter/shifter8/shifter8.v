module shifter8(clk, reset_n, op, shamt, d_in, d_out);
	input clk, reset_n;
	input [2:0] op;
	input [1:0] shamt; // shift amount
	input [7:0] d_in;
	output [7:0] d_out;
	
	wire[7:0] d_next;
	
	// instances of cc_logic and _register8_r
	cc_logic U0(.op(op), .shamt(shamt), .d_in(d_in), .d_out(d_out), .d_next(d_next));
	_register8_r U1(.clk(clk), .reset_n(reset_n), .d(d_next), .q(d_out));
	
endmodule