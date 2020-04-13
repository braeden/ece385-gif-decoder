#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "structs.h"
static char VALID_HEADER[3] = "GIF";
FILE *fileptr;

void skipToTerm() {
	while (fgetc(fileptr) != 0x00)
		printf("skipping");
}

int main() {
	fileptr = fopen("sample2.gif", "rb");

	HeaderBlock header;
	LSD descriptor;
	fread(&header, 1, sizeof(header), fileptr);
	fread(&descriptor, 1, sizeof(descriptor), fileptr);
	for (int i = 0; i < 3; i++) {
		assert(header.signature[i] == VALID_HEADER[i]);
	}
	assert(descriptor.packedField.globalColorFlag);	 //For all intents we're going to need a global table

	int readlGlobalColorSize = 0x2 << descriptor.packedField.globalColorSize;  // 2^(N+1) actual colors
																			   // printf("%d, %d\n", test, packed.actual_color_size);
	colorTableEntry *globalTable = (colorTableEntry *)
		malloc(readlGlobalColorSize * sizeof(colorTableEntry));	 // We can make this 256 for hardware

	//Read global color table and print it
	fread(globalTable, 3, readlGlobalColorSize, fileptr);

	for (int i = 0; i < readlGlobalColorSize; i++) {
		printf("[#");
		for (int j = 0; j < 3; j++) {
			printf("%02x", globalTable[i].RGB[j]);
		}
		printf("],");
	}
	printf("\n");

	//Now let's load our images
	for (int imageCount = 0; imageCount < 1; imageCount++) {
		GCE gce;
		imageDescriptor imgDesc;
		colorTableEntry *localTable = NULL;
		unsigned char c = fgetc(fileptr);
		while (c == 0x21) {
			// We encountered a extension block...
			unsigned char label = fgetc(fileptr);
			switch (label) {
			case 0xFF: {
				// Application Extension
				printf("Loaded AEB\n");
				assert(fgetc(fileptr) == 0x0B);
				fseek(fileptr, 11, SEEK_CUR);
				assert(fgetc(fileptr) == 0x03);
				assert(fgetc(fileptr) == 0x01);
				fseek(fileptr, 3, SEEK_CUR);
				//Read repeat count if we care
				break;
			}
			case 0xF9: {
				printf("Loaded GCE\n");
				fread(&gce, 1, sizeof(gce), fileptr);
				break;
			}
			case 0xFE: {
				//Comment Extension
				skipToTerm();
				break;
			}
			default: {
				unsigned char bytesToSkip = fgetc(fileptr);
				fseek(fileptr, bytesToSkip, SEEK_CUR);
				skipToTerm();
				break;
			}
			}
			c = fgetc(fileptr);
		}
		ungetc(c, fileptr);

		fread(&imgDesc, 1, sizeof(imgDesc), fileptr);
		assert(imgDesc.imgSeperator == 0x2C);
		if (imgDesc.packedField.localColorFlag) {
			// Load a local color table
			int realLocalColorSize = 0x2 << imgDesc.packedField.localColorSize;	 // 2^(N+1) actual colors
			localTable = (colorTableEntry *)
				malloc(realLocalColorSize * sizeof(colorTableEntry));
			fread(localTable, 3, realLocalColorSize, fileptr);
		}


		//Let's yeet that image data into something

		unsigned char LZWMinCode = getc(fileptr);
		printf("%02x\n", LZWMinCode);
		char * data = NULL;
		int dataSize = 0;
		unsigned char bytesInSubblock = getc(fileptr);
		while (bytesInSubblock) {
			// printf("%02x\n", bytes_in_subblock);
			subblock cur_sb;
			cur_sb.subblockSize = bytesInSubblock;
			fread(&cur_sb.blocks, 1, cur_sb.subblockSize, fileptr);

			// malloc mem and read N bytes in

			bytesInSubblock = getc(fileptr);
		}
		// Write developed image frame to some sort of storage


		// Check if we're at end of file:
		// and break loop!
		c = fgetc(fileptr);
		ungetc(c, fileptr);
		if (c == 0x3B) {
			break;
		}
		free(localTable);
	}

	free(globalTable);
	printf("\n");
	fclose(fileptr);
}
