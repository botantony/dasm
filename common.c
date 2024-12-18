#include "common.h"

const Word instructions[] = {
    "MOV",
    "ADD",
    "SUB",
    "MUL",
    "DIV",
    "IN",
    "OUT",
};

const Word registers[] = {
    "A", "B", "C", "D",
};

// binary masks for each instruction, they should correspond to `instrunctions` table
const uint8_t instruction_masks[] = {
    0b00000000,
    0b10000000,
    0b11000000,
    0b11100000,
    0b11010000,
    0b10100000,
    0b10110000,
};

const uint8_t incorrect_token = 0b11111111;

// for some reason default initialization adds "garbage" values
void init_word(Word output) {
    for (int i = 0; i < WORD_LEN; ++i) {
         output[i] = 0;
    }
}

int parse_number(const Word num) {
    int return_value = 0;
    size_t cursor = 0;
    // validate number
    if (num[0] == '0') {
        cursor = 2;
        if (num[1] == 'b') {
            // handle binary separately because strtol does not support binary nymbers like "0b111"
            Word binary;
            init_word(binary);
            for (; cursor < WORD_LEN; ++cursor) {
                char current_char = num[cursor];
                binary[cursor - 2] = current_char;
                if (isspace(current_char) || current_char == '\0') break;
                if (!(current_char == '0' || current_char == '1')) {
                    fprintf(stderr, "Incorrect binary number \"%s\"!\n", num);
                    return -1;
                }
            }
            return_value = (uint8_t)strtol(binary, NULL, 2);
            return return_value & 0b01111111;
        } else if (num[1] == 'x') {
            for (; cursor < WORD_LEN; ++cursor) {
                char current_char = num[cursor];
                if (isspace(current_char) || current_char == '\0') break;
                bool is_hex = (current_char >= '0' && current_char <= '9') || (current_char >= 'a' && current_char <= 'f');
                if (!is_hex) {
                    fprintf(stderr, "Incorrect hex number \"%s\"!\n", num);
                    return -1;
                }
            }
        } else {
            fprintf(stderr, "Unrecognized number \"%s\"!\n", num);
            return -1;
        }
    } else {
        for (; cursor < WORD_LEN; ++cursor) {
            char current_char = num[cursor];
            if (isspace(current_char) || current_char == '\0') break;
            if (!(current_char >= '0' && current_char <= '9')) {
                fprintf(stderr, "Unrecognized number \"%s\"!\n", num);
                return -1;
            }
        }
    }
    return_value = (uint8_t)strtol(num, NULL, 0);
    return return_value & 0b01111111;
}
