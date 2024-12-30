module ns_logic(Ta, Tal, Tb, Tbl, D, Q);
	input Ta, Tal, Tb, Tbl;
	input [2:0] Q;
	output [2:0] D;
	
	wire Q2_bar, Q1_bar, Q0_bar, Ta_bar, Tal_bar, Tb_bar, Tbl_bar;
	wire w2_D2, w1_D2, w0_D2; // used for finding D2
	wire w3_D0, w2_D0, w1_D0, w0_D0; // used for finding D0
	
	// get inverse
	_inv U0_inv(.a(Q[2]), .y(Q2_bar));
	_inv U1_inv(.a(Q[1]), .y(Q1_bar));
	_inv U2_inv(.a(Q[0]), .y(Q0_bar));
	_inv U3_inv(.a(Ta), .y(Ta_bar));
	_inv U4_inv(.a(Tal), .y(Tal_bar));
	_inv U5_inv(.a(Tb), .y(Tb_bar));
	_inv U6_inv(.a(Tbl), .y(Tbl_bar));	
	
	// assign D2
	_and3 U7_and3(.a(Q2_bar), .b(Q[1]), .c(Q[0]), .y(w2_D2));
	_and2 U8_and2(.a(Q[2]), .b(Q1_bar), .y(w1_D2));
	_and2 U9_and2(.a(Q[2]), .b(Q0_bar), .y(w0_D2));
	
	_or3 U10_or3(.a(w2_D2), .b(w1_D2), .c(w0_D2), .y(D[2]));
	
	// assign D1
	_xor2 U11_xor2(.a(Q[1]), .b(Q[0]), .y(D[1]));	
	
	// assign D0
	_and4 U12_and4(.a(Q2_bar), .b(Q1_bar), .c(Q0_bar), .d(Ta_bar), .y(w3_D0));
	_and4 U13_and4(.a(Q2_bar), .b(Q[1]), .c(Q0_bar), .d(Tal_bar), .y(w2_D0));
	_and4 U14_and4(.a(Q[2]), .b(Q1_bar), .c(Q0_bar), .d(Tb_bar), .y(w1_D0));
	_and4 U15_and4(.a(Q[2]), .b(Q[1]), .c(Q0_bar), .d(Tbl_bar), .y(w0_D0));
	
	_or4 U16_or4(.a(w3_D0), .b(w2_D0), .c(w1_D0), .d(w0_D0), .y(D[0]));
	
endmodule