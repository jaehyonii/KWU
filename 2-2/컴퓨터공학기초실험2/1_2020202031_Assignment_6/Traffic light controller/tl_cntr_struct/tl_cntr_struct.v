module tl_cntr_struct(clk, reset_n, Ta, Tb, La, Lb);
	input clk, reset_n, Ta, Tb;
	output [1:0]La, Lb;
	
	wire [1:0]w_D, w_Q;
	
	ns_logic U0(.Ta(Ta), .Tb(Tb), .D(w_D), .Q(w_Q)); // Next State Logic
	_register2_r_async U1(.clk(clk), .reset_n(reset_n), .D(w_D), .Q(w_Q)); // State Register
	o_logic U2(.Q(w_Q), .La(La), .Lb(Lb)); // Output Logic

endmodule