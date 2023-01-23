#include <preprocessor.h>
#include <debug.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <list.h>

#warning make better version of this

char* copy_line(char* input, char* output) {
	int i = 0;
	while (input[i] != '\n' && input[i] != '\0') {
		output[i] = input[i];
		i++;
	}

	return &input[i + (input[i] == '\0' ? 0 : 1)];
}

bool starts_with(char* with, char* src) {
	int len = strlen(with);
	for (int i = 0; i < len; i++) {
		if (with[i] != src[i]) {
			return false;
		}
	}

	return true;
}

char* copy_until(char until, char* src, char* dest) {
	int i = 0;
	while (src[i] != '\0' && src[i] != until) {
		dest[i] = src[i];
		i++;
	}

	return &src[i + (src[i] == '\0' ? 0 : 1)];
}

bool compare(char* str, char* check, int check_len) {
	for (int i = 0; i < check_len; i++) {
		if (str[i] != check[i]) {
			return false;
		}
	}

	return true;
}

char* strreplace(char* src, char* str, char* rep) {
	int len = strlen(src);
	int str_len = strlen(str);
	int rep_len = strlen(rep);

	for (int i = 0; i < len - str_len + 1; i++) {
		if (compare(&src[i], str, str_len)) {
			char* start = src;
			char* end = &src[i + strlen(str)];

			char* result = (char*) malloc(len + rep_len);
			assert(result != NULL);
			memset(result, 0, len + rep_len);

			strncpy(result, start, i);
			strcat(result, rep);
			strcat(result, end);

			return result;
		}
	}

	return nullptr;
}

char* load_file(char* input_path) {
	debugf("Loading %s...\n", input_path);
	FILE* f = fopen(input_path, "r");
	assert(f != nullptr);

	fseek(f, 0, SEEK_END);
	size_t size = ftell(f);
	fseek(f, 0, SEEK_SET);

	char* s = (char*) malloc(size + 1);
	memset(s, 0, size + 1);
	fread(s, size, 1, f);

	fclose(f);

	return s;
}

#define INCLUDE "$include("
#define DEFINE "$define "

struct define_list_t {
	char name[0xff];
	char value[0xff];
};

char* preprocess(char* input) {
	debugf("Preprocessing...\n");

	int size = 0;
	char* output = nullptr;

	debugf("Doing pass 1...\n");
	char* curr = input;
	do {
		char line[0xff] = { 0 };
		curr = copy_line(curr, line);

		if (starts_with((char*) INCLUDE, line)) {
			char include[0xff] = { 0 };
			copy_until(')', &line[sizeof(INCLUDE) - 1], include);

			char* file_code = load_file(include);

			int new_size = strlen(file_code) + 1;
			output = (char*) realloc(output, size + new_size);
			strcpy(&output[size], file_code);
			output[size + new_size - 1] = '\n';
			size += new_size;

			free(file_code);

		} else {
			int new_size = strlen(line) + 1;
			output = (char*) realloc(output, size + new_size);
			strcpy(&output[size], line);
			output[size + new_size - 1] = '\n';
			size += new_size;
		}
	} while (curr[0] != '\0');

	
	output = (char*) realloc(output, size + 1);
	output[size] = '\0';

	input = output;
	output = nullptr;
	size = 0;

	debugf("Doing pass 2...\n");
	curr = input;
	list<define_list_t> defines = list<define_list_t>(10);

	do {
		char line[0xff] = { 0 };
		curr = copy_line(curr, line);

		if (starts_with((char*) DEFINE, line)) {
			char define_name[0xff] = { 0 };
			char* end = copy_until(' ', &line[sizeof(DEFINE) - 1], define_name);

			define_list_t def = { 0 };
			strcpy(def.name, define_name);
			strcpy(def.value, end);
			defines.add(def);
		} else {
			int new_size = strlen(line) + 1;
			output = (char*) realloc(output, size + new_size);
			strcpy(&output[size], line);
			output[size + new_size - 1] = '\n';
			size += new_size;
		}
	} while (curr[0] != '\0');

	output = (char*) realloc(output, size + 1);
	output[size] = '\0';

	free(input);

	debugf("Doing pass 3...\n");
	for (int i = 0; i < defines.length; i++) {
		if (defines.data[i].taken) {
			bool finished = false;
			while (!finished) {
				char* old = output;
				output = strreplace(output, defines.data[i].data.name, defines.data[i].data.value);
				if (output != nullptr) {
					free(old);
				} else {
					output = old;
					finished = true;
				}
			}
		}
	}

	return output;
}