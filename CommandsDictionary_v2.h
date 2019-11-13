DEF_CMD(push, 1, {
		StackPush(&stk, *((int*) &buf[(cursor + 1)]));
		cursor += 5;
	})

DEF_CMD(push, 111, {
		int regInd = *((int*)& buf[(cursor + 1)]);
		StackPush(&stk, reg[regInd]);

		cursor += 5;
	})

DEF_CMD(pop, 2, {
		elem_t pop = {};
		StackPop(&stk, &pop);

		cursor++;
	})

DEF_CMD(pop, 121, {
		int regInd = *((int*)& buf[(cursor + 1)]);
		StackPop(&stk, &reg[regInd]);

		cursor += 5;
	})

DEF_CMD(sum, 3, {
		elem_t elem1 = {}; elem_t elem2 = {};
	
		StackPop(&stk, &elem2);
		StackPop(&stk, &elem1);
	
		StackPush(&stk, elem1 + elem2);

		cursor++;
	})

DEF_CMD(sub, 4, {
		elem_t elem1 = {}; elem_t elem2 = {};
	
		StackPop(&stk, &elem2);
		StackPop(&stk, &elem1);
	
		StackPush(&stk, elem1 - elem2);

		cursor++;
	})

DEF_CMD(mul, 5, {
		elem_t elem1 = {}; elem_t elem2 = {};
	
		StackPop(&stk, &elem2);
		StackPop(&stk, &elem1);
	
		StackPush(&stk, elem1 * elem2);

		cursor++;
	})

DEF_CMD(div, 6, {
		elem_t elem1 = {}; elem_t elem2 = {};
	
		StackPop(&stk, &elem2);
		StackPop(&stk, &elem1);
	
		StackPush(&stk, elem1 / elem2);

		cursor++;
	})

DEF_CMD(in, 7, {
		elem_t elem = {};
		printf("¬вод: ");
		scanf("%d", &elem);

		StackPush(&stk, elem);

		cursor++;
	})

DEF_CMD(out, 8, {
		elem_t pop = {};
		StackPop(&stk, &pop);

		fprintf(fout, "%d\n", pop);

		cursor++;
	})


DEF_CMD(jmp, 9, {
	cursor = *((int*) &buf[cursor+1]);
	})

DEF_CMD(ja, 9, {
	cursor = *((int*) &buf[cursor+1]);
	})

DEF_CMD(end, 10, { ended = 1; })