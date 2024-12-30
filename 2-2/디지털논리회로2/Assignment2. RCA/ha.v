module ha(a, b, s, co);
	input a, b;
	output co, s;
	
	// co = a & b;
	_and2 nd(a, b, co);
	
	// s = a ^ b;
	_xor2 xr(a, b, s);
endmodule
