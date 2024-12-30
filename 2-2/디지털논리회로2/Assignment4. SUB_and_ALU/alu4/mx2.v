module mx2(d0, d1, s, y);
	input d0, d1;
	input s;
	output y;
	
	wire sb, w0, w1;
	
	/* ((d0s')'(d1s)')' */
	_inv iv0(.a(s), .y(sb)); // sb = s'
	_nand2 nd20(.a(d0), .b(sb), .y(w0)); // w0 = (d0s')'
	_nand2 nd21(.a(d1), .b(s), .y(w1)); // w1 = (d1s)'
	_nand2 nd22(.a(w0), .b(w1), .y(y)); // y = (w0w1)' = ((d0s')'(d1s)')'
endmodule