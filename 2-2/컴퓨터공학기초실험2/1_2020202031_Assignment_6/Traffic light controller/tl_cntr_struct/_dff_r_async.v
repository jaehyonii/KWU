module _dff_r_async(clk, reset_n, d, q);
	input clk, reset_n, d;
	output q;
	reg q;
	
	// detect rising clk, falling reset_n
	always@(posedge clk or negedge reset_n) begin
		if(reset_n == 0)	q <= 1'b0; // reset when reset_n is low
		else					q <= d; // assign d to q when clk is rising
	end
	
endmodule