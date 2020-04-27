module lookup_reg (
	// Avalon Clock Input
    // input logic output_en,
	// input logic [7:0] addr,
    // output logic [7:0] VGA_R, VGA_G, VGA_B // VGA RGB output

	input logic CLK,
	
	// Avalon Reset Input
	input logic RESET,
	
	// Avalon-MM Slave Signals
	input  logic AVL_READ,					// Avalon-MM Read
	input  logic AVL_WRITE,					// Avalon-MM Write
	input  logic AVL_CS,						// Avalon-MM Chip Select
	input  logic [3:0] AVL_BYTE_EN,		// Avalon-MM Byte Enable
	input  logic [8:0] AVL_ADDR,			// Avalon-MM Address
	input  logic [31:0] AVL_WRITEDATA,	// Avalon-MM Write Data
	output logic [31:0] AVL_READDATA,	// Avalon-MM Read Data
	
	// Exported Conduit
	output logic [8255:0] EXPORT_DATA		// Exported Conduit Signal to LEDs
);


///I have no idea if this is right...
logic [257:0][31:0] regs;

// logic [7:0] Red, Green, Blue;

// // Output colors to VGA
// assign VGA_R = Red;
// assign VGA_G = Green;
// assign VGA_B = Blue;
// Output the stuffs here, like width 16 bits, height 16 bits, 
// These can be packed together ready , framecount 
// so we need 258 32 bit locations
// and be able to write from software


always_comb begin
	// AVL_READDATA = 32'bX;
	// if (AVL_CS && AVL_READ) begin
	// 	AVL_READDATA = regs[AVL_ADDR];
	// end
    // Red = 8'b0;
    // Green = 8'b0;
    // Blue = 8'b0;
    // if (output_en) begin
    //     Red = regs[addr][31:24];
    //     Green = regs[addr][23:16];
    //     Blue = regs[addr][15:8];
    // end

	AVL_READDATA = 32'bX;
	if (AVL_CS && AVL_READ) begin
		AVL_READDATA = regs[AVL_ADDR];
	end

	//[31: 24][23:16][15:8][7:0]

end

always_ff @(posedge CLK) begin 
    if (RESET) begin
        for (int i = 0; i<258; i++) begin
            regs[i] <= 32'b0;
        end
    end
	if (AVL_CS && AVL_WRITE) begin
		case(AVL_BYTE_EN)
			4'b1111: regs[AVL_ADDR] <= AVL_WRITEDATA;
			4'b1100: regs[AVL_ADDR][31:16] <= AVL_WRITEDATA[31:16];
			4'b0011: regs[AVL_ADDR][15:0] <= AVL_WRITEDATA[15:0];
			4'b1000: regs[AVL_ADDR][31:24] <= AVL_WRITEDATA[31:24];
			4'b0100: regs[AVL_ADDR][23:16] <= AVL_WRITEDATA[23:16];
			4'b0010: regs[AVL_ADDR][15:8] <= AVL_WRITEDATA[15:8];
			4'b0001: regs[AVL_ADDR][7:0] <= AVL_WRITEDATA[7:0];
			// default:
		endcase
	end



	// // Set export data to the first 2 and last 2 bytes of AES key
	// EXPORT_DATA[31:16] <= regs[0][31:16];
	// EXPORT_DATA[15:0] <= regs[3][15:0];

    for (int i = 0; i<258; i++) {
        //EXPORT_DATA[(i+1)*31:i*31]
        EXPORT_DATA[32*i+31:32*i] <= regs[i];
    }

end

endmodule
