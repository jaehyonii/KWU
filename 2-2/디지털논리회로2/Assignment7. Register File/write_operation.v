module write_operation(Addr, we, wEn);
	input we;
	input [2:0] Addr; // 
	output [7:0] wEn; // select which index we will update in register
	
	wire [7:0] w_a;
	
	// one hot decoder
	_3_to_8_decoder U0_3_to_8_decoder(.d(Addr), .q(w_a));
	
	// if we == 1'b1, and gates will result same value with w_a
	// if we == 1'b0, and gates always result 0
	assign wEn = (we == 1'b1) ? w_a : 8'b0;
	
endmodule