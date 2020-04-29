#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gif_data.h"
#include "lzw.h"
#include "structs.h"

static char VALID_HEADER[3] = "GIF";
static unsigned char *frameptr = (unsigned char *)0x00419450;
static uint32_t *ocmptr = (uint32_t *)0x00001000;
static int ON_NIOS = 0;

unsigned char *fileptr;

void checkPacked() {
	assert(sizeof(HeaderBlock) == 6);
	assert(sizeof(LSD) == 7);
	assert(sizeof(GCE) == 6);
	assert(sizeof(imageDescriptor) == 10);
	assert(sizeof(colorTableEntry) == 3);
}

unsigned char *read(void *dest, int size, int segments, unsigned char *src) {
	memcpy(dest, src, size * segments);
	return src + size * segments;
}
//
unsigned char getch(unsigned char *ptr) {
	return *fileptr;
}

unsigned char *seek(unsigned char *ptr, int size) {
	ptr += size;
	return ptr;
}

void skipToTerm() {
	while (getch(fileptr) != 0x00) {
		fileptr++;
	}
	printf("Skipping!\n");

	fileptr++;
}

void writeSRAM() {
	for (int i = 0; i < _gif_len; i++) {
		fileptr[i] = _gif[i];
	}
	for (int i = 0; i < _gif_len; i++) {
		assert(fileptr[i] == _gif[i]);
	}
}

void printAligned(unsigned char *ptr, int width, int size, int color) {
	for (int i = 0; i < size; i++) {
		if (i % width == 0) {
			printf("\n");
		}
		if (color)
			printf("\033[38;5;%dm", ptr[i]);
		printf("%02x", ptr[i]);
	}
	printf("\033[0m");
	printf("\n");
}

int main() {
	checkPacked();

	unsigned char *fileChunk = NULL;

	if (!ON_NIOS) {
		FILE *f = NULL;
		f = fopen("sampleanim.gif", "rb");
		if (f == NULL) {
			printf("File did not load!\n");
			exit(0);
		}
		fseek(f, 0, SEEK_END);
		long fsize = ftell(f);
		fseek(f, 0, SEEK_SET);

		fileChunk = malloc(fsize + 1);
		fileptr = fileChunk;
		fread(fileptr, 1, fsize, f);
		fclose(f);
	} else {
		//Set pointer from SRAM
		fileptr = (unsigned char *)0x00400050;
	}
	// Write SRAM if we want:
	// writeSRAM();

	HeaderBlock header;
	LSD descriptor;

	fileptr = read(&header, 1, sizeof(header), fileptr);
	fileptr = read(&descriptor, 1, sizeof(descriptor), fileptr);
	for (int i = 0; i < 3; i++) {
		assert(header.signature[i] == VALID_HEADER[i]);
	}
	assert(descriptor.packedField.globalColorFlag);	 //For all intents we're going to need a global table

	int readlGlobalColorSize = 0x2 << descriptor.packedField.globalColorSize;  // 2^(N+1) actual colors
	colorTableEntry *globalTable = (colorTableEntry *)
		malloc(readlGlobalColorSize * sizeof(colorTableEntry));	 // We can make this 256 for hardware

	printf("Dimensions (Canvas): %dx%d\n", descriptor.canvasWidth, descriptor.canvasHeight);

	//Read global color table and print it
	fileptr = read(globalTable, 3, readlGlobalColorSize, fileptr);

	for (int i = 0; i < readlGlobalColorSize; i++) {
		printf("[#");
		for (int j = 0; j < 3; j++) {
			printf("%02x", globalTable[i].RGB[j]);
		}
		printf("],");
	}
	printf("\n");

	//////////////////////////////////////////////////
	int canvasSize = descriptor.canvasWidth * descriptor.canvasHeight;
	unsigned char *currentFrame = (char *)calloc(canvasSize, 1);	 // This holds the current full canvas size exported frame
	//Hardware needs full canvas size frames, not variable each time.

	int8_t totalFrameCount = 0;
	while (1) {
		GCE gce;
		imageDescriptor imgDesc;
		colorTableEntry *localTable = NULL;
		unsigned char c = *fileptr;
		fileptr++;

		while (c == 0x21) {
			// We encountered a extension block...
			unsigned char label = getch(fileptr);
			fileptr++;
			switch (label) {
			case 0xFF: {
				// Application Extension
				printf("Loaded AEB\n");
				assert(getch(fileptr) == 0x0B);
				fileptr++;
				fileptr = seek(fileptr, 11);
				assert(getch(fileptr) == 0x03);
				fileptr++;
				assert(getch(fileptr) == 0x01);
				fileptr++;
				fileptr = seek(fileptr, 3);
				//Read repeat count if we care
				break;
			}
			case 0xF9: {
				printf("Loaded GCE\n");
				fileptr = read(&gce, 1, sizeof(gce), fileptr);
				break;
			}
			case 0xFE: {
				//Comment Extension
				skipToTerm();
				break;
			}
			default: {
				unsigned char bytesToSkip = getch(fileptr);
				fileptr++;
				fileptr = seek(fileptr, bytesToSkip);
				skipToTerm();
				break;
			}
			}
			c = getch(fileptr);
			fileptr++;
		}
		fileptr--;

		fileptr = read(&imgDesc, 1, sizeof(imgDesc), fileptr);
		assert(imgDesc.imgSeperator == 0x2C);
		if (imgDesc.packedField.localColorFlag) {
			// Load a local color table
			printf("Local color table\n");
			int realLocalColorSize = 0x2 << imgDesc.packedField.localColorSize;	 // 2^(N+1) actual colors
			localTable = (colorTableEntry *)
				malloc(realLocalColorSize * sizeof(colorTableEntry));
			fileptr = read(localTable, 3, realLocalColorSize, fileptr);
		}

		//Let's yeet that image data into something
		unsigned char LZWMinCode = getch(fileptr);
		fileptr++;
		printf("%02x\n", LZWMinCode);
		unsigned char *data = NULL;
		unsigned int dataSize = 0;
		unsigned char bytesInSubblock = getch(fileptr);
		fileptr++;
		while (bytesInSubblock) {
			data = realloc(data, (sizeof(char) * (dataSize + bytesInSubblock)));
			fileptr = read(data + dataSize, 1, bytesInSubblock, fileptr);
			dataSize += bytesInSubblock;
			bytesInSubblock = getch(fileptr);
			fileptr++;
		}
		unsigned char *dataOut = (unsigned char *)malloc(imgDesc.imgHeight * imgDesc.imgWidth);
		// Write developed image frame to some sort of storage
		uncompress(LZWMinCode, data, dataSize, dataOut);

		printf("\n%02x, %02x", imgDesc.imgTop, imgDesc.imgLeft);

		// Update the full canvas from new dataOut data.
		for (int x = 0; x < descriptor.canvasWidth; x++) {
			for (int y = 0; y < descriptor.canvasHeight; y++) {
				if (y >= imgDesc.imgTop && y < imgDesc.imgTop + imgDesc.imgHeight &&
					x >= imgDesc.imgLeft && x < imgDesc.imgLeft + imgDesc.imgWidth) {
					currentFrame[y * descriptor.canvasWidth + x] = dataOut[(y - imgDesc.imgTop) * imgDesc.imgWidth + (x - imgDesc.imgLeft)];
				}
			}
		}

		// printAligned(dataOut, imgDesc.imgWidth, imgDesc.imgWidth * imgDesc.imgHeight, 1);
		// printAligned(currentFrame, descriptor.canvasWidth, canvasSize, 1);

		if (ON_NIOS) {
			memcpy(frameptr, currentFrame, canvasSize);
		}
		c = getch(fileptr);

		printf("\nFrame done: %d | size: %dx%d\n", totalFrameCount, imgDesc.imgWidth, imgDesc.imgHeight);

		free(localTable);
		free(data);
		free(dataOut);
		if (c == 0x3B) {
			printf("\nEOF found\n");
			break;
		}
		totalFrameCount++;
	}

	if (ON_NIOS) {
		for (int i = 0; i < 258; i++) {
			ocmptr[i] = 0;
		}
		for (int i = 0; i < readlGlobalColorSize; i++) {
			for (int j = 0; j < 3; j++) {
				ocmptr[i] += globalTable[i].RGB[j] << (24 - j * 8);
			}
		}

		ocmptr[256] += descriptor.canvasWidth << 16;
		ocmptr[256] += descriptor.canvasHeight;

		ocmptr[257] += totalFrameCount << 24;  //last set of bytes
		ocmptr[257] += 1;
	}
	free(fileChunk);
	free(currentFrame);
	free(globalTable);
	printf("\n");
	printf("\nDONE\n");
}
