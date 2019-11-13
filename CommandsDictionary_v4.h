#ifndef PROCESSOR
#include "Constants.h"
#endif

DEF_CMD(push, 1, PUSH_TYPE, {
		StackPush(&stk, *((float*) &buf[(cursor + 1)]));
		cursor += 5;
	})

DEF_CMD(push, 111, PUSH_TYPE, {
		int regInd = *((char*)& buf[(cursor + 1)]);
		StackPush(&stk, reg[regInd]);

		cursor += 2;
	})

DEF_CMD(pop, 2, POP_TYPE, {
		elem_t pop = {};
		StackPop(&stk, &pop);

		cursor++;
	})

DEF_CMD(pop, 121, POP_TYPE, {
		int regInd = *((char*)& buf[(cursor + 1)]);
		StackPop(&stk, &reg[regInd]);

		cursor += 2;
	})

DEF_CMD(sum, 3, SIMPLE_TYPE, {
		elem_t elem1 = {}; elem_t elem2 = {};
	
		StackPop(&stk, &elem2);
		StackPop(&stk, &elem1);
	
		StackPush(&stk, elem1 + elem2);

		cursor++;
	})

DEF_CMD(sub, 4, SIMPLE_TYPE, {
		elem_t elem1 = {}; elem_t elem2 = {};
	
		StackPop(&stk, &elem2);
		StackPop(&stk, &elem1);
	
		StackPush(&stk, elem1 - elem2);

		cursor++;
	})

DEF_CMD(mul, 5, SIMPLE_TYPE, {
		elem_t elem1 = {}; elem_t elem2 = {};
	
		StackPop(&stk, &elem2);
		StackPop(&stk, &elem1);
	
		StackPush(&stk, elem1 * elem2);

		cursor++;
	})

DEF_CMD(div, 6, SIMPLE_TYPE, {
		elem_t elem1 = {}; elem_t elem2 = {};
	
		StackPop(&stk, &elem2);
		StackPop(&stk, &elem1);
	
		StackPush(&stk, elem1 / elem2);

		cursor++;
	})

DEF_CMD(in, 7, SIMPLE_TYPE, {
		elem_t elem = {};
		printf("¬вод: ");
		scanf("%f", &elem);

		StackPush(&stk, elem);

		cursor++;
	})

DEF_CMD(out, 8, SIMPLE_TYPE, {
		elem_t pop = {};
		StackPop(&stk, &pop);

		fprintf(fout, "%g\n", pop);

		cursor++;
	})

DEF_CMD(:, 9, LABEL_TYPE, {})

DEF_CMD(jmp, 10, JUMPER_TYPE, {
		cursor = *((int*) &buf[cursor+1]);
	})

DEF_CMD(ja, 11, JUMPER_TYPE, {
		elem_t reg = {};
		elem_t value = {};

		StackPop(&stk, &value);
		StackPop(&stk, &reg);

		StackPush(&stk, reg);
		StackPush(&stk, value);

		if (reg > value) {
			cursor = *((int*) &buf[cursor + 1]);
		}
		else {
			cursor += 5;
		}
	})

DEF_CMD(ju, 12, JUMPER_TYPE, {
		elem_t reg = {};
		elem_t value = {};

		StackPop(&stk, &value);
		StackPop(&stk, &reg);

		StackPush(&stk, reg);
		StackPush(&stk, value);

		if (reg < value) {
			cursor = *((int*) &buf[cursor + 1]);
		}
		else {
			cursor += 5;
		}
	})

DEF_CMD(je, 13, JUMPER_TYPE, {
		elem_t reg = {};
		elem_t value = {};

		StackPop(&stk, &value);
		StackPop(&stk, &reg);

		StackPush(&stk, reg);
		StackPush(&stk, value);

		if (reg == value) {
			cursor = *((int*) &buf[cursor + 1]);
		}
		else {
			cursor += 5;
		}
	})

DEF_CMD(jae, 14, JUMPER_TYPE, {
		elem_t reg = {};
		elem_t value = {};

		StackPop(&stk, &value);
		StackPop(&stk, &reg);

		StackPush(&stk, reg);
		StackPush(&stk, value);

		if (reg >= value) {
			cursor = *((int*) &buf[cursor + 1]);
		}
		else {
			cursor += 5;
		}
	})

DEF_CMD(jue, 15, JUMPER_TYPE, {
		elem_t reg = {};
		elem_t value = {};

		StackPop(&stk, &value);
		StackPop(&stk, &reg);

		StackPush(&stk, reg);
		StackPush(&stk, value);

		if (reg <= value) {
			cursor = *((int*) &buf[cursor + 1]);
		}
		else {
			cursor += 5;
		}
	})

DEF_CMD(jne, 16, JUMPER_TYPE, {
		elem_t reg = {};
		elem_t value = {};

		StackPop(&stk, &value);
		StackPop(&stk, &reg);

		StackPush(&stk, reg);
		StackPush(&stk, value);

		if (reg != value) {
			cursor = *((int*) &buf[cursor + 1]);
		}
		else {
			cursor += 5;
		}
	})

DEF_CMD(func, 17, FUNCTION_TYPE, {})

DEF_CMD(ret, 18, SIMPLE_TYPE, {
		elem_t nextCursPos = 0;
		if (StackPop(&retAdresses, &nextCursPos) != 0) {
			return 1;
		}
		cursor = *((int*) &nextCursPos);
	})

DEF_CMD(call, 19, CALL_TYPE, {
		int nextCursPos = cursor + 1 + sizeof(int);
		StackPush(&retAdresses, *((elem_t*) &nextCursPos));
		cursor = *((int*) &buf[cursor + 1]);
	})

DEF_CMD(end, 20, END_TYPE, { ended = 1; })