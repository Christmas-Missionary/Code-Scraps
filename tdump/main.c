#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Returns a heap-allocated char *, DO NOT LOSE THIS!! IT MUST BE FREED!!!
char * alloc_file(const char * restrict name, const char * restrict mode){
	FILE * reader = fopen(name, mode);
	if (reader == NULL){
    	printf("File `%s` does not exist.\n", name);
		return NULL;
	}

	fseek(reader, 0, SEEK_END);
	const size_t reader_size = (size_t)ftell(reader);
	rewind(reader);

	if (reader_size == -1UL){
		fclose(reader);
		printf("Finding size of reader was unsuccessful.\n");
		return NULL;
	}
	if (reader_size == 0){
		fclose(reader);
		printf("There is nothing in the file.\n");
		return NULL;
	}

	char * res = malloc(reader_size + 1);
	if (res == NULL){
		printf("Not enought memory to load in file.");
		return NULL;
	}

	fread(res, reader_size, 1, reader);
	res[reader_size] = 0;
	fclose(reader);
	return res;
}

char * bintext(const char * input){
	const char * const original = input;
	char buffer [CHAR_BIT + 1];
	buffer[CHAR_BIT] = 0;
	for (; *input != 0; input++){
		char to_read = *input;
		for (int i = 0; i < CHAR_BIT; i++, to_read <<= 1){
			buffer[i] = (char)((to_read & '\200') ? '1' : '0'); // '\200' is 0b10000000;
		}
		printf("%c -> %s\n", *input, buffer);
	}
	return (char *)original;
}

int main(const int argc, const char ** argv){
	if (argc < 2){
		printf("There is nothing to show as bits!\nType some text!\n");
		return 0;
	}
	if (argv[1][0] == 'f' && argv[1][1] == 0 && argc > 2){
		const char * const file_input = alloc_file(argv[2], "r");
		if (file_input == NULL){
			return 1;
		}
		free(bintext(file_input));
		printf("     00000000\n");
		return 0;
	}
	for (int j = 1; j < argc; j++){
		bintext(argv[j]);
		printf("%c %s\n", ' ', (j + 1 == argc) ? "   00000000" : "-> 00100000"); // There is a space between each main argument
	}
	return 0;
}
