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
        // description.img_height = 0;
        // description.img_left = 0;
        // description.img_width = 0;
        // description.img_top = 0;

		fread(&description, 1, 10, fileptr);
		assert(description.img_seperator == 0x2C);
		
		//Let's yeet that image data into something

		unsigned char LZW_min_code = getc(fileptr);
		printf("%02x\n", LZW_min_code);
		printf("%02x\n",description.packed_field);
	}

	free(global_table);
	//Don't forget to free local_table too
	printf("\n");
	fclose(fileptr);
}