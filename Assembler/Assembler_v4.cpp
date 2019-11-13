#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <locale.h>
#include <string.h>


/**
*	��������� ������ �� ������� ����� ������ (EOF, '\n', ' ', '\0')
*
*	@param[in] ch ������
*
*	@return true - ������ ��������; false - �� ��������.
*/

bool IsNEndChar(char ch) {
	if ((ch != EOF) && (ch != '\n') && (ch != ' ') && (ch != '\0')) {
		return true;
	}
	return false;
}


/**
*	��������� ������ �� �����
*
*	@param[in] str ������, � ������� ���� �������
*	@param[in] num ������������ ���������� �������� � ������ '\0'
*	@param[in] f ����
*	@param[in] NEndChar �������, ����������� ������ �� ������� ����� ������
*
*	@return ��������� �� ���������� ������ (��������� � str)
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
*	���� ������� � ��������������� ������
*
*	@param[in] commName ��������� ������������� �������
*
*	@return �������� ������������� �������; -1 - ���� ������� �� �������
*/

char FindCommandS(char* commName) {
	assert(commName != NULL);

#define DEF_CMD(name, num)\
	if (strcmp(commName, #name)==0) {\
		return num;\
	}

#include "..\CommandsDictionary.h"
#undef DEF_CMD

	return -1;
}


/**
*	����������� ���������, ���������� � �����, ������� ����������� ���� (mshn)
*
*	@param[in] fin ������� ����
*	@param[in] fout �������� ����
*	@param[in] commandMaxLength ������������ ����� ������� � ��������� �������������
*
*	@return 0 - ���������� ������ �������; 1 - ������ ����������
*/

int CreateMshnFile(FILE* fin, FILE* fout, const int commandMaxLength) {
	assert(fin != NULL);
	assert(fout != NULL);
	assert(commandMaxLength > 0);

	int curParam = 0;
	char curCommandI = 0;
	char* curCommandS = (char*)calloc(commandMaxLength + 1, sizeof(char));

	do {
		Fgets_toSpace(curCommandS, commandMaxLength, fin, IsNEndChar);
		curCommandI = FindCommandS(curCommandS);

		if (curCommandI == -1) {
			return 1;
		}

		if (strcmp(curCommandS, "push") == 0) {
			fscanf(fin, "%d\n", &curParam);
			fprintf(fout, "%c", curCommandI);
			fwrite(&curParam, sizeof(int), 1, fout);
		}
		else {
			fscanf(fin, "\n");
			fprintf(fout, "%c", curCommandI);
		}
	} while (strcmp(curCommandS, "end") != 0);
	free(curCommandS);

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
	const int commandMaxLength = 100;

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

	if (CreateMshnFile(fin, fout, commandMaxLength) == 1) {
		printf("������ ����������\n");
		err = 1;
	}

	fclose(fin);
	fclose(fout);

	return err;
}