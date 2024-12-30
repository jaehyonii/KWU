module cla4(a, b, ci, s, co);
	input [3:0] a, b;
	input ci;
	output [3:0] s;
	output co;
	
	wire [2:0] c;
	
	fa_v2 U0_fa(a[0], b[0], ci, s[0]);
	fa_v2 U1_fa(a[1], b[1], c[0], s[1]);
	fa_v2 U2_fa(a[2], b[2], c[1], s[2]);
	fa_v2 U3_fa(a[3], b[3], c[2], s[3]);
	clb4 U4_clb4(a, b, ci, c[0], c[1], c[2], co);
endmodule
