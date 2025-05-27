#pragma once

// #include "LittleFS.h"
#include <vector>
#include "memory_handler.h"
#include "string.h"
#include "math.h"
#include <memory>

#define MAX_MODS_SIZE 256
#define MM_FILE_NAME "mods.dat"

class MemoryManager {
    struct Mod {
        uint16_t next_addr;
        uint8_t id;
        uint16_t size;
    };

    struct DefaultData {
        uint16_t cur_mod;
        uint16_t first_mod_addr;
    };

    uint16_t cur_mod_addr; // инициализируется при создании. 0, если режимов нет.
    Memory memory;

    MemoryManager(const char* filename) {
        memory = Memory{filename};
        memory.read(offsetof(DefaultData, cur_mod), cur_mod_addr);
    }

    MemoryManager(const MemoryManager&) = delete;
    MemoryManager& operator= (const MemoryManager&) = delete;

    // возвращает 0 если режимов нет
    uint16_t get_first_mod_addr() {
        uint16_t addr = 0;
        memory.read(offsetof(DefaultData, first_mod_addr), addr);
        return addr;
    }

    bool set_cur_mod(uint16_t val) {
        if (val == 0 || val >= sizeof(DefaultData)) {
            cur_mod_addr = val;
// write delay
            memory.write(offsetof(DefaultData, cur_mod), val);
            return true;
        } else {
            out("MemoryManager: Error set_cur_mod: out of memory\n");
            return false;
        }
    }

    void mod_memory_shift(uint16_t src_addr, uint16_t dst_addr) {
        Mod mod;
        memory.read(src_addr, mod);
        memory.write(dst_addr, mod);

        //printf("src:%d dst:%d next_addr:%d id:%d size:%d\n", );
        for (uint16_t i = 0; i < mod.size; ++i) {
            uint8_t tmp;
            memory.read(src_addr + sizeof(Mod) + i, tmp);
            memory.write(dst_addr + sizeof(Mod) + i, tmp);
        }

        if (cur_mod_addr == src_addr) {
            set_cur_mod(dst_addr);
        }
    }

    // ребалансировка памяти, нужна когда закончилось место в конце файла и были
    // удалены какие то режимы. Т.е. в памяти есть пустые не используемы участки.
    // Все данные смещаются так, чтобы закрыть неиспользуемые пробелы памяти
    void rebalance_mod_list() {
        uint16_t cur_addr = get_first_mod_addr();
        uint16_t offset = 0;
        Mod mod;

        if (!cur_addr) { // если режимов нет, ливаем
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

    bool add_mod_with_rebalace(uint8_t mod_id, uint16_t mod_size, bool need_rebalace) {
        uint16_t addr = cur_mod_addr;
        uint16_t addr_tmp = addr;
        uint32_t new_addr;
        uint16_t size;

        if (addr) {
            while (addr_tmp) { // ищем адрес последнего режима
                memory.read(addr, addr_tmp); // читаем значение по текущему адресу и кладём в переменную этого адреса
                addr = addr_tmp? addr_tmp : addr;
            }
            memory.read(addr + offsetof(Mod, size), size); // читаем сайз последнего режима
            new_addr = (uint32_t)addr + size + sizeof(Mod); // добавляем к текущему адресу сайз и размер структуры Mod, получаем адрес следующего режима
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

    uint16_t get_prev_link_addr(uint16_t addr) {
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

    uint16_t get_mod_addr_by_num(uint8_t num, uint16_t &prev_addr) {
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

    uint16_t get_mod_addr_by_num(uint8_t num) {
        uint16_t prev_addr;
        return get_mod_addr_by_num(num, prev_addr);
    }

    bool remove_mode_by_addr(uint16_t addr, uint16_t prev_addr) {
        uint16_t next_addr;

        if (!cur_mod_addr || !addr || !prev_addr) {
            return false;
        }

        memory.read(addr, next_addr);
        memory.write(prev_addr, next_addr);

        if (!next_addr && prev_addr == offsetof(DefaultData, first_mod_addr)) { // если режимов больше нет
            set_cur_mod(0);
        } else if (cur_mod_addr == addr) { // если удалённый режим был текущим
            if (!next_addr) { // если удалили последний режим
                set_cur_mod(prev_addr); // устанавливаем в качестве текущего предыдущий
            } else {
                set_cur_mod(next_addr); // иначе следующий
            }
        }

        return true;
    }

    bool remove_mode_by_addr(uint16_t addr) {
        return remove_mode_by_addr(addr, get_prev_link_addr(addr));
    }

public:
    ~MemoryManager() {}

    static MemoryManager& instance() {
        static MemoryManager instance(MM_FILE_NAME);
        return instance;
    }

    void clear_memory() {
        memory.clear();
    }

    // ----------------- Функции изменения списка модов -----------------

    // Добавление нового режима в конец списка с заданным id и size.
    // После добавления режима, указатель на текущий режим установится
    // на только что добавленный.
    // Все поля аргументов режима могут содержать мусор, поэтому нужно их
    // инициализировать. Значения аргументов режима устанавливается
    // функцией save_mod_var
    bool add_mod(uint8_t id, uint16_t size) {
        return add_mod_with_rebalace(id, size, true);
    }

    // Удаление текущего выбранного режима
    // При удалении в качестве текущего выбирается следующий режим,
    // если он существует, иначе предыдущий. Если режимов не осталось,
    // текущей режим будет установлен в 0.
    bool remove_mod() {
        return remove_mode_by_addr(cur_mod_addr);
    }

    // Удаление режима в определённой позиции.
    bool remove_mod(uint8_t num) {
        uint16_t prev_addr;
        uint16_t addr = get_mod_addr_by_num(num, prev_addr);
        return remove_mode_by_addr(addr, prev_addr);
    }

    // Уданение всех режимов из памяти (по сути зануление указателя на первый режим)
    void remove_all_mods() {
        set_cur_mod(0);
        memory.write(offsetof(DefaultData, first_mod_addr), 0);
    }

    // --------------- Функции выбора текущего режима ----------------

    // Сменить указатель текущего режима на следующий.
    // Если текущий режим последний, то выбирается первый.
    void next_mod() {
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

    // Сменить указатель текущего режима на предыдущий.
    // Если текущий режим первый, то выбирается последний.
    void prev_mod() {
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

    // Установить указатель текущего режима в конкретную позицию.
    // начальный режим имеет индекс 1
    bool set_mod(uint8_t num) {
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

    // получить номер текущего режима в памяти
    // начальный режим имеет индекс 1
    uint8_t get_cur_mod_num() {
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

    // получить кол-во режимов из памяти
    uint8_t get_mod_amount() {
        uint16_t addr = get_first_mod_addr();
        uint8_t mods_amt = 0;

        while (addr) {
            memory.read(addr, addr);
            mods_amt++;
        }
        return mods_amt;
    }

    // получить id всех режимов из памяти
    // возвращает область памяти со списком режимов
    std::unique_ptr<uint8_t[]> get_mod_list() {
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

    // --------------- Функции работы с текущим режимом ----------------

    void load_mod_id(uint8_t &val) {
        if (!cur_mod_addr) {
            return;
        }
        memory.read(cur_mod_addr + offsetof(Mod, id), val);
    }

    void load_mod_size(uint16_t &val) {
        if (!cur_mod_addr) {
            return;
        }
        memory.read(cur_mod_addr + offsetof(Mod, size), val);
    }

    template <class T>
    bool load_mod_var(uint8_t offset, T &val) {
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

    // Сохранение значений текущего режима в память.
    // offset считается относительно начала переменных самого режима.
    // offset = 0 это первый аргумент режима
    // offset = sizeof(first_var) это второй аргумент и т.д.
    template <class T>
    bool save_mod_var(uint16_t offset, T &val) {
        uint16_t size;
        memory.read(cur_mod_addr + offsetof(Mod, size), size);
        if (offset + sizeof(T) <= size) {
            // write delay;
            memory.write(cur_mod_addr + sizeof(Mod) + offset, val);
            return true;
        } else {
            out("MemoryManager: Error save_mod_var: offset out of range\n");
        }
        return false;
    }
};