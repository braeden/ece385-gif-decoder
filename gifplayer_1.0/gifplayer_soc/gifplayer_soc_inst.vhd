	component gifplayer_soc is
		port (
			clk_clk                   : in    std_logic                       := 'X';             -- clk
			lookup_export_export_data : out   std_logic_vector(8255 downto 0);                    -- export_data
			reset_reset_n             : in    std_logic                       := 'X';             -- reset_n
			sdram_clk_clk             : out   std_logic;                                          -- clk
			sdram_wire_addr           : out   std_logic_vector(12 downto 0);                      -- addr
			sdram_wire_ba             : out   std_logic_vector(1 downto 0);                       -- ba
			sdram_wire_cas_n          : out   std_logic;                                          -- cas_n
			sdram_wire_cke            : out   std_logic;                                          -- cke
			sdram_wire_cs_n           : out   std_logic;                                          -- cs_n
			sdram_wire_dq             : inout std_logic_vector(31 downto 0)   := (others => 'X'); -- dq
			sdram_wire_dqm            : out   std_logic_vector(3 downto 0);                       -- dqm
			sdram_wire_ras_n          : out   std_logic;                                          -- ras_n
			sdram_wire_we_n           : out   std_logic;                                          -- we_n
			sram_wire_DQ              : inout std_logic_vector(15 downto 0)   := (others => 'X'); -- DQ
			sram_wire_ADDR            : out   std_logic_vector(19 downto 0);                      -- ADDR
			sram_wire_LB_N            : out   std_logic;                                          -- LB_N
			sram_wire_UB_N            : out   std_logic;                                          -- UB_N
			sram_wire_CE_N            : out   std_logic;                                          -- CE_N
			sram_wire_OE_N            : out   std_logic;                                          -- OE_N
			sram_wire_WE_N            : out   std_logic;                                          -- WE_N
			sw_wire_export            : in    std_logic_vector(7 downto 0)    := (others => 'X')  -- export
		);
	end component gifplayer_soc;

	u0 : component gifplayer_soc
		port map (
			clk_clk                   => CONNECTED_TO_clk_clk,                   --           clk.clk
			lookup_export_export_data => CONNECTED_TO_lookup_export_export_data, -- lookup_export.export_data
			reset_reset_n             => CONNECTED_TO_reset_reset_n,             --         reset.reset_n
			sdram_clk_clk             => CONNECTED_TO_sdram_clk_clk,             --     sdram_clk.clk
			sdram_wire_addr           => CONNECTED_TO_sdram_wire_addr,           --    sdram_wire.addr
			sdram_wire_ba             => CONNECTED_TO_sdram_wire_ba,             --              .ba
			sdram_wire_cas_n          => CONNECTED_TO_sdram_wire_cas_n,          --              .cas_n
			sdram_wire_cke            => CONNECTED_TO_sdram_wire_cke,            --              .cke
			sdram_wire_cs_n           => CONNECTED_TO_sdram_wire_cs_n,           --              .cs_n
			sdram_wire_dq             => CONNECTED_TO_sdram_wire_dq,             --              .dq
			sdram_wire_dqm            => CONNECTED_TO_sdram_wire_dqm,            --              .dqm
			sdram_wire_ras_n          => CONNECTED_TO_sdram_wire_ras_n,          --              .ras_n
			sdram_wire_we_n           => CONNECTED_TO_sdram_wire_we_n,           --              .we_n
			sram_wire_DQ              => CONNECTED_TO_sram_wire_DQ,              --     sram_wire.DQ
			sram_wire_ADDR            => CONNECTED_TO_sram_wire_ADDR,            --              .ADDR
			sram_wire_LB_N            => CONNECTED_TO_sram_wire_LB_N,            --              .LB_N
			sram_wire_UB_N            => CONNECTED_TO_sram_wire_UB_N,            --              .UB_N
			sram_wire_CE_N            => CONNECTED_TO_sram_wire_CE_N,            --              .CE_N
			sram_wire_OE_N            => CONNECTED_TO_sram_wire_OE_N,            --              .OE_N
			sram_wire_WE_N            => CONNECTED_TO_sram_wire_WE_N,            --              .WE_N
			sw_wire_export            => CONNECTED_TO_sw_wire_export             --       sw_wire.export
		);

