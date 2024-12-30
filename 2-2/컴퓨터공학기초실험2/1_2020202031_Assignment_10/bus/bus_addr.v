module bus_addr(address, s0_sel, s1_sel);
	input [7:0]  address;
	output reg	 s0_sel, s1_sel;
	
	always @(address) begin
		if(address[7:5] == 3'b000)			{s0_sel, s1_sel} = 2'b10; // slave0 memory map
		else if(address[7:5] == 3'b001)	{s0_sel, s1_sel} = 2'b01; // slave1 memory map
		else										{s0_sel, s1_sel} = 2'b00; // not included in any memory maps
	end
	
endmodule
