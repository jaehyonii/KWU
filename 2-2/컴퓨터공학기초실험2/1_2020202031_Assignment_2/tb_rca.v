`timescale 1ns/100ps

module tb_rca;
	reg[3:0] tb_a, tb_b;
	reg tb_ci;
	wire[3:0] tb_s;
	wire tb_co;
	
	rca4 rca1(tb_a, tb_b, tb_ci, tb_s, tb_co);
	
initial
begin
	// checking sum performance
	#0		tb_a = 4'b0000; tb_b = 4'b0000; tb_ci = 0; // tb_s = 0000, tb_co = 0; expected
	#10	tb_ci = 1; // tb_s = 0001, tb_co = 0; expected
	#10	tb_a = 4'b0001; tb_b = 4'b0001; tb_ci = 1; // tb_s = 0011, tb_co = 0; expected
	#10	tb_a = 4'b0000; tb_b = 4'b1111; tb_ci = 0; // tb_s = 1111, tb_co = 0; expected
	#10	tb_a = 4'b1010; tb_b = 4'b0101; tb_ci = 0; // tb_s = 1111, tb_co = 0; expected
	#10	tb_a = 4'b0101; tb_b = 4'b1010; tb_ci = 0; // tb_s = 1111, tb_co = 0; expected

	//checking carryout performance
	#10	tb_a = 4'b1111; tb_b = 4'b0000; tb_ci = 1; // tb_s = 0000, tb_co = 1; expected
	#10	tb_a = 4'b1111; tb_b = 4'b1111; tb_ci = 1; // tb_s = 1111, tb_co = 1; expected
	
	#10	$finish;

end
endmodule
