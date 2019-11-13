#pragma once

typedef float elem_t;             ///<��� ��������� �����


/**
*	����
*/

#pragma pack(1)
struct stack_t {
#ifdef _DEBUG
	int secureVarBeg = 0;                      ///<������ ���������
#endif

	elem_t* data = {};                         ///<�������� �����
	const int delta = 5;                       ///<�������� ������ ��� �����������
	int curMaxSize = 0;                        ///<���������� ���������, ���������� � ������
	int size = 0;                              ///<��������� ������ �����
	const elem_t emptyelem = 339615136492207134461438014706212143104.0;      ///<�������, ��������������� �������
#ifdef _DEBUG
	char name[30] = "";                        ///<��� �����
	int err = 0;                               ///<��� ������, ������������ � �����:\n
											   ///0 - ��� ������\n
											   ///1 - ����� �� ������� ������� �����\n
											   ///2 - ����� �� ������ ������� �����\n
											   ///3, 4 - ��������� �������������� ������ ��� ������ ���������
											   ///5 - �������� ���-�����
	unsigned int hash = 0;                     ///<���-�����
	int secureVarEnd = 0;                      ///<������ ���������
#endif
};
#pragma pack()

stack_t StackConstructor(const char*);

int StackPush(stack_t*, elem_t);

int StackPop(stack_t*, elem_t*);

int StackDestructor(stack_t*);