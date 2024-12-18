#include "common.h"

uint8_t emulator_registers[4] = {0, 0, 0, 0};

int exec_instruction(int instruction) {
    if (instruction >> 7 == 0) {
        emulator_registers[3] = instruction;
        return 0;
    }
    uint8_t tmp_mask = instruction & 0b11110000;
    bool is_valid = false;
    uint8_t instruction_id = 0;
    for (uint8_t i = 1; i < ARRAY_SIZE(instruction_masks); ++i) { // skip `MOV`
        if (tmp_mask == instruction_masks[i]) {
            is_valid = true;
            instruction_id = i;
            break;
        }
    }
    if (!is_valid) {
        fprintf(stderr, "Unknown instruction \"%x\"!\n", instruction);
        return -1;
    }
    uint8_t first_register = (instruction & 0b00001100) >> 2;
    uint8_t second_register = instruction & 0b00000011;
    Word tmp_input;
    int tmp_number;

    switch (instruction_id) {
        case INS_ADD:
            emulator_registers[first_register] += emulator_registers[second_register];
            break;
        case INS_SUB:
            emulator_registers[first_register] -= emulator_registers[second_register];
            break;
        case INS_MUL:
            emulator_registers[first_register] *= emulator_registers[second_register];
            break;
        case INS_DIV:
            emulator_registers[first_register] /= emulator_registers[second_register];
            break;
        case INS_IN:
            scanf("%15s", tmp_input);
            tmp_number = parse_number(tmp_input);
            if (tmp_number == -1) {
                fprintf(stderr, "Invalid number \"%s\"!\n", tmp_input);
                return -1;
            }
            emulator_registers[second_register] = (uint8_t)tmp_number;
            break;
        case INS_OUT:
            printf("%d\n", emulator_registers[second_register]);
            break;
        default:
            fprintf(stderr, "Unexpected instruction \"%x\"!\n", instruction);
            return -1;
    }

    return 0;
}

int main(int argc, char **argv) {
    FILE *obj = fopen(argv[1], "r");
    int c;
    int code = 0;
    if (obj == NULL) {
        fprintf(stderr, "Not able to open a file with a name \"%s\"!\n", argv[1]);
        return 1;
    }

    while ((c = fgetc(obj)) != EOF && code != -1)
    {
        code = exec_instruction(c);
    }

    fclose(obj);
    return 0;
}
