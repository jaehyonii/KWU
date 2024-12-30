module tl_cntr_w_left_struct(clk, reset_n, Ta, Tal, Tb, Tbl, La, Lb);
	input clk, reset_n, Ta, Tal, Tb, Tbl;
	output [1:0] La, Lb;
	
	wire [2:0]w_D, w_Q;
	
	ns_logic U0(.Ta(Ta), .Tal(Tal), .Tb(Tb), .Tbl(Tbl), .D(w_D), .Q(w_Q)); // Next State Logic
	_register3_r U1(.clk(clk), .reset_n(reset_n), .D(w_D), .Q(w_Q)); // State Register
	o_logic U2(.Q(w_Q), .La(La), .Lb(Lb)); // Output Logic
	
endmodule