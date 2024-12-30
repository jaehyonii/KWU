module mx2(y, d0, d1, s); // 2 input MUX
	input d0, d1, s; // 3 input
	output y; // output
	
	/* ((d0s')'(d1s)')' */
	_inv iv0(.y(sb), .a(s));
	_nand2 nd20(.y(w0), .a(d0), .b(sb)); // w0 = (d0s')'
	_nand2 nd21(.y(w1), .a(d1), .b(s)); // w1 = (d1s)'
	_nand2 nd22(.y(y), .a(w0), .b(w1)); // y = (w0w1)' = ((d0s')'(d1s)')'
	
endmodule
