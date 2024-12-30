module mx2(y, d0, d1, s); // 2 input MUX
	input d0, d1, s; // 3 input
	output y; // output
	
	assign y=(s==0)?d0:d1; // if(s=0) y=d0		else y=d1
	
endmodule
