	gifplayer_soc u0 (
		.clk_clk                   (<connected-to-clk_clk>),                   //           clk.clk
		.reset_reset_n             (<connected-to-reset_reset_n>),             //         reset.reset_n
		.sdram_clk_clk             (<connected-to-sdram_clk_clk>),             //     sdram_clk.clk
		.sdram_wire_addr           (<connected-to-sdram_wire_addr>),           //    sdram_wire.addr
		.sdram_wire_ba             (<connected-to-sdram_wire_ba>),             //              .ba
		.sdram_wire_cas_n          (<connected-to-sdram_wire_cas_n>),          //              .cas_n
		.sdram_wire_cke            (<connected-to-sdram_wire_cke>),            //              .cke
		.sdram_wire_cs_n           (<connected-to-sdram_wire_cs_n>),           //              .cs_n
		.sdram_wire_dq             (<connected-to-sdram_wire_dq>),             //              .dq
		.sdram_wire_dqm            (<connected-to-sdram_wire_dqm>),            //              .dqm
		.sdram_wire_ras_n          (<connected-to-sdram_wire_ras_n>),          //              .ras_n
		.sdram_wire_we_n           (<connected-to-sdram_wire_we_n>),           //              .we_n
		.sram_wire_DQ              (<connected-to-sram_wire_DQ>),              //     sram_wire.DQ
		.sram_wire_ADDR            (<connected-to-sram_wire_ADDR>),            //              .ADDR
		.sram_wire_LB_N            (<connected-to-sram_wire_LB_N>),            //              .LB_N
		.sram_wire_UB_N            (<connected-to-sram_wire_UB_N>),            //              .UB_N
		.sram_wire_CE_N            (<connected-to-sram_wire_CE_N>),            //              .CE_N
		.sram_wire_OE_N            (<connected-to-sram_wire_OE_N>),            //              .OE_N
		.sram_wire_WE_N            (<connected-to-sram_wire_WE_N>),            //              .WE_N
		.sw_wire_export            (<connected-to-sw_wire_export>),            //       sw_wire.export
		.lookup_export_export_data (<connected-to-lookup_export_export_data>)  // lookup_export.export_data
	);

