#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <locale.h>
#include <string.h>


///**
//*	Проверяет символ на признак конца строки (EOF, '\n', ' ', '\0')
//*
//*	@param[in] ch Символ
//*
//*	@return true - символ конечный; false - не конечный.
//*/
//
//bool IsNEndChar(char ch) {
//	if ((ch != EOF) && (ch != '\n') && (ch != ' ') && (ch != '\0')) {
//		return true;
//	}
//	return false;
//}
//
//
///**
//*	Считывает строку из файла
//*
//*	@param[in] str Строка, в которую надо запсать
//*	@param[in] num Максимальное количество символов с учетом '\0'
//*	@param[in] f Файл
//*	@param[in] NEndChar Функция, проверяющая символ на признак конца строки
//*
//*	@return Указатель на полученную строку (совпадает с str)
//*/
//
//char* Fgets_toSpace(char* str, int num, FILE* f, bool (*NEndChar)(char), char* endCh=NULL) {
//	assert(str != NULL);
//	assert(f != NULL);
//
//	int i = 0;
//	char curch = getc(f);
//	while (NEndChar(curch) && i < num-1) {
//		str[i] = curch;
//		curch = getc(f);
//		i++;
//	}
//	str[i] = '\0';
//
//	if (endCh != NULL) {
//		*endCh = curch;
//	}
//
//	return str;
//}


/**
*	Ищет команду с соответствующим именем в CommandsDictionary.h.
 Если в нем несколько одноименных команд, возвращает числовое представление первой.
*
*	@param[in] commName Строковое представление команды
*
*	@return Числовое представление команды; -1 - если команда не нашлась
*/

char FindCommandS(char* commName) {
	assert(commName != NULL);

#define DEF_CMD(name, num)\
	if (strncmp(commName, #name, sizeof(#name) - 1) == 0) {\
		return num;\
	}

#include "..\CommandsDictionary.h"
#undef DEF_CMD

	return -1;
}


/**
*	Определяет числовое представление регистра
*
*	@param[in] regS Регистр в строковом представлении
*
*	@return Регистр в числовом представлении; -1 - неверные входные данные
*/

int DetReg(char* regS) {
	assert(regS != 0);

	int res = 0;
	if (regS[1] == 'x') {
		switch (regS[0]) {
		case 'a': return 1; break;
		case 'b': return 2; break;
		case 'c': return 3; break;
		case 'd': return 4; break;
		default: return -1; break;
		}
	}
	
	return -1;
}

/**
*	Определяет тип команды push и записывает ее числовое представление и ее параметр в числовом виде
*
*	@param[in] commTNum Временное числовое представление команды
*	@param[out] commNum Команда в числовом представлении
*	@param[in] paramS Параметр в строковом представлении, начиная с пробела
*	@param[out] paramI Параметр в числовом представлении
*	@param[out] NParams Количество параметров (всегда 1)
*
*	@return 1 - Неверный формат параметра; 0 - все нормально
*/

int DetPush(const char commTNum, char* commNum, char* paramS, int* paramI, int* NParams) {
	assert(commNum != NULL);
	assert(paramS != NULL);
	assert(paramI != NULL);
	assert(NParams != NULL);

	if (paramS[2] == 'x') {
		*paramI = DetReg(&paramS[1]);
		if (*paramI == -1) {
			return 1;
		}
		*commNum = 100 + commTNum * 10 + 1;
	}
	else {
		*paramI = atoi(&paramS[1]);
		*commNum = commTNum;
	}
	*NParams = 1;

	return 0;
}


/**
*	Определяет тип команды pop и записывает ее числовое представление и ее параметр (если есть) в числовом виде
*
*	@param[in] commTNum Временное числовое представление команды
*	@param[out] commNum Команда в числовом представлении
*	@param[in] paramS Параметр в строковом представлении, начиная с пробела
*	@param[out] paramI Параметр в числовом представлении
*	@param[out] NParams Количество параметров (0 или 1)
*
*	@return 1 - Неверный формат параметра; 0 - все нормально
*/

int DetPop(const char commTNum, char* commNum, char* paramS, int* paramI, int* NParams) {
	assert(commNum != NULL);
	assert(paramS != NULL);
	assert(paramI != NULL);
	assert(NParams != NULL);

	if (paramS[0] == '\0') {
		*commNum = commTNum;
		*NParams = 0;
	}
	else {
		if (paramS[2] == 'x') {
			*paramI = DetReg(&paramS[1]);
			if (*paramI == -1) {
				return 1;
			}
			*commNum = 100 + commTNum * 10 + 1;
			*NParams = 1;
		}
		else return 1;
	}

	return 0;
}

//int WritePush(FILE* fin, FILE* fout, char commandI) {
//	assert(fin != NULL);
//	assert(fout != NULL);
//	assert(commandI > 0);
//
//	char paramS[15] = "";
//	int paramI = 0;
//
//	fscanf(fin, "%s\n", paramS);
//
//	if (DetPush(&commandI, paramS, &paramI) != 0) {
//		return 1;
//	}
//
//	fprintf(fout, "%c", commandI);
//	fwrite(&paramI, sizeof(int), 1, fout);
//
//	return 0;
//}
//
//int WritePop(FILE* fin, FILE* fout, char commandI, char nextCh) {
//	assert(fin != NULL);
//	assert(fout != NULL);
//	assert(commandI > 0);
//
//	char paramS[15] = "";
//	int paramI = 0;
//
//	if (nextCh == ' ') {
//		fscanf(fin, "%s\n", paramS);
//		paramI = DetReg(paramS);
//		if (paramI == -1) {
//			return 1;
//		}
//
//		commandI = 100 + commandI * 10 + 1;
//		fprintf(fout, "%c", commandI);
//		fwrite(&paramI, sizeof(int), 1, fout);
//	}
//	else if (nextCh == '\n') {
//		fprintf(fout, "%c", commandI);
//	}
//	else return 1;
//
//	return 0;
//}


/**
*	Создает итоговую строку путем объединения номера команды и параметров
*
*	@param[out] stringI Итоговая строка
*	@param[in] commNum Числовое представление команды
*	@param[in] paramsI Массив параметров
*	@param[in] NParams Количество парметров
*
*	@return 0 - все прошло нормально
*/

int MakeStr(char* stringI, const char commNum, int* paramsI, const int NParams) {
	assert(stringI != NULL);
	assert(commNum > 0);
	assert(paramsI != NULL);

	sprintf(stringI, "%c", commNum);
	for (int i = 0; i < NParams; i++) {
		char curBytes[4] = { *((char*)& paramsI[i]), *((char*)& paramsI[i] + 1), *((char*)& paramsI[i] + 2), *((char*)& paramsI[i] + 3) };
		sprintf(&stringI[1 + i * sizeof(int)], "%s", curBytes);
	}

	return 0;
}


/**
*	Конвертирует строку исходного кода в строку исполняемого кода
*
*	@param[in] stringS Строка исзодного кода
*	@param[out] stringI Строка исполняемого кода
*	@param[out] strLength Длина строки исполняемого кода
*
*	@return 1 - Ошибка синтаксиса; 0 - все прошло нормально
*/

int ConvertString(char* stringS, char* stringI, int* strLength) {
	assert(stringS != NULL);
	assert(stringI != NULL);
	assert(strLength != NULL);

	char commTNum = FindCommandS(stringS);
	if (commTNum == -1) {
		return 1;
	}
	
	const char push[] = "push";
	const char pop[] = "pop";

	char commNum = 0;
	int paramsI[100] = {};
	int NParams = 0;
	if (strncmp(stringS, push, sizeof(push) - 1) == 0) {

		if (DetPush(commTNum, &commNum, &stringS[sizeof(push)-1], &paramsI[0], &NParams) != 0) {
			return 1;
		}
	}
	else if (strncmp(stringS, pop, sizeof(pop) - 1) == 0) {

		if (DetPop(commTNum, &commNum, &stringS[sizeof(pop) - 1], &paramsI[0], &NParams) != 0) {
			return 1;
		}
	}
	else {
		commNum = commTNum;
		NParams = 0;
	}

	MakeStr(stringI, commNum, paramsI, NParams);
	*strLength = 1 + NParams * sizeof(int);

	return 0;
}

/**
*	Компилирует программу, записанную в файле, создает исполняемый файл (mshn)
*
*	@param[in] fin Входной файл
*	@param[in] fout Выходной файл
*	@param[in] stringMaxLength Максимальная длина строки в исходном и исполняемом файлах
*
*	@return 1 - ошибка компиляции; 0 - компиляция прошла успешно;
*/

int CreateMshnFile(FILE* fin, FILE* fout, const int stringMaxLength) {
	assert(fin != NULL);
	assert(fout != NULL);
	assert(stringMaxLength > 0);

	char* curStringI = (char*)calloc(stringMaxLength + 1, sizeof(char));
	char* curStringS = (char*)calloc(stringMaxLength + 1, sizeof(char));

	do {

		char nextCh = 0;
		fscanf(fin, "%[^\n]s", curStringS);
		assert(curStringS[stringMaxLength] == '\0');
		fscanf(fin, "%*c");

		int curStringILength = 0;
		if (ConvertString(curStringS, curStringI, &curStringILength) != 0) {
			return 1;
		}
		assert(curStringILength <= stringMaxLength);

		fwrite(curStringI, sizeof(char), curStringILength, fout);

	} while (strcmp(curStringS, "end") != 0);
	free(curStringS);
	free(curStringI);

	return 0;
}


/**
*	Осуществляет взаимодействие с ползователем, пока он не введет названия входного и выходного файлов. Если
Вводится символ '-', то открываются вайлы по умолчанию.
*
*	@param[in] fin Входной файл
*	@param[in] fout Выходной файл, бинарный режим
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
		*fin = fopen(defaultInFName, "r");
		*fout = fopen(defaultOutFName, "wb");
		return 0;
	}

	*fin = fopen(inoutFName, "r");
	while (*fin == NULL) {
		printf("Файл не найден.\n");
		printf("Введите имя файла с исходным кодом: ");
		scanf("%200s%*c", inoutFName);
		*fin = fopen(inoutFName, "r");
	}

	printf("Введите имя исполняемого файла: ");
	scanf("%200s%*c", inoutFName);
	*fout = fopen(inoutFName, "wb");
	while (*fin == NULL) {
		printf("Файл не найден.\n");
		printf("Введите имя исполняемого файла: ");
		scanf("%200s%*с", inoutFName);
		*fout = fopen(inoutFName, "wb");
	}
	return 0;
}

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "Russian");

	int err = 0;
	const int stringMaxLength = 100;

	FILE* fin = {};
	FILE* fout = {};
	if (argc == 1) {
		const char defaultInFName[] = "input.asm";
		const char defaultOutFName[] = "output.mshn";

		GetFilesFromConsole(&fin, &fout, defaultInFName, defaultOutFName);
	}
	else if (argc==3) {
		fin = fopen(argv[1], "r");
		fout = fopen(argv[2], "wb");
	}
	else {
		return 2;
	}

	if (CreateMshnFile(fin, fout, stringMaxLength) == 1) {
		printf("Ошибка компиляции\n");
		err = 1;
	}

	fclose(fin);
	fclose(fout);

	return err;
}