#include <errors.h>
#include <preprocessor.h>
#include <stdio.h>
#include <string.h>

// has to be allocated. Has to be freed by the user as well
void preprocess(char* code, size_t* code_len) {
	// go through the text character by character. If after a \n or in the beginning of the file the word found is "import" followed by "filename.asm"
	// read that files contents and preprocess them as well
	// add the two strings together
	bool import_found = false;

	char c;
	char buffer[BUFF_SIZE];
	int bi = 0;

	for (int i = 0; i < (int)*code_len; i++, bi++) {
		c = code[i];
		buffer[bi] = c;

		// clear the buffer on \n
		if (c == '\n') {
			bi = 0;
			continue;
		}

		if (import_found && buffer[bi] == '"') {
			throw_error("import", false);
			int len_of_file_path = bi - IMPORT_LENGTH - END_OF_IMPORT_LENGTH;
			char file_path[len_of_file_path];

			// get file path
			for (int j = 0; j < len_of_file_path; j++) {
				file_path[j] = buffer[bi - len_of_file_path + j];
			}
			file_path[len_of_file_path] = '\0';

			FILE* fp;
			fp = fopen(file_path, "r");
			if (fp == NULL) {
				throw_error("File Not Found!", true);
			}
			fclose(fp);
			// load imported

			import_found = false;
			bi = 0;
		}

		if (bi == IMPORT_LENGTH && strcmp("import \"", buffer) == 0) {
			// check if those 6 characters equal import
			throw_error("import", false);
			import_found = true;
		}

		printf("%d\n", bi);
		buffer[bi] = c;
	}
}
