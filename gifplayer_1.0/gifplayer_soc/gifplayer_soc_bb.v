
module gifplayer_soc (
	clk_clk,
	reset_reset_n,
	sdram_clk_clk,
	sdram_wire_addr,
	sdram_wire_ba,
	sdram_wire_cas_n,
	sdram_wire_cke,
	sdram_wire_cs_n,
	sdram_wire_dq,
	sdram_wire_dqm,
	sdram_wire_ras_n,
	sdram_wire_we_n,
	sram_wire_DQ,
	sram_wire_ADDR,
	sram_wire_LB_N,
	sram_wire_UB_N,
	sram_wire_CE_N,
	sram_wire_OE_N,
	sram_wire_WE_N,
	sw_wire_export);	

	input		clk_clk;
	input		reset_reset_n;
	output		sdram_clk_clk;
	output	[12:0]	sdram_wire_addr;
	output	[1:0]	sdram_wire_ba;
	output		sdram_wire_cas_n;
	output		sdram_wire_cke;
	output		sdram_wire_cs_n;
	inout	[31:0]	sdram_wire_dq;
	output	[3:0]	sdram_wire_dqm;
	output		sdram_wire_ras_n;
	output		sdram_wire_we_n;
	inout	[15:0]	sram_wire_DQ;
	output	[19:0]	sram_wire_ADDR;
	output		sram_wire_LB_N;
	output		sram_wire_UB_N;
	output		sram_wire_CE_N;
	output		sram_wire_OE_N;
	output		sram_wire_WE_N;
	input	[7:0]	sw_wire_export;
endmodule
