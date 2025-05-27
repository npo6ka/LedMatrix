#include "memory_manager.h"

MemoryManager::MemoryManager(const char* filename) {
    memory = MemoryHandler{};
    memory.openFile(filename);
    memory.read(offsetof(DefaultData, cur_mod), cur_mod_addr);
}

uint16_t MemoryManager::get_first_mod_addr() {
    uint16_t addr = 0;
    memory.read(offsetof(DefaultData, first_mod_addr), addr);
    return addr;
}

bool MemoryManager::set_cur_mod(uint16_t val) {
    if (val == 0 || val >= sizeof(DefaultData)) {
        cur_mod_addr = val;
        memory.write(offsetof(DefaultData, cur_mod), val);
        return true;
    } else {
        out("MemoryManager: Error set_cur_mod: out of memory\n");
        return false;
    }
}

void MemoryManager::mod_memory_shift(uint16_t src_addr, uint16_t dst_addr) {
    Mod mod;
    memory.read(src_addr, mod);
    memory.write(dst_addr, mod);

    for (uint16_t i = 0; i < mod.size; ++i) {
        uint8_t tmp;
        memory.read(src_addr + sizeof(Mod) + i, tmp);
        memory.write(dst_addr + sizeof(Mod) + i, tmp);
    }

    if (cur_mod_addr == src_addr) {
        set_cur_mod(dst_addr);
    }
}

void MemoryManager::rebalance_mod_list() {
    uint16_t cur_addr = get_first_mod_addr();
    uint16_t offset = 0;
    Mod mod;

    if (!cur_addr) {
        return;
    }

    if (cur_addr != sizeof(DefaultData)) {
        mod_memory_shift(cur_addr, sizeof(DefaultData));
        cur_addr = sizeof(DefaultData);
        memory.write(offsetof(DefaultData, first_mod_addr), cur_addr);
    }
    memory.read(cur_addr, mod);

    while (mod.next_addr) {
        offset = mod.next_addr - cur_addr - mod.size - sizeof(Mod);
        if (offset) {
            uint16_t cur_offset = mod.next_addr - offset;
            mod_memory_shift(mod.next_addr, cur_offset);
            mod.next_addr = cur_offset;
        }
        memory.write(cur_addr, mod.next_addr);
        cur_addr = mod.next_addr;
        memory.read(cur_addr, mod);
    }
}

bool MemoryManager::add_mod_with_rebalace(uint8_t mod_id, uint16_t mod_size, bool need_rebalace) {
    uint16_t addr = cur_mod_addr;
    uint16_t addr_tmp = addr;
    uint32_t new_addr;
    uint16_t size;

    if (addr) {
        while (addr_tmp) {
            memory.read(addr, addr_tmp);
            addr = addr_tmp? addr_tmp : addr;
        }
        memory.read(addr + offsetof(Mod, size), size);
        new_addr = (uint32_t)addr + size + sizeof(Mod);
    } else {
        addr = offsetof(DefaultData, first_mod_addr);
        new_addr = sizeof(DefaultData);
    }

    if (new_addr + sizeof(Mod) + mod_size < MAX_FILE_SIZE) {
        memory.write(addr, static_cast<uint16_t>(new_addr));
        memory.write(new_addr, Mod{0, mod_id, mod_size});
        set_cur_mod(new_addr);
        return true;
    } else {
        if (need_rebalace) {
            rebalance_mod_list();
            return add_mod_with_rebalace(mod_id, mod_size, false);
        } else {
            out("MemoryManager: Error add_new_mod: out of memory\n");
            return false;
        }
    }
}

uint16_t MemoryManager::get_prev_link_addr(uint16_t addr) {
    if (!addr || !cur_mod_addr) {
        return 0;
    }

    if (addr < sizeof(DefaultData)) {
        out("MemoryManager: Error get_prev_link_addr\n");
        return 0;
    }

    uint16_t prev_addr = offsetof(DefaultData, first_mod_addr);
    uint16_t cur_addr = get_first_mod_addr();
    while (cur_addr != addr && cur_addr) {
        prev_addr = cur_addr;
        memory.read(cur_addr, cur_addr);
    }

    if (cur_addr == addr) {
        return prev_addr;
    } else {
        out("MemoryManager: Error get_prev_link_addr: mod not found\n");
    }
    return 0;
}

uint16_t MemoryManager::get_mod_addr_by_num(uint8_t num, uint16_t &prev_addr) {
    prev_addr = offsetof(DefaultData, first_mod_addr);
    uint16_t cur_addr = get_first_mod_addr();
    uint16_t mod_num = 1;

    if (cur_addr) {
        for (; mod_num != num && cur_addr; ++mod_num) {
            prev_addr = cur_addr;
            memory.read(cur_addr, cur_addr);
        }
        if (mod_num == num) {
            return cur_addr;
        } else {
            out("MemoryManager: Error get_mod_addr_by_num: mod not found\n");
        }
    }
    return 0;
}

uint16_t MemoryManager::get_mod_addr_by_num(uint8_t num) {
    uint16_t prev_addr;
    return get_mod_addr_by_num(num, prev_addr);
}

bool MemoryManager::remove_mode_by_addr(uint16_t addr, uint16_t prev_addr) {
    uint16_t next_addr;

    if (!cur_mod_addr || !addr || !prev_addr) {
        return false;
    }

    memory.read(addr, next_addr);
    memory.write(prev_addr, next_addr);

    if (!next_addr && prev_addr == offsetof(DefaultData, first_mod_addr)) {
        set_cur_mod(0);
    } else if (cur_mod_addr == addr) {
        if (!next_addr) {
            set_cur_mod(prev_addr);
        } else {
            set_cur_mod(next_addr);
        }
    }

    return true;
}

bool MemoryManager::remove_mode_by_addr(uint16_t addr) {
    return remove_mode_by_addr(addr, get_prev_link_addr(addr));
}

MemoryManager::~MemoryManager() {}

MemoryManager& MemoryManager::instance() {
    static MemoryManager instance(MM_FILE_NAME);
    return instance;
}

void MemoryManager::clear_meёmory() {
    memory.clear();
}

bool MemoryManager::add_mod(uint8_t id, uint16_t size) {
    return add_mod_with_rebalace(id, size, true);
}

bool MemoryManager::remove_mod() {
    return remove_mode_by_addr(cur_mod_addr);
}

bool MemoryManager::remove_mod(uint8_t num) {
    uint16_t prev_addr;
    uint16_t addr = get_mod_addr_by_num(num, prev_addr);
    return remove_mode_by_addr(addr, prev_addr);
}

void MemoryManager::remove_all_mods() {
    set_cur_mod(0);
    memory.write(offsetof(DefaultData, first_mod_addr), 0);
}

void MemoryManager::next_mod() {
    if (!cur_mod_addr) {
        return;
    }
    uint16_t addr = 0;
    memory.read(cur_mod_addr, cur_mod_addr);

    if (!cur_mod_addr) {
        cur_mod_addr = get_first_mod_addr();
    }
    set_cur_mod(cur_mod_addr);
}

void MemoryManager::prev_mod() {
    if (!cur_mod_addr) {
        return;
    }
    cur_mod_addr = get_prev_link_addr(cur_mod_addr);
    if (cur_mod_addr == offsetof(DefaultData, first_mod_addr)) {
        uint16_t addr = cur_mod_addr;
        while (addr) {
            cur_mod_addr = addr;
            memory.read(addr, addr);
        }
    }
    set_cur_mod(cur_mod_addr);
}

bool MemoryManager::set_mod(uint8_t num) {
    if (!cur_mod_addr || num == 0) {
        return false;
    }
    uint16_t mod_addr = get_first_mod_addr();
    for(int i = 1; i < num && mod_addr; ++i) {
        memory.read(mod_addr, mod_addr);
    }
    if (mod_addr) {
        return set_cur_mod(mod_addr);
    } else {
        return false;
    }
}

uint8_t MemoryManager::get_cur_mod_num() {
    if (!cur_mod_addr) {
        return 0;
    }
    uint8_t num = 1;
    uint16_t addr = get_first_mod_addr();
    while (addr != cur_mod_addr && addr) {
        memory.read(addr, addr);
        num++;
    }
    return num;
}

uint8_t MemoryManager::get_mod_amount() {
    uint16_t addr = get_first_mod_addr();
    uint8_t mods_amt = 0;

    while (addr) {
        memory.read(addr, addr);
        mods_amt++;
    }
    return mods_amt;
}

std::unique_ptr<uint8_t[]> MemoryManager::get_mod_list() {
    uint16_t faddr = get_first_mod_addr();
    uint16_t addr = faddr;
    uint8_t mods_amt = 0;

    while (addr) {
        memory.read(addr, addr);
        mods_amt++;
    }
    std::unique_ptr<uint8_t[]> ids(new uint8_t[mods_amt]);
    uint8_t cur_mod = 0;
    while (faddr) {
        memory.read(faddr + offsetof(Mod, id), ids[cur_mod]);
        cur_mod++;
        memory.read(faddr, faddr);
    }
    return ids;
}

void MemoryManager::load_mod_id(uint8_t &val) {
    if (!cur_mod_addr) {
        return;
    }
    memory.read(cur_mod_addr + offsetof(Mod, id), val);
}

void MemoryManager::load_mod_size(uint16_t &val) {
    if (!cur_mod_addr) {
        return;
    }
    memory.read(cur_mod_addr + offsetof(Mod, size), val);
}

template <class T>
bool MemoryManager::load_mod_var(uint8_t offset, T &val) {
    if (!cur_mod_addr) {
        return false;
    }
    uint16_t size;
    memory.read(cur_mod_addr + offsetof(Mod, size), size);
    if (offset + sizeof(T) <= size) {
        memory.read(cur_mod_addr + sizeof(Mod) + offset, val);
        return true;
    } else {
        out("MemoryManager: Error load_mod_var: offset out of range\n");
    }
    return false;
}

template <class T>
bool MemoryManager::save_mod_var(uint16_t offset, T &val) {
    uint16_t size;
    memory.read(cur_mod_addr + offsetof(Mod, size), size);
    if (offset + sizeof(T) <= size) {
        memory.write(cur_mod_addr + sizeof(Mod) + offset, val);
        return true;
    } else {
        out("MemoryManager: Error save_mod_var: offset out of range\n");
    }
    return false;
} 