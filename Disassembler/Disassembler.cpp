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
*	���������� ������ ���������� ����� � ������ ������� '\r'
*
*	@param[in] f ����
*
*	@return ������
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
*	������� ��������� � ����������� � ������� �����
*
*	@param[in] infile ����
*
*	@return ���������
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

///**
//*	���� ������� � ��������������� �������� ���������
//*
//*	@param[in] commNum �������� ������������� �������
//*	@param[out] commName ��������� ������������� �������
//*
//*	@return 0 - ������� �������; -1 - �� �������
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

struct label_t {
	int adress = 0;
	char name[100] = "";
};

struct func_t {
	int adress = 0;
	char name[100] = "";
};


/**
*	��������� ����� � ������ �����
*
*	@param labels ������ �����
*	@param[in] labelAdress �����, ���� ����� �������� �����
*	@param[in] labelsCount ���������� ����� �� ����������
*
*	@return 0 - ��� ������ ���������
*/

int AddLabel(label_t* labels, const int labelAdress, const int labelsCount) {
	assert(labels != NULL);

	char name[15] = {};
	sprintf(name, "%s%d", "Label", labelsCount + 1);
	strcpy(labels[labelsCount].name, name);
	labels[labelsCount].adress = labelAdress;

	return 0;
}


/**
*	��������� ������� � ������ �������
*
*	@param funcs ������ �����
*	@param[in] funcAdress �����, ���� ����� �������� �������
*	@param[in] funcsCount ���������� ������� �� ����������
*
*	@return 0 - ��� ������ ���������
*/

int AddFunc(func_t* funcs, const int funcAdress, const int funcsCount) {
	assert(funcs != NULL);

	char name[15] = {};
	sprintf(name, "%s%d", "Function", funcsCount + 1);
	strcpy(funcs[funcsCount].name, name);
	funcs[funcsCount].adress = funcAdress;

	return 0;
}


/**
*	���� ����� � �������
*
*	@param[in] buf ����� � ����������� �����
*	@param labels ������ �����
*	@param funcs ������ �������
*	@param[out] labelsCount ���������� �����
*	@param[out] funcsCount ���������� �������
*
*	@return 1 - ������ � ����������� ���� (������� �� �������); 0 - ��� ������ ���������
*/

int FindLabelsAndFuncs(char* buf, label_t* labels, func_t* funcs, int* labelsCount, int* funcsCount) {
	assert(buf != NULL);
	assert(labels != NULL);
	assert(funcs != NULL);
	assert(labelsCount != NULL);
	assert(funcsCount != NULL);

#define DEF_CMD(name, num, type, codeProc, codeDisass, len)\
	case num:\
	curCommLen = len;\
	curCommType = type;\
	break;

	int cursor = 0;
	int curCommLen = 0;
	int curCommandI = 0;
	int curCommType = 0;
	int labelsC = 0;
	int funcsC = 0;
	do {

#define DONT_INCLUDE
		curCommandI = buf[cursor];
		switch (curCommandI) {
#include "..\CommandsDictionary.h"
		default: return 1;
		}

		switch (curCommType) {
		case JUMPER_TYPE:
			AddLabel(labels, *((int*)(buf + cursor + 1)), labelsC);
			labelsC++;
			break;
		case CALL_TYPE:
			AddFunc(funcs, *((int*)(buf + cursor + 1)), funcsC);
			funcsC++;
			break;
		}

		cursor += curCommLen;
	} while (curCommType != END_TYPE);

#undef DEF_CMD
#undef DONT_INCLUDE

	*labelsCount = labelsC;
	*funcsCount = funcsC;

	return 0;
}


/**
*	���� ����� �� ��������� ������
*
*	@param[in] labels ������ �����
*	@param[in] cuAdr �����
*	@param[in] labelsCount ���������� �����
*	@param[out] index ������ ��������� ����� � ������� �����
*
*	@return 1 - ����� �� �������; 0 - ����� �������, ��� ������ ���������
*/

int FindLabelOnAdr(label_t* labels, const int curAdr, const int labelsCount, int* index) {
	assert(labels != NULL);
	assert(index != NULL);

	for (int i = 0; i < labelsCount; i++) {
		if (labels[i].adress == curAdr) {
			*index = i;
			return 0;
		}
	}

	return 1;
}


/**
*	���� ������� �� ��������� ������
*
*	@param[in] funcs ������ �������
*	@param[in] cuAdr �����
*	@param[in] funcsCount ���������� �������
*	@param[out] index ������ ��������� ������� � ������� �������
*
*	@return 1 - ������� �� �������; 0 - ������� �������, ��� ������ ���������
*/

int FindFuncOnAdr(func_t* funcs, const int curAdr, const int funcsCount, int* index) {
	assert(funcs != NULL);
	assert(index != NULL);

	for (int i = 0; i < funcsCount; i++) {
		if (funcs[i].adress == curAdr) {
			*index = i;
			return 0;
		}
	}

	return 1;
}


/**
*	������������� ����������� ��� � ��������
*
*	@param[in] buf ����� � ����������� �����
*	@param[out] ����, � ������� ��������� �������� ���
*
*	@return 1 - ������ ������������ (������� �� �������); 0 - ��� ������ ���������
*/

int CreateAsmFile(char* buf, FILE* fout) {
	assert(buf != NULL);
	assert(fout != NULL);

	label_t labels[100] = {};
	func_t funcs[100] = {};
	int labelsCount = 0;
	int funcsCount = 0;
	FindLabelsAndFuncs(buf, labels, funcs, &labelsCount, &funcsCount);

#define DEF_CMD(Name, num, type, codeProc, codeDisass)\
	case num: {\
	char commName[] = #Name;\
	\
	int ind = 0;\
	if (FindLabelOnAdr(labels, cursor, labelsCount, &ind) == 0){\
		fprintf(fout, ": %s\n", labels[ind].name);\
	}\
	else if (FindFuncOnAdr(funcs, cursor, funcsCount, &ind) == 0){\
		fprintf(fout, "func %s\n", funcs[ind].name);\
	}\
	\
	codeDisass;\
	break;\
	}

	char curCommandI = 0;
	int cursor = 0;
	int ended = 0;
	do {

#define DONT_INCLUDE
		curCommandI = buf[cursor];
		switch (curCommandI) {
#include "..\CommandsDictionary.h"
		default: return 1;
		}
		
	} while (!ended);

#undef DEF_CMD
#undef DONT_INCLUDE

	return 0;
}

///**
//*	������������� ���������, ���������� � �����, ������� ���� � �������� ����� (asm)
//*
//*	@param[in] fin ������� ����
//*	@param[in] fout �������� ����
//*	@param[in] NCommands ���������� ������ � ����� My Assembler
//*
//*	@return 0 - ���������� ������ �������; 1 - ������ ����������
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
		printf("������ ������������");
		err = 1;
	}

	fclose(fout);

	return err;
}