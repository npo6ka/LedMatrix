#pragma once

#include "inttypes.h"

void add_mod(uint8_t mod_id);
void remove_mode(uint8_t mod_num);
void validate_memory();
void debug_mem(uint16_t addr, uint8_t size);
void print_all_mods();