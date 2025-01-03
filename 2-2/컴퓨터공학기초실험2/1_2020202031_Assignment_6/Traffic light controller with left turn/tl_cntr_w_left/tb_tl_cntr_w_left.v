`timescale 1ns/100ps

module tb_tl_cntr_w_left;
	reg tb_clk, tb_reset_n, tb_Ta, tb_Tal, tb_Tb, tb_Tbl;
	wire [1:0] tb_La, tb_Lb;
	
	tl_cntr_w_left U0(.clk(tb_clk), .reset_n(tb_reset_n), .Ta(tb_Ta), .Tal(tb_Tal), .Tb(tb_Tb), .Tbl(tb_Tbl), .La(tb_La), .Lb(tb_Lb));

	always begin
		#5; tb_clk = ~tb_clk;
	end
	
	initial begin
		tb_clk = 0; tb_reset_n = 0; {tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b0000; #2;
		tb_reset_n = 1;
		// state: S0
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b1000; #10;
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b1001; #10;
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b1010; #10;
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b1100; #10;
		
		// state: S1
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b0000; #10;
		
		// state: S2
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b1000; #10;
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b0100; #10;
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b0101; #10;
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b0110; #10;
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b1100; #10;
		
		// state: S3
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b0000; #10;
		
		// state: S4
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b0100; #10;
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b0010; #10;
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b0011; #10;
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b0110; #10;
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b1010; #10;
		
		// state: S5
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b0000; #10;
		
		// // state: S6
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b0010; #10;
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b0001; #10;
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b1001; #10;
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b0101; #10;
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b0011; #10;
		
		// // state: S7
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b0000; #10;
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b0001; #10;
		
		$stop;
	end
endmodule