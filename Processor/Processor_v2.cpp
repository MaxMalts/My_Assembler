#include <stdio.h>
#include <assert.h>
#include <locale.h>
#include "stack.h"

int RunProgram(FILE** fin, FILE** fout) {
	assert(fin != NULL);
	assert(fout != NULL);

	stack_t stk = StackConstructor("stk");

	int commI = 0;
	int param = 0;
	do {
		elem_t elem1 = {}, elem2 = {};
		elem_t pop = {};
		fscanf(*fin, "%d", &commI);
		switch (commI) {
		case 1:
			fscanf(*fin, "%d", &param);
			StackPush(&stk, param);
			break;
		case 2:
			StackPop(&stk, &pop);
			fprintf(*fout, "%d\n", pop);
			break;
		case 3:
			StackPop(&stk, &elem2);
			StackPop(&stk, &elem1);

			StackPush(&stk, elem1 + elem2);
			break;
		case 4:
			StackPop(&stk, &elem2);
			StackPop(&stk, &elem1);

			StackPush(&stk, elem1 - elem2);
			break;
		case 5:
			StackPop(&stk, &elem2);
			StackPop(&stk, &elem1);

			StackPush(&stk, elem1 * elem2);
			break;
		case 6:
			StackPop(&stk, &elem2);
			StackPop(&stk, &elem1);

			StackPush(&stk, elem1 / elem2);
			break;
		case 7:
			break;
		}
	} while (commI != 7);

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
	scanf("%200s%*c", inoutFName);

	if (inoutFName[0] == '-') {
		*fin = fopen(defaultInFName, "r");
		*fout = fopen(defaultOutFName, "w");
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
		fin = fopen(argv[1], "r");
		fout = fopen(argv[2], "w");
	}
	else {
		return 2;
	}

	RunProgram(&fin, &fout);

	fclose(fin);
	fclose(fout);

	return 0;
}