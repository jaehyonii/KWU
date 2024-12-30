module _dff_en(clk, en, d, q);
	input		clk, en, d;
	output	q;
	
	wire w_d; // connect mx2 output and _dff input d
	
	mx2	U0_mx2(.d0(q), .d1(d), .s(en), .y(w_d)); // select value between q and d
	_dff	U1_dff(.clk(clk), .d(w_d), .q(q), .q_bar());
	
endmodule