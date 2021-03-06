#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <locale.h>
#include <string.h>
#include <ctype.h>
#include "..\Constants.h"


/**
*	��������� ������ �� ������� ����� ������ (EOF, '\n', ' ', '\0')
*
*	@param[in] ch ������
*
*	@return true - ������ ��������; false - �� ��������.
*/

bool IsEndChar(char ch) {
	if ((ch == EOF) || (ch == '\n') || (ch == ' ') || (ch == '\0')) {
		return true;
	}
	return false;
}


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

//const char push[] = "push";
//const char pop[] = "pop";
//const char labelCh = ':';

/**
*	���� ������� � ��������������� ������ � CommandsDictionary.h.
 ���� � ��� ��������� ����������� ������, ���������� �������� ������������� ������.
*
*	@param[in] commName ��������� ������������� �������, ��������, � �����������
*	@param[out] number �������� ������������� ������� (������)
*	@param[out] commType ��� �������
*
*	@return -1 - ���� ������� �� �������; 0 - ��� ������ ���������
*/

int FindCommandS(char* stringS, int* number, int* commType, int* nameLen) {
	assert(stringS != NULL);
	assert(number != NULL);
	assert(commType != NULL);

#define DEF_CMD(name, num, type)\
	if (strncmp(stringS, #name, sizeof(#name) - 1) == 0 && IsEndChar(stringS[sizeof(#name)-1])) {\
		*number = num;\
		*commType = type;\
		*nameLen = sizeof(#name) - 1;\
		return 0;\
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

char DetReg(char* regS) {
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
*	@param[out] paramILen ����� ��������� � ������
*
*	@return 1 - �������� ������ ���������; 0 - ��� ���������
*/

int DetPush(const char commTNum, char* commNum, char* paramS, char* paramI, int* paramILen) {
	assert(commNum != NULL);
	assert(paramS != NULL);
	assert(paramS[0] == ' ');
	assert(paramI != NULL);
	assert(paramILen != NULL);

	if (paramS[2] == 'x') {
		*paramI = DetReg(&paramS[1]);
		if (*paramI == -1) {
			return 1;
		}
		*paramILen = 1;
		*commNum = 100 + commTNum * 10 + 1;
	}
	else if (paramS[1] == '[') {
		if (paramS[3] == 'x') {
			*paramI = DetReg(&paramS[2]);
			if (*paramI == -1) {
				return 1;
			}
			*paramILen = 1;
			*commNum = 100 + commTNum * 10 + 3;
		}
		else if (isdigit(paramS[2])) {
			*((int*)paramI) = atoi(&paramS[2]);
			*paramILen = sizeof(int);
			*commNum = 100 + commTNum * 10 + 2;
		}
		else return 1;
	}
	else if (isdigit(paramS[1]) || (paramS[1] == '-' && isdigit(paramS[2]))) {
		*((float*)paramI) = atof(&paramS[1]);
		*commNum = commTNum;
		*paramILen = sizeof(float);
	}
	else return 1;

	return 0;
}


/**
*	���������� ��� ������� pop � ���������� �� �������� ������������� � �� �������� (���� ����) � �������� ����
*
*	@param[in] commTNum ��������� �������� ������������� �������
*	@param[out] commNum ������� � �������� �������������
*	@param[in] paramS �������� � ��������� �������������, ������� � �������
*	@param[out] paramI �������� � �������� �������������
*	@param[out] paramILen ����� ��������� � ������
*
*	@return 1 - �������� ������ ���������; 0 - ��� ���������
*/

int DetPop(const char commTNum, char* commNum, char* paramS, char* paramI, int* paramILen) {
	assert(commNum != NULL);
	assert(paramS != NULL);
	assert(paramI != NULL);
	assert(paramILen != NULL);

	if (paramS[0] == '\0') {
		*commNum = commTNum;
		*paramILen = 0;
	}
	else {
		if (paramS[2] == 'x') {
			*paramI = DetReg(&paramS[1]);
			if (*paramI == -1) {
				return 1;
			}
			*commNum = 100 + commTNum * 10 + 1;
			*paramILen = 1;
		}
		else if (paramS[1] == '[') {
			if (paramS[3] == 'x') {
				*paramI = DetReg(&paramS[2]);
				if (*paramI == -1 || paramS[4] != ']') {
					return 1;
				}
				*commNum = 100 + commTNum * 10 + 3;
				*paramILen = 1;
			}
			else if (isdigit(paramS[2])) {
				*((int*)paramI) = atoi(&paramS[2]);
				*paramILen = sizeof(int);
				*commNum = 100 + commTNum * 10 + 2;
			}
			else return 1;
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
*	@param[in] commI �������� ������������� �������
*	@param[in] paramsI ������ ����������
*	@param[in] paramsILen ������ ���������� � ������
*
*	@return 0 - ��� ������ ���������
*/

int CombineStr(char* stringI, const char commI, char* paramsI, const int paramsILen) {
	assert(stringI != NULL);
	assert(commI > 0);
	assert(paramsI != NULL);

	sprintf(stringI, "%c", commI);

	for (int i = 0; i < paramsILen; i++) {
		stringI[1 + i] = paramsI[i];
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

int ConvertString(char* stringS, char* stringI, int* strLength, int commTNum, int commType, const int commNameLen) {
	assert(stringS != NULL);
	assert(stringI != NULL);
	assert(strLength != NULL);
	
	char commNum = 0;
	char paramsI[4 * 10] = {};
	int paramsILen = 0;
	switch (commType) {
	case PUSH_TYPE:
		if (DetPush(commTNum, &commNum, &stringS[commNameLen], &paramsI[0], &paramsILen) != 0) {
			return 1;
		}
		break;
	case POP_TYPE:
		if (DetPop(commTNum, &commNum, &stringS[commNameLen], &paramsI[0], &paramsILen) != 0) {
			return 1;
		}
		break;
	case JUMPER_TYPE:
	case CALL_TYPE:
		commNum = commTNum;
		*((int*) paramsI) = 0;
		paramsILen = sizeof(int);
		break;
	case SIMPLE_TYPE:
	case END_TYPE:
		commNum = commTNum;
		paramsILen = 0;
		break;
	default:
		assert(0);
	}

	CombineStr(stringI, commNum, paramsI, paramsILen);
	*strLength = 1 + paramsILen;

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
*	��������� ����� � ������ �����
*
*	@param[in] name ��� �����, ������� � �������
*	@param labels ������ �����
*	@param[in] adress ����� �����
*	@param labelCount ���������� �����
*
*	@return 1 - ��� ����� �������� ������; 0 - ��� ������ ���������
*/

int AddLabel(char* name, label_t* labels, const int adress, int* labelsCount) {
	assert(name != NULL);
	assert(labels != NULL);
	assert(labelsCount != NULL);
	assert(name[0] == ' ');

	if (!isalpha(name[1])) {
		return 1;
	}
	
	strcpy(labels[*labelsCount].name, &name[1]);
	labels[*labelsCount].adress = adress;
	(*labelsCount)++;

	return 0;
}


/**
*	��������� ������� � ������ ���������
*
*	@param[in] name ��� �����
*	@param jumpers ������ ���������
*	@param[in] adress ����� ��������
*	@param jumpersCount ���������� ���������
*
*	@return 1 - ��� ����� �������� ������; 0 - ��� ������ ���������
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
*	��������� ������ ����� � ��������
*
*	@param[in] fout �������� ���� � ����������� �����
*	@param[in] labels ������ �����
*	@param[in] jumpers ������ ���������
*	@param[in] labelsCount ���������� �����
*	@param[in] jumpersCount ���������� ���������
*
*	@return 1 - ��� ������ �� ��������� �� ������� �����; 0 - ��� ������ ���������
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
		if (!found)
			return 1;
	}

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

	label_t labels[100] = {};
	jumper_t jumpers[100] = {};
	int jumpersCount = 0;

	char* curStringI = (char*)calloc(stringMaxLength + 1, sizeof(char));
	char* curStringS = (char*)calloc(stringMaxLength + 1, sizeof(char));
	int curAdress = 0;
	int labelsCount = 0;
	int commType = 0;

	do {

		int scanRet = 0;
		do {
			scanRet = fscanf(fin, "%[^\n]s", curStringS);
			fscanf(fin, "%*c");
		} while (scanRet == 0);
		assert(curStringS[stringMaxLength] == '\0');

		int commTNum = 0;
		int commNameLen = 0;
		if (FindCommandS(curStringS, &commTNum, &commType, &commNameLen)) {
			return 1;
		}

		switch (commType) {
		case LABEL_TYPE:
		case FUNCTION_TYPE: {
			if (AddLabel(&curStringS[commNameLen], labels, curAdress, &labelsCount) != 0) {
				return 1;
			}
			break;
		}

		case JUMPER_TYPE:
		case CALL_TYPE: {
			int curStringILength = 0;
			if (ConvertString(curStringS, curStringI, &curStringILength, commTNum, commType, commNameLen) != 0) {
				return 1;
			}
			assert(curStringILength <= stringMaxLength);

			if (AddJumper(&curStringS[commNameLen], jumpers, curAdress, &jumpersCount) != 0) {
				return 1;
			}

			fwrite(curStringI, sizeof(char), curStringILength, fout);
			curAdress += curStringILength;
			break;
		}
		
		default: {
			int curStringILength = 0;
			if (ConvertString(curStringS, curStringI, &curStringILength, commTNum, commType, commNameLen) != 0) {
				return 1;
			}
			assert(curStringILength <= stringMaxLength);

			fwrite(curStringI, sizeof(char), curStringILength, fout);
			curAdress += curStringILength;
			break;
		}
		}

	} while (commType != END_TYPE);
	free(curStringS);
	free(curStringI);

	if (PasteLabels(fout, labels, jumpers, labelsCount, jumpersCount) != 0) {
		return 1;
	}

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
	setlocale(LC_CTYPE, "Russian");

	int err = 0;
	const int stringMaxLength = strMaxLength;

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