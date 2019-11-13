#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <locale.h>
#include <string.h>


/**
*	Ищет команду с соответствующим числовым значением
*
*	@param[in] commNum Числовое представление команды
*	@param[out] commName Строковое представление команды
*
*	@return 0 - команда нашлась; -1 - не нашлась
*/

int FindCommandI(const char commNum, char* commName) {

#define DEF_CMD(name, num)\
	if (commNum==num) {\
		strcpy(commName, #name);\
		return 0;\
	}

#include "..\CommandsDictionary.h"
#undef DEF_CMD

	return -1;
}


/**
*	Декомпилирует программу, записанную в файле, создает файл с исходным кодом (asm)
*
*	@param[in] fin Входной файл
*	@param[in] fout Выходной файл
*	@param[in] NCommands Количество команд в языке My Assembler
*
*	@return 0 - компиляция прошла успешно; 1 - ошибка компиляции
*/

int CreateAsmFile(FILE* fin, FILE* fout, const int commandMaxLength) {
	assert(fin != NULL);
	assert(fout != NULL);
	assert(commandMaxLength > 0);

	int curParam = 0;
	int curCommandI = 0;
	char* curCommandS = (char*)calloc(commandMaxLength + 1, sizeof(char));

	do {
		fread(&curCommandI, sizeof(char), 1, fin);
		if (FindCommandI(curCommandI, curCommandS) == -1) {
			return 1;
		}

		if (strcmp(curCommandS, "push") == 0) {
			fread(&curParam, sizeof(int), 1, fin);
			fprintf(fout, "%s %d\n", curCommandS, curParam);
		}
		else {
			fprintf(fout, "%s\n", curCommandS);
		}
	} while (strcmp(curCommandS, "end") != 0);
	free(curCommandS);

	return 0;
}


/**
*	Осуществляет взаимодействие с ползователем, пока он не введет названия входного и выходного файлов. Если
Вводится символ '-', то открываются вайлы по умолчанию.
*
*	@param[in] fin Входной файл, бинарный режим
*	@param[in] fout Выходной файл
*	@param[in] defaultInFName Имя входного файла по умолчанию
*	@param[in] defaultOutFName Имя выходного файла по умолчанию
*
*	@return 0
*/

int GetFilesFromConsole(FILE** fin, FILE** fout, const char* defaultInFName, const char* defaultOutFName) {
	char inoutFName[200] = {};
	printf("Введите имя исполняемого файла: ");
	scanf("%200s%*c", inoutFName);

	if (inoutFName[0] == '-') {
		*fin = fopen(defaultInFName, "rb");
		*fout = fopen(defaultOutFName, "w");
		return 0;
	}

	*fin = fopen(inoutFName, "rb");
	while (*fin == NULL) {
		printf("Файл не найден.\n");
		printf("Введите имя исполняемого файла: ");
		scanf("%200s%*c", inoutFName);
		*fin = fopen(inoutFName, "rb");
	}

	printf("Введите имя файла с исходным кодом: ");
	scanf("%200s%*c", inoutFName);
	*fout = fopen(inoutFName, "w");
	while (*fin == NULL) {
		printf("Файл не найден.\n");
		printf("Введите имя файла с исходным кодом: ");
		scanf("%200s%*c", inoutFName);
		*fout = fopen(inoutFName, "w");
	}
	return 0;
}

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "Russian");

	const int commandMaxLength = 100;

	FILE* fin = {};
	FILE* fout = {};
	if (argc == 1) {
		const char defaultInFName[] = "input.mshn";
		const char defaultOutFName[] = "output.asm";

		GetFilesFromConsole(&fin, &fout, defaultInFName, defaultOutFName);
	}
	else if (argc == 3) {
		fin = fopen(argv[1], "rb");
		fout = fopen(argv[2], "w");
	}
	else {
		return 2;
	}

	if (CreateAsmFile(fin, fout, commandMaxLength) == 1) {
		printf("Ошибка декомпиляции");
	}

	fclose(fin);
	fclose(fout);
}