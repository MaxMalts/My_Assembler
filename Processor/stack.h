#pragma once

typedef float elem_t;             ///<Тим элементов стэка


/**
*	Стэк
*/

#pragma pack(1)
struct stack_t {
#ifdef _DEBUG
	int secureVarBeg = 0;                      ///<Первая канарейка
#endif

	elem_t* data = {};                         ///<Элементы стэка
	const int delta = 5;                       ///<Значение дельта для реаллокации
	int curMaxSize = 0;                        ///<Количество элементов, выделенных в памяти
	int size = 0;                              ///<Настоящий размер стэка
	const elem_t emptyelem = 339615136492207134461438014706212143104.0;      ///<Элемент, соответствующий пустому
#ifdef _DEBUG
	char name[30] = "";                        ///<Имя стэка
	int err = 0;                               ///<Код ошибки, содержащейся в стэке:\n
											   ///0 - нет ошибок\n
											   ///1 - выход за вержнюю границу стэка\n
											   ///2 - выход за нижнюю границу стэка\n
											   ///3, 4 - испорчена соответственно первая или правая канарейка
											   ///5 - неверная хэш-сумма
	unsigned int hash = 0;                     ///<Хэш-сумма
	int secureVarEnd = 0;                      ///<Вторая канарейка
#endif
};
#pragma pack()

stack_t StackConstructor(const char*);

int StackPush(stack_t*, elem_t);

int StackPop(stack_t*, elem_t*);

int StackDestructor(stack_t*);