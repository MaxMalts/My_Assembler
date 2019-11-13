#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <locale.h>
#include <string.h>
#include "..\CommandsDictionary.h"


/**
*	Ищет команду с соответствующим числовым значением
*
*	@param[in] num Числовое значение
*	@param[in] NCommands Количество команд в массиве команд
*
*	@return Индекс команды в массиве команд, если команда нашлась; -1 - если не нашлась
*/

int FindCommandI(int num, const int NCommands) {
	assert(NCommands > 0);

	for (int index = 0; index < NCommands; index++) {
		if (commands[index].commandI == num) {
			return index;
		}
	}
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

int CreateAsmFile(FILE* fin, FILE* fout, const int NCommands) {
	assert(fin != NULL);
	assert(fout != NULL);
	assert(NCommands > 0);

	int curParam = 0;
	int curCommandIndex = 0;
	int curCommandI = 0;

	do {
		fscanf(fin, "%d", &curCommandI);
		curCommandIndex = FindCommandI(curCommandI, NCommands);
		if (curCommandIndex == -1) {
			return 1;
		}

		if (commands[curCommandIndex].hasParam) {
			fscanf(fin, "%d\n", &curParam);
			fprintf(fout, "%s %d\n", commands[curCommandIndex].commandS, curParam);
		}
		else {
			fscanf(fin, "\n");
			fprintf(fout, "%s\n", commands[curCommandIndex].commandS);
		}
	} while (strcmp(commands[curCommandIndex].commandS, "end") != 0);

	return 0;
}


/**
*	Осуществляет взаимодействие с ползователем, пока он не введет названия входного и выходного файлов. Если
Вводится символ '-', то открываются вайлы по умолчанию.
*
*	@param[in] fin Входной файл
*	@param[in] fout Выходной файл
*	@param[in] defaultInFName Имя входного файла по умолчанию
*	@param[in] defaultOutFName Имя выходного файла по умолчанию
*
*	@return 0
*/

int GetFilesFromConsole(FILE** fin, FILE** fout, const char* defaultInFName, const char* defaultOutFName) {
	char inoutFName[200] = {};
	printf("Введите имя исполняемого файла: ");
	scanf("%200s", inoutFName);

	if (inoutFName[0] == '-') {
		*fin = fopen(defaultInFName, "r");
		*fout = fopen(defaultOutFName, "w");
		return 0;
	}

	*fin = fopen(inoutFName, "r");
	while (*fin == NULL) {
		printf("Файл не найден.\n");
		printf("Введите имя исполняемого файла: ");
		scanf("%200s", inoutFName);
		*fin = fopen(inoutFName, "r");
	}

	printf("Введите имя файла с исходным кодом: ");
	scanf("%200s", inoutFName);
	*fout = fopen(inoutFName, "w");
	while (*fin == NULL) {
		printf("Файл не найден.\n");
		printf("Введите имя файла с исходным кодом: ");
		scanf("%200s", inoutFName);
		*fout = fopen(inoutFName, "w");
	}
	return 0;
}

int main() {
	setlocale(LC_ALL, "Russian");

	const char defaultInFName[] = "input.mshn";
	const char defaultOutFName[] = "output.asm";
	const int commandMaxLength = commMaxLength;
	const int NCommands = NComm;

	FILE* fin = {};
	FILE* fout = {};
	GetFilesFromConsole(&fin, &fout, defaultInFName, defaultOutFName);

	if (CreateAsmFile(fin, fout, NCommands) == 1) {
		printf("Ошибка декомпиляции");
		getchar();
	}

	fclose(fin);
	fclose(fout);
}