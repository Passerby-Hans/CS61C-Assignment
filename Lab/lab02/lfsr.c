#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lfsr.h"

uint16_t get_bit(uint16_t x, uint16_t n) {
    return (x >> n) & 1u;
}

void lfsr_calculate(uint16_t *reg) {
    /* YOUR CODE HERE */
    uint16_t zero_bit = get_bit(*reg, 0);
    uint16_t two_bit = get_bit(*reg, 2);
    uint16_t three_bit = get_bit(*reg, 3);
    uint16_t five_bit = get_bit(*reg, 5);
    uint16_t result = (zero_bit ^ two_bit ^ three_bit ^ five_bit);
    *reg = (*reg >> 1) | (result << 15);
}

