module _dff_r_sync_async(clk, reset_n, d, q_sync, q_async);
	input clk, reset_n, d;
	output q_sync, q_async;
	
	_dff_r_sync	U0_dff_r_sync(.clk(clk), .reset_n(reset_n), .d(d), .q(q_sync));
	_dff_r_async U1_dff_r_async(.clk(clk), .reset_n(reset_n), .d(d), .q(q_async));

endmodule