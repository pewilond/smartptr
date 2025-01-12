#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

#include "Sequence.h"
#include <memory>
#include <stdexcept>

#define EMPTY "DynamicArray is empty"
#define OUT_OF_RANGE "Index out of range"
#define SUBSEQ_ERR "Invalid subsequence indices"

template <typename T>
class DynamicArray : public Sequence<T> {
private:
    std::unique_ptr<T[]> data;
    int capacity;
    int length;

    void resize(int newCapacity) {
        auto newData = std::make_unique<T[]>(newCapacity);
        for (int i = 0; i < length; ++i) {
            newData[i] = std::move(data[i]);
        }
        data = std::move(newData);
        capacity = newCapacity;
    }

public:
    DynamicArray() : data(nullptr), capacity(0), length(0) {}

    ~DynamicArray() override = default;

    T GetFirst() const override {
        if (length == 0)
            throw std::out_of_range(EMPTY);
        return data[0];
    }

    T GetLast() const override {
        if (length == 0)
            throw std::out_of_range(EMPTY);
        return data[length - 1];
    }

    T Get(int index) const override {
        if (index < 0 || index >= length)
            throw std::out_of_range(OUT_OF_RANGE);
        return data[index];
    }

    Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override {
        if (startIndex < 0 || endIndex >= length || startIndex > endIndex)
            throw std::out_of_range(SUBSEQ_ERR);

        auto* subseq = new DynamicArray<T>();
        for (int i = startIndex; i <= endIndex; ++i) {
            subseq->Append(data[i]);
        }
        return subseq;
    }

    int GetLength() const override {
        return length;
    }

    void Append(const T& item) override {
        if (length == capacity) {
            int newCapacity = capacity == 0 ? 1 : capacity * 2;
            resize(newCapacity);
        }
        data[length++] = item;
    }

    void Prepend(const T& item) override {
        if (length == capacity) {
            int newCapacity = capacity == 0 ? 1 : capacity * 2;
            resize(newCapacity);
        }
        for (int i = length; i > 0; --i) {
            data[i] = std::move(data[i - 1]);
        }
        data[0] = item;
        ++length;
    }

    void InsertAt(const T& item, int index) override {
        if (index < 0 || index > length)
            throw std::out_of_range(OUT_OF_RANGE);
        if (length == capacity) {
            int newCapacity = capacity == 0 ? 1 : capacity * 2;
            resize(newCapacity);
        }
        for (int i = length; i > index; --i) {
            data[i] = std::move(data[i - 1]);
        }
        data[index] = item;
        ++length;
    }

    Sequence<T>* Concat(Sequence<T>* list) const override {
        auto* newArray = new DynamicArray<T>();
        for (int i = 0; i < length; ++i) {
            newArray->Append(data[i]);
        }
        for (int i = 0; i < list->GetLength(); ++i) {
            newArray->Append(list->Get(i));
        }
        return newArray;
    }
};

#endif // DYNAMICARRAY_H
