#pragma once

const int commMaxLength = 20;    ///<������������ ����� �������

const int NComm = 7;    ///<���������� ������ � ����� My Assembler

/**
*	���������� � �������
*/

struct command_t {
	char commandS[commMaxLength] = {};    ///<������� � ��������� �������������
	int commandI = 0;                     ///<������� � ��������� �������������
	int hasParam = 0;                     ///<���� �� � ������� ��������
};

const command_t Push = { "push", 1, 1 };

const command_t Pop = { "pop", 2, 0 };

const command_t Sum = { "sum", 3, 0 };

const command_t Sub = { "sub", 4, 0 };

const command_t Mul = { "mul", 5, 0 };

const command_t Div = { "div", 6, 0 };

const command_t End = { "end", 7, 0 };

const command_t commands[] = { (command_t)Push, (command_t)Pop, (command_t)Sum,
                               (command_t)Sub, (command_t)Mul, (command_t)Div, (command_t)End };    ///<������ ���� ������ ����� My Assembler