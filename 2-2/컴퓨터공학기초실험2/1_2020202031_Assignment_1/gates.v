module _nand2(y, a, b); // 2 input nand gate
	input a, b; // 2 input
	output y; // output
	
	assign y = ~(a & b); // nand = not and
	
endmodule


module _inv(y, a); // inverter
	input a; // input
	output y; // output
	
	assign y = ~a; // not
	
endmodule
