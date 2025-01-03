module cla8(a, b, ci, s, co);
	input [7:0] a, b; // set input
	input ci;
	output [7:0] s; // set output
	output co;
	wire c1; // set wire
	
	/* connect 8 4-bit CLAs to make 32-bit CLA 
	all cla4 delivers its co to ci port of next cla4*/
	cla4 U0_cla4(.a(a[3:0]), .b(b[3:0]), .ci(ci), .s(s[3:0]), .co(c1));
	cla4 U1_cla4(.a(a[7:4]), .b(b[7:4]), .ci(c1), .s(s[7:4]), .co(co));
	
endmodule
