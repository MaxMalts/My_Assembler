// ������:
// v1 - ���� �������������� �������
// v2 - ������������ ����

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "stack.h"

#ifdef _DEBUG
#define PrintStk_OK(stk) StackDump(&stk, __FILE__, __LINE__, __FUNCTION__, "just looking");
#define PrintStk_NOK(stk) StackDump(&stk, __FILE__, __LINE__, __FUNCTION__, "stack has an error");
#else
#define PrintStk_OK(stk) ;
#define PrintStk_NOK(stk) ;
#endif


unsigned int CalcHash(stack_t*);

/**
*	����������� elem_t � ������
*
*	@param[in] elem �������
*
*	@return ��������� �� ������
*/

char* elem_tToStr(elem_t elem) {

	const int elem_tMaxStrSize = 20;

	char* str = (char*)calloc(elem_tMaxStrSize, sizeof(char));
	itoa(elem, str, 10);
	return str;
}


/**
*	������� ���������� � �����
*
*	@param[in] stk ����
*	@param[in] file �������� �����, ����� ������� �������
*	@param[in] line ����� ������, �� ������� ������� �������
*	@param[in] function ��� �������, �� ������� ������� �������
*	@param[in] reason �������, �� ������� ������� �������
*/

#ifdef _DEBUG
void StackDump(stack_t* stk, const char* file, const int line, const char* function, const char* reason) {
	char status[10] = "ok";
	if (stk->err != 0) {
		strcpy(status, "ERR");
	}

	printf("\nInfo about a stack from file: %s, function: %s, line: %d, reason: %s:\n", file, function, line, reason);
	printf("stack_t \"%s\" (%p):    (%s)\n", stk->name, stk, status);
	printf("\tsecureVarBeg = %d\n", stk->secureVarBeg);
	printf("\tsize = %d\n", stk->size);
	printf("\tdelta = %d\n", stk->delta);

	printf("\tdata[%d] (%p):\n", stk->curMaxSize, &stk->data);
	for (int i = 0; i < stk->curMaxSize; i++) {
		char* elemStr = elem_tToStr(stk->data[i]);
		if (stk->data[i] == stk->emptyelem) {
			printf("\t\t[%d] = %s (poison),\n", i, elemStr);
		}
		else {
			printf("\t\t[%d] = %s,\n", i, elemStr);
		}
		free(elemStr);
	}

	printf("\thash = %u\n", stk->hash);
	printf("\tsecureVarEnd = %d\n", stk->secureVarEnd);
	printf("err = %d\n\n\n", stk->err);
}
#endif

/**
*	��������� ���� � ���������� � ���� ��� ������
*
*	@param[in] stk ����
*
*	@return fasle - ���� ������������; true - ���� ����������
*/

#ifdef _DEBUG
bool StkOk(stack_t* stk) {
	if (stk->size > stk->curMaxSize) {
		stk->err = 1;
		return false;
	}
	if (stk->size < 0) {
		stk->err = 2;
		return false;
	}
	if (stk->secureVarBeg != 0) {
		stk->err = 3;
		return false;
	}
	if (stk->secureVarEnd != 0) {
		stk->err = 4;
		return false;
	}
	if (stk->hash != CalcHash(stk)) {
		stk->err = 5;
		return false;
	}
	stk->err = 0;
	return true;
}
#endif


/**
*	��������� ���-����� �����
*
*	@param[in] stk ����
*
*	@return �������� ���-�����
*/

#ifdef _DEBUG
unsigned int CalcHash(stack_t* stk) {
	assert(stk != NULL);

	unsigned int hash = 0;
	for (int i = 0; i < stk->curMaxSize; i++) {
		for (int j = 0; j < sizeof(elem_t); j++) {
			char curByte = (char)(*((char*)& stk->data[i] + j));
			hash = hash + curByte * 2;
		}
	}
	for (int j = 0; j < sizeof(int); j++) {
		char curByte = (char)(*((char*)& stk->delta + j));
		hash = hash + curByte * 2;
	}
	for (int j = 0; j < sizeof(int); j++) {
		char curByte = (char)(*((char*)& stk->curMaxSize + j));
		hash = hash + curByte * 2;
	}
	for (int j = 0; j < sizeof(int); j++) {
		char curByte = (char)(*((char*)& stk->size + j));
		hash = hash + curByte * 2;
	}
	for (int j = 0; j < sizeof(elem_t); j++) {
		char curByte = (char)(*((char*)& stk->emptyelem + j));
		hash = hash + curByte * 2;
	}
#ifdef _DEBUG
	for (int i = 0; i < 30; i++) {
		char curByte = stk->name[i];
		hash = hash + curByte * 2;
	}
	for (int j = 0; j < sizeof(int); j++) {
		char curByte = (char)(*((char*)& stk->err + j));
		hash = hash + curByte * 2;
	}
#endif

	return hash;
}
#endif


/**
*	������������� ���-����� �����
*
*	@param[in] stk ����
*
*	@return 1 - ������ � ����� ����� �������������� ����; 0 - ��� ������ ���������
*/

#ifdef _DEBUG
int RecalcHash(stack_t* stk) {
	assert(stk != NULL);

	stk->hash = CalcHash(stk);

#ifdef _DEBUG
	if (!StkOk(stk)) {
		PrintStk_NOK(*stk);
		return 2;
	}
#endif

	return 0;
}
#endif

/**
*	������� ����� ����, �������� ��� "�������" ����������
*
*	@param[in] name ��� �����
*
*	@return ��������� �� ��������� ����
*/

stack_t StackConstructor(const char* name) {
	stack_t stk = {};
#ifdef _DEBUG
	strcpy(stk.name, name);
	stk.err = 0;
#endif

	elem_t emptyelem = stk.emptyelem;
	stk.data = (elem_t*)calloc(10, sizeof(elem_t));
	for (int i = 0; i < 10; i++) {
		stk.data[i] = emptyelem;
	}
	stk.size = 0;
	stk.curMaxSize = 10;

#ifdef _DEBUG
	RecalcHash(&stk);

	if (StkOk(&stk)) {
		PrintStk_OK(stk);
	}
	else {
		PrintStk_NOK(stk);
	}
#endif
	return stk;
}


/**
*	����������� ����
*
*	@param[in] stk ����
*	@param[in] newSize ����� ������ ����� (� ���������)
*	@param[in] elemSize ������ ������ �������� (� ������)
*
*	@return 1 - �������� �� ������ ����� ����������; 2 - �� ������� �������������� ����; 0 - ��� ������ ���������
*/

int IncreaseStack(stack_t* stk, const int newSize, const size_t elemSize) {
	assert(stk!=NULL);

#ifdef _DEBUG
	StackDump(stk, __FILE__, __LINE__, __FUNCTION__, "Before increasing stack");
#endif

	stk->data = (elem_t*)realloc(stk->data, newSize*elemSize);
	if (stk->data == NULL) {
		return 2;
	}

	elem_t stkemptyelem = stk->emptyelem;
	for (int i = stk->curMaxSize; i < newSize; i++) {
		stk->data[i] = stkemptyelem;
	}
	stk->curMaxSize = newSize;

#ifdef _DEBUG
	StackDump(stk, __FILE__, __LINE__, __FUNCTION__, "After increasing stack");
#endif

#ifdef _DEBUG
	RecalcHash(stk);

	if (StkOk(stk)) {
		PrintStk_OK(*stk);
	}
	else {
		PrintStk_NOK(*stk);
		return 1;
	}
#endif
	return 0;
}

/**
*	����� ������� �� �����
*
*	@param[in] stk ����
*	@param[in] value �������� ������ ��������
*
*	@return 0 - ��� ������ ���������; 2 - �� ������� ��������� ������ �����; 1 - �������� �� ������
*/

int StackPush(stack_t* stk, elem_t value) {
	assert(stk != NULL);
#ifdef _DEBUG
	if (!StkOk(stk)) {
		PrintStk_NOK(*stk);
		return 1;
	}
#endif

	if (stk->size == stk->curMaxSize) {
		if (IncreaseStack(stk, stk->curMaxSize * 2, sizeof(elem_t)) != 0) {
			return 2;
		}
	}
	stk->data[stk->size++] = value;

#ifdef _DEBUG
	RecalcHash(stk);

	if (StkOk(stk)) {
		PrintStk_OK(*stk);
	}
	else {
		PrintStk_NOK(*stk);
		return 1;
	}
#endif
	return 0;
}


/**
*	��������� ����
*
*	@param[in] stk ����
*	@param[in] newSize ����� ������ ����� (� ���������)
*	@param[in] elemSize ������ ������ �������� (� ������)
*
*	@return 1 - �������� �� ������ ����� ����������; 2 - �� ������� �������������� ����; 0 - ��� ������ ���������
*/

int DecreaseStack(stack_t* stk, const int newSize, const size_t elemSize) {
	assert(stk != NULL);

#ifdef _DEBUG
	StackDump(stk, __FILE__, __LINE__, __FUNCTION__, "Before decreasing stack");
#endif

	stk->data = (elem_t*)realloc(stk->data, newSize*elemSize);
	if (stk->data == NULL) {
		return 2;
	}
	stk->curMaxSize = newSize;

#ifdef _DEBUG
	StackDump(stk, __FILE__, __LINE__, __FUNCTION__, "After decreasing stack");
#endif

#ifdef _DEBUG
	RecalcHash(stk);

	if (StkOk(stk)) {
		PrintStk_OK(*stk);
	}
	else {
		PrintStk_NOK(*stk);
		return 1;
	}
#endif
	return 0;
}

/**
*	����� ������� �� �����
*
*	@param[in] stk ����
*	@param[in] variable ���������� ��� �������� ��������. ���� ���� ������, �������� ����������
*
*	@return 3 - ���� ��������� ������� ��������� ����; 2 - � ����� ��� ���������; 
1 - �������� �� ������; 0 - ��� ������ ���������
*/

int StackPop(stack_t* stk, elem_t* variable) {
	assert(stk != NULL);
#ifdef _DEBUG
	if (StkOk(stk)) {
		PrintStk_OK(*stk);
	}
	else {
		PrintStk_NOK(*stk);
		return 1;
	}
#endif

	int err = 0;
	if (stk->size > 0) {
		*variable = stk->data[--stk->size];
		stk->data[stk->size] = stk->emptyelem;

		if (stk->size < (stk->curMaxSize / 2) - stk->delta) {
			assert(stk->data[stk->size] == stk->emptyelem);
			if (DecreaseStack(stk, stk->size + 1, sizeof(elem_t)) != 0) {
				err = 3;
			}
		}
	}
	else {
		err = 2;
	}

#ifdef _DEBUG
	RecalcHash(stk);

	if (StkOk(stk)) {
		PrintStk_OK(*stk);
	}
	else {
		PrintStk_NOK(*stk);
		return 1;
	}
#endif

	return err;
}


/**
*	������� ����
*
*	@param[in] stk ����
*
*	@return 0 - ��� ������ ���������; 1 - �������� �� ������
*/

int StackDestructor(stack_t* stk) {
	assert(stk != NULL);
#ifdef _DEBUG
	if (!StkOk(stk)) {
		PrintStk_NOK(*stk);
		return 1;
	}
#endif
	free(stk->data);
	stk->size = 0;
	stk->curMaxSize = 0;
#ifdef _DEBUG
	strcpy(stk->name, "");
	stk->err = 0;
	stk->hash = 0;
#endif
	return 0;
}