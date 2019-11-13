#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <locale.h>
#include <string.h>
#include "..\Constants.h"

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
*	Определяет символьное представление регистра
*
*	@param[in] regI Регистр в числовом представлении
*
*	@return Регистр в символьном представлении; -1 - неверные входные данные
*/

char DetRegRev(const int regI) {

	int res = 0;
	switch (regI) {
	case 1: return 'a'; break;
	case 2: return 'b'; break;
	case 3: return 'c'; break;
	case 4: return 'd'; break;
	default: return -1; break;
	}

	return -1;
}

///**
//*	Ищет команду с соответствующим числовым значением
//*
//*	@param[in] commNum Числовое представление команды
//*	@param[out] commName Строковое представление команды
//*
//*	@return 0 - команда нашлась; -1 - не нашлась
//*/
//
//int FindCommandI(const char commNum, char* commName) {
//
//#define DEF_CMD(name, num)\
//	if (commNum==num) {\
//		strcpy(commName, #name);\
//		return 0;\
//	}
//
//#include "..\CommandsDictionary.h"
//#undef DEF_CMD
//
//	return -1;
//}

int CreateAsmFile(char* buf, FILE* fout) {
	assert(buf != NULL);
	assert(fout != NULL);

	

#define DEF_CMD(name, num, type, codeProc, codeDisass)\
	case num: {\
	char commName[]=#name;\
	codeDisass;\
	break;\
	}

	char curCommandI = 0;
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

///**
//*	Декомпилирует программу, записанную в файле, создает файл с исходным кодом (asm)
//*
//*	@param[in] fin Входной файл
//*	@param[in] fout Выходной файл
//*	@param[in] NCommands Количество команд в языке My Assembler
//*
//*	@return 0 - компиляция прошла успешно; 1 - ошибка компиляции
//*/
//
//int CreateAsmFile(FILE* fin, FILE* fout, const int commandMaxLength) {
//	assert(fin != NULL);
//	assert(fout != NULL);
//	assert(commandMaxLength > 0);
//
//	int curParam = 0;
//	char curCommandI = 0;
//	char* curCommandS = (char*)calloc(commandMaxLength + 1, sizeof(char));
//
//	do {
//		fread(&curCommandI, sizeof(char), 1, fin);
//		if (FindCommandI(curCommandI, curCommandS) == -1) {
//			return 1;
//		}
//
//		if (curCommandI == 1) {
//			fread(&curParam, sizeof(int), 1, fin);
//			fprintf(fout, "%s %d\n", curCommandS, curParam);
//
//		}
//		else if (curCommandI == 111) {
//			fread(&curParam, sizeof(int), 1, fin);
//			char curReg = DetRegRev(curParam);;
//			if (curReg == -1) {
//				return 1;
//			}
//
//			fprintf(fout, "%s %cx\n", curCommandS, curReg);
//		}
//		else if (curCommandI == 121) {
//			fread(&curParam, sizeof(int), 1, fin);
//			char curReg = DetRegRev(curParam);;
//			if (curReg == -1) {
//				return 1;
//			}
//
//			fprintf(fout, "%s %cx\n", curCommandS, curReg);
//		}
//		else {
//			fprintf(fout, "%s\n", curCommandS);
//		}
//	} while (strcmp(curCommandS, "end") != 0);
//	free(curCommandS);
//
//	return 0;
//}


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
	setlocale(LC_CTYPE, "Russian");

	int err = 0;
	const int stringMaxLength = strMaxLength;

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

	struct buf_t buf = GetFileData(fin);
	fclose(fin);

	if (CreateAsmFile(buf.buffer, fout) == 1) {
		printf("Ошибка декомпиляции");
		err = 1;
	}

	fclose(fin);
	fclose(fout);

	return err;
}