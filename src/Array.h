/*
    MIT License

    Copyright (c) 2024 MMZBin

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#ifndef MMZ_ARRAY_H
#define MMZ_ARRAY_H

#include <Arduino.h>

/*
    WARNING: If you access an Array object with the subscript operator when the array is not set, the program will crash.
             This is because the exception mechanism is not available in the Arduino environment.
*/

template<typename T>
class Array {
public:
    Array() : array_(nullptr), size_(0), isSealed_(false) {}
    Array(uint16_t size, T defaultValue, bool isSealed=false) : array_(nullptr), size_(size), isSealed_(isSealed) {
        if (size_ != 0) {
            array_ = (T *)malloc(size_ * sizeof(T));

            for (uint16_t i = 0; i < size_; i++) {
                array_[i] = defaultValue;
            }
        }
    }

    T& operator[](const uint16_t index) {
        #ifdef assert
            assert((size_ != 0) && (array_ != nullptr));
        #endif

        if (index >= size_) {
            return array_[size_ - 1];
        }

        return array_[index];
    }

    uint8_t add(T item) {
        if (isSealed_) { return 1; }

        size_++;

        if (array_ == nullptr) {
            array_ = (T *)malloc(sizeof(T));
        } else {
            T* array = (T *)realloc(array_, size_ * sizeof(T));
            if (array == NULL) { return 1; }
            array_ = array;
        }

        if (array_ == NULL) { return 1; }

        array_[size_ - 1] = item;

        return 0;
    }

    uint8_t remove(uint16_t index) {
        if ((size_ == 0) || (index >= size_) || (array_ == nullptr) || (isSealed_)) { return 1; }

        if (size_ == 0) { array_ = nullptr; }

        for (uint16_t i = index; i < size_ - 1; i++) {
            array_[i] = array_[i + 1];
        }

        size_--;

        if (size_ > 0) {
            array_ = (T *)realloc(array_, size_ * sizeof(T));
            if (array_ == NULL) { return 1; }
        } else {
            free(array_);
            array_ = nullptr;
        }

        return 0;
    }

    void seal() { isSealed_ = true; }

    bool isSealed() { return isSealed_; }

    T* getRawArray() const {
        return array_;
    }

    uint16_t getSize() const { return size_; }

    T *begin() { return array_; }
    const T *begin() const { return array_; }
    T *end() { return array_ + size_; }
    const T *end() const { return array_ + size_; }

    ~Array() {
        free(array_);
    }

private:
    T* array_;
    uint16_t size_;

    bool isSealed_;
};

#endif
