#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <locale.h>
#include <string.h>
#include "..\CommandsDictionary.h"


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
*	@param[in] name ���
*	@param[in] NCommands ���������� ������ � ������� ������
*
*	@return ������ ������� � ������� ������, ���� ������� �������; -1 - ���� �� �������
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
*	����������� ���������, ���������� � �����, ������� ����������� ���� (mshn)
*
*	@param[in] fin ������� ����
*	@param[in] fout �������� ����
*	@param[in] commandMaxLength ������������ ����� ������� � ��������� �������������
*	@param[in] NCommands ���������� ������ � ����� My Assembler
*
*	@return 0 - ���������� ������ �������; 1 - ������ ����������
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
*	������������ �������������� � ������������, ���� �� �� ������ �������� �������� � ��������� ������. ����
�������� ������ '-', �� ����������� ����� �� ���������.
*
*	@param[in] fin ������� ����
*	@param[in] fout �������� ����
*	@param[in] defaultInFName ��� �������� ����� �� ���������
*	@param[in] defaultOutFName ��� ��������� ����� �� ���������
*
*	@return 0
*/

int GetFilesFromConsole(FILE** fin, FILE** fout, const char* defaultInFName, const char* defaultOutFName) {
	char inoutFName[200] = {};
	printf("������� ��� ����� � �������� �����: ");
	scanf("%200s", inoutFName);

	if (inoutFName[0] == '-') {
		*fin = fopen(defaultInFName, "r");
		*fout = fopen(defaultOutFName, "w");
		return 0;
	}

	*fin = fopen(inoutFName, "r");
	while (*fin == NULL) {
		printf("���� �� ������.\n");
		printf("������� ��� ����� � �������� �����: ");
		scanf("%200s", inoutFName);
		*fin = fopen(inoutFName, "r");
	}

	printf("������� ��� ������������ �����: ");
	scanf("%200s", inoutFName);
	*fout = fopen(inoutFName, "w");
	while (*fin == NULL) {
		printf("���� �� ������.\n");
		printf("������� ��� ������������ �����: ");
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
		printf("������ ����������\n");
		err = 1;
	}

	fclose(fin);
	fclose(fout);

	return err;
}