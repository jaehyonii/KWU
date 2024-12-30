module Register_file(clk, reset_n, wAddr, wData, we, rAddr, rData);
	input clk, reset_n, we;
	input [2:0] wAddr, rAddr;
	input [31:0] wData;
	output [31:0] rData;

	wire [7:0] wEn;
	wire [31:0] from_reg[7:0];

	// write operation
	write_operation U0(.Addr(wAddr), .we(we), .wEn(wEn));
	
	// update proper index data
	register32_8 U1(.clk(clk), .reset_n(reset_n), .en(wEn), .d_in(wData), .d_out0(from_reg[0]), .d_out1(from_reg[1]), .d_out2(from_reg[2]),
					.d_out3(from_reg[3]), .d_out4(from_reg[4]), .d_out5(from_reg[5]), .d_out6(from_reg[6]), .d_out7(from_reg[7]));
	
	// read operation
	read_operation U2(.Addr(rAddr), .Data(rData), .from_reg0(from_reg[0]), .from_reg1(from_reg[1]), .from_reg2(from_reg[2]),
						.from_reg3(from_reg[3]), .from_reg4(from_reg[4]), .from_reg5(from_reg[5]), .from_reg6(from_reg[6]), .from_reg7(from_reg[7]));
	
endmodule