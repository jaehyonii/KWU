module ha(a, b, s, co);
	input a, b; // set input
	output s, co; // set output
	
	assign s = a ^ b; // sum = a xor b
	assign co = a & b; // co = a and b
endmodule