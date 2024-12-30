module cntr8(clk, reset_n, inc, load, d_in, d_out, o_state);
	input clk, reset_n, inc, load;
	input [7:0] d_in;
	output [7:0] d_out;
	output [2:0] o_state;
	
	wire [2:0] next_state;
	wire [2:0] state;	
	
	assign o_state = state;
	
	ns_logic U0(.load(load), .inc(inc), .state(state), .next_state(next_state));
	_register3_r U1(.clk(clk), .reset_n(reset_n), .d(next_state), .q(state));
	os_logic U2(.state(state), .d_in(d_in), .d_out(d_out));
	
endmodule