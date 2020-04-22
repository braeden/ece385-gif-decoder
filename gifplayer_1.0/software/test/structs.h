#include <stdint.h>

#pragma pack(push, 1)
// Need to make sure compiler packs structs
// and obeys bitfields and system is little-endian

// Header (6 bytes)
typedef struct __packed__ {
	unsigned char sig[3];
	unsigned char version[3];
} HeaderBlock;

// Logical Screen Descriptor (7 bytes)
typedef struct LogicalScreenDescriptor {
	uint16_t canvasWidth;
	uint16_t canvasHeight;
	struct packedLSD {
		unsigned char globalColorSize : 3;
		unsigned char sortFlag : 1;
		unsigned char colorResolution : 3;
		unsigned char globalColorFlag : 1;
	} packedField;
	unsigned char backgroundColorIndex;	 // default color
	unsigned char pixelAspectRatio;		 // we don't care
} LSD;

// Graphics Control Extension (optional)
typedef struct GraphicsControlExtension {
	// unsigned char extensionIntro;  // 0x21
	// unsigned char gcLabel;		   // 0xF9 = GCE
	unsigned char byteSize;
	struct packedGCE {
		unsigned char transparentColorFlag : 1;
		unsigned char userInputFlag : 1;
		unsigned char disposalMethod : 3;
		unsigned char _future : 3;
	} packedField;
	uint16_t delayTime;
	unsigned char transparentColorIndex;
	unsigned char blockTerm;  // 0x00
} GCE;

// Image Descriptor (a 10 bytes block to start each img)
typedef struct imageDescriptor {
	unsigned char imgSeperator;	 // 0x2C
	uint16_t imgLeft;
	uint16_t imgTop;
	uint16_t imgWidth;
	uint16_t imgHeight;
	struct packedIMG {
		unsigned char localColorSize : 3;
		unsigned char _future : 2;
		unsigned char sortFlag : 1;
		unsigned char interlaceFlag : 1;
		unsigned char localColorFlag : 1;
	} packedField;
} imageDescriptor;

#pragma pack(pop)

// Subblock w/ size and array of chars w/ variable wize
typedef struct dictionary_entry_t {
	unsigned char byte;
	int prev;
	int len;
} dictionary_entry_t;

typedef struct colorTableEntry {
	unsigned char RGB[3];
} colorTableEntry;
