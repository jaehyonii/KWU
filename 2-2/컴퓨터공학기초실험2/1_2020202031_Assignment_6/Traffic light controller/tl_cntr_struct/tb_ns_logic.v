`timescale 1ns/100ps

module tb_ns_logic;
	reg tb_Ta, tb_Tb;
	reg [1:0] tb_Q;
	wire [1:0] tb_D;
	
	// instance of ns_logic
	ns_logic U0_ns_logic(.Ta(tb_Ta), .Tb(tb_Tb), .D(tb_D), .Q(tb_Q));
	
	initial begin
		tb_Q = 2'b00; tb_Ta = 0; tb_Tb = 0; #5; // Q: 00, D_Expected: 01
		tb_Q = 2'b00; tb_Ta = 0; tb_Tb = 1; #5; // Q: 00, D_Expected: 01
		tb_Q = 2'b00; tb_Ta = 1; tb_Tb = 0; #5; // Q: 00, D_Expected: 00
		tb_Q = 2'b00; tb_Ta = 1; tb_Tb = 1; #5; // Q: 00, D_Expected: 00
		tb_Q = 2'b01; tb_Ta = 0; tb_Tb = 0; #5; // Q: 01, D_Expected: 10
		tb_Q = 2'b01; tb_Ta = 0; tb_Tb = 1; #5; // Q: 01, D_Expected: 10
		tb_Q = 2'b01; tb_Ta = 1; tb_Tb = 0; #5; // Q: 01, D_Expected: 10
		tb_Q = 2'b01; tb_Ta = 1; tb_Tb = 1; #5; // Q: 01, D_Expected: 10
		tb_Q = 2'b10; tb_Ta = 0; tb_Tb = 1; #5; // Q: 10, D_Expected: 10
		tb_Q = 2'b10; tb_Ta = 1; tb_Tb = 1; #5; // Q: 10, D_Expected: 10
		tb_Q = 2'b10; tb_Ta = 0; tb_Tb = 0; #5; // Q: 10, D_Expected: 11
		tb_Q = 2'b10; tb_Ta = 1; tb_Tb = 0; #5; // Q: 10, D_Expected: 11
		tb_Q = 2'b11; tb_Ta = 0; tb_Tb = 0; #5; // Q: 11, D_Expected: 00
		tb_Q = 2'b11; tb_Ta = 1; tb_Tb = 0; #5; // Q: 11, D_Expected: 00
		tb_Q = 2'b11; tb_Ta = 0; tb_Tb = 1; #5; // Q: 11, D_Expected: 00
		tb_Q = 2'b11; tb_Ta = 1; tb_Tb = 1; #5; // Q: 11, D_Expected: 00
		$stop;
	end
	
endmodule