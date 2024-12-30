module o_logic(Q, La, Lb);
	input [1:0]Q;
	output [1:0]La, Lb;
	
	wire Q1_bar;
	
	// LA1 assign
	assign La[1] = Q[1];
	
	// LA0 assign
	_inv U0_inv(.a(Q[1]), .y(Q1_bar));
	_and2 U1_and2(.a(Q1_bar), .b(Q[0]), .y(La[0]));
	
	// LB1 assign
	_inv U2_inv(.a(Q[1]), .y(Lb[1]));
	
	// LB0 assign
	_and2 U3_and2(.a(Q[1]), .b(Q[0]), .y(Lb[0]));
	
endmodule