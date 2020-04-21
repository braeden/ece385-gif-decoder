#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "structs.h"
volatile unsigned char* sram_ptr = 0x00400000;

int main() {

	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 16; j++) {
			printf("%02x ", *sram_ptr);
			sram_ptr = sram_ptr + 8;
		}
		printf("\n");
	}
	printf("done");
}
