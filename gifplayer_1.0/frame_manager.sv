module frame_manager ( 
    input logic Clk, Reset,
    input logic [9:0] DrawX, DrawY,
    input logic VGA_CLK,
    input logic [257:0][31:0] lookup_table,
    input logic [15:0] width, height,
    input logic [7:0] totalFrameCount,
    input logic HARDWARE_EN,
    input logic key,
    input logic speed_en,
    
    inout  wire [15:0]   sram_wire_DQ,              //     Should be input only
    output wire [19:0]   sram_wire_ADDR,            //              .ADDR
    output wire          sram_wire_LB_N,            //              .LB_N
    output wire          sram_wire_UB_N,            //              .UB_N
    output wire          sram_wire_CE_N,            //              .CE_N
    output wire          sram_wire_OE_N,            //              .OE_N
    output wire          sram_wire_WE_N,            //              .WE_N

    output logic [7:0] VGA_R, VGA_G, VGA_B, // VGA RGB output
    output logic [7:0] current_frame_out

);
    logic [25:0] displayCount = 0;
    // logic [15:0] width; //Get these from SRAM or OCM somehow
    // logic [15:0] height;
    // logic [7:0] totalFrameCount = 0;
    logic [7:0] currentFrameCount = 0;
    logic [31:0] currentColors;
    logic [15:0] data_out;
    //logic [11:0] sram_offset;
    logic access_upper = 0;

    always_ff @ (posedge VGA_CLK)
    begin
        access_upper <= DrawX == 10'b0 ? 1'b0 : ~access_upper;
        //access_upper <= sram_offset[0];
        displayCount <= displayCount + 1'b1;

        if ((speed_en && displayCount >= 26'd12500000) || (~speed_en && displayCount == ~26'b0)) begin
            displayCount <= 26'b0;
            if (totalFrameCount <= currentFrameCount) begin
                currentFrameCount <= 8'b0;
            end else begin
                currentFrameCount <= currentFrameCount + 1'b1;
            end
        end
    end

	always_ff @ (posedge Clk)
	begin
        // if (Reset) begin
        //     currentFrameCount <= 1'b0;
        // end
        data_out <= sram_wire_DQ;
	end

    always_comb 
    begin
        //sram_offset = 12'b0;
        current_frame_out = currentFrameCount;
        VGA_R = 8'h3f; 
        VGA_G = 8'h7f - {1'b0, DrawY[9:3]};
        VGA_B = 8'h4f - {1'b0, DrawX[9:3]};
        sram_wire_WE_N = 1'b1; //False
        sram_wire_UB_N = 1'b0; //T
        sram_wire_LB_N = 1'b0; //T
        sram_wire_CE_N = 1'b0; //T
        sram_wire_OE_N = 1'b0; //T
        currentColors = 32'b0;
        sram_wire_ADDR = 19'b0;
        // sram_wire_DQ = 16'bZ;


        if (HARDWARE_EN) begin
            // sram_wire_ADDR = 8'bX;
            if ( (unsigned'(DrawX) < unsigned'(width)) && (unsigned'(DrawY) < unsigned'(height)) ) begin

                //sram_offset = DrawY*width + DrawX + currentFrameCount*height*width;
                //sram_wire_ADDR = 19'hCA28 + (sram_offset>>1);

                sram_wire_ADDR = 19'hCA28 + ((DrawY*width + DrawX)>>1) + ((currentFrameCount*height*width)>>1);
                //sram_wire_ADDR = (2*19'hCA28 + DrawY*width + DrawX + currentFrameCount*height*width)>>1; // This is the working one
                //sram_wire_ADDR = 19'h0;//19'h19450; //+(currentFrameCount*height*width)+DrawY*width+DrawX;

                //fix this later to switch upper/lower bytes
                if (access_upper == key) begin
                    currentColors = lookup_table[data_out[15:8]];
                end else begin
                    currentColors = lookup_table[data_out[7:0]];
                end
                //sram_wire_DQ[7:0]];
                VGA_R = currentColors[31:24];
                VGA_G = currentColors[23:16];
                VGA_B = currentColors[15:8];
            end

        end
    end

endmodule