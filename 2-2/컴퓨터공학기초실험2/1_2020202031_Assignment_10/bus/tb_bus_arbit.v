`timescale 1ns/100ps

module tb_bus_arbit;
	reg  tb_clk, tb_reset_n, tb_m0_req, tb_m1_req;
	wire tb_m0_grant, tb_m1_grant;
	
	always begin
		#5; tb_clk = ~tb_clk;
	end
	
	bus_arbit U0(.clk(tb_clk), .reset_n(tb_reset_n), .m0_req(tb_m0_req), .m1_req(tb_m1_req), .m0_grant(tb_m0_grant), .m1_grant(tb_m1_grant));
	
	initial begin
		tb_clk = 0; tb_reset_n = 0; #3;
		
		tb_reset_n = 1;
		// M0_grant -> M0_grant		
		tb_m0_req = 0; tb_m1_req = 0; #10;
		tb_m0_req = 1; tb_m1_req = 0; #10;
		tb_m0_req = 1; tb_m1_req = 1; #10;
		
		// M0_grant -> M1_grant
		tb_m0_req = 0; tb_m1_req = 1; #10;
		
		// M1_grant -> M0_grant
		tb_m0_req = 0; tb_m1_req = 0; #10;
		tb_m0_req = 1; tb_m1_req = 0; #10;
		
		// M1_grant -> M1_grant
		tb_m0_req = 0; tb_m1_req = 1; #10;
		tb_m0_req = 1; tb_m1_req = 1; #10;
		
		// always M0_grant
		tb_reset_n = 0; #5;
		
		$stop;
	end
	
endmodule
