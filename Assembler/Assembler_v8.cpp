#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <locale.h>
#include <string.h>
#include <ctype.h>
#include "..\Constants.h"


/**
*	Проверяет символ на признак конца строки (EOF, '\n', ' ', '\0')
*
*	@param[in] ch Символ
*
*	@return true - символ конечный; false - не конечный.
*/

bool IsEndChar(char ch) {
	if ((ch == EOF) || (ch == '\n') || (ch == ' ') || (ch == '\0')) {
		return true;
	}
	return false;
}


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

const char push[] = "push";
const char pop[] = "pop";
const char labelCh = ':';

/**
*	Ищет команду с соответствующим именем в CommandsDictionary.h.
 Если в нем несколько одноименных команд, возвращает числовое представление первой.
*
*	@param[in] commName Строковое представление команды, возможно, с параметрами
*	@param[out] number Числовое представление команды (первой)
*	@param[out] commType Тип команды
*
*	@return -1 - если команда не нашлась; 0 - все прошло нормально
*/

int FindCommandS(char* commName, int* number, int* commType) {
	assert(commName != NULL);
	assert(number != NULL);
	assert(commType != NULL);

#define DEF_CMD(name, num, type)\
	if (strncmp(commName, #name, sizeof(#name) - 1) == 0 && IsEndChar(commName[sizeof(#name)-1])) {\
		*number=num;\
		*commType=type;\
		return 0;\
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

int CombineStr(char* stringI, const char commNum, int* paramsI, const int NParams) {
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

int ConvertString(char* stringS, char* stringI, int* strLength, int commTNum, int commType) {
	assert(stringS != NULL);
	assert(stringI != NULL);
	assert(strLength != NULL);

	/*int commTNum = 0;
	if (FindCommandS(stringS, &commTNum, commType) == 1) {
		return 1;
	}*/
	
	char commNum = 0;
	int paramsI[100] = {};
	int NParams = 0;
	switch (commType) {
	case PUSH_TYPE:
		if (DetPush(commTNum, &commNum, &stringS[sizeof(push) - 1], &paramsI[0], &NParams) != 0) {
			return 1;
		}
		break;
	case POP_TYPE:
		if (DetPop(commTNum, &commNum, &stringS[sizeof(pop) - 1], &paramsI[0], &NParams) != 0) {
			return 1;
		}
		break;
	case JUMPER_TYPE:
		commNum = commTNum;
		paramsI[0] = 0;
		NParams = 1;
		break;
	case SIMPLE_TYPE:
		commNum = commTNum;
		NParams = 0;
		break;
	default:
		assert(0);
	}

	CombineStr(stringI, commNum, paramsI, NParams);
	*strLength = 1 + NParams * sizeof(int);

	return 0;
}

struct label_t {
	int adress = 0;
	char name[100] = "";
};

struct jumper_t {
	int adress = 0;
	char name[100] = "";
};

//int CreateLabels(FILE* fin, label_t* labels, const int stringMaxLength) {
//	assert(fin != NULL);
//	assert(labels != NULL);
//	assert(stringMaxLength > 0);
//
//	char* curString = (char*)calloc(stringMaxLength + 1, sizeof(char));
//	int labelsCount = 0;
//	int curAdress = 0;
//	int curLength = 0;
//
//	do {
//
//		fscanf(fin, "%[^\n]s", curString);
//		assert(curString[stringMaxLength] == '\0');
//		fscanf(fin, "%*c");
//
//		if (FindCommandS(curString, &curLength) == -1) {
//			return 1;
//		}
//		if (curString[0] == ':') {
//			strcpy(labels[labelsCount].name, &curString[1]);
//		}
//
//	} while (strcmp(curString, "end") != 0);
//}


/**
*	Добавляет метку в массив меток
*
*	@param[in] name Имя метки
*	@param labels Массив меток
*	@param[in] adress Адрес метки
*	@param labelCount Количество меток
*
*	@return 1 - имя имеет неверный фармат; 0 - все прошло нормально
*/

int AddLabel(char* name, label_t* labels, const int adress, int* labelsCount) {
	assert(name != NULL);
	assert(labels != NULL);
	assert(labelsCount != NULL);

	if (!isalpha(name[0])) {
		return 1;
	}
	
	strcpy(labels[*labelsCount].name, name);
	labels[*labelsCount].adress = adress;
	(*labelsCount)++;

	return 0;
}


/**
*	Добавляет джампер в массив джамперов
*
*	@param[in] name Имя метки
*	@param jumpers Массив джамперов
*	@param[in] adress Адрес джампера
*	@param jumpersCount Количество джамперов
*
*	@return 1 - имя имеет неверный фармат; 0 - все прошло нормально
*/

int AddJumper(char* name, jumper_t* jumpers, const int adress, int* jumpersCount) {
	assert(name != NULL);
	assert(jumpers != NULL);
	assert(jumpersCount != NULL);
	assert(name[0] == ' ');

	if (!isalpha(name[1])) {
		return 1;
	}

	strcpy(jumpers[*jumpersCount].name, &name[1]);
	jumpers[*jumpersCount].adress = adress;
	(*jumpersCount)++;

	return 0;
}


/**
*	Вставляет адреса меток в джамперы
*
*	@param[in] fout Выходной файл с исполняемым кодом
*	@param[in] labels Массив меток
*	@param[in] jumpers Массив джамперов
*	@param[in] labelsCount Количество меток
*	@param[in] jumpersCount Количество джамперов
*
*	@return 1 - для одного из джамперов не нашлась метка; 0 - все прошло нормально
*/

int PasteLabels(FILE* fout, label_t* labels, jumper_t* jumpers, const int labelsCount, const int jumpersCount) {
	assert(fout != NULL);
	assert(labels != NULL);
	assert(jumpers != NULL);

	for (int curJumper = 0; curJumper < jumpersCount; curJumper++) {
		int found = 0;
		for (int curLabel = 0; curLabel < labelsCount; curLabel++) {
			if (strcmp(jumpers[curJumper].name, labels[curLabel].name) == 0) {
				fseek(fout, jumpers[curJumper].adress + 1, SEEK_SET);
				fwrite(&labels[curLabel].adress, sizeof(int), 1, fout);
				found = 1;
				break;
			}
		}
		if (!found) return 1;
	}

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

	label_t labels[100] = {};
	jumper_t jumpers[100] = {};
	int jumpersCount = 0;

	char* curStringI = (char*)calloc(stringMaxLength + 1, sizeof(char));
	char* curStringS = (char*)calloc(stringMaxLength + 1, sizeof(char));
	int curAdress = 0;
	int labelsCount = 0;

	do {

		fscanf(fin, "%[^\n]s", curStringS);
		assert(curStringS[stringMaxLength] == '\0');
		fscanf(fin, "%*c");

		int commTNum = 0;
		int commType = 0;
		if (FindCommandS(curStringS, &commTNum, &commType)) {
			return 1;
		}

		switch (commType) {
		case LABEL_TYPE: {
			if (AddLabel(&curStringS[1], labels, curAdress, &labelsCount) != 0) {
				return 1;
			}
			break;
		}
		case JUMPER_TYPE: {
			int curStringILength = 0;
			if (ConvertString(curStringS, curStringI, &curStringILength, commTNum, commType) != 0) {
				return 1;
			}
			assert(curStringILength <= stringMaxLength);

			if (AddJumper(strchr(curStringS, ' '), jumpers, curAdress, &jumpersCount) != 0) {
				return 1;
			}

			fwrite(curStringI, sizeof(char), curStringILength, fout);
			curAdress += curStringILength;
			break;
		}
		default: {
			int curStringILength = 0;
			if (ConvertString(curStringS, curStringI, &curStringILength, commTNum, commType) != 0) {
				return 1;
			}
			assert(curStringILength <= stringMaxLength);

			fwrite(curStringI, sizeof(char), curStringILength, fout);
			curAdress += curStringILength;
			break;
		}
		}


	} while (strcmp(curStringS, "end") != 0);
	free(curStringS);
	free(curStringI);

	if (PasteLabels(fout, labels, jumpers, labelsCount, jumpersCount) != 0) {
		return 1;
	}

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