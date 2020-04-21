/************************************************************************
gifplayer Quartus Project Top Level

by Ian Rudnick and Braeden Smith
for ECE 385 final project

This file instantiates and connects the highest-level modules.
The goal of this project is to create a gif decoder that can play gif
files stored in memory on a monitor.
************************************************************************/

module gifplayer_top (
	input  logic        CLOCK_50,
	input  logic [3:0]  KEY,
	input  logic [7:0]	SW,
	output logic [6:0]  HEX0, HEX1,
	
	// VGA Interface 
	output logic [7:0]  VGA_R,        //VGA Red
						VGA_G,        //VGA Green
						VGA_B,        //VGA Blue
	output logic        VGA_CLK,      //VGA Clock
						VGA_SYNC_N,   //VGA Sync signal
						VGA_BLANK_N,  //VGA Blank signal
						VGA_VS,       //VGA vertical sync signal
						VGA_HS,       //VGA horizontal sync signal
					
	// SRAM Interface for Nios II Software
	inout  wire  [15:0] SRAM_DQ,     //  sram_wire.DQ
	output logic [19:0] SRAM_ADDR,   //           .ADDR
	output logic        SRAM_LB_N,   //           .LB_N
	 			      	SRAM_UB_N,   //           .UB_N
	        			SRAM_CE_N,   //           .CE_N
	        			SRAM_OE_N,   //           .OE_N
		        		SRAM_WE_N,   //           .WE_N

	// SDRAM Interface for Nios II Software
	output logic [12:0] DRAM_ADDR,    //SDRAM Address 13 Bits
	inout  wire  [31:0] DRAM_DQ,      //SDRAM Data 32 Bits
	output logic [1:0]  DRAM_BA,      //SDRAM Bank Address 2 Bits
	output logic [3:0]  DRAM_DQM,     //SDRAM Data Mast 4 Bits
	output logic        DRAM_RAS_N,   //SDRAM Row Address Strobe
						DRAM_CAS_N,   //SDRAM Column Address Strobe
						DRAM_CKE,     //SDRAM Clock Enable
						DRAM_WE_N,    //SDRAM Write Enable
						DRAM_CS_N,    //SDRAM Chip Select
						DRAM_CLK      //SDRAM Clock
);

    logic Reset_h, Clk;
    //logic [9:0] DrawX, DrawY;
    
    assign Clk = CLOCK_50;
    always_ff @ (posedge Clk) begin
        Reset_h <= ~(KEY[0]);        // The push buttons are active low
    end

	// Instantiation of Qsys design
	gifplayer_soc gifplayer_soc (
		.clk_clk(CLOCK_50),								// Clock input
		.reset_reset_n(KEY[0]),							// Reset key
		.sdram_wire_addr(DRAM_ADDR),					// sdram_wire.addr
		.sdram_wire_ba(DRAM_BA),						// sdram_wire.ba
		.sdram_wire_cas_n(DRAM_CAS_N),					// sdram_wire.cas_n
		.sdram_wire_cke(DRAM_CKE),						// sdram_wire.cke
		.sdram_wire_cs_n(DRAM_CS_N),					// sdram.cs_n
		.sdram_wire_dq(DRAM_DQ),						// sdram.dq
		.sdram_wire_dqm(DRAM_DQM),						// sdram.dqm
		.sdram_wire_ras_n(DRAM_RAS_N),					// sdram.ras_n
		.sdram_wire_we_n(DRAM_WE_N),					// sdram.we_n
		.sdram_clk_clk(DRAM_CLK),						// Clock out to SDRAM
		.sw_wire_export(SW),
		.sram_wire_DQ(SRAM_DQ),     					//  sram_wire.DQ
		.sram_wire_ADDR(SRAM_ADDR),   					//           .ADDR
		.sram_wire_LB_N(SRAM_LB_N),   					//           .LB_N
		.sram_wire_UB_N(SRAM_UB_N),   					//           .UB_N
		.sram_wire_CE_N(SRAM_CE_N),   					//           .CE_N
		.sram_wire_OE_N(SRAM_OE_N),   					//           .OE_N
		.sram_wire_WE_N(SRAM_WE_N)   					//           .WE_N
	);


	// VGA_CLK needs to be 25 MHz
	vga_clk vga_clk_instance(.inclk0(Clk), .c0(VGA_CLK) );

	// Hex display will display something.
	hexdriver hexdrv0 (
		.In(4'b0000),
		.Out(HEX0)
	);
	hexdriver hexdrv1 (
		.In(4'b1111),
		.Out(HEX1)
	);
	// hexdriver hexdrv2 (
	// 	.In(AES_EXPORT_DATA[11:8]),
	//    .Out(HEX2)
	// );
	// hexdriver hexdrv3 (
	// 	.In(AES_EXPORT_DATA[15:12]),
	//    .Out(HEX3)
	// );
	// hexdriver hexdrv4 (
	// 	.In(AES_EXPORT_DATA[19:16]),
	//    .Out(HEX4)
	// );
	// hexdriver hexdrv5 (
	// 	.In(AES_EXPORT_DATA[23:20]),
	//    .Out(HEX5)
	// );
	// hexdriver hexdrv6 (
	// 	.In(AES_EXPORT_DATA[27:24]),
	//    .Out(HEX6)
	// );
	// hexdriver hexdrv7 (
	// 	.In(AES_EXPORT_DATA[31:28]),
	//    .Out(HEX7)
	// );

endmodule

