module rca_clk(clk, a, b, ci, s, co);
	input clk; // set input
	input [31:0] a,b;
	input ci;
	output [31:0] s; // set output
	output co;

	reg [31:0] reg_a, reg_b; // set reg
	reg reg_ci;
	reg [31:0] reg_s;
	reg reg_co;
	
	/* output value of rca32 */
	wire [31:0] wire_s;
	wire wire_co;
	
	/* when clock rising, reg value will accept input value of D-flipflop */
	always@(posedge clk)
	begin
		reg_a <= a;
		reg_b <= b;
		reg_ci <= ci;
		reg_s <= wire_s;
		reg_co <= wire_co;
	end
	
	rca32 U0_rca32(.a(reg_a), .b(reg_b), .ci(reg_ci), .s(wire_s), .co(wire_co));
	
	assign s = reg_s;
	assign co = reg_co;
endmodule