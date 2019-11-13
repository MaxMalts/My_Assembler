#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <locale.h>
#include <string.h>


///**
//*	��������� ������ �� ������� ����� ������ (EOF, '\n', ' ', '\0')
//*
//*	@param[in] ch ������
//*
//*	@return true - ������ ��������; false - �� ��������.
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
//*	��������� ������ �� �����
//*
//*	@param[in] str ������, � ������� ���� �������
//*	@param[in] num ������������ ���������� �������� � ������ '\0'
//*	@param[in] f ����
//*	@param[in] NEndChar �������, ����������� ������ �� ������� ����� ������
//*
//*	@return ��������� �� ���������� ������ (��������� � str)
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
*	���� ������� � ��������������� ������ � CommandsDictionary.h.
 ���� � ��� ��������� ����������� ������, ���������� �������� ������������� ������.
*
*	@param[in] commName ��������� ������������� �������
*
*	@return �������� ������������� �������; -1 - ���� ������� �� �������
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
*	���������� �������� ������������� ��������
*
*	@param[in] regS ������� � ��������� �������������
*
*	@return ������� � �������� �������������; -1 - �������� ������� ������
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
*	���������� ��� ������� push � ���������� �� �������� ������������� � �� �������� � �������� ����
*
*	@param[in] commTNum ��������� �������� ������������� �������
*	@param[out] commNum ������� � �������� �������������
*	@param[in] paramS �������� � ��������� �������������, ������� � �������
*	@param[out] paramI �������� � �������� �������������
*	@param[out] NParams ���������� ���������� (������ 1)
*
*	@return 1 - �������� ������ ���������; 0 - ��� ���������
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
*	���������� ��� ������� pop � ���������� �� �������� ������������� � �� �������� (���� ����) � �������� ����
*
*	@param[in] commTNum ��������� �������� ������������� �������
*	@param[out] commNum ������� � �������� �������������
*	@param[in] paramS �������� � ��������� �������������, ������� � �������
*	@param[out] paramI �������� � �������� �������������
*	@param[out] NParams ���������� ���������� (0 ��� 1)
*
*	@return 1 - �������� ������ ���������; 0 - ��� ���������
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
*	������� �������� ������ ����� ����������� ������ ������� � ����������
*
*	@param[out] stringI �������� ������
*	@param[in] commNum �������� ������������� �������
*	@param[in] paramsI ������ ����������
*	@param[in] NParams ���������� ���������
*
*	@return 0 - ��� ������ ���������
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
*	������������ ������ ��������� ���� � ������ ������������ ����
*
*	@param[in] stringS ������ ��������� ����
*	@param[out] stringI ������ ������������ ����
*	@param[out] strLength ����� ������ ������������ ����
*
*	@return 1 - ������ ����������; 0 - ��� ������ ���������
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
*	����������� ���������, ���������� � �����, ������� ����������� ���� (mshn)
*
*	@param[in] fin ������� ����
*	@param[in] fout �������� ����
*	@param[in] stringMaxLength ������������ ����� ������ � �������� � ����������� ������
*
*	@return 1 - ������ ����������; 0 - ���������� ������ �������;
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
*	������������ �������������� � ������������, ���� �� �� ������ �������� �������� � ��������� ������. ����
�������� ������ '-', �� ����������� ����� �� ���������.
*
*	@param[in] fin ������� ����
*	@param[in] fout �������� ����, �������� �����
*	@param[in] defaultInFName ��� �������� ����� �� ���������
*	@param[in] defaultOutFName ��� ��������� ����� �� ���������
*
*	@return 0
*/

int GetFilesFromConsole(FILE** fin, FILE** fout, const char* defaultInFName, const char* defaultOutFName) {
	char inoutFName[200] = {};
	printf("������� ��� ����� � �������� �����: ");
	scanf("%200s%*c", inoutFName);

	if (inoutFName[0] == '-') {
		*fin = fopen(defaultInFName, "r");
		*fout = fopen(defaultOutFName, "wb");
		return 0;
	}

	*fin = fopen(inoutFName, "r");
	while (*fin == NULL) {
		printf("���� �� ������.\n");
		printf("������� ��� ����� � �������� �����: ");
		scanf("%200s%*c", inoutFName);
		*fin = fopen(inoutFName, "r");
	}

	printf("������� ��� ������������ �����: ");
	scanf("%200s%*c", inoutFName);
	*fout = fopen(inoutFName, "wb");
	while (*fin == NULL) {
		printf("���� �� ������.\n");
		printf("������� ��� ������������ �����: ");
		scanf("%200s%*�", inoutFName);
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
		printf("������ ����������\n");
		err = 1;
	}

	fclose(fin);
	fclose(fout);

	return err;
}