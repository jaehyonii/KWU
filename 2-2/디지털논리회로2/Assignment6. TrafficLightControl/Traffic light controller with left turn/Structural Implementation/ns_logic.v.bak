module ns_logic(Ta, Tb, D, Q);
	input Ta, Tb;
	input [1:0] Q;
	output [1:0] D;
	
	wire Q1_bar, Q0_bar, Ta_bar, Tb_bar;
	wire w_1, w_0;
	
	// assign D1
	_xor2 U0_xor2(.a(Q[1]), .b(Q[0]), .y(D[1]));
	
	// assign D0
	_inv U1_inv(.a(Q[1]), .y(Q1_bar));
	_inv U2_inv(.a(Q[0]), .y(Q0_bar));
	_inv U3_inv(.a(Ta), .y(Ta_bar));
	_inv U4_inv(.a(Tb), .y(Tb_bar));
	_and3 U5_and3(.a(Q1_bar), .b(Q0_bar), .c(Ta_bar), .y(w_1));
	_and3 U6_and3(.a(Q[1]), .b(Q0_bar), .c(Tb_bar), .y(w_0));
	_or2 U7_or2(.a(w_1), .b(w_0), .y(D[0]));
	
endmodule