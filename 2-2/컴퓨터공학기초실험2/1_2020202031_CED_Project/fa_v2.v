module fa_v2(a, b, ci, s);
	input a, b, ci; // set input
	output s; // set output
	wire w0; // set wire
	
	// s = a^b^ci;
	_xor2 U0_xor2(.a(a), .b(b), .y(w0));
	_xor2 U1_xor2(.a(w0), .b(ci), .y(s));
endmodule