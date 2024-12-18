#ifndef COMMON_H_
#define COMMON_H_

#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WORD_LEN 16
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

typedef char Word[WORD_LEN];

extern const Word instructions[7];
extern const Word registers[4];
extern const uint8_t instruction_masks[7];

// macros that simplify indexing instruction masks
#define INS_MOV  0
#define INS_ADD  1
#define INS_SUB  2
#define INS_MUL  3
#define INS_DIV  4
#define INS_IN   5
#define INS_OUT  6

extern const uint8_t incorrect_token;

void init_word(Word output);
int parse_number(const Word num);

#endif // COMMON_H_
