#pragma once

#include "stdint.h"

template<typename T, uint32_t max_size>
class Queue
{
private:
    uint32_t start_pos;
    uint32_t end_pos;
    bool is_empty;
    T buf[max_size];
public:
    Queue() {
        clear();
    };

    void clear() {
        start_pos = 0;
        end_pos = 0;
        is_empty = true;
    }

    void push(const T &tr) {
        if (size() < max_size - 1) {
            buf[end_pos] = tr;
            end_pos = (end_pos + 1) % max_size;
        } else {
            out("Queue error: overflow buffer\n");
        }
    }

    const T &front() const {
        return buf[start_pos];
    }

    void pop() {
        if (start_pos != end_pos) {
            buf[start_pos] = T{};
            start_pos = (start_pos + 1) % max_size;
        }
    }

    uint32_t size() {
        return (end_pos + max_size - start_pos) % max_size;
    }

    void debug() {
        out("bgn: %d end %d\n", start_pos, end_pos);
        for (int i = 0; i < max_size; ++i) {
            out("%d ", int(buf[i]));
        }
        out("\n");
    }
};