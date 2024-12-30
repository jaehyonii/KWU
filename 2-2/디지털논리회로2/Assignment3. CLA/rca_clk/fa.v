module fa(a, b, ci, s, co); 
	input a, b, ci;
	output s, co;
	
	wire [2:0]w;
	
	ha U0_ha(.a(b), .b(ci), .s(w[0]), .co(w[1])); // put b, ci as input and put w[0], w[1] as output of halfadder
	ha U1_ha(.a(a), .b(w[0]), .s(s), .co(w[2])); // put a, w[0] as input and put s, w[2] as output of halfadder
	_or2 U2_or(.a(w[1]),.b(w[2]), .y(co)); // put w[1], w[2] as input and put co as output of OR gate
endmodule