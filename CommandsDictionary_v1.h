#pragma once

const int commMaxLength = 20;    ///<Максимальная длина команды

const int NComm = 7;    ///<Количество команд в языке My Assembler

/**
*	Информация о команде
*/

struct command_t {
	char commandS[commMaxLength] = {};    ///<Команда в строковом представлении
	int commandI = 0;                     ///<Команда в численном представлении
	int hasParam = 0;                     ///<Есть ли у команды параметр
};

const command_t Push = { "push", 1, 1 };

const command_t Pop = { "pop", 2, 0 };

const command_t Sum = { "sum", 3, 0 };

const command_t Sub = { "sub", 4, 0 };

const command_t Mul = { "mul", 5, 0 };

const command_t Div = { "div", 6, 0 };

const command_t End = { "end", 7, 0 };

const command_t commands[] = { (command_t)Push, (command_t)Pop, (command_t)Sum,
                               (command_t)Sub, (command_t)Mul, (command_t)Div, (command_t)End };    ///<Массив всех команд языка My Assembler