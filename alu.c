#include <stdio.h>
#include <stdint.h>
#include "alu.h"

#define MASK 0x0F

struct ALUResult add(uint8_t A, uint8_t B) {
    struct ALUResult output = {0};

    // 4-bit calculation
    uint16_t sum = (uint16_t)A + B;
    output.result = (uint8_t)(sum & MASK);

    // Carry flag
    if (sum > MASK) {
        output.flags |= FLAG_CARRY;
    }
    // Zero flag
    if (output.result == 0) {
        output.flags |= FLAG_ZERO;
    }
    // Negative flag (bit 3)
    if (output.result & 0x08) {
        output.flags |= FLAG_NEGATIVE;
    }
    // Overflow flag (signed 2's complement)
    uint8_t signA = A & 0x08;
    uint8_t signB = B & 0x08;
    uint8_t signR = output.result & 0x08;

    if ((signA == signB) && (signA != signR)) {
        output.flags |= FLAG_OVERFLOW;
    }

    return output;
}

struct ALUResult subtract(uint8_t A, uint8_t B) {
    struct ALUResult output = {0};

    // 4-bit calculation
    output.result = (uint8_t)((A - B) & MASK);

    // Borrow / Carry flag
    if (A < B) {
        output.flags |= FLAG_CARRY;
    }
    // Zero flag
    if (output.result == 0) {
        output.flags |= FLAG_ZERO;
    }
    // Negative flag (bit 3)
    if (output.result & 0x08) {
        output.flags |= FLAG_NEGATIVE;
    }
    // Overflow flag for subtraction
    uint8_t signA = A & 0x08;
    uint8_t signB = B & 0x08;
    uint8_t signR = output.result & 0x08;

    if ((signA != signB) && (signA != signR)) {
        output.flags |= FLAG_OVERFLOW;
    }

    return output;
}

struct ALUResult bitwise_and(uint8_t A, uint8_t B) {
    struct ALUResult output = {0};

    output.result = (A & B) & MASK;

    if (output.result == 0) {
        output.flags |= FLAG_ZERO;
    }
    if (output.result & 0x08) {
        output.flags |= FLAG_NEGATIVE;
    }

    return output;
}

struct ALUResult bitwise_or(uint8_t A, uint8_t B) {
    struct ALUResult output = {0};

    output.result = (A | B) & MASK;

    if (output.result == 0) {
        output.flags |= FLAG_ZERO;
    }
    if (output.result & 0x08) {
        output.flags |= FLAG_NEGATIVE;
    }

    return output;
}

int main(void) {
    unsigned int input;
    uint8_t A, B;
    int operation;
    struct ALUResult output = {0};

    printf("Enter A (0-15): ");
    if (scanf("%u", &input) != 1 || input > 15) {
        printf("Invalid input.\n");
        return 1;
    }
    A = (uint8_t)input;

    printf("Enter B (0-15): ");
    if (scanf("%u", &input) != 1 || input > 15) {
        printf("Invalid input.\n");
        return 1;
    }
    B = (uint8_t)input;

    printf("Enter operation (0=ADD, 1=SUB, 2=AND, 3=OR): ");
    if (scanf("%d", &operation) != 1 ||
        operation < 0 || operation > 3) {
        printf("Invalid operation.\n");
        return 1;
    }

    switch (operation) {
        case 0:
            output = add(A, B);
            break;
        case 1:
            output = subtract(A, B);
            break;
        case 2:
            output = bitwise_and(A, B);
            break;
        case 3:
            output = bitwise_or(A, B);
            break;
    }

    printf("\nResult: %u\n", (unsigned int)output.result);
    printf("Binary: %u%u%u%u\n",
        (output.result >> 3) & 1,
        (output.result >> 2) & 1,
        (output.result >> 1) & 1,
        output.result & 1);

    // Read packed bits from output.flags using bitwise AND (&)
    printf("Zero: %u\n", (output.flags & FLAG_ZERO) ? 1 : 0);
    printf("Carry/Borrow: %u\n", (output.flags & FLAG_CARRY) ? 1 : 0);
    printf("Negative: %u\n", (output.flags & FLAG_NEGATIVE) ? 1 : 0);
    printf("Overflow: %u\n", (output.flags & FLAG_OVERFLOW) ? 1 : 0);

    return 0;
}