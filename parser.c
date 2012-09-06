#include "parser.h"

nodeADT parse(FILE * file, int state) {
	nodeADT first = NULL;
	nodeADT current;
	tNumbers vecWhile;
	tNumbers vecIf;
	tNumbers vecBalance;
	vecWhile.numbers = malloc((sizeof(int)) * BLOQUE);
	vecWhile.pos = 0;
	vecIf.numbers = malloc(sizeof(int) * BLOQUE);
	vecIf.pos = 0;
	vecBalance.numbers = calloc(sizeof(int), BLOQUE);
	vecBalance.pos = 0;
	char * string = malloc(sizeof(char) * BLOQUE);
	int index = 0;
	int c;
	Commands com = empty;
	int isFirst = TRUE;
	Stack stack;

	stack_init(&stack);

	while ((c = fgetc(file)) != EOF) {
		switch (c) {
		case '(':
			vecBalance.numbers = resizeMemInt(vecBalance.pos,
					vecBalance.numbers);
			(vecBalance.numbers)[vecBalance.pos] = c;
			toUpperString(string, index);
			com = getCommand(string, index);

			if (isFirst) {
				first = newNode(com);
				current = first;
				isFirst = FALSE;
			} else {
				nodeADT next = newNode(com);
				addNext(current, next);
				current = next;
			}
			index = 0;
			break;
		case ')':
			if (vecBalance.numbers[vecBalance.pos] != '(') {
				if (state) {
					if (index != 0 && com == empty) {
						fatal("Invalid parser\n");
					}
					fseek(file, -1, SEEK_CUR);
					return first;
				} else {
					fatal("Invalid parser\n");
				}
			} else {
				vecBalance.numbers[vecBalance.pos] = 0;
			}
			if (index == 0) {
				if (com != cz) {
					fatal("Invalid parser\n");
				} else {
					addParam(current, -1);
				}

			} else {
				if (com == endif) {
					if (vecIf.numbers[(vecIf.pos) - 1]
							!= toInt(string, index)) {
						fatal("Invalid parser\n");
					}
					(vecIf.pos) -= 1;

					nodeADT returnTO = (nodeADT) stack_top(&stack);
					stack_pop(&stack, 0);
					addReturn(current, returnTO); //le digo al ENDIF cual es su IF correspondiente
					addJump(returnTO, current); //le digo al IF cual es su ENDIF correspondiente
					addParam(current, getParam(returnTO));

				} else if (com == endwhile) {
					if (vecWhile.numbers[(vecWhile.pos) - 1]
							!= toInt(string, index)) {
						fatal("Invalid parser\n");
					}
					(vecWhile.pos) -= 1;

					nodeADT returnTO = (nodeADT) stack_top(&stack); //castear, maybe
					stack_pop(&stack, 0);
					addReturn(current, returnTO);
					addJump(returnTO, current);
					addParam(current, getParam(returnTO));

				} else if (com == cz) {
					fatal("Invalid parser\n");
				} else {
					hasNumbers(string, index);

					addParam(current, toInt(string, index));
				}
				vecBalance.numbers[vecBalance.pos] = 0;
				index = 0;
			}
			break;
		case ',':
			if (index != 0 && (com == ifa || com == whilea)) {
				hasNumbers(string, index);
				addParam(current, toInt(string, index));
				if (com == ifa) {
					vecIf.numbers = resizeMemInt(vecIf.pos, vecIf.numbers);
					vecIf.numbers[(vecIf.pos)++] = toInt(string, index);
				} else {
					vecWhile.numbers = resizeMemInt(vecWhile.pos,
							vecWhile.numbers);
					vecWhile.numbers[(vecWhile.pos)++] = toInt(string, index);
				}

				stack_push(&stack, current);

				addExe(current, parse(file, TRUE));

			} else {
				fatal("Invalid parser\n");
			}
			break;
		case ' ':
		case '\n':
		case '\t':
			break;
		default:
			string = resizeMemChar(index, string);
			string[index++] = c;
			break;
		}
	}
	if (vecBalance.numbers[vecBalance.pos] != 0) {
		fatal("Invalid parser\n");
	}
	return first;
}

Commands getCommand(char * string, int index) {
	Commands com = empty;

	if (strncmp(string, "INC", index) == 0) {
		com = inc;
	} else if (strncmp(string, "DEC", index) == 0) {
		com = dec;
	} else if (strncmp(string, "MR", index) == 0) {
		com = mr;
	} else if (strncmp(string, "ML", index) == 0) {
		com = ml;
	} else if (strncmp(string, "CZ", index) == 0) {
		com = cz;
	} else if (strncmp(string, "IF", index) == 0) {
		com = ifa;
	} else if (strncmp(string, "ENDIF", index) == 0) {
		com = endif;
	} else if (strncmp(string, "WHILE", index) == 0) {
		com = whilea;
	} else if (strncmp(string, "ENDWHILE", index) == 0) {
		com = endwhile;
	} else {
		fatal("Invalid parser\n");
	}
	return com;
}
