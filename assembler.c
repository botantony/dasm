#include "common.h"

void trim_str_left(const char *str, const size_t size, size_t *index) {
    char current_char = *index;
    while (isspace(current_char) && *index < size) {
        *index += 1;
        current_char = str[*index];
    }
}

bool is_empty(char *s) {
    int i = 0;
    while (s[i] != '\0') {
        if (s[i] == ';') {
            while (s[i] != '\0' && s[i] != '\n') {
                i++;
            }
        } else if (!(isspace(s[i]) || s[i] == '\0' || s[i] == '\n')) {
            return false;
        }
        i++;
    }
    return true;
}

bool get_next_token(Word output, const char *str, const size_t size, size_t *cursor) {
    char current_char = str[*cursor];
    size_t word_index = 0;
    while ((isalnum(current_char) || current_char == '-') && *cursor < size && output[WORD_LEN - 1] == '\0') {
        output[word_index] = current_char;
        word_index++;
        current_char = str[*cursor + word_index];
    }
    if (output[WORD_LEN - 1] != '\0') {
        fprintf(stderr, "Long token \"%s\"!\n", output);
        return false;
    }
    *cursor += word_index;
    return true;
}

bool parse_separated_tokens(Word first_token, Word second_token, const char *str, const size_t size, size_t *cursor) {
    bool flag = true;
    flag = get_next_token(first_token, str, size, cursor);
    if (!flag) return flag;
    if (str[*cursor] == ',') {
        *cursor += 1;
    } else {
        fprintf(stderr, "Unrecognized token \"%c\" on string \"%s\"!\n", str[*cursor], str);
        return false;
    }
    trim_str_left(str, size, cursor);
    *cursor += 1;
    if (*cursor >= size) {
        fprintf(stderr, "Second operand on string \"%s\" is not provided!\n", str);
        return false;
    }
    flag = get_next_token(second_token, str, size, cursor);
    return flag;
}

bool parse_string(const char *str, const size_t size, uint8_t *return_value) {
    bool flag = true;
    *return_value = incorrect_token;
    size_t cursor = 0;
    trim_str_left(str, size, &cursor);
    if (cursor >= size) return return_value;

    // parse the mnemonic/instruction
    Word instruction_token = {0};
    init_word(instruction_token);
    flag = get_next_token(instruction_token, str, size, &cursor);
    if (!flag) return flag;
    int instruction_id = 0;

    for (int i = 0; i < ARRAY_SIZE(instructions); ++i) {
        if (strcmp(instructions[i], instruction_token) == 0) {
            *return_value = instruction_masks[i];
            instruction_id = i;
        }
    }

    if (*return_value == incorrect_token) {
        fprintf(stderr, "Unrecognized token \"%s\" on string \"%s\"!\n", instruction_token, str);
        return false;
    }

    // handle operands
    cursor++;
    trim_str_left(str, size, &cursor);
    if (cursor >= size) {
        fprintf(stderr, "Missing operand in this instruction: \"%s\"!", str);
        return false;
    }
    Word first_operand;
    Word second_operand;
    init_word(first_operand);
    init_word(second_operand);
    uint8_t registers_mask = 0;
    bool valid_register = false;
    switch (instruction_id) {
        case INS_MOV:
            get_next_token(first_operand, str, size, &cursor);
            int number = parse_number(first_operand);
            if (number == -1) return false;
            *return_value = number;
            break;
        case INS_ADD:
        case INS_SUB:
        case INS_MUL:
        case INS_DIV:
            parse_separated_tokens(first_operand, second_operand, str, size, &cursor);
            for (int i = 0; i < ARRAY_SIZE(registers); ++i) {
                if (strcmp(registers[i], first_operand) == 0) {
                    registers_mask <<= 2;
                    registers_mask += i;
                    valid_register = true;
                }
            }
            if (!valid_register) {
                fprintf(stderr, "Unrecognized register \"%s\" on string \"%s\"!\n", first_operand, str);
                return false;
            }
            valid_register = false;
            for (int i = 0; i < ARRAY_SIZE(registers); ++i) {
                if (strcmp(registers[i], second_operand) == 0) {
                    registers_mask <<= 2;
                    registers_mask += i;
                    valid_register = true;
                }
            }
            if (!valid_register) {
                fprintf(stderr, "Unrecognized register \"%s\" on string \"%s\"!\n", second_operand, str);
                return false;
            }
            valid_register = false;
            *return_value |= registers_mask;
            break;
        case INS_IN:
        case INS_OUT:
            get_next_token(first_operand, str, size, &cursor);
            for (int i = 0; i < ARRAY_SIZE(registers); ++i) {
                if (strcmp(registers[i], first_operand) == 0) {
                    registers_mask <<= 2;
                    registers_mask += i;
                    valid_register = true;
                }
            }
            if (!valid_register) {
                fprintf(stderr, "Unrecognized register \"%s\" on string \"%s\"!\n", first_operand, str);
                return false;
            }
            valid_register = false;
            *return_value |= registers_mask;
            break;
        default:
            printf("No way\n");
            return false;
    }

    // check if there are nay other characters in the string
    char current_char = 'a';
    while (current_char != '\n' && current_char != '\0' && current_char != ';' && cursor < size) {
        current_char = str[cursor];
        if (!(isspace(current_char) || current_char == ';')) {
            Word tmp_token;
            init_word(tmp_token);
            get_next_token(tmp_token, str, size, &cursor);
            fprintf(stderr, "Unexpected token \"%s\" on string \"%s\"!\n", tmp_token, str);
            return false;
        }
        cursor++;
    }

    return true;
}

int main(int argc, char *argv[])
{
    FILE *f = fopen(argv[1], "r");
    FILE *o = fopen(argv[2], "w");
    if (f == NULL) {
        fprintf(stderr, "Not able to open a file with a name \"%s\"!\n", argv[1]);
        return 1;
    }
    if (o == NULL) {
        fprintf(stderr, "Not able to open a file with a name \"%s\"!\n", argv[1]);
        return 1;
    }

    for (;;) {
        char line[80];
        char *token;

        if (fgets(line, 80, f) == NULL) break;
        if (is_empty(line)) continue;
        size_t size = strlen(line);
        uint8_t output;
        bool code = parse_string(line, size, &output);

        if (!code) {
            fclose(f);
            fclose(o);
            return -1;
        }

        fputc(output, o);
    }

    fclose(f);
    fclose(o);
    return 0;
}
