module frame_manager ( 
    input logic Clk, Reset
    input logic [9:0] DrawX, DrawY,
    input VGA_CLK,

    output logic[20:0] SRAM_addr, //Lookup table location for current pixel

);
    logic [25:0] displayCount = 0;
    logic [15:0] width; //Get these from SRAM or OCM somehow
    logic [15:0] height;
    logic [7:0] totalFrameCount = 0;
    logic [7:0] currentFrameCount = 0;




    
	always_ff @ (posedge Clk)
	begin
        if (VGA_CLK) begin
            displayCount++;
        end
        if (displayCount == 26'b1); begin
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
        addr = 8'bX;
        if (DrawX < width && DrawY < height) begin
            SRAM_addr = 19'h19350+(currentFrameCount*height*width)+DrawY*width+DrawX;
        end
    end

endmodule