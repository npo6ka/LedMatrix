#include <stdio.h>
#include "effect_list/effectslist.h"
#include "memory/property.h"
#include "memory/memory_manger.h"

void setup() {
    EffectsList::getInstance().setEffect(0);
}


template <class F>
bool check_mod(MemoryManager &mmg, F func, uint8_t req_mod_num, uint8_t req_mod_amnt) {
    func();
    uint8_t mod_num = mmg.get_cur_mod_num();
    uint8_t mod_amount = mmg.get_mod_amount();
    if (mod_num != req_mod_num || mod_amount != req_mod_amnt) {
        printf("Mod Test failed: %d:%d / %d:%d\n", mod_num, req_mod_num, mod_amount, req_mod_amnt);
        return false;
    }
    return true;
}

bool check_mods() {
    MemoryManager &mmg = MemoryManager::instance();

    bool flag = true;
    auto add_new_mods = [&mmg](uint8_t id, uint16_t size, uint8_t val) {
        if (mmg.add_mod(id, size)) {
            for (int i = 0; i < size; ++i) {
                mmg.save_mod_var(i, val);
            }
        }
    };

    // memory rebalance test
    mmg.clear_memory();
    add_new_mods(0xf0, 8196, 1); // used: 8196+
    add_new_mods(0xf1, 8196, 2); // used: 16392+
    mmg.remove_mod(1);
    add_new_mods(0xf3, 4096, 3); // used: 20488+
    add_new_mods(0xf4, 8196, 4); // used: 28684+
    add_new_mods(0xf5, 4096, 5); // used: 32780+
    add_new_mods(0xf6, 8196, 6); // used: 40976+
    add_new_mods(0xf7, 8196, 7); // used: 49172+
    add_new_mods(0xf8, 4096, 8); // used: 53268+
    add_new_mods(0xf9, 4096, 9); // used: 57364+
    add_new_mods(0xfa, 8196, 0xa); // used: 65560+ должно не хватить памяти
    mmg.remove_mod(6); // delete f7
    mmg.remove_mod(6); // delete f8
    add_new_mods(0xf7, 8196, 0xc);
    add_new_mods(0xf8, 4096, 0xd);
    add_new_mods(0xfb, 8196, 0xb); // out of memory
    mmg.remove_mod(); // delete f8
    add_new_mods(0xfb, 8196, 0xb);
    add_new_mods(0xf8, 4096, 0xd); // out of memory

    flag &= check_mod(mmg, [&mmg]{ mmg.set_mod(10); }, 9, 9);
    flag &= check_mod(mmg, [&mmg]{ mmg.remove_all_mods(); }, 0, 0);

    flag &= check_mod(mmg, [&mmg]{ mmg.add_mod(0, 4); }, 1, 1);
    flag &= check_mod(mmg, [&mmg]{ mmg.add_mod(1, 8); }, 2, 2);
    flag &= check_mod(mmg, [&mmg]{ mmg.add_mod(2, 4); }, 3, 3);
    flag &= check_mod(mmg, [&mmg]{ mmg.set_mod(0); }, 3, 3);
    flag &= check_mod(mmg, [&mmg]{ mmg.set_mod(2); }, 2, 3);
    flag &= check_mod(mmg, [&mmg]{ mmg.set_mod(5); }, 2, 3);
    flag &= check_mod(mmg, [&mmg]{ mmg.add_mod(3, 16); }, 4, 4);
    flag &= check_mod(mmg, [&mmg]{ mmg.set_mod(2); }, 2, 4);
    flag &= check_mod(mmg, [&mmg]{ mmg.remove_mod(); }, 2, 3);
    flag &= check_mod(mmg, [&mmg]{ mmg.remove_mod(); }, 2, 2);
    flag &= check_mod(mmg, [&mmg]{ mmg.remove_mod(); }, 1, 1);
    flag &= check_mod(mmg, [&mmg]{ mmg.remove_mod(); }, 0, 0);
    flag &= check_mod(mmg, [&mmg]{ mmg.remove_mod(); }, 0, 0);

    flag &= check_mod(mmg, [&mmg]{ mmg.add_mod(0, 4); }, 1, 1);
    flag &= check_mod(mmg, [&mmg]{ mmg.add_mod(1, 16); }, 2, 2);
    flag &= check_mod(mmg, [&mmg]{ mmg.add_mod(2, 32); }, 3, 3);
    flag &= check_mod(mmg, [&mmg]{ mmg.add_mod(3, 20); }, 4, 4);
    flag &= check_mod(mmg, [&mmg]{ mmg.add_mod(4, 12); }, 5, 5);
    flag &= check_mod(mmg, [&mmg]{ mmg.add_mod(5, 16); }, 6, 6);

    uint8_t* mods = mmg.get_mod_list();
    if (mods[0] != 0 && mods[1] != 1 && mods[2] != 2 &&
        mods[3] != 3 && mods[4] != 4 && mods[5] != 5)
    {
        flag = false;
    }
    delete[] mods;

    flag &= check_mod(mmg, [&mmg]{ mmg.remove_mod(3); }, 5, 5);
    flag &= check_mod(mmg, [&mmg]{ mmg.remove_mod(10); }, 5, 5);
    flag &= check_mod(mmg, [&mmg]{ mmg.remove_mod(5); }, 4, 4);

    flag &= check_mod(mmg, [&mmg]{ mmg.set_mod(1); }, 1, 4);
    flag &= check_mod(mmg, [&mmg]{ mmg.prev_mod(); }, 4, 4);
    flag &= check_mod(mmg, [&mmg]{ mmg.next_mod(); }, 1, 4);
    flag &= check_mod(mmg, [&mmg]{ mmg.next_mod(); }, 2, 4);
    flag &= check_mod(mmg, [&mmg]{ mmg.next_mod(); }, 3, 4);
    flag &= check_mod(mmg, [&mmg]{ mmg.next_mod(); }, 4, 4);
    flag &= check_mod(mmg, [&mmg]{ mmg.prev_mod(); }, 3, 4);

    flag &= check_mod(mmg, [&mmg]{ mmg.remove_mod(3); }, 3, 3);
    flag &= check_mod(mmg, [&mmg]{ mmg.remove_mod(1); }, 2, 2);
    flag &= check_mod(mmg, [&mmg]{ mmg.remove_mod(1); }, 1, 1);
    flag &= check_mod(mmg, [&mmg]{ mmg.add_mod(1, 16); }, 2, 2);
    flag &= check_mod(mmg, [&mmg]{ mmg.add_mod(2, 32); }, 3, 3);
    flag &= check_mod(mmg, [&mmg]{ mmg.remove_all_mods(); }, 0, 0);

    flag &= check_mod(mmg, [&mmg]{ mmg.prev_mod(); }, 0, 0);
    flag &= check_mod(mmg, [&mmg]{ mmg.next_mod(); }, 0, 0);
    flag &= check_mod(mmg, [&mmg]{ mmg.remove_mod(); }, 0, 0);
    flag &= check_mod(mmg, [&mmg]{ mmg.remove_mod(1); }, 0, 0);
    flag &= check_mod(mmg, [&mmg]{ mmg.set_mod(1); }, 0, 0);
    flag &= check_mod(mmg, [&mmg]{ mmg.set_mod(0); }, 0, 0);
    flag &= check_mod(mmg, [&mmg]{ mmg.remove_all_mods(); }, 0, 0);

    return flag;
}

int main(int argc, char* argv[]) {
    setup();

    //check_mods();

    /*EffectsList::getInstance().onTick();
    SavedVar<uint8_t> val = 1;
    printf("a = %d\n", val.get());*/

    // EffectsList::getInstance().nextEffect();

    return 0;
}

