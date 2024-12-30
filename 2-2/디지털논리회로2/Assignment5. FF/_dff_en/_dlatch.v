module _dlatch(clk,d,q,q_bar);
	input clk;
	input d;
	output q, q_bar;
	reg q; 
 
	always @(clk or d) begin // when clk or d signal changes
		if(clk == 1) // if clk is on
			q <= d; // put input d to output q
	end
	assign q_bar = ~q;
endmodule