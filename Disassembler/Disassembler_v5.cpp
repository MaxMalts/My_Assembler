#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <locale.h>
#include <string.h>


/**
*	���������� ���������� ������������� ��������
*
*	@param[in] regI ������� � �������� �������������
*
*	@return ������� � ���������� �������������; -1 - �������� ������� ������
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

/**
*	���� ������� � ��������������� �������� ���������
*
*	@param[in] commNum �������� ������������� �������
*	@param[out] commName ��������� ������������� �������
*
*	@return 0 - ������� �������; -1 - �� �������
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
*	������������� ���������, ���������� � �����, ������� ���� � �������� ����� (asm)
*
*	@param[in] fin ������� ����
*	@param[in] fout �������� ����
*	@param[in] NCommands ���������� ������ � ����� My Assembler
*
*	@return 0 - ���������� ������ �������; 1 - ������ ����������
*/

int CreateAsmFile(FILE* fin, FILE* fout, const int commandMaxLength) {
	assert(fin != NULL);
	assert(fout != NULL);
	assert(commandMaxLength > 0);

	int curParam = 0;
	char curCommandI = 0;
	char* curCommandS = (char*)calloc(commandMaxLength + 1, sizeof(char));

	do {
		fread(&curCommandI, sizeof(char), 1, fin);
		if (FindCommandI(curCommandI, curCommandS) == -1) {
			return 1;
		}

		if (curCommandI == 1) {
			fread(&curParam, sizeof(int), 1, fin);
			fprintf(fout, "%s %d\n", curCommandS, curParam);

		}
		else if (curCommandI == 111) {
			fread(&curParam, sizeof(int), 1, fin);
			char curReg = DetRegRev(curParam);;
			if (curReg == -1) {
				return 1;
			}

			fprintf(fout, "%s %cx\n", curCommandS, curReg);
		}
		else if (curCommandI == 121) {
			fread(&curParam, sizeof(int), 1, fin);
			char curReg = DetRegRev(curParam);;
			if (curReg == -1) {
				return 1;
			}

			fprintf(fout, "%s %cx\n", curCommandS, curReg);
		}
		else {
			fprintf(fout, "%s\n", curCommandS);
		}
	} while (strcmp(curCommandS, "end") != 0);
	free(curCommandS);

	return 0;
}


/**
*	������������ �������������� � ������������, ���� �� �� ������ �������� �������� � ��������� ������. ����
�������� ������ '-', �� ����������� ����� �� ���������.
*
*	@param[in] fin ������� ����, �������� �����
*	@param[in] fout �������� ����
*	@param[in] defaultInFName ��� �������� ����� �� ���������
*	@param[in] defaultOutFName ��� ��������� ����� �� ���������
*
*	@return 0
*/

int GetFilesFromConsole(FILE** fin, FILE** fout, const char* defaultInFName, const char* defaultOutFName) {
	char inoutFName[200] = {};
	printf("������� ��� ������������ �����: ");
	scanf("%200s%*c", inoutFName);

	if (inoutFName[0] == '-') {
		*fin = fopen(defaultInFName, "rb");
		*fout = fopen(defaultOutFName, "w");
		return 0;
	}

	*fin = fopen(inoutFName, "rb");
	while (*fin == NULL) {
		printf("���� �� ������.\n");
		printf("������� ��� ������������ �����: ");
		scanf("%200s%*c", inoutFName);
		*fin = fopen(inoutFName, "rb");
	}

	printf("������� ��� ����� � �������� �����: ");
	scanf("%200s%*c", inoutFName);
	*fout = fopen(inoutFName, "w");
	while (*fin == NULL) {
		printf("���� �� ������.\n");
		printf("������� ��� ����� � �������� �����: ");
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
		printf("������ ������������");
	}

	fclose(fin);
	fclose(fout);
}