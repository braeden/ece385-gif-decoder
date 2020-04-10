#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "structs.h"
static char VALID_HEADER[3] = "GIF";
FILE *fileptr;

int main() {
	fileptr = fopen("sample2.gif", "rb");

	start header;
	packed_field_LSD packed;
	fread(&header, 1, 13, fileptr);
	for (int i = 0; i < 3; i++) {
		assert(header.signature[i] == VALID_HEADER[i]);
	}
	packed.global_color_flag = header.packed_details >> 7;
	assert(packed.global_color_flag);  //For all intents we're going to need a global table

	packed.color_resolution = (header.packed_details >> 4) & 7;
	packed.global_color_size = header.packed_details & 7;
	packed.actual_color_size = 0x2 << packed.global_color_size;	 // 2^(N+1) actual colors
																 // printf("%d, %d\n", test, packed.actual_color_size);
	color_table_entry *global_table = (color_table_entry *)
		malloc(packed.actual_color_size * sizeof(color_table_entry));  // We can make this 256 for hardware

	//Read global color table and print it
	fread(global_table, 3, packed.actual_color_size, fileptr);
	for (int i = 0; i < packed.actual_color_size; i++) {
		printf("[#");
		for (int j = 0; j < 3; j++) {
			printf("%02x", global_table[i].RGB[j]);
		}
		printf("],");
	}
	printf("\n");

	// // Let's peek the next char to check if it has 0x21
	// // which means we have an optional GCE next
	// unsigned char c = fgetc(fileptr);
	// ungetc(c, fileptr);

	//Now let's load our images
	for (int image_count = 0; image_count < 1; image_count++) {
		unsigned char c = fgetc(fileptr);
		ungetc(c, fileptr);
		if (c == 0x21) {
			GCE gce;
			fread(&gce, 1, 8, fileptr);
			printf("Loaded GCE\n");
			//Do stuff later, w/ transparency + animation
		}
		image_descriptor description;
		packed_field_IMG packed_local;

		fread(&description.img_seperator, 1, 1, fileptr);
		fread(&description.img_left, 1, 2, fileptr);
		fread(&description.img_top, 1, 2, fileptr);
		fread(&description.img_width, 1, 2, fileptr);
        fread(&description.img_height, 1, 2, fileptr);
        fread(&description.packed_field, 1, 1, fileptr);
		assert(description.img_seperator == 0x2C);
		packed_local.local_color_flag = description.packed_field >> 7;
		if (packed_local.local_color_flag) {
			// Load a local color table
			packed_local.interlace_flag = (description.packed_field >> 6) & 1;
			packed_local.sort_flag = (description.packed_field >> 5) & 1;
			packed_local.local_color_size = description.packed_field & 7;
			packed_local.actual_color_size = 0x2 << packed_local.local_color_size;	// 2^(N+1) actual colors
			color_table_entry *local_table = (color_table_entry *)
				malloc(packed_local.actual_color_size * sizeof(color_table_entry));
			fread(global_table, 3, packed_local.actual_color_size, fileptr);
		}
		//Let's yeet that image data into something

		unsigned char LZW_min_code = getc(fileptr);
		printf("%02x\n", LZW_min_code);
		unsigned char bytes_in_subblock = getc(fileptr);
		while (bytes_in_subblock) {
			// printf("%02x\n", bytes_in_subblock);
			subblock cur_sb;
			cur_sb.subblock_size = bytes_in_subblock;
			fread(&cur_sb.blocks, 1, cur_sb.subblock_size, fileptr);

			// malloc mem and read N bytes in

			bytes_in_subblock = getc(fileptr);
		}
		// Write developed image frame to some sort of storage

		// Check if we're at end of file:
		// and break loop!
		c = fgetc(fileptr);
		ungetc(c, fileptr);
		if (c == 0x3B) {
			break;
		}
	}

	free(global_table);
	//Don't forget to free local_table too
	printf("\n");
	fclose(fileptr);
}