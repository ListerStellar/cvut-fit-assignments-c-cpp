#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct {
	char* data;
	size_t size;
	size_t capacity;
} t_string;

t_string initString() {
	t_string string;
	string.data = (char*)malloc(sizeof(char));
	string.size = 0;
	string.capacity = 1;
	return string;
}

void addCharToString(t_string* string, char val) {
	if ((string->size + (size_t)1) > string->capacity) {
		string->capacity *= 2;
		string->data = (char*)realloc(string->data, string->capacity * sizeof(char));
	}
	(string->data)[(string->size)++] = val;
}

void destroyString(t_string* string) {
	free(string->data);
}

void printString(const t_string* string) {
	for (int i = 0; i < (int)string->size; i++) {
		printf("%c", string->data[i]);
	}
}

char* getChar(t_string* string, int length, int x, int y) {
	return &(string->data[x * length + y]);
}

bool checkValidCoords(int x, int y, int max_x, int max_y) {
	return x >= 0 && x < max_x && y >= 0 && y < max_y;
}

bool checkWord(t_string* string, const t_string* word, int x, int y, int pl_x, int pl_y, int max_x, int max_y, int length) {
	int new_x, new_y;
	for (int i = 0; i < (int)word->size; i++) {
		new_x = x + i * pl_x, new_y = y + i * pl_y;
		if (!checkValidCoords(new_x, new_y, max_x, max_y) || tolower(*getChar(string, length, new_x, new_y)) != word->data[i]) {
			return false;
		}
	}
	return true;
}

void upperWord(t_string* string, const t_string* word, int x, int y, int pl_x, int pl_y, int length) {
	int new_x, new_y;
	for (int i = 0; i < (int)word->size; i++) {
		new_x = x + i * pl_x, new_y = y + i * pl_y;
		*getChar(string, length, new_x, new_y) = toupper(*getChar(string, length, new_x, new_y));
	}
}

void deleteWord(t_string* string, const t_string* word, int length) {
	int row_num = ((int)string->size - 1) / length, col_num = length, deleted_num = 0;
	for (int i = 0; i < row_num; i++) {
		for (int j = 0; j < col_num; j++) {
			if (tolower(*getChar(string, length, i, j)) == word->data[0]) {
				if (checkWord(string, word, i, j, 0, 1, row_num, col_num, length)) { deleted_num++; upperWord(string, word, i, j, 0, 1, length); }
				if (checkWord(string, word, i, j, 0, -1, row_num, col_num, length)) { deleted_num++; upperWord(string, word, i, j, 0, -1, length); }
				if (checkWord(string, word, i, j, 1, 0, row_num, col_num, length)) { deleted_num++; upperWord(string, word, i, j, 1, 0, length); }
				if (checkWord(string, word, i, j, -1, 0, row_num, col_num, length)) { deleted_num++; upperWord(string, word, i, j, -1, 0, length); }
				if (checkWord(string, word, i, j, 1, 1, row_num, col_num, length)) { deleted_num++; upperWord(string, word, i, j, 1, 1, length); }
				if (checkWord(string, word, i, j, -1, -1, row_num, col_num, length)) { deleted_num++; upperWord(string, word, i, j, -1, -1, length); }
				if (checkWord(string, word, i, j, 1, -1, row_num, col_num, length)) { deleted_num++; upperWord(string, word, i, j, 1, -1, length); }
				if (checkWord(string, word, i, j, -1, 1, row_num, col_num, length)) { deleted_num++; upperWord(string, word, i, j, -1, 1, length); }
			}
		}
	}
	printString(word);
	printf(": %dx\n", deleted_num);
}

void findWord(t_string* string, const t_string* word, int length) {
	int row_num = ((int)string->size - 1) / length, col_num = length, found_num = 0;
	for (int i = 0; i < row_num; i++) {
		for (int j = 0; j < col_num; j++) {
			if (tolower(*getChar(string, length, i, j)) == word->data[0]) {
				if (checkWord(string, word, i, j, 0, 1, row_num, col_num, length)) found_num++;
				if (checkWord(string, word, i, j, 0, -1, row_num, col_num, length)) found_num++;
				if (checkWord(string, word, i, j, 1, 0, row_num, col_num, length)) found_num++;
				if (checkWord(string, word, i, j, -1, 0, row_num, col_num, length)) found_num++;
				if (checkWord(string, word, i, j, 1, 1, row_num, col_num, length)) found_num++;
				if (checkWord(string, word, i, j, -1, -1, row_num, col_num, length)) found_num++;
				if (checkWord(string, word, i, j, 1, -1, row_num, col_num, length)) found_num++;
				if (checkWord(string, word, i, j, -1, 1, row_num, col_num, length)) found_num++;
			}
		}
	}
	printString(word);
	printf(": %dx\n", found_num);
}

void printSecret(const t_string* string) {
	printf("Tajenka:\n");
	bool isVoid = true;
	int pintedSymbols = 0;
	for (int i = 0; i < (int)string->size; i++) {
		if (islower(string->data[i])) {
			printf("%c", string->data[i]);
			pintedSymbols++;
			isVoid = false;
			if (pintedSymbols % 60 == 0) printf("\n");
		}
	}
	if (!isVoid) printf("\n");
}

int main() {
	printf("Osmismerka:\n");
	char input; int length = -1;
	t_string string = initString();
	while (string.size < 2 || !(string.data[string.size - (size_t)1] == '\n' && string.data[string.size - (size_t)2] == '\n')) {
		if (scanf("%c", &input) != 1 || (!islower(input) && input != '.' && input != '\n')) {
			destroyString(&string);
			printf("Nespravny vstup.\n");
			return 1;
		}
		addCharToString(&string, input);
		if (length == -1 && input == '\n') length = string.size;
	}
	if (((string.size - (size_t)1) % length != 0) || string.size < 3) {
		destroyString(&string);
		printf("Nespravny vstup.\n");
		return 2;
	}
	while (scanf("%c", &input) == 1) {
		if (input != '?' && input != '#' && input != '-') {
			destroyString(&string);
			printf("Nespravny vstup.\n");
			return 3;
		}
		if (input == '-') {
			t_string toDelete = initString();
			input = ' ';
			while (isspace(input)) {
				if (scanf("%c", &input) != 1) {
					destroyString(&string);
					destroyString(&toDelete);
					printf("Nespravny vstup.\n");
					return 6;
				}
			}
			while (input != '\n') {
				addCharToString(&toDelete, input);
				if (scanf("%c", &input) != 1 || (!islower(input) && input != '\n')) {
					destroyString(&string);
					destroyString(&toDelete);
					printf("Nespravny vstup.\n");
					return 7;
				}
			}
			if (toDelete.size < 2) {
				destroyString(&string);
				destroyString(&toDelete);
				printf("Nespravny vstup.\n");
				return 8;
			}
			deleteWord(&string, &toDelete, length);
			destroyString(&toDelete);
		}
		else if (input == '#') {
			t_string toFind = initString();
			input = ' ';
			while (isspace(input)) {
				if (scanf("%c", &input) != 1) {
					destroyString(&string);
					destroyString(&toFind);
					printf("Nespravny vstup.\n");
					return 9;
				}
			}
			while (input != '\n') {
				addCharToString(&toFind, input);
				if (scanf("%c", &input) != 1 || (!islower(input) && input != '\n')) {
					destroyString(&string);
					destroyString(&toFind);
					printf("Nespravny vstup.\n");
					return 10;
				}
			}
			if (toFind.size < 2) {
				destroyString(&string);
				destroyString(&toFind);
				printf("Nespravny vstup.\n");
				return 11;
			}
			findWord(&string, &toFind, length);
			destroyString(&toFind);
		}
		else {
			if (scanf("%c", &input) != 1 || input != '\n') {
				destroyString(&string);
				printf("Nespravny vstup.\n");
				return 4;
			}
			printSecret(&string);
		}
	}
	if (!feof(stdin)) {
		destroyString(&string);
		printf("Nespravny vstup.\n");
		return 5;
	}
	destroyString(&string);
	return 0;
}