#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef struct dictionary_entry_t {
	unsigned char byte;
	int32_t prev;
	int32_t len;
} dictionary_entry_t;

void uncompress(int32_t code_length,
				const unsigned char *input,
				int32_t input_length,
				unsigned char *out) {
	printf("uncompress!\n");
	// Source for this function is:
	// https://commandlinefanatic.com/cgi-bin/showarticle.cgi?article=art011
	int32_t i, bit;
	int32_t code, prev = -1;
	dictionary_entry_t *dictionary;
	int32_t dictionary_ind;
	uint32_t mask = 0x01;
	int32_t reset_code_length;
	int32_t clear_code;	 // This varies depending on code_length
	int32_t stop_code;	 // one more than clear code
	int32_t match_len;

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
		printf("setup dictionary start\n");
		dictionary[dictionary_ind].byte = dictionary_ind;
		// XXX this only works because prev is a 32-bit int32_t (> 12 bits)
		dictionary[dictionary_ind].prev = -1;
		dictionary[dictionary_ind].len = 1;
	}

	// 2^code_len + 1 is the special "end" code; don't give it an entry here
	dictionary_ind++;
	dictionary_ind++;

	// TODO verify that the very last byte is clear_code + 1
	while (input_length) {
		printf("main loop | code_length: %d\n", code_length);
		code = 0x0;
		// Always read one more bit than the code length
		for (i = 0; i < (code_length + 1); i++) {
			// This is different than in the file read example; that
			// was a call to "next_bit"

			 printf("    new code:    %02x\n", code);
			 printf("    mask:        %02x\n", mask);
			 printf("    bit:         %02x\n", bit);
			 printf("    input:       %02x\n", *input);


			bit = (*input & mask) ? 1 : 0;
			mask <<= 1;

			if (mask == 0x100) {
				mask = 0x01;
				input++;
				input_length--;
			}

			code = code | (bit << i);
		}
        printf("new code: %02x\n", code);


		if (code == clear_code) {
			code_length = reset_code_length;
			dictionary = (dictionary_entry_t *)realloc(dictionary,
													   sizeof(dictionary_entry_t) * (1 << (code_length + 1)));

			for (dictionary_ind = 0;
				 dictionary_ind < (1 << code_length);
				 dictionary_ind++) {
				dictionary[dictionary_ind].byte = dictionary_ind;
				// XXX this only works because prev is a 32-bit int32_t (> 12 bits)
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
			fprintf(stderr, "code = %.02x, and dictionary_ind = %.02x\n",
					code, dictionary_ind);

			if (code > dictionary_ind) {
				fprintf(stderr, "code = %.02x, but dictionary_ind = %.02x\n",
						code, dictionary_ind);
				exit(0);
			}

			// Special handling for KwKwK
			if (code == dictionary_ind) {
				int32_t ptr = prev;

				while (dictionary[ptr].prev != -1) {
					ptr = dictionary[ptr].prev;
				}
				dictionary[dictionary_ind].byte = dictionary[ptr].byte;
			} else {
				int32_t ptr = code;
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

		// Now copy the dictionary entry backwards int32_to "out"
		match_len = dictionary[code].len;
		while (code != -1) {
			out[dictionary[code].len - 1] = dictionary[code].byte;
			if (dictionary[code].prev == code) {
				fprintf(stderr, "internal error; self-reference.");
				exit(0);
			}
			code = dictionary[code].prev;
		}

		out += match_len;
	}
	free(dictionary);
}
