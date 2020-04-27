module sram_mux(
    input logic select,

    inout  wire [15:0]   sram_wire_DQ_hw,              //     sram_wire.DQ
    input wire [19:0]   sram_wire_ADDR_hw,            //              .ADDR
    input wire          sram_wire_LB_N_hw,            //              .LB_N
    input wire          sram_wire_UB_N_hw,            //              .UB_N
    input wire          sram_wire_CE_N_hw,            //              .CE_N
    input wire          sram_wire_OE_N_hw,            //              .OE_N
    input wire          sram_wire_WE_N_hw,            //              .WE_N
    
    inout  wire [15:0]   sram_wire_DQ_sw,              //     sram_wire.DQ
    input wire [19:0]   sram_wire_ADDR_sw,            //              .ADDR
    input wire          sram_wire_LB_N_sw,            //              .LB_N
    input wire          sram_wire_UB_N_sw,            //              .UB_N
    input wire          sram_wire_CE_N_sw,            //              .CE_N
    input wire          sram_wire_OE_N_sw,            //              .OE_N
    input wire          sram_wire_WE_N_sw,            //              .WE_N



    inout  wire [15:0]   sram_wire_DQ_out,              //     sram_wire.DQ
    output wire [19:0]   sram_wire_ADDR_out,            //              .ADDR
    output wire          sram_wire_LB_N_out,            //              .LB_N
    output wire          sram_wire_UB_N_out,            //              .UB_N
    output wire          sram_wire_CE_N_out,            //              .CE_N
    output wire          sram_wire_OE_N_out,            //              .OE_N
    output wire          sram_wire_WE_N_out,            //              .WE_N
);

always_comb begin
    if (select) begin
        sram_wire_DQ_out = sram_wire_DQ_hw; 
        sram_wire_ADDR_out = sram_wire_ADDR_hw;
        sram_wire_LB_N_out = sram_wire_LB_N_hw;
        sram_wire_UB_N_out = sram_wire_UB_N_hw;
        sram_wire_CE_N_out = sram_wire_CE_N_hw;
        sram_wire_OE_N_out = sram_wire_OE_N_hw;
        sram_wire_WE_N_out = sram_wire_WE_N_hw;
    end else begin
        sram_wire_DQ_out = sram_wire_DQ_sw; 
        sram_wire_ADDR_out = sram_wire_ADDR_sw;
        sram_wire_LB_N_out = sram_wire_LB_N_sw;
        sram_wire_UB_N_out = sram_wire_UB_N_sw;
        sram_wire_CE_N_out = sram_wire_CE_N_sw;
        sram_wire_OE_N_out = sram_wire_OE_N_sw;
        sram_wire_WE_N_out = sram_wire_WE_N_sw;
    end
end

endmodule