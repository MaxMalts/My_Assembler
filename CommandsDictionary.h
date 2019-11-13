#ifndef DONT_INCLUDE
#include "Constants.h"
#endif

DEF_CMD(push, 1, PUSH_TYPE, {

		StackPush(&stk, *((float*) &buf[(cursor + 1)]));
		cursor += 5;

	}, {

		float param = *((float*) &buf[(cursor + 1)]);
		fprintf(fout, "%s %g\n", commName, param);

		cursor += 5;

	}, 5)

DEF_CMD(push, 111, PUSH_TYPE, {

		int regInd = *((char*) &buf[(cursor + 1)]);
		StackPush(&stk, reg[regInd]);

		cursor += 2;

	}, {

		char regName[3] = "";
		regName[0] = DetRegRev(*((char*) &buf[(cursor + 1)]));
		regName[1] = 'x';

		fprintf(fout, "%s %s\n", commName, regName);

		cursor += 2;

	}, 2)

DEF_CMD(push, 112, PUSH_TYPE, {

		int ramInd = *((int*) &buf[(cursor + 1)]);
		StackPush(&stk, ram[ramInd]);

		cursor += 5;

	}, {

		int ramInd = *((int*) &buf[(cursor + 1)]);
		fprintf(fout, "%s [%d]\n", commName, ramInd);

		cursor += 5;

	}, 5)

DEF_CMD(push, 113, PUSH_TYPE, {

		int regInd = *((char*) &buf[(cursor + 1)]);
		int ramInd = (int)reg[regInd];
		StackPush(&stk, ram[ramInd]);

		cursor += 2;

	}, {
		
		char regName[3] = "";
		regName[0] = DetRegRev(*((char*) &buf[(cursor + 1)]));
		regName[1] = 'x';

		fprintf(fout, "%s [%s]\n", commName, regName);

		cursor += 2;

	}, 2)

DEF_CMD(pop, 2, POP_TYPE, {

		elem_t pop = {};
		StackPop(&stk, &pop);

		cursor++;

	}, {

		fprintf(fout, "%s\n", commName);

		cursor++;

	}, 1)

DEF_CMD(pop, 121, POP_TYPE, {

		int regInd = *((char*)& buf[(cursor + 1)]);
		StackPop(&stk, &reg[regInd]);

		cursor += 2;

	}, {

		char regName[3] = "";
		regName[0] = DetRegRev(*((char*)& buf[(cursor + 1)]));
		regName[1] = 'x';

		fprintf(fout, "%s %s\n", commName, regName);

		cursor += 2;

	}, 2)

DEF_CMD(pop, 122, POP_TYPE, {

		int ramInd = *((int*) &buf[(cursor + 1)]);
		StackPop(&stk, &ram[ramInd]);

		cursor += 5;

	}, {

		int ramInd = *((int*)& buf[(cursor + 1)]);
		fprintf(fout, "%s [%d]\n", commName, ramInd);

		cursor += 5;

	}, 5)

DEF_CMD(pop, 123, POP_TYPE, {

		int regInd = *((char*) &buf[(cursor + 1)]);
		int ramInd = (int)reg[regInd];
		StackPop(&stk, &ram[ramInd]);

		cursor += 2;

	}, {

		char regName[3] = "";
		regName[0] = DetRegRev(*((char*)& buf[(cursor + 1)]));
		regName[1] = 'x';

		fprintf(fout, "%s [%s]\n", commName, regName);

		cursor += 2;

	}, 2)

DEF_CMD(add, 3, SIMPLE_TYPE, {

		elem_t elem1 = {}; elem_t elem2 = {};
	
		StackPop(&stk, &elem2);
		StackPop(&stk, &elem1);
	
		StackPush(&stk, elem1 + elem2);

		cursor++;

	}, {

		fprintf(fout, "%s\n", commName);

		cursor++;

	}, 1)

DEF_CMD(sub, 4, SIMPLE_TYPE, {
		
		elem_t elem1 = {}; elem_t elem2 = {};
	
		StackPop(&stk, &elem2);
		StackPop(&stk, &elem1);
	
		StackPush(&stk, elem1 - elem2);

		cursor++;

	}, {

		fprintf(fout, "%s\n", commName);

		cursor++;

	}, 1)

DEF_CMD(mul, 5, SIMPLE_TYPE, {
		
		elem_t elem1 = {}; elem_t elem2 = {};
	
		StackPop(&stk, &elem2);
		StackPop(&stk, &elem1);
	
		StackPush(&stk, elem1 * elem2);

		cursor++;

	}, {

		fprintf(fout, "%s\n", commName);

		cursor++;

	}, 1)

DEF_CMD(div, 6, SIMPLE_TYPE, {

		elem_t elem1 = {}; elem_t elem2 = {};
	
		StackPop(&stk, &elem2);
		StackPop(&stk, &elem1);
	
		StackPush(&stk, elem1 / elem2);

		cursor++;

	}, {

		fprintf(fout, "%s\n", commName);

		cursor++;

	}, 1)

DEF_CMD(sqrt, 21, SIMPLE_TYPE, {

		elem_t elem = {};
	
		StackPop(&stk, &elem);
	
		StackPush(&stk, sqrt(elem));

		cursor++;

	}, {

		fprintf(fout, "%s\n", commName);

		cursor++;

	}, 1)

DEF_CMD(in, 7, SIMPLE_TYPE, {

		elem_t elem = {};
		printf("¬вод: ");
		scanf("%f", &elem);

		StackPush(&stk, elem);

		cursor++;

	}, {

		fprintf(fout, "%s\n", commName);

		cursor++;

	}, 1)

DEF_CMD(out, 8, SIMPLE_TYPE, {

		elem_t pop = {};
		StackPop(&stk, &pop);

		fprintf(fout, "%g\n", pop);

		cursor++;

	}, {

		fprintf(fout, "%s\n", commName);

		cursor++;

	}, 1)

DEF_CMD(:, 9, LABEL_TYPE, {}, {}, 0)

DEF_CMD(jmp, 10, JUMPER_TYPE, {

		cursor = *((int*) &buf[cursor+1]);

	}, {

		int param = *((int*) &buf[cursor + 1]);
		if (FindLabelOnAdr(labels, param, labelsCount, &ind) != 0){
			return 1;
		}
		fprintf(fout, "%s %s\n", commName, labels[ind].name);
		
		cursor += 5;

	}, 5)

DEF_CMD(ja, 11, JUMPER_TYPE, {

		elem_t reg = {};
		elem_t value = {};

		StackPop(&stk, &value);
		StackPop(&stk, &reg);

		if (reg > value) {
			cursor = *((int*) &buf[cursor + 1]);
		}
		else {
			cursor += 5;
		}

	}, {

		int param = *((int*)& buf[cursor + 1]);
		if (FindLabelOnAdr(labels, param, labelsCount, &ind) != 0) {
			return 1;
		}
		fprintf(fout, "%s %s\n", commName, labels[ind].name);

		cursor += 5;

	}, 5)

DEF_CMD(ju, 12, JUMPER_TYPE, {

		elem_t reg = {};
		elem_t value = {};

		StackPop(&stk, &value);
		StackPop(&stk, &reg);

		if (reg < value) {
			cursor = *((int*) &buf[cursor + 1]);
		}
		else {
			cursor += 5;
		}

	}, {

		int param = *((int*)& buf[cursor + 1]);
		if (FindLabelOnAdr(labels, param, labelsCount, &ind) != 0) {
			return 1;
		}
		fprintf(fout, "%s %s\n", commName, labels[ind].name);

		cursor += 5;

	}, 5)

DEF_CMD(je, 13, JUMPER_TYPE, {

		elem_t reg = {};
		elem_t value = {};

		StackPop(&stk, &value);
		StackPop(&stk, &reg);

		if (reg == value) {
			cursor = *((int*) &buf[cursor + 1]);
		}
		else {
			cursor += 5;
		}

	}, {

		int param = *((int*)& buf[cursor + 1]);
		if (FindLabelOnAdr(labels, param, labelsCount, &ind) != 0) {
			return 1;
		}
		fprintf(fout, "%s %s\n", commName, labels[ind].name);

		cursor += 5;

	}, 5)

DEF_CMD(jae, 14, JUMPER_TYPE, {

		elem_t reg = {};
		elem_t value = {};

		StackPop(&stk, &value);
		StackPop(&stk, &reg);

		if (reg >= value) {
			cursor = *((int*) &buf[cursor + 1]);
		}
		else {
			cursor += 5;
		}

	}, {

		int param = *((int*)& buf[cursor + 1]);
		if (FindLabelOnAdr(labels, param, labelsCount, &ind) != 0) {
			return 1;
		}
		fprintf(fout, "%s %s\n", commName, labels[ind].name);

		cursor += 5;

	}, 5)

DEF_CMD(jue, 15, JUMPER_TYPE, {

		elem_t reg = {};
		elem_t value = {};

		StackPop(&stk, &value);
		StackPop(&stk, &reg);

		if (reg <= value) {
			cursor = *((int*) &buf[cursor + 1]);
		}
		else {
			cursor += 5;
		}

	}, {

		int param = *((int*)& buf[cursor + 1]);
		if (FindLabelOnAdr(labels, param, labelsCount, &ind) != 0) {
			return 1;
		}
		fprintf(fout, "%s %s\n", commName, labels[ind].name);

		cursor += 5;

	}, 5)

DEF_CMD(jne, 16, JUMPER_TYPE, {

		elem_t reg = {};
		elem_t value = {};

		StackPop(&stk, &value);
		StackPop(&stk, &reg);

		if (reg != value) {
			cursor = *((int*) &buf[cursor + 1]);
		}
		else {
			cursor += 5;
		}

	}, {

		int param = *((int*)& buf[cursor + 1]);
		if (FindLabelOnAdr(labels, param, labelsCount, &ind) != 0) {
			return 1;
		}
		fprintf(fout, "%s %s\n", commName, labels[ind].name);

		cursor += 5;

	}, 5)

DEF_CMD(func, 17, FUNCTION_TYPE, {}, {}, 0)

DEF_CMD(ret, 18, SIMPLE_TYPE, {

		elem_t nextCursPos = 0;
		if (StackPop(&retAdresses, &nextCursPos) != 0) {
			return 1;
		}
		cursor = *((int*) &nextCursPos);

	}, {

		fprintf(fout, "%s\n", commName);

		cursor++;

	}, 1)

DEF_CMD(call, 19, CALL_TYPE, {

		int nextCursPos = cursor + 1 + sizeof(int);
		StackPush(&retAdresses, *((elem_t*) &nextCursPos));
		cursor = *((int*) &buf[cursor + 1]);

	}, {

		int param = *((int*)& buf[cursor + 1]);
		if (FindFuncOnAdr(funcs, param, funcsCount, &ind) != 0) {
			return 1;
		}
		fprintf(fout, "%s %s\n", commName, funcs[ind].name);

		cursor += 5;

	}, 5)

DEF_CMD(end, 20, END_TYPE, {
	
		ended = 1;
	
	}, {
	
		fprintf(fout, "%s\n", commName);
		ended = 1;

	}, 1)