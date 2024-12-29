#pragma once

#include "stdint.h"

template <class T>
struct RangeIterator
{
    using iterator = RangeIterator;

    RangeIterator(T value, T step) : _value(value), _step(step) {}

    T operator*() const { return _value; }

    iterator& operator++() { _value += _step; return *this; }

    iterator operator++(int) { RangeIterator tmp = *this; ++(*this); return tmp; }

    friend bool operator==(const iterator& a, const iterator& b) { return a._value >= b._value; };

    friend bool operator!=(const iterator& a, const iterator& b) { return !(a == b); };

private:
    T _value;
    T _step;
};

template <class T>
struct RangeReverseIterator
{
    using iterator = RangeReverseIterator;

    RangeReverseIterator(T value, T step) : _value(value), _step(step) {}

    T operator*() const { return _value - 1; }

    iterator& operator++() { _value -= _step; return *this; }

    iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }

    friend bool operator==(const iterator& a, const iterator& b) { return a._value <= b._value; };

    friend bool operator!=(const iterator& a, const iterator& b) { return !(a == b); };

private:
    T _value;
    T _step;
};

template <class T, template <class> class Iterator = RangeIterator>
struct Range
{
    static constexpr bool is_reversed = fl::is_same<Iterator<T>, RangeReverseIterator<T>>::value;

    Range(T from, T to, T step = 1) : _from(from), _to(to), _step(step) {}

    Range step(T value) const {
        return Range(_from, _to, value);
    }

    Range<T, RangeReverseIterator> reverse() const {
        static_assert(!is_reversed);
        return Range<T, RangeReverseIterator>(_to, _from, _step);
    }
    
    Iterator<T> begin() const { return Iterator<T>(_from, _step); }
    Iterator<T> end() const { return Iterator<T>(_to, 0); }

private:
    T _from;
    T _to;
    T _step;
};
