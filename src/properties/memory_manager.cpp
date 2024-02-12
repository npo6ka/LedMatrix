#include "debug_lib.h"
#include <eeprom.h>
#include "constants.h"

static uint16_t read_address(uint32_t offset) {
    // if (offset < 4095) {
    //     uint16_t ret_val = EEPROM.read(offset);
    //     ret_val = ret_val << 8 | EEPROM.read(offset + 1);
    //     return ret_val;
    // } else {
    //     out("Error reading ROM: out of memory\n");
    // }

    return 0;
}

static void write_address(uint32_t offset, uint16_t data) {
    // if (offset < 4095) {
    //     EEPROM.write(offset + 1, data & 0xff);
    //     EEPROM.write(offset, (data & 0xff00) >> 8);
    // } else {
    //     out("Error writing to ROM: out of memory\n");
    // }
}

static uint8_t find_free_slot(uint16_t &slot) {
    //поиск пустого места среди адресов на дату режимов
    // while (read_address(MOD_LIST_OFFSET + slot * 2) != 0) {
    //     if (slot >= MOD_LIST_MAX_SIZE) {
    //         return 1; // Достигнуто максимальное кол-во режимов в ПЗУ
    //     }
    //     slot++;
    // }

    return 0;
}

static uint16_t get_mod_size(uint16_t mod_addr) {
    // uint8_t mod_id = EEPROM.read(mod_addr);
    // if (mod_id < mods_size) {
    //     return mods[mod_id]->size + 1;
    // }
    return -1;
}

static void memory_fragmentation() {
    uint16_t cur_mod = 0;

    while (read_address(MOD_LIST_OFFSET + cur_mod * 2) != 0 && cur_mod < MOD_LIST_MAX_SIZE) {
        uint16_t old_mod_addr = read_address(MOD_LIST_OFFSET + cur_mod * 2);
        uint16_t mod_size = get_mod_size(old_mod_addr);
        uint16_t new_mod_addr = MOD_DATA_OFFSET;

        if (cur_mod != 0) {
            uint16_t prev_mod_addr = read_address(MOD_LIST_OFFSET + (cur_mod - 1) * 2);
            new_mod_addr = prev_mod_addr + get_mod_size(prev_mod_addr);
        }

        if (old_mod_addr != new_mod_addr) {
            for (int i = 0; i < mod_size; ++i) {
                EEPROM.write(new_mod_addr + i, EEPROM.read(old_mod_addr + i));
            }
            write_address(MOD_LIST_OFFSET + cur_mod * 2, new_mod_addr);
        }

        cur_mod++;
    }

}

static uint8_t find_free_address(uint16_t &addr, uint16_t free_slot) {
    addr = MOD_DATA_OFFSET;

    // считаеим  следующий незанятый адрес, если 0, то начало дата сета
    if (free_slot) {
        uint16_t prev_addr = read_address(MOD_LIST_OFFSET + (free_slot - 1) * 2);
        uint16_t mod_size = get_mod_size(prev_addr);

        if (mod_size == -1) {
            return 2; // прочитан неверный id режима
        }
        addr = prev_addr + mod_size;
    }

    return 0;
}

// static int8_t add_mod(uint8_t mod_id, Mode *m) {
    // uint16_t write_addres = MOD_DATA_OFFSET;
    // uint16_t free_slot = 0;
    // uint8_t err = 0;

    // if (find_free_slot(free_slot)) {
    //     return 1;  // Достигнуто максимальное кол-во режимов в ПЗУ
    // }

    // err = find_free_address(write_addres, free_slot);
    // if (err) {
    //     return err;
    // }

    // if (write_addres + m->size + 1 > MOD_DATA_OFFSET + MOD_DATA_SIZE) {
    //     memory_fragmentation();

    //     err = find_free_address(write_addres, free_slot);
    //     if (err) {
    //         return err;
    //     }

    //     if (write_addres + m->size + 1 > MOD_DATA_OFFSET + MOD_DATA_SIZE) {
    //         return 3; // закончилась память для данных режиов
    //     }
    // }

    // write_address(MOD_LIST_OFFSET + free_slot * 2, write_addres);
    // EEPROM.write(write_addres, mod_id);

    // //init all paramets for mod
    // for (int i = 0; i < m->size; ++i) {
    //     EEPROM.write(write_addres + i + 1, i + mod_id);
    // }
//     return 0;
// }

void add_mod(uint8_t mod_id) {
    // uint8_t err = add_mod(mod_id, mods[mod_id]);

    // switch(err) {
    //     case 1:
    //         out("Достигнуто максимальное кол-во режимов в ПЗУ\n");
    //         break;
    //     case 2:
    //         out("прочитан неверный id режима\n");
    //         break;
    //     case 3:
    //         out("Закончилась память для данных режиов\n");
    //         break;
    // }
}

void remove_mode(uint8_t mod_num) {
    uint16_t cur_addr = MOD_LIST_OFFSET + mod_num * 2;

    if (cur_addr >= MOD_DATA_OFFSET || !read_address(cur_addr)) {
        out("Удаление несуществующего мода\n");
        return;
    }

    while (cur_addr + 2 < MOD_DATA_OFFSET && read_address(cur_addr + 2) != 0) {
        write_address(cur_addr, read_address(cur_addr + 2));
        cur_addr += 2;
    }

    if (cur_addr < MOD_DATA_OFFSET) {
        write_address(cur_addr, 0);
    }
}

void validate_memory() {
    uint8_t slot = 0;
    while (slot < MOD_LIST_MAX_SIZE && read_address(MOD_LIST_OFFSET + slot * 2) != 0) {
        uint16_t addr = read_address(MOD_LIST_OFFSET + slot * 2);
        uint16_t mod_size = get_mod_size(addr);

        if (addr + mod_size >= MOD_DATA_OFFSET + MOD_DATA_SIZE) {
            remove_mode(slot);
            out("Modsize changed for mod id: %d. Removed this mod of memory\n", slot);
        } else {
            if (slot + 1 < MOD_LIST_MAX_SIZE && read_address(MOD_LIST_OFFSET + (slot + 1) * 2) != 0) {
                uint16_t next_mod_addr = read_address(MOD_LIST_OFFSET + (slot + 1) * 2);
                if (next_mod_addr - addr < mod_size) {
                    remove_mode(slot);
                    out("Modsize changed for mod id: %d. Removed this mod of memory\n", slot);
                } else {
                    slot++;
                }
            } else {
                slot++;
            }
        }
    }
}

void debug_mem(uint16_t addr, uint8_t size) {
    out("0x%02x: ", addr);
    for (int i = 0; i < size; ++i) {
        out("0x%02x ", EEPROM.read(addr + i));
    }
    out("\n");
}

void print_all_mods() {
    int addr = MOD_LIST_OFFSET;
    while (read_address(addr) != 0 && addr < MOD_DATA_OFFSET) {
        debug_mem(read_address(addr), get_mod_size(read_address(addr)));
        addr += 2;
    }
    out("\n\n");
}