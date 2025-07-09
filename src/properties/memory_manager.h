#pragma once

#include "libs/file_handler.h"
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

    MemoryManager(const char* filename);
    MemoryManager(const MemoryManager&) = delete;
    MemoryManager& operator= (const MemoryManager&) = delete;

    // возвращает 0 если режимов нет
    uint16_t get_first_mod_addr();
    bool set_cur_mod(uint16_t val);
    void mod_memory_shift(uint16_t src_addr, uint16_t dst_addr);

    // ребалансировка памяти, нужна когда закончилось место в конце файла и были
    // удалены какие то режимы. Т.е. в памяти есть пустые не используемы участки.
    // Все данные смещаются так, чтобы закрыть неиспользуемые пробелы памяти
    void rebalance_mod_list();
    bool add_mod_with_rebalace(uint8_t mod_id, uint16_t mod_size, bool need_rebalace);
    uint16_t get_prev_link_addr(uint16_t addr);
    uint16_t get_mod_addr_by_num(uint8_t num, uint16_t &prev_addr);
    uint16_t get_mod_addr_by_num(uint8_t num);
    bool remove_mode_by_addr(uint16_t addr, uint16_t prev_addr);
    bool remove_mode_by_addr(uint16_t addr);

public:
    ~MemoryManager() = default;
    static MemoryManager& instance();

    // Занулить всю память
    void clear_memory();

    // ----------------- Функции изменения списка модов -----------------

    // Добавление нового режима в конец списка с заданным id и size.
    // После добавления режима, указатель на текущий режим установится
    // на только что добавленный.
    // Все поля аргументов режима могут содержать мусор, поэтому нужно их
    // инициализировать. Значения аргументов режима устанавливается
    // функцией save_mod_var
    bool add_mod(uint8_t id, uint16_t size);

    // Удаление текущего выбранного режима
    // При удалении в качестве текущего выбирается следующий режим,
    // если он существует, иначе предыдущий. Если режимов не осталось,
    // текущей режим будет установлен в 0.
    bool remove_mod();

    // Удаление режима в определённой позиции.
    bool remove_mod(uint8_t num);

    // Уданение всех режимов из памяти (по сути зануление указателя на первый режим)
    void remove_all_mods();

    // --------------- Функции выбора текущего режима ----------------

    // Сменить указатель текущего режима на следующий.
    // Если текущий режим последний, то выбирается первый.
    void next_mod();

    // Сменить указатель текущего режима на предыдущий.
    // Если текущий режим первый, то выбирается последний.
    void prev_mod();

    // Установить указатель текущего режима в конкретную позицию.
    // начальный режим имеет индекс 1
    bool set_mod(uint8_t num);

    // получить номер текущего режима в памяти
    // начальный режим имеет индекс 1
    uint8_t get_cur_mod_num();

    // получить кол-во режимов из памяти
    uint8_t get_mod_amount();

    // получить id всех режимов из памяти
    // возвращает область памяти со списком режимов
    std::unique_ptr<uint8_t[]> get_mod_list();

    // --------------- Функции работы с текущим режимом ----------------

    uint8_t load_mod_id();
    uint16_t load_mod_size();

    template <class T>
    bool load_mod_var(uint8_t offset, T &val);

    // Сохранение значений текущего режима в память.
    // offset считается относительно начала переменных самого режима.
    // offset = 0 это первый аргумент режима
    // offset = sizeof(first_var) это второй аргумент и т.д.
    template <class T>
    bool save_mod_var(uint16_t offset, T &val);

private:
    uint16_t _cur_mod_addr; // инициализируется при создании. 0, если режимов нет.
    FileHandler _file;
};