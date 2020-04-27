module frame_manager ( 
    input logic Clk, Reset,
    input logic [9:0] DrawX, DrawY,
    input logic VGA_CLK,
    input logic [8:0][31:0] lookup_table,
    input logic [16:0] width, height,
    input logic [8:0] totalFrameCount,
    input logic HARDWARE_EN,
    
    inout  wire [15:0]   sram_wire_DQ,              //     Should be input only
    output wire [19:0]   sram_wire_ADDR,            //              .ADDR
    output wire          sram_wire_LB_N,            //              .LB_N
    output wire          sram_wire_UB_N,            //              .UB_N
    output wire          sram_wire_CE_N,            //              .CE_N
    output wire          sram_wire_OE_N,            //              .OE_N
    output wire          sram_wire_WE_N,            //              .WE_N

    output logic [7:0] VGA_R, VGA_G, VGA_B // VGA RGB output

);
    logic [25:0] displayCount = 0;
    // logic [15:0] width; //Get these from SRAM or OCM somehow
    // logic [15:0] height;
    // logic [7:0] totalFrameCount = 0;
    logic [7:0] currentFrameCount = 0;
    logic [31:0] currentColors;

	always_ff @ (posedge Clk)
	begin
        if (VGA_CLK) begin
            displayCount++;
        end
        if (displayCount == 26'b1) begin
            displayCount <= 26'b0;
            if (totalFrameCount == currentFrameCount) begin
                currentFrameCount <= 8'b0;
            end else begin
                currentFrameCount++;
            end
        end
	end
    always_comb
    begin

        VGA_R = 8'b0;
        VGA_G = 8'b0;
        VGA_B = 8'b0;
        sram_wire_WE_N = 1'b1; //False
        sram_wire_UB_N = 1'b0; //T
        sram_wire_LB_N = 1'b1; //F
        sram_wire_CE_N = 1'b0; //T
        sram_wire_OE_N = 1'b0; //T
        currentColors = 32'b0;
        sram_wire_ADDR = 19'b0;
        
        if (HARDWARE_EN) begin
            sram_wire_ADDR = 8'bX;
            if (DrawX < width && DrawY < height) begin
                sram_wire_ADDR = 19'h19450+(currentFrameCount*height*width)+DrawY*width+DrawX;
                //fix this later to switch upper/lower bytes
                currentColors = lookup_table[sram_wire_DQ[15:8]];
                VGA_R = currentColors[31:24];
                VGA_G = currentColors[23:16];
                VGA_B = currentColors[15:8];
            end

        end
    end

endmodule