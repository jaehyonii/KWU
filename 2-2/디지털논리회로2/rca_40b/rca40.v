module rca_40b(S, A, B, Cout, Cin);

	input [39:0]  A, B;
	input			  Cin;
	output [39:0] S;
	output		  Cout;
	
	
	wire [39:0] Carry; // save Cout
	full_adder fa1(A[0], B[0], Cin, Carry[0], S[0]); // using input Cin for setting wire Carry
	
	genvar i;
	generate
	for(i=1;i<40;i=i+1) begin: rca_loop // loop
		full_adder fa2(A[i], B[i], Carry[i-1], Carry[i], S[i]); // using full adder 39times 
	end
	
	assign Cout=Carry[39];
	endgenerate
	
endmodule

module full_adder(input A, B, Cin, output Cout, Sum);
	assign w1 = A ^ B; // XOR operator
	assign w2 = A & B; // AND operator
	assign w3 = A & Cin; // AND operator
	assign w4 = B & Cin; // AND operator
	
	assign Cout = w2 | w3 | w4; // OR operator
	assign Sum = w1 ^ Cin; // XOR operator
endmodule
