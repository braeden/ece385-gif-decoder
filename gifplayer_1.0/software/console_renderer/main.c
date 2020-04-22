#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "structs.h"
static char VALID_HEADER[3] = "GIF";
FILE *fileptr;

void uncompress(int code_length,
				const unsigned char *input,
				int input_length,
				unsigned char *out) {
	// Source for this function is:
	// https://commandlinefanatic.com/cgi-bin/showarticle.cgi?article=art011
	int i, bit;
	int code, prev = -1;
	dictionary_entry_t *dictionary;
	int dictionary_ind;
	unsigned int mask = 0x01;
	int reset_code_length;
	int clear_code;	 // This varies depending on code_length
	int stop_code;	 // one more than clear code
	int match_len;

	clear_code = 1 << (code_length);
	stop_code = clear_code + 1;
	// To handle clear codes
	reset_code_length = code_length;

	// Create a dictionary large enough to hold "code_length" entries.
	// Once the dictionary overflows, code_length increases
	dictionary = (dictionary_entry_t *)
		malloc(sizeof(dictionary_entry_t) * (1 << (code_length + 1)));

	// Initialize the first 2^code_len entries of the dictionary with their
	// indices.  The rest of the entries will be built up dynamically.

	// Technically, it shouldn't be necessary to initialize the
	// dictionary.  The spec says that the encoder "should output a
	// clear code as the first code in the image data stream".  It doesn't
	// say must, though...
	for (dictionary_ind = 0;
		 dictionary_ind < (1 << code_length);
		 dictionary_ind++) {
		dictionary[dictionary_ind].byte = dictionary_ind;
		// XXX this only works because prev is a 32-bit int (> 12 bits)
		dictionary[dictionary_ind].prev = -1;
		dictionary[dictionary_ind].len = 1;
	}

	// 2^code_len + 1 is the special "end" code; don't give it an entry here
	dictionary_ind++;
	dictionary_ind++;

	// TODO verify that the very last byte is clear_code + 1
	while (input_length) {
		code = 0x0;
		// Always read one more bit than the code length
		for (i = 0; i < (code_length + 1); i++) {
			// This is different than in the file read example; that
			// was a call to "next_bit"
			bit = (*input & mask) ? 1 : 0;
			mask <<= 1;

			if (mask == 0x100) {
				mask = 0x01;
				input++;
				input_length--;
			}

			code = code | (bit << i);
		}

		if (code == clear_code) {
			code_length = reset_code_length;
			dictionary = (dictionary_entry_t *)realloc(dictionary,
													   sizeof(dictionary_entry_t) * (1 << (code_length + 1)));

			for (dictionary_ind = 0;
				 dictionary_ind < (1 << code_length);
				 dictionary_ind++) {
				dictionary[dictionary_ind].byte = dictionary_ind;
				// XXX this only works because prev is a 32-bit int (> 12 bits)
				dictionary[dictionary_ind].prev = -1;
				dictionary[dictionary_ind].len = 1;
			}
			dictionary_ind++;
			dictionary_ind++;
			prev = -1;
			continue;
		} else if (code == stop_code) {
			if (input_length > 1) {
				fprintf(stderr, "Malformed GIF (early stop code)\n");
				exit(0);
			}
			break;
		}

		// Update the dictionary with this character plus the _entry_
		// (character or string) that came before it
		if ((prev > -1) && (code_length < 12)) {
			if (code > dictionary_ind) {
				fprintf(stderr, "code = %.02x, but dictionary_ind = %.02x\n",
						code, dictionary_ind);
				exit(0);
			}

			// Special handling for KwKwK
			if (code == dictionary_ind) {
				int ptr = prev;

				while (dictionary[ptr].prev != -1) {
					ptr = dictionary[ptr].prev;
				}
				dictionary[dictionary_ind].byte = dictionary[ptr].byte;
			} else {
				int ptr = code;
				while (dictionary[ptr].prev != -1) {
					ptr = dictionary[ptr].prev;
				}
				dictionary[dictionary_ind].byte = dictionary[ptr].byte;
			}

			dictionary[dictionary_ind].prev = prev;

			dictionary[dictionary_ind].len = dictionary[prev].len + 1;

			dictionary_ind++;

			// GIF89a mandates that this stops at 12 bits
			if ((dictionary_ind == (1 << (code_length + 1))) &&
				(code_length < 11)) {
				code_length++;

				dictionary = (dictionary_entry_t *)realloc(dictionary,
														   sizeof(dictionary_entry_t) * (1 << (code_length + 1)));
			}
		}

		prev = code;

		// Now copy the dictionary entry backwards into "out"
		match_len = dictionary[code].len;
		while (code != -1) {
			out[dictionary[code].len - 1] = dictionary[code].byte;
			if (dictionary[code].prev == code) {
				fprintf(stderr, "Internal error; self-reference.");
				exit(0);
			}
			code = dictionary[code].prev;
		}

		out += match_len;
	}
	free(dictionary);
}

void skipToTerm() {
	while (fgetc(fileptr) != 0x00)
		printf("skipping");
}

int main() {
	fileptr = 0x00400002;

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

	// for (int i = 0; i < readlGlobalColorSize; i++) {
	// 	printf("[#");
	// 	for (int j = 0; j < 3; j++) {
	// 		printf("%02x", globalTable[i].RGB[j]);
	// 	}
	// 	printf("],");
	// }
	// printf("\n");

	//Now let's load our images
	while (1) {
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
		unsigned char *data = NULL;
		unsigned int dataSize = 0;
		unsigned char bytesInSubblock = getc(fileptr);
		while (bytesInSubblock) {
			data = realloc(data, (sizeof(char) * (dataSize + bytesInSubblock)));
			fread(data, 1, bytesInSubblock, fileptr);
			dataSize += bytesInSubblock;
			bytesInSubblock = getc(fileptr);
		}

		// for (int i = 0; i < dataSize; i++) {
		// 	printf("%02x,", data[i]);
		// }
		unsigned char *dataOut = malloc(imgDesc.imgHeight * imgDesc.imgWidth);

		// Write developed image frame to some sort of storage
		uncompress(LZWMinCode, data, dataSize, dataOut);
		for (int i = 0; i < imgDesc.imgHeight * imgDesc.imgWidth; i++) {
			if (i % imgDesc.imgWidth == 0) {
				printf("\n");
			}
			printf("\033[38;5;%dm", dataOut[i]);
			printf("%02x ", dataOut[i]);
			printf("\033[0m");
		}

		// Here we have completed image data
		// We should write: check if localColorFlag then use that to grab colors from data[i]
		// We also need to save the image dimensions and start cords.

		// Check if we're at end of file:
		// and break loop!
		c = fgetc(fileptr);
		ungetc(c, fileptr);
		printf("\nFrame done\n");

		free(localTable);
		free(data);
		free(dataOut);
		if (c == 0x3B) {
			printf("\nEOF found\n");
			break;
		}
	}

	free(globalTable);
	printf("\n");
	fclose(fileptr);
}
