#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#define RADIX_COUNT 69
#define ALPHABET_RANGE 25
#define PUNCT_RANGE 57
#define DIGIT_RANGE 67
#define UNKNOWN_RANGE 68

size_t find_longeststring(char** strings, size_t string_count) {
	size_t max_len = 0, match_index = 0;
	for(size_t i=0;i<string_count;i++) {
		char* string = strings[i];
		size_t string_len = strlen(string);
		if(string_len > max_len) {
			match_index = i;
			max_len = string_len;
		}
	}
	return match_index;
}

void radix_print(char** radix_array[RADIX_COUNT], size_t radix_sizes[RADIX_COUNT]) {
	for(size_t i=0;i<RADIX_COUNT;i++) {
		printf("Radix %zu: [", i);
		for(size_t j=0;j<radix_sizes[i];j++) {
			printf("\"%s\"", radix_array[i][j]);
			if(j != radix_sizes[i]-1) {
				printf(", ");
			}
		}
		printf("]\n");
	}
}

void swap_strings(char** a, char** b) {
	char* temp = *a;
	*a = *b, *b = temp;
}

void radix_groupstrings_byletter(char** radix_array[RADIX_COUNT], size_t radix_sizes[RADIX_COUNT], char** strings,
	size_t string_count, size_t string_index) {
	int max_intlen = 0;
	for(size_t i=0;i<string_count;i++) {
		char* string = strings[i];
		size_t string_len = strlen(string),
		       index = RADIX_COUNT-1;
		char curr_char = '\0';
		if(string_index < string_len) {
			curr_char = string[string_index];
			if(isalpha(curr_char)) {
				index = tolower(curr_char) - 'a';
			} else if(ispunct(curr_char)) {
				if(curr_char <= '/' && curr_char >= '!') {
					index = 26 + curr_char - '!';
				} else if(curr_char <= '@' && curr_char >= ':') {
					index = 41 + curr_char - ':';
				} else if(curr_char <= '`' && curr_char >= '[') {
					index = 48 + curr_char - '[';
				} else if(curr_char <= '~' && curr_char >= '{') {
					index = 54 + curr_char - '{';
				}
			} else if(isdigit(curr_char) && string_len) {
				index = 58 + curr_char - '0';
			}
		} else if(string_index >= string_len && isdigit(string[string_len-1])) {
			index = 58;
		}
		size_t* radix_size = &radix_sizes[index];
		if(!radix_array[index]) {
			radix_array[index] = malloc(sizeof(char*));
			*radix_size = 1;
		} else {
			(*radix_size)++;
			radix_array[index] = realloc(radix_array[index], radix_sizes[index] * sizeof(char*));
		}
		radix_array[index][(*radix_size)-1] = string;
	}
}

void radix_arraymove_tostrings_ascending(char** radix_array[RADIX_COUNT], size_t radix_sizes[RADIX_COUNT],
	char** strings, size_t string_count) {
	size_t index = 0;
	for(size_t i=UNKNOWN_RANGE;i<RADIX_COUNT;i++) {
		for(size_t j=0;j<radix_sizes[i];j++) {
			strings[index++] = radix_array[i][j];
		}
	}

	for(size_t i=PUNCT_RANGE+1;i<DIGIT_RANGE+1;i++) {
		for(size_t j=0;j<radix_sizes[i];j++) {
			strings[index++] = radix_array[i][j];
		}
	}

	for(size_t i=ALPHABET_RANGE+1;i<PUNCT_RANGE+1;i++) {
		for(size_t j=0;j<radix_sizes[i];j++) {
			strings[index++] = radix_array[i][j];
		}
	}

	for(size_t i=0;i<ALPHABET_RANGE+1;i++) {
		for(size_t j=0;j<radix_sizes[i];j++) {
			strings[index++] = radix_array[i][j];
		}
	}
}

void radix_arraymove_tostrings_descendingvalues(char** radix_array[RADIX_COUNT], size_t radix_sizes[RADIX_COUNT],
	size_t radix_index, char** strings, size_t* string_index) {
	size_t i = radix_index;
	if(!radix_array[i]) {
		return;
	}
	for(size_t j=radix_sizes[i]-1;j>=0;) {
		strings[(*string_index)++] = radix_array[i][j];
		if(!j) {
			break;
		}
		j--;
	}

}

void radix_arraymove_tostrings_descending(char** radix_array[RADIX_COUNT], size_t radix_sizes[RADIX_COUNT],
	char** strings, size_t string_count) {
	size_t index = 0;
	// Move the alphabet range first
	for(size_t i=ALPHABET_RANGE;i>=0;) {
		radix_arraymove_tostrings_descendingvalues(radix_array, radix_sizes, i, strings, &index);
		if(!i) {
			break;
		}
		i--;
	}
	// Move the punctuation range second
	for(size_t i=PUNCT_RANGE;i>=ALPHABET_RANGE+1;) {
		radix_arraymove_tostrings_descendingvalues(radix_array, radix_sizes, i, strings, &index);
		if(i == ALPHABET_RANGE+1) {
			break;
		}
		i--;
	}

	// Move the unknown range finally
	radix_arraymove_tostrings_descendingvalues(radix_array, radix_sizes, UNKNOWN_RANGE, strings, &index);

	// Digit range
	for(size_t i=DIGIT_RANGE;i>=PUNCT_RANGE+1;) {
		radix_arraymove_tostrings_descendingvalues(radix_array, radix_sizes, i, strings, &index);
		if(i == PUNCT_RANGE+1) {
			break;
		}
		i--;
	}
}

void radix_arraymove_tostrings(char** radix_array[RADIX_COUNT], size_t radix_sizes[RADIX_COUNT],
	char** strings, size_t string_count, bool ascending) {
	if(ascending) {
		radix_arraymove_tostrings_ascending(radix_array, radix_sizes, strings, string_count);
		return;
	}
	radix_arraymove_tostrings_descending(radix_array, radix_sizes, strings, string_count);
}

void radix_arraydestroy(char** radix_array[RADIX_COUNT], size_t radix_sizes[RADIX_COUNT]) {
	for(size_t i=0;i<RADIX_COUNT;i++) {
		free(radix_array[i]); radix_array[i] = NULL;
	}
	memset(radix_array, 0, RADIX_COUNT * sizeof(char**));
	memset(radix_sizes, 0, RADIX_COUNT * sizeof(size_t));
}

bool isnumber(const char* string) {
	size_t i=0;
	while(string[i]) {
		if(!isdigit(string[i])) {
			return false;
		}
		i++;
	}
	return true;
}

void radix_arraygroup_numbers(char** radix_array[RADIX_COUNT], size_t radix_sizes[RADIX_COUNT], char** strings,
	size_t string_index, size_t max_numlen, size_t* number_indexes, size_t number_count) {
	for(size_t i=0;i<number_count;i++) {
		size_t number_index = number_indexes[i];
		char* number = strings[number_index];
		size_t number_len = strlen(number);
		size_t index = 58,
		       number_indexpad = max_numlen - string_index;
		if(number_indexpad <= number_len && number_len < max_numlen) {
			index += number[number_len - number_indexpad] - '0';
		} else if(string_index < number_len && number_len == max_numlen) {
			index += number[string_index] - '0';
		}
		size_t* radix_size = &radix_sizes[index];
		if(!radix_array[index]) {
			radix_array[index] = malloc(sizeof(char*));
			*radix_size = 1;
		} else {
			(*radix_size)++;
			radix_array[index] = realloc(radix_array[index], (*radix_size) * sizeof(char*));
		}
		radix_array[index][(*radix_size)-1] = number;
	}
}


void radix_arraymove_numberstostrings(char** radix_array[RADIX_COUNT], size_t radix_sizes[RADIX_COUNT],
	char** strings, size_t* number_indexes, size_t number_count, bool sort_ascending) {
	size_t number_index = 0;
	if(sort_ascending) {
		for(size_t i=PUNCT_RANGE+1;i<DIGIT_RANGE+1;i++) {
			if(!radix_array[i]) {
				continue;
			}
			for(size_t j=0;j<radix_sizes[i];j++) {
				size_t string_index = number_indexes[number_index++];
				strings[string_index] = radix_array[i][j];
			}
		}
		return;
	}
	for(size_t i=DIGIT_RANGE;i>=PUNCT_RANGE+1;) {
		if(radix_array[i]) {
			for(size_t j=radix_sizes[i]-1;j>=0;) {
				size_t string_index = number_indexes[number_index++];
				strings[string_index] = radix_array[i][j];
				if(!j) {
					break;
				}
				j--;
			}
		}
		if(i == PUNCT_RANGE+1) {
			break;
		}
		i--;
	}
}

void radix_arraysort_numbers(char** radix_array[RADIX_COUNT], size_t radix_sizes[RADIX_COUNT],
	char** strings, size_t string_count, bool sort_ascending) {
	bool found_number = false;
	size_t max_numlen = 0, end_numpos = 0;
	size_t* number_indexes = NULL;
	size_t number_count = 0;
	for(size_t i=0;i<string_count;i++) {
		if(isnumber(strings[i])) {
			size_t* new_ptr = realloc(number_indexes, (number_count+1) * sizeof(size_t));
			if(!new_ptr) {
				break;
			}
			size_t num_len = strlen(strings[i]);
			if(num_len > max_numlen) {
				max_numlen = num_len;
			}
			new_ptr[number_count++] = i;
			number_indexes = new_ptr;
			found_number = true;
			continue;
		}
	}
	if(!found_number) {
		return;
	}
	for(size_t i=max_numlen-1;i>=0;) {
		bool ascending_status = true;
		radix_arraygroup_numbers(radix_array, radix_sizes, strings, i, max_numlen, number_indexes, number_count);
		if(!sort_ascending && i == 0) {
			ascending_status = false;
		}
		radix_arraymove_numberstostrings(radix_array, radix_sizes, strings, number_indexes, number_count, ascending_status);
		radix_arraydestroy(radix_array, radix_sizes);
		if(i == 0) {
			break;
		}
		i--;
	}
	free(number_indexes);
}

void sort_strings(char** strings, size_t string_count, bool sort_ascending) {
	char** radix_array[RADIX_COUNT] = {0};
	size_t radix_sizes[RADIX_COUNT] = {0};
	size_t longest_index = find_longeststring(strings, string_count),
	       longest_stringlen = strlen(strings[longest_index]);
	char* longest_string = strings[longest_index];
	for(size_t i=longest_stringlen-1;i>=0;) {
		bool ascending_status = true;
		radix_groupstrings_byletter(radix_array, radix_sizes, strings, string_count, i);
		if(i == 0 && !sort_ascending) {
			ascending_status = false;
		}
		radix_arraymove_tostrings(radix_array, radix_sizes, strings, string_count, ascending_status);
		radix_arraydestroy(radix_array, radix_sizes);
		if(i == 0) {
			break;
		}
		i--;
	}
	radix_arraysort_numbers(radix_array, radix_sizes, strings, string_count, sort_ascending);
}
void print_strings(char** strings, size_t string_count) {
	printf("{\n");
	for(size_t i=0;i<string_count;i++) {
		printf("\t");
		puts(strings[i]);
	}
	printf("}\n");
}

void destroy_strings(char** strings, size_t string_count) {
	for(size_t i=0;i<string_count;i++) {
		free(strings[i]);
	}
	free(strings);
}

int main(int argc, char** argv) {
	bool sort_ascending = true;
	if(argc < 2) {
		fprintf(stderr,
			"Usage: %s [FILE] [ORDER]...\n"
			"Sorts strings in a given file based on the given order.\n"
			"if no order is provided it sorts in ascending order\n"
			"Order:\n"
			"a - sorts in ascending order\n"
			"d - Sorts in descending order\n\n"
			"Note: The input file should contain strings,"
		        " one per line.\n",
			argv[0]
		);
		return -1;
	}
	if(argc > 2 && strcmp(argv[2], "d") == 0) {
		sort_ascending = false;
	}
	FILE* f = fopen(argv[1], "r");
	if(!f) {
		return -1;
	}
	char** strings = NULL, *line = NULL;
	size_t string_count = 0, line_len = 0;
	while(getline(&line, &line_len, f) != -1) {
		line_len = strlen(line);
		line = realloc(line, line_len);
		line[line_len-1] = '\0';
		string_count++;
		strings = realloc(strings, string_count * sizeof(char*));
		strings[string_count-1] = line;
		line = NULL, line_len = 0;
	}
	puts("Before being sorted:");
	print_strings(strings, string_count);
	sort_strings(strings, string_count, sort_ascending);
	puts("After being sorted:");
	print_strings(strings, string_count);
	destroy_strings(strings, string_count);
	fclose(f);
	return 0;
}
