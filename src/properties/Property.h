#pragma once

enum {
    PR_UINT8 = 0,
    PR_UINT16,
    PR_UINT32,
    PR_UINT64,
    PR_FLOAT,
    PR_DOUBLE,
    PR_CHAR,
    UNSUPPORT
};

template<typename T>
class Property
{
    T val;

    uint8_t get_type_for_val(uint8_t val) {
        return PR_UINT8;
    }

    uint8_t get_type_for_val(uint16_t val) {
        return PR_UINT16;
    }

    uint8_t get_type_for_val(uint32_t val) {
        return PR_UINT32;
    }

    uint8_t get_type_for_val(uint64_t val) {
        return PR_UINT64;
    }

    uint8_t get_type_for_val(float val) {
        return PR_FLOAT;
    }

    uint8_t get_type_for_val(double val) {
        return PR_DOUBLE;
    }

    uint8_t get_type_for_val(char *val) {
        return PR_CHAR;
    }

    uint8_t get_type_for_val(T val) {
        return UNSUPPORT;
    }
public:
    Property() = default;
    ~Property() = default;

    T get() {
        return val;
    }

    void set(T new_val) {
        val = new_val;
    }

    size_t get_size() {
        return sizeof(val);
    }

    uint8_t get_type() {
        return get_type_for_val(val);
    }
};
