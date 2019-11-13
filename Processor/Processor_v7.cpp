#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <locale.h>
#include "stack.h"

struct buf_t {
	char* buffer = 0;
	int binSize = 0;
};


/**
*	Определяет размер текстового файла с учетом символа '\r'
*
*	@param[in] f Файл
*
*	@return Размер
*/

int GetFileSize(FILE* file) {
	assert(file != NULL);
	int fseekRet = fseek(file, 0, SEEK_END);
	assert(fseekRet == 0);

	int fileSize = ftell(file);
	assert(fileSize != -1);

	fseekRet = fseek(file, 0, SEEK_SET);
	assert(fseekRet == 0);
	return fileSize;
}

/**
*	Создает структуру с информацией о входном файле
*
*	@param[in] infile Файл
*
*	@return Структура
*/

struct buf_t GetFileData(FILE* file) {
	assert(file != NULL);

	int binSize = GetFileSize(file);
	char* buf = (char*)calloc(binSize + sizeof(int) + 1, sizeof(char));
	int freadout = fread(buf, sizeof(char), binSize, file);
	assert(freadout == binSize);

	struct buf_t out;
	out.buffer = buf;
	out.binSize = binSize;

	return out;
}


/**
*	Выполняет программу
*
*	@param[in] buf Буффер с кодом программы
*	@param[in] fout Выходной файл
*
*	@return 1 - ошибка при выполнении; 0 - выполнено успешно
*/

int RunProgram(char* buf, FILE* fout) {
	assert(buf != NULL);
	assert(fout != NULL);

	stack_t stk = StackConstructor("stk");
	int reg[5] = {};
	stack_t retAdresses = StackConstructor("retAdresses");

#define DEF_CMD(name, num, type, code)\
	case num: code; break;

	char curCommandI = 0;
	int curParam = 0;
	int cursor = 0;
	int ended = 0;
	do {

#define PROCESSOR
		curCommandI = buf[cursor];
		switch (curCommandI) {
#include "..\CommandsDictionary.h"
		default: return 1;
		}

	} while (!ended);

#undef DEF_CMD

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
	printf("Введите имя файла с исходным кодом: ");
	scanf("%200s%*c", inoutFName);

	if (inoutFName[0] == '-') {
		*fin = fopen(defaultInFName, "rb");
		*fout = fopen(defaultOutFName, "w");
		return 0;
	}

	*fin = fopen(inoutFName, "rb");
	while (*fin == NULL) {
		printf("Файл не найден.\n");
		printf("Введите имя файла с исходным кодом: ");
		scanf("%200s%*c", inoutFName);
		*fin = fopen(inoutFName, "rb");
	}

	printf("Введите имя исполняемого файла: ");
	scanf("%200s%*c", inoutFName);
	*fout = fopen(inoutFName, "w");
	while (*fin == NULL) {
		printf("Файл не найден.\n");
		printf("Введите имя исполняемого файла: ");
		scanf("%200s%*с", inoutFName);
		*fout = fopen(inoutFName, "w");
	}
	return 0;
}

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "Russian");

	FILE* fin = {};
	FILE* fout = {};
	if (argc == 1) {
		const char defaultInFName[] = "input.mshn";
		const char defaultOutFName[] = "output.txt";

		GetFilesFromConsole(&fin, &fout, defaultInFName, defaultOutFName);
	}
	else if (argc == 3) {
		fin = fopen(argv[1], "rb");
		fout = fopen(argv[2], "w");
	}
	else {
		return 2;
	}

	struct buf_t buf = GetFileData(fin);

	int err = RunProgram(buf.buffer, fout);

	fclose(fin);
	fclose(fout);
	getchar();
	return err;
}