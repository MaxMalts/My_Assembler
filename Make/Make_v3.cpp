#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>


/**
*	Выводит все доступные команды в консоль
*/

void Help() {
	printf(\
	"\nДоступны следующие режимы:\n\n\
	ass - компилировать исходник\n\n\
	run - запустить программу\n\n\
	ass_and_run - компилировать и запустить программу\n\n\
	disass - декомпилировать программу\n\n\
	exit - выйти из программы\n\n\n"\
	);
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
		printf("Файл не найден.\n");
		printf(outputText);
		scanf("%200s%*c", FName);
		testFile = fopen(FName, mode);
	}
	fclose(testFile);

	return 0;
}


/**
*	Вывает программу Assembler, передавая в нее имена входного и выходного файлов
*
*	@return Код ошибки, возвращенной Assembler-ом
*/

int Assemble() {
	int err = 0;
	char finName[200] = {};
	char foutName[200] = {};
	GetFNameFromConsole(finName, "Default_Files\\input.asm", "Введите имя входного файла: ", "r");
	GetFNameFromConsole(foutName, "Default_Files\\output.mshn", "Введите имя выходного файла: ", "w");

	char sysCommand[420] = {};
	sprintf(sysCommand, "%s %s %s", "Assembler.exe", finName, foutName);
	err = system(sysCommand);

	return err;
}


/**
*	Вывает программу Processor, передавая в нее имена входного и выходного файлов
*
*	@return Код ошибки, возвращенной Processor-ом
*/

int Run() {
	int err = 0;
	char finName[200] = {};
	char foutName[200] = {};
	GetFNameFromConsole(finName, "Default_Files\\input.mshn", "Введите имя входного файла: ", "r");
	GetFNameFromConsole(foutName, "Default_Files\\output.txt", "Введите имя выходного файла: ", "w");

	char sysCommand[420] = {};
	sprintf(sysCommand, "%s %s %s", "Processor.exe", finName, foutName);
	err = system(sysCommand);

	return err;
}


/**
*	Вывает программу Assembler, передавая в нее имена входного и выходного файлов
*
*	@param[out] Код ошибки, возвращенной Assembler-ом
*	@param[out] Код ошибки, возвращенной Processor-ом
*
*	@return 0 - коды ошибок равны 0; 1 - хотя бы один из кодов ошибок не равен 0
*/

int AssembleAndRun(int* assErr, int* procErr) {
	char finName[200] = {};
	char fcompiledName[200] = {};
	char foutName[200] = {};
	GetFNameFromConsole(finName, "Default_Files\\input.asm", "Введите имя входного файла: ", "r");
	GetFNameFromConsole(fcompiledName, "Default_Files\\compiled.mshn", "Введите имя исполняемого файла: ", "w");
	GetFNameFromConsole(foutName, "Default_Files\\output.txt", "Введите имя выходного файла: ", "w");

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
*	Вызывает Disassembler, передавая в него имена входного и выходного файлов
*
*	@return Код ошибки, возвращенной Disassembler-ом
*/

int Disassemble() {
	int err = 0;
	char finName[200] = {};
	char foutName[200] = {};
	GetFNameFromConsole(finName, "Default_Files\\input.mshn", "Введите имя входного файла: ", "r");
	GetFNameFromConsole(foutName, "Default_Files\\output.asm", "Введите имя выходного файла: ", "w");

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
		printf("Введите режим программы. Для помощи введите \"help\": ");
		scanf("%200s%*c", command);

			if (strcmp(command, "help") == 0) {
				Help();
			}
			else if (strcmp(command, "ass") == 0) {
				int err = Assemble();
				if (err == 0) {
					printf("Выполнено успешно\n\n");
				}
				else {
					printf("Ошибка при выполнении (%d)\n\n", err);
				}
			}
			else if (strcmp(command, "run") == 0) {
				int err = Run();
				if (err == 0) {
					printf("Выполнено успешно\n\n");
				}
				else {
					printf("Ошибка при выполнении (%d)\n\n", err);
				}
			}
			else if (strcmp(command, "ass_and_run") == 0) {
				int assErr = 0, procErr = 0;
				AssembleAndRun(&assErr, &procErr);
				if (assErr == 0 && procErr == 0) {
					printf("Выполнено успешно\n\n");
				}
				else {
					printf("Ошибка при выполнении (%d, %d)\n\n", assErr, procErr);
				}
			}
			else if (strcmp(command, "disass") == 0) {
				int err = Disassemble();
				if (err == 0) {
					printf("Выполнено успешно\n\n");
				}
				else {
					printf("Ошибка при выполнении (%d)\n\n", err);
				}
			}
			else if (strcmp(command, "exit") == 0) {
				exited = 1;
			}
			else {
				printf("Нет такой команды\n");
			}
	}

	return 0;
}