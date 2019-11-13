#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <locale.h>
#include <string.h>
#include "..\CommandsDictionary.h"


/**
*	Проверяет символ на признак конца строки (EOF, '\n', ' ', '\0')
*
*	@param[in] ch Символ
*
*	@return true - символ конечный; false - не конечный.
*/

bool IsNEndChar(char ch) {
	if ((ch != EOF) && (ch != '\n') && (ch != ' ') && (ch != '\0')) {
		return true;
	}
	return false;
}


/**
*	Считывает строку из файла
*
*	@param[in] str Строка, в которую надо запсать
*	@param[in] num Максимальное количество символов с учетом '\0'
*	@param[in] f Файл
*	@param[in] NEndChar Функция, проверяющая символ на признак конца строки
*
*	@return Указатель на полученную строку (совпадает с str)
*/

char* Fgets_toSpace(char* str, int num, FILE* f, bool (*NEndChar)(char)) {
	assert(str != NULL);
	assert(f != NULL);

	int i = 0;
	char curch = getc(f);
	while (NEndChar(curch) && i < num-1) {
		str[i] = curch;
		curch = getc(f);
		i++;
	}
	str[i] = '\0';

	return str;
}


/**
*	Ищет команду с соответствующим именем
*
*	@param[in] name Имя
*	@param[in] NCommands Количество команд в массиве команд
*
*	@return Индекс команды в массиве команд, если команда нашлась; -1 - если не нашлась
*/

int FindCommandS(char* name, const int NCommands) {
	assert(name != NULL);
	assert(NCommands > 0);

	for (int index = 0; index < NCommands; index++) {
		if (strcmp(commands[index].commandS, name) == 0) {
			return index;
		}
	}
	return -1;
}


/**
*	Компилирует программу, записанную в файле, создает исполняемый файл (mshn)
*
*	@param[in] fin Входной файл
*	@param[in] fout Выходной файл
*	@param[in] commandMaxLength Максимальная длина команды в строковом представлении
*	@param[in] NCommands Количество команд в языке My Assembler
*
*	@return 0 - компиляция прошла успешно; 1 - ошибка компиляции
*/

int CreateMshnFile(FILE* fin, FILE* fout, const int commandMaxLength, const int NCommands) {
	assert(fin != NULL);
	assert(fout != NULL);
	assert(commandMaxLength > 0);
	assert(NCommands > 0);

	int curParam = 0;
	int curCommandIndex = 0;
	char* curCommandS = (char*)calloc(commandMaxLength + 1, sizeof(char));

	do {
		Fgets_toSpace(curCommandS, commandMaxLength, fin, IsNEndChar);
		curCommandIndex = FindCommandS(curCommandS, NCommands);
		if (curCommandIndex == -1) {
			return 1;
		}

		if (commands[curCommandIndex].hasParam) {
			fscanf(fin, "%d\n", &curParam);
			fprintf(fout, "%d %d\n", commands[curCommandIndex].commandI, curParam);
		}
		else {
			fscanf(fin, "\n");
			fprintf(fout, "%d\n", commands[curCommandIndex].commandI);
		}
	} while (strcmp(curCommandS, "end") != 0);
	free(curCommandS);

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
	printf("Введите имя файла с исходным кодом: ");
	scanf("%200s", inoutFName);

	if (inoutFName[0] == '-') {
		*fin = fopen(defaultInFName, "r");
		*fout = fopen(defaultOutFName, "w");
		return 0;
	}

	*fin = fopen(inoutFName, "r");
	while (*fin == NULL) {
		printf("Файл не найден.\n");
		printf("Введите имя файла с исходным кодом: ");
		scanf("%200s", inoutFName);
		*fin = fopen(inoutFName, "r");
	}

	printf("Введите имя исполняемого файла: ");
	scanf("%200s", inoutFName);
	*fout = fopen(inoutFName, "w");
	while (*fin == NULL) {
		printf("Файл не найден.\n");
		printf("Введите имя исполняемого файла: ");
		scanf("%200s", inoutFName);
		*fout = fopen(inoutFName, "w");
	}
	return 0;
}

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "Russian");

	int err = 0;
	const int commandMaxLength = commMaxLength;
	const int NCommands = NComm;

	FILE* fin = {};
	FILE* fout = {};
	if (argc == 1) {
		const char defaultInFName[] = "input.asm";
		const char defaultOutFName[] = "output.mshn";

		GetFilesFromConsole(&fin, &fout, defaultInFName, defaultOutFName);
	}
	else if (argc==3) {
		fin = fopen(argv[1], "r");
		fout = fopen(argv[2], "w");
	}
	else {
		return 2;
	}

	if (CreateMshnFile(fin, fout, commandMaxLength, NCommands) == 1) {
		printf("Ошибка компиляции\n");
		err = 1;
	}

	fclose(fin);
	fclose(fout);

	return err;
}