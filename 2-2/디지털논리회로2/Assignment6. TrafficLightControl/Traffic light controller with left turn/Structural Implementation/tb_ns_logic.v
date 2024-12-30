`timescale 1ns/100ps

module tb_ns_logic;
	reg tb_Ta, tb_Tal, tb_Tb, tb_Tbl;
	reg [2:0] tb_Q;
	wire [2:0] tb_D;
	
	ns_logic U0(.Ta(tb_Ta), .Tal(tb_Tal), .Tb(tb_Tb), .Tbl(tb_Tbl), .D(tb_D), .Q(tb_Q));
	
	initial begin
		tb_Q = 3'b000;
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b1000; #5;
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b1001; #5;
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b0100; #5;
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b0101; #5;
		
		tb_Q = 3'b001;
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b1000; #5;
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b1011; #5;
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b0100; #5;
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b0111; #5;
		
		tb_Q = 3'b010;
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b0110; #5;
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b0111; #5;
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b1010; #5;
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b1011; #5;
		
		tb_Q = 3'b011;
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b0110; #5;
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b0111; #5;
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b1000; #5;
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b1001; #5;
		
		tb_Q = 3'b100;
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b0010; #5;
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b0111; #5;
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b1000; #5;
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b1101; #5;
		
		tb_Q = 3'b101;
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b0110; #5;
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b0111; #5;
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b1100; #5;
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b1101; #5;
		
		tb_Q = 3'b110;
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b0001; #5;
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b0011; #5;
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b1000; #5;
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b1010; #5;
		
		tb_Q = 3'b111;
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b0001; #5;
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b0011; #5;
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b1000; #5;
		{tb_Ta, tb_Tal, tb_Tb, tb_Tbl} = 4'b1010; #5;
		
		
		$stop;
	end
	
endmodule