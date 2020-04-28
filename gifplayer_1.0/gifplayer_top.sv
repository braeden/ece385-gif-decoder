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
	output logic [6:0]  HEX0, HEX1, HEX2, HEX3, HEX4, HEX5, HEX6, HEX7,
	
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
    logic [9:0] DrawX, DrawY;
	logic BEGIN;
	logic [8255:0] lookup_output;
	logic [257:0][31:0] lookup_output_array;
    logic [15:0] width;
	logic [15:0] height;
	logic [7:0] frame_count;
	logic HARDWARE_EN;

	// SRAM Interface for Nios II Software
	wire  [15:0] SRAM_DQ_SW; 
	logic [19:0] SRAM_ADDR_SW;   //           .ADDR
	logic        SRAM_LB_N_SW,   //           .LB_N
					SRAM_UB_N_SW,   //           .UB_N
					SRAM_CE_N_SW,   //           .CE_N
					SRAM_OE_N_SW,   //           .OE_N
					SRAM_WE_N_SW;   //           .WE_N

	// SRAM Interface for hardware
	wire  [15:0] SRAM_DQ_HW;
	logic [19:0] SRAM_ADDR_HW;   //           .ADDR
	logic        SRAM_LB_N_HW,   //           .LB_N
					SRAM_UB_N_HW,   //           .UB_N
					SRAM_CE_N_HW,   //           .CE_N
					SRAM_OE_N_HW,   //           .OE_N
					SRAM_WE_N_HW;   //           .WE_N




    assign Clk = CLOCK_50;
    always_ff @ (posedge Clk) begin
        Reset_h <= ~(KEY[0]);        // The push buttons are active low
    end

	always_comb begin
	//This might be 256+258
		for (int i=0; i<258; i++)  begin
			lookup_output_array[i] = lookup_output[32*i +: 32];
		end

		width = lookup_output_array[9'd256][31:16];
		height = lookup_output_array[9'd256][15:0];
		frame_count = lookup_output_array[9'd257][31:24];
		HARDWARE_EN = lookup_output_array[9'd257][0];
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
		.sram_wire_WE_N(SRAM_WE_N),   					//           .WE_N

		.lookup_export_export_data(lookup_output)
	);


	// VGA_CLK needs to be 25 MHz
	vga_clk vga_clk_instance(.inclk0(Clk), .c0(VGA_CLK) );


    // VGA controller from lab 8
    VGA_controller vga_controller_instance(
        .Clk(Clk), 
        .Reset(Reset_h), 
        .VGA_CLK(VGA_CLK),
        .DrawY(DrawY), 
        .DrawX(DrawX),
        .VGA_HS(VGA_HS),
        .VGA_VS(VGA_VS),
        .VGA_BLANK_N(VGA_BLANK_N)
    );

	// frame_state_machine frame_state_machine_instance(
	// // input -> clk, reset, some way to talk to SRAM (or a OCM that software can write to)
	// // 
	// // output -> BEGIN? Maybe SETUP->

	// //Maybe several states used to load data into OCM...
	// // 
	// 	.Clk(Clk),
	// 	.Reset(Reset_h),
	// 	.DrawStart(1'b1), //Replace this with software indication

	// 	.BEGIN(BEGIN)

	// );

	// Make a avalon interface like lab 9, in the always comb section, output a set of wire continously, that hold width height, ect
	//Avalon should hold lookup table+ stuff. 
	//It should have a variable output from a address.
	// Tristate the SRAM from NIOS, use a bit in OCM to tell if we're done


	//Some small async OCM -> space for [8:0] Framecount, [16:0] width, [height]
	//example (clk, reset, width, framect, height)
	//output width, framect, height


	frame_manager frame_manager_instance (
		.Clk(Clk),
		.Reset(Reset_h),
		.VGA_CLK(VGA_CLK),
		.lookup_table(lookup_output_array),
		.width(width),
		.height(height),
		.totalFrameCount(frame_count),
		.HARDWARE_EN(HARDWARE_EN),


		.sram_wire_DQ(SRAM_DQ_HW),     					//  sram_wire.DQ
		.sram_wire_ADDR(SRAM_ADDR_HW),   					//           .ADDR
		.sram_wire_LB_N(SRAM_LB_N_HW),   					//           .LB_N
		.sram_wire_UB_N(SRAM_UB_N_HW),   					//           .UB_N
		.sram_wire_CE_N(SRAM_CE_N_HW),   					//           .CE_N
		.sram_wire_OE_N(SRAM_OE_N_HW),   					//           .OE_N
		.sram_wire_WE_N(SRAM_WE_N_HW),   					//           .WE_N

		.VGA_R(VGA_R),
		.VGA_G(VGA_G),
		.VGA_B(VGA_B)
	);


	// sram_mux sram_mux_instance (
	// 	.select(1'b0),//HARDWARE_EN), // 0 = software, 1 = hardware
		
	// 	.sram_wire_DQ_sw(SRAM_DQ_SW),     					//  sram_wire.DQ
	// 	.sram_wire_ADDR_sw(SRAM_ADDR_SW),   					//           .ADDR
	// 	.sram_wire_LB_N_sw(SRAM_LB_N_SW),   					//           .LB_N
	// 	.sram_wire_UB_N_sw(SRAM_UB_N_SW),   					//           .UB_N
	// 	.sram_wire_CE_N_sw(SRAM_CE_N_SW),   					//           .CE_N
	// 	.sram_wire_OE_N_sw(SRAM_OE_N_SW),   					//           .OE_N
	// 	.sram_wire_WE_N_sw(SRAM_WE_N_SW), 
		
	// 	.sram_wire_DQ_hw(SRAM_DQ_HW),     					//  sram_wire.DQ
	// 	.sram_wire_ADDR_hw(SRAM_ADDR_HW),   					//           .ADDR
	// 	.sram_wire_LB_N_hw(SRAM_LB_N_HW),   					//           .LB_N
	// 	.sram_wire_UB_N_hw(SRAM_UB_N_HW),   					//           .UB_N
	// 	.sram_wire_CE_N_hw(SRAM_CE_N_HW),   					//           .CE_N
	// 	.sram_wire_OE_N_hw(SRAM_OE_N_HW),   					//           .OE_N
	// 	.sram_wire_WE_N_hw(SRAM_WE_N_HW), 

	// 	//outputs
	// 	.sram_wire_DQ_out(SRAM_DQ),     					//  sram_wire.DQ
	// 	.sram_wire_ADDR_out(SRAM_ADDR),   					//           .ADDR
	// 	.sram_wire_LB_N_out(SRAM_LB_N),   					//           .LB_N
	// 	.sram_wire_UB_N_out(SRAM_UB_N),   					//           .UB_N
	// 	.sram_wire_CE_N_out(SRAM_CE_N),   					//           .CE_N
	// 	.sram_wire_OE_N_out(SRAM_OE_N),   					//           .OE_N
	// 	.sram_wire_WE_N_out(SRAM_WE_N)
	// );

	// frameCount instance (
	// 	//Maybe should be a submodule, keep track of current frame. 
	// // );

	// lookup_reg lookup_reg(
	// 	//We should get software to write this directy (avalon MM bus)
	// 	//input = index, reset, clk, enable
	// 	//if (addr != 8'bX) -> return value at [8:0 (256 choices of index)][32:0 (24 bits of data)]
	// 	//output -> VGA_R, G, B, return black otherwise
	// 	//if (addr) return
	// 	.Clk(Clk),
	// 	.Reset(Reset_h),
	// 	.output_en(addr != 20'bX),//BEGIN | 1'b1);
	// 	.addr(lookup_addr),
	// 	.VGA_R(VGA_R),
	// 	.VGA_G(VGA_G),
	// 	.VGA_B(VGA_B)
	// );


	// Hex display will display something.
	hexdriver hexdrv0 (
		.In(lookup_output[3:0]),
		.Out(HEX0)
	);
	hexdriver hexdrv1 (
		.In(lookup_output[7:4]),
		.Out(HEX1)
	);
	hexdriver hexdrv2 (
		.In(lookup_output[11:8]),
	   .Out(HEX2)
	);
	hexdriver hexdrv3 (
		.In(width[3:0]),
	   .Out(HEX3)
	);
	hexdriver hexdrv4 (
		.In(width[7:4]),
	   .Out(HEX4)
	);
	hexdriver hexdrv5 (
		.In(height[3:0]),
	   .Out(HEX5)
	);
	hexdriver hexdrv6 (
		.In(height[7:4]),
	   .Out(HEX6)
	);
	hexdriver hexdrv7 (
		.In(HARDWARE_EN),
	   .Out(HEX7)
	);

endmodule

