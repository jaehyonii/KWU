module o_logic(Q, La, Lb);
	input [2:0]Q;
	output [1:0]La, Lb;
	
	wire Q2_bar, Q1_bar, Q0_bar;
	wire w0_La1;
	wire w1_La0, w0_La0;
	wire w0_Lb1;
	wire w1_Lb0, w0_Lb0;
	
	// get inverse
	_inv U0_inv(.a(Q[2]), .y(Q2_bar));
	_inv U1_inv(.a(Q[1]), .y(Q1_bar));
	_inv U2_inv(.a(Q[0]), .y(Q0_bar));
	
	// LA1 assign
	_and2 U3_and2(.a(Q[1]), .b(Q0_bar), .y(w0_La1));
	
	_or2 U4_or2(.a(w0_La1), .b(Q[2]), .y(La[1]));
	
	// LA0 assign
	_and2 U5_and2(.a(Q2_bar), .b(Q[1]), .y(w1_La0));
	_and2 U6_and2(.a(Q2_bar), .b(Q[0]), .y(w0_La0));
	
	_or2 U7_or2(.a(w1_La0), .b(w0_La0), .y(La[0]));
	
	// LB1 assign
	_and2 U8_and2(.a(Q[1]), .b(Q0_bar), .y(w0_Lb1));
	
	_or2 U9_or2(.a(Q2_bar), .b(w0_Lb1), .y(Lb[1]));
	
	// LB0 assign
	_and2 U10_and2(.a(Q[2]), .b(Q[1]), .y(w1_Lb0));
	_and2 U11_and2(.a(Q[2]), .b(Q[0]), .y(w0_Lb0));
	_or2 U12_or2(.a(w1_Lb0), .b(w0_Lb0), .y(Lb[0]));
	
endmodule