#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"

static char VALID_HEADER[3] = "GIF";
unsigned char * fileptr;

// struct __packed__
//{
//	unsigned char c;
//} s1;

unsigned char* read(unsigned char * dest, int size, int segments, unsigned char * src) {
	memcpy(dest, src, size*segments);
	src += size*segments;
	return src;
	//This might need to be passed by address
}
//
unsigned char getch(unsigned char *ptr) {
//	unsigned char temp = *ptr;
//	return temp;
	//Might need to increment pointer in main program
	return *fileptr;
}

unsigned char* seek(unsigned char *ptr, int size) {
	ptr += size;
	return ptr;
}

void skipToTerm() {
	while (getch(fileptr) != 0x00)
		fileptr++;
		printf("skipping");
}

int main() {
	fileptr = 0x00400002;

	HeaderBlock header;
	printf("%02x \n ", *fileptr);
	LSD descriptor;
	printf("%d \n", sizeof(descriptor));

//	fread(&header, 1, sizeof(header), fileptr);
//	fread(&descriptor, 1, sizeof(descriptor), fileptr);
	fileptr = read(&header, 1, sizeof(header), fileptr);
	fileptr = read(&descriptor, 1, sizeof(descriptor), fileptr);
	for (int i = 0; i < 3; i++) {
		assert(header.sig[i] == VALID_HEADER[i]);
	}
	unsigned char testing = *fileptr;
	printf("%02x | test\n", testing);
//	unsigned char *y = fileptr+1;
	//Im done for tonight, this doesn't make an
	fileptr++;
	testing = *fileptr;
	printf("%02x | test\n", testing);
	assert(descriptor.packedField.globalColorFlag);	 //For all intents we're going to need a global table

	int readlGlobalColorSize = 0x2 << descriptor.packedField.globalColorSize;  // 2^(N+1) actual colors
	// printf("%d, %d\n", test, packed.actual_color_size);
	colorTableEntry *globalTable = (colorTableEntry *)
		malloc(readlGlobalColorSize * sizeof(colorTableEntry));	 // We can make this 256 for hardware

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
int x = 0;
	 while (0) {
		 printf("herehere\n");
		 	GCE gce;
	 		imageDescriptor imgDesc;
	 		colorTableEntry *localTable = NULL;
//
//	 		unsigned char testing = *fileptr;
//	 		printf("%02x | test\n", testing);
//	 		fileptr = fileptr+1;
//			testing = *fileptr;
//		 		printf("%02x | test\n", testing);

	 		break;

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
//	 		ungetc(c, fileptr);

	 		fileptr = read(&imgDesc, 1, sizeof(imgDesc), fileptr);
	 		assert(imgDesc.imgSeperator == 0x2C);
	 		if (imgDesc.packedField.localColorFlag) {
	 			// Load a local color table
	 			int realLocalColorSize = 0x2 << imgDesc.packedField.localColorSize;	 // 2^(N+1) actual colors
	 			localTable = (colorTableEntry *)
	 				malloc(realLocalColorSize * sizeof(colorTableEntry));
	 			fileptr = read(localTable, 3, realLocalColorSize, fileptr);
	 		}

	 		//Let's yeet that image data into something
	 		unsigned char LZWMinCode = getch(fileptr); fileptr++;
	 		printf("%02x\n", LZWMinCode);
	 		unsigned char *data = NULL;
	 		unsigned int dataSize = 0;
	 		unsigned char bytesInSubblock = getch(fileptr); fileptr++;
	 		while (bytesInSubblock) {
	 			data = realloc(data, (sizeof(char) * (dataSize + bytesInSubblock)));
	 			fileptr = read(data, 1, bytesInSubblock, fileptr);
	 			dataSize += bytesInSubblock;
	 			bytesInSubblock = getch(fileptr);
	 			fileptr++;
	 		}

	 		// for (int i = 0; i < dataSize; i++) {
	 		// 	printf("%02x,", data[i]);
	 		// }
//	 		unsigned char *dataOut = malloc(imgDesc.imgHeight * imgDesc.imgWidth);

	 		// Write developed image frame to some sort of storage
//	 		uncompress(LZWMinCode, data, dataSize, dataOut);
//	 		for (int i = 0; i < imgDesc.imgHeight * imgDesc.imgWidth; i++) {
//	 			if (i % imgDesc.imgWidth == 0) {
//	 				printf("\n");
//	 			}
//	 			printf("\033[38;5;%dm", dataOut[i]);
//	 			printf("%02x ", dataOut[i]);
//	 			printf("\033[0m");
//	 		}

	 		// Here we have completed image data
	 		// We should write: check if localColorFlag then use that to grab colors from data[i]
	 		// We also need to save the image dimensions and start cords.

	 		// Check if we're at end of file:
	 		// and break loop!
	 		c = getch(fileptr);
//	 		ungetc(c, fileptr);

	 		printf("\nFrame done\n");

	 		free(localTable);
	 		free(data);
//	 		free(dataOut);
	 		if (c == 0x3B) {
	 			printf("\nEOF found\n");
	 			break;
	 		}
	 		x++;
	 	}

	 	free(globalTable);
	 	printf("\n");
	 	fclose(fileptr);












	for (int i = 0; i<0; i++) {
		printf("%02x ", *fileptr);
		fileptr++;
	}
	printf("\n");
//	assert(descriptor.packedField.globalColorFlag);

	printf("done");
}
