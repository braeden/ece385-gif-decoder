/************************************************************************
Color_Mapper.sv
by Ian Rudnick and Braeden Smith
4/27/2020

for ece385-gif-decoder

Decides which color to be output to VGA for each pixel.
Designed based on ECE 385 Lab 8 color mapper.
For Braeden Smith and Ian Rudnick's ECE 385 Final Project
************************************************************************/

module  color_mapper (
    input        [9:0] DrawX, DrawY,       // Current pixel coordinates
    output logic [7:0] VGA_R, VGA_G, VGA_B // VGA RGB output
);
    
    logic [7:0] Red, Green, Blue;
    
    // Output colors to VGA
    assign VGA_R = Red;
    assign VGA_G = Green;
    assign VGA_B = Blue;

    // PROPOSED ALGORITHM
    // Look up the size of current gif.
    // Center gif position on screen by subtracting gif size from screen size or something
    // Look at OCM color table at index of pixel we are drawing
    // Set Red, Green, and Blue received from OCM
    // profit

    always_comb begin
        // Temporary background with nice color gradient
        Red = 8'h3f; 
        Green = 8'h00;
        Blue = 8'h7f - {1'b0, DrawX[9:3]};
    end
    
endmodule
