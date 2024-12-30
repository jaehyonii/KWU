module cla_clk(clk, a, b, ci, s, co);
	input clk; // set input
	input [31:0] a, b;
	input ci;
	output [31:0] s; // set output
	output co;
	
	reg [31:0] reg_a, reg_b; // set reg
	reg reg_ci;
	reg [31:0] reg_s;
	reg reg_co;
	
	wire [31:0] wire_s; // set wire
	wire wire_co;

	// D-flipflop
	always @ (posedge clk) // when clock rising
	begin // deliver value to D-flipflop
		reg_a <= a;
		reg_b <= b;
		reg_ci <= ci;
		reg_s <= wire_s;
		reg_co <= wire_co;
	end
	
	cla32 U0_cla32(.a(reg_a), .b(reg_b), .ci(reg_ci), .s(wire_s), .co(wire_co));

	assign s = reg_s;
	assign co = reg_co;
endmodule
