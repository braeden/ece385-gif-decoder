#include <stdint.h>

// Core image structure

typedef struct HeaderBlock {
	// header (6 bytes)
	unsigned char signature[3];
	unsigned char version[3];
} HeaderBlock;

// Logical Screen Descriptor (7 bytes)
typedef struct LogicalScreenDescriptor {
	uint16_t canvasWidth;
	uint16_t canvasHeight;
	unsigned char packedDetails;
	unsigned char backgroundColorIndex;  // default color
	unsigned char pixelAspectRatio;	   // we don't care
} LSD;

// Graphics Control Extension (optional)
typedef struct GraphicsControlExtension {
	unsigned char extensionIntro;	// 0x21
	unsigned char gcLabel;			// 0xF9 = GCE
	unsigned char byteSize;
	unsigned char packedField;
	uint16_t delayTime;
	unsigned char transparentColorIndex;
	unsigned char blockTerm;  // 0x00
} GCE;

// Image Descriptor (a 10 bytes block to start each img)
typedef struct imageDescriptor {
	unsigned char imgSeperator;  // 0x2C
	uint16_t imgLeft;
	uint16_t imgTop;
	uint16_t imgWidth;
	uint16_t imgHeight;
	unsigned char packedField;
} imageDescriptor;

// Subblock w/ size and array of chars w/ variable wize
typedef struct subblock {
	unsigned char subblockSize;
	unsigned char blocks[256];	// Make this variable pls
} subblock;

// Helper structs (packed fields/colors)

typedef struct packed_field_LSD {
	unsigned char global_color_flag;
	unsigned char color_resolution;
	unsigned char sort_flag;
	unsigned char global_color_size;
	int actual_color_size;
} packed_field_LSD;

typedef struct packed_field_IMG {
	unsigned char local_color_flag;
	unsigned char interlace_flag;
	unsigned char sort_flag;
	unsigned char local_color_size;
	int actual_color_size;
} packed_field_IMG;

typedef struct color_table_entry {
	unsigned char RGB[3];
} color_table_entry;
