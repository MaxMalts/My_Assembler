#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>


/**
*	������� ��� ��������� ������� � �������
*/

void Help() {
	printf(\
	"\n�������� ��������� ������:\n\n\
	ass - ������������� ��������\n\n\
	run - ��������� ���������\n\n\
	ass_and_run - ������������� � ��������� ���������\n\n\
	disass - ��������������� ���������\n\n\
	exit - ����� �� ���������\n\n\n"\
	);
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

int GetFNameFromConsole(char* FName, const char* defaultInFName, const char* outputText, const char* mode) {
	FILE* testFile = {};

	printf(outputText);
	scanf("%200s%*c", FName);

	if (FName[0] == '-') {
		strcpy(FName, defaultInFName);
		return 0;
	}

	testFile = fopen(FName, mode);
	while (testFile == NULL) {
		printf("���� �� ������.\n");
		printf(outputText);
		scanf("%200s%*c", FName);
		testFile = fopen(FName, mode);
	}
	fclose(testFile);

	return 0;
}


/**
*	������ ��������� Assembler, ��������� � ��� ����� �������� � ��������� ������
*
*	@return ��� ������, ������������ Assembler-��
*/

int Assemble() {
	int err = 0;
	char finName[200] = {};
	char foutName[200] = {};
	GetFNameFromConsole(finName, "Default_Files\\input.asm", "������� ��� �������� �����: ", "r");
	GetFNameFromConsole(foutName, "Default_Files\\output.mshn", "������� ��� ��������� �����: ", "w");

	char sysCommand[420] = {};
	sprintf(sysCommand, "%s %s %s", "Assembler.exe", finName, foutName);
	err = system(sysCommand);

	return err;
}


/**
*	������ ��������� Processor, ��������� � ��� ����� �������� � ��������� ������
*
*	@return ��� ������, ������������ Processor-��
*/

int Run() {
	int err = 0;
	char finName[200] = {};
	char foutName[200] = {};
	GetFNameFromConsole(finName, "Default_Files\\input.mshn", "������� ��� �������� �����: ", "r");
	GetFNameFromConsole(foutName, "Default_Files\\output.txt", "������� ��� ��������� �����: ", "w");

	char sysCommand[420] = {};
	sprintf(sysCommand, "%s %s %s", "Processor.exe", finName, foutName);
	err = system(sysCommand);

	return err;
}


/**
*	������ ��������� Assembler, ��������� � ��� ����� �������� � ��������� ������
*
*	@param[out] ��� ������, ������������ Assembler-��
*	@param[out] ��� ������, ������������ Processor-��
*
*	@return 0 - ���� ������ ����� 0; 1 - ���� �� ���� �� ����� ������ �� ����� 0
*/

int AssembleAndRun(int* assErr, int* procErr) {
	char finName[200] = {};
	char fcompiledName[200] = {};
	char foutName[200] = {};
	GetFNameFromConsole(finName, "Default_Files\\input.asm", "������� ��� �������� �����: ", "r");
	GetFNameFromConsole(fcompiledName, "Default_Files\\compiled.mshn", "������� ��� ������������ �����: ", "w");
	GetFNameFromConsole(foutName, "Default_Files\\output.txt", "������� ��� ��������� �����: ", "w");

	char sysCommand[420] = {};
	sprintf(sysCommand, "%s %s %s", "Assembler.exe", finName, fcompiledName);
	*assErr = system(sysCommand);

	sprintf(sysCommand, "%s %s %s", "Processor.exe", fcompiledName, foutName);
	*procErr = system(sysCommand);

	if (*assErr != 0 || *procErr != 0) {
		return 1;
	}
	return 0;
}


/**
*	�������� Disassembler, ��������� � ���� ����� �������� � ��������� ������
*
*	@return ��� ������, ������������ Disassembler-��
*/

int Disassemble() {
	int err = 0;
	char finName[200] = {};
	char foutName[200] = {};
	GetFNameFromConsole(finName, "Default_Files\\input.mshn", "������� ��� �������� �����: ", "r");
	GetFNameFromConsole(foutName, "Default_Files\\output.asm", "������� ��� ��������� �����: ", "w");

	char sysCommand[420] = {};
	sprintf(sysCommand, "%s %s %s", "Disassembler.exe", finName, foutName);
	err = system(sysCommand);

	return err;
}

int main() {

	setlocale(LC_ALL, "Russian");

	char command[200] = {};
	int exited = 0;
	while (!exited) {
		printf("������� ����� ���������. ��� ������ ������� \"help\": ");
		scanf("%200s%*c", command);

			if (strcmp(command, "help") == 0) {
				Help();
			}
			else if (strcmp(command, "ass") == 0) {
				int err = Assemble();
				if (err == 0) {
					printf("��������� �������\n\n");
				}
				else {
					printf("������ ��� ���������� (%d)\n\n", err);
				}
			}
			else if (strcmp(command, "run") == 0) {
				int err = Run();
				if (err == 0) {
					printf("��������� �������\n\n");
				}
				else {
					printf("������ ��� ���������� (%d)\n\n", err);
				}
			}
			else if (strcmp(command, "ass_and_run") == 0) {
				int assErr = 0, procErr = 0;
				AssembleAndRun(&assErr, &procErr);
				if (assErr == 0 && procErr == 0) {
					printf("��������� �������\n\n");
				}
				else {
					printf("������ ��� ���������� (%d, %d)\n\n", assErr, procErr);
				}
			}
			else if (strcmp(command, "disass") == 0) {
				int err = Disassemble();
				if (err == 0) {
					printf("��������� �������\n\n");
				}
				else {
					printf("������ ��� ���������� (%d)\n\n", err);
				}
			}
			else if (strcmp(command, "exit") == 0) {
				exited = 1;
			}
			else {
				printf("��� ����� �������\n");
			}
	}

	return 0;
}