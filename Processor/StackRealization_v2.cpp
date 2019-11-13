// Версии:
// v1 - стэк фиксированного размера
// v2 - динамический стэк

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
*	Преобразует elem_t в строку
*
*	@param[in] elem Элемент
*
*	@return Указатель на строку
*/

char* elem_tToStr(elem_t elem) {

	const int elem_tMaxStrSize = 20;

	char* str = (char*)calloc(elem_tMaxStrSize, sizeof(char));
	itoa(elem, str, 10);
	return str;
}


/**
*	Выводит информацию о стэке
*
*	@param[in] stk Стэк
*	@param[in] file Название файла, окуда вызвали функцию
*	@param[in] line Номер строки, из которой вызвали функцию
*	@param[in] function Имя функции, из которой вызвали функцию
*	@param[in] reason Причина, по которой вызвали функцию
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
*	Проверяет стэк и записывает в него код ошибки
*
*	@param[in] stk Стэк
*
*	@return fasle - стэк некорректный; true - стэк корректный
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
*	Вычисляет хэш-сумму стэка
*
*	@param[in] stk Стэк
*
*	@return Значение хэш-суммы
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
*	Пересчитывает хэш-сумму стэка
*
*	@param[in] stk Стэк
*
*	@return 1 - ошибка в стэке после пересчитывания хэша; 0 - все прошло нормально
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
*	Создает новый стэк, заполняя его "пустыми" элементами
*
*	@param[in] name Имя стэка
*
*	@return Указатель на созданный стэк
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
*	Увеличивает стэк
*
*	@param[in] stk Стэк
*	@param[in] newSize Новый размер стэка (в элементах)
*	@param[in] elemSize Размер одного элемента (в байтах)
*
*	@return 1 - проблемы со стэком после увеличения; 2 - не удалось реаллокировать стэк; 0 - все прошло нормально
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
*	Берет элемент их стэка
*
*	@param[in] stk Стэк
*	@param[in] value Значение нового элемента
*
*	@return 0 - все прошло нормально; 2 - не удалось увеличить размер стэка; 1 - проблемы со стэком
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
*	Уменьшает стэк
*
*	@param[in] stk Стэк
*	@param[in] newSize Новый размер стэка (в элементах)
*	@param[in] elemSize Размер одного элемента (в байтах)
*
*	@return 1 - проблемы со стэком после уменьшения; 2 - не удалось реаллокировать стэк; 0 - все прошло нормально
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
*	Берет элемент их стэка
*
*	@param[in] stk Стэк
*	@param[in] variable Переменная для возврата элемента. Если стэк пустой, остается нетронутой
*
*	@return 3 - Была неудачная попытка уменьшить стэк; 2 - в стэке нет элементов; 
1 - проблемы со стэком; 0 - все прошло нормально
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
*	Удаляет стэк
*
*	@param[in] stk Стэк
*
*	@return 0 - все прошло нормально; 1 - проблемы со стэком
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