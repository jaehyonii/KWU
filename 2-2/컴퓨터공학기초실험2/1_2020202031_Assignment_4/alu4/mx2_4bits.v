module mx2_4bits(d0, d1, s, y);
	input [3:0] d0, d1;
	input s;
	output [3:0] y;
	
	
	mx2 U0_mx2(.d0(d0[0]), .d1(d1[0]), .s(s), .y(y[0])); // put ind0 to mx2
	mx2 U1_mx2(.d0(d0[1]), .d1(d1[1]), .s(s), .y(y[1])); // put ind1 to mx2
	mx2 U2_mx2(.d0(d0[2]), .d1(d1[2]), .s(s), .y(y[2])); // put ind2 to mx2
	mx2 U3_mx2(.d0(d0[3]), .d1(d1[3]), .s(s), .y(y[3])); // put ind3 to mx2
endmodule
