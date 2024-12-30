module ha(a, b, s, co);
	input a, b;
	output s, co;
	
	assign s = a ^ b; // sum = a xor b
	assign co = a & b; // co = a and b
endmodule