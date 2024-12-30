// y = ~a;
module _inv(a, y);
	input a;
	output y;
	assign y = ~a;
endmodule

// y = a & b;
module _and2(a, b ,y);
	input a, b;
	output y;
	assign y = a & b;
endmodule

// y = a | b;
module _or2(a, b, y);
	input a, b;
	output y;
	
	assign y = a | b;	
endmodule

// y = ~(a & b);
module _nand2(y, a, b);
	input a, b;
	output y;
	
	assign y = ~(a & b);
endmodule

// y = a ^ b;
module _xor2(a, b, y);
	input a, b;
	output y;
	wire inv_a, inv_b;
	wire w0, w1;	
	
	_inv invA(a, inv_a);
	_inv invB(b, inv_b);
	_and2 and1(inv_a, b, w0);
	_and2 and2(a, inv_b, w1);
	_or2 or1(w0, w1, y);
endmodule
	