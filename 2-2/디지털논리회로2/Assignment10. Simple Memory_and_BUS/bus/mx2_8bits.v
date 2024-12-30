module mx2_8bits(d0, d1, s, y);
	input [7:0] d0, d1;
	input s;
	output [7:0] y;

	assign y=(s==0)?d0:d1; // if(s=0) y=d0		else y=d1
endmodule
