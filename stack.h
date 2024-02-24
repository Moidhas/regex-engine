#ifndef __STACK_H__
#define __STACK_H__

#include <stdexcept>
#include <utility>

template <typename T> class Stack {
    int top;
    int capacity;
    T *buffer;
    
    void expandAlloc() {
        capacity *= 2;
        T *newBuffer = new T[capacity];
        for (int i = 0; i < top; ++i) {
            newBuffer[i] = std::move(buffer[i]);
        }
        delete[] buffer;
        buffer = newBuffer;
    }

public:
    Stack(): top{-1}, capacity{4}, buffer{new T[capacity]} {}

    ~Stack() { delete[] buffer;  }

    void push(T item) {
        ++top;
        if (top == capacity) {
            expandAlloc();
        }

        buffer[top] = item;
    }

    bool isEmpty() { return top == -1; }

    T pop() { 
        if (top < 0) throw std::out_of_range{"popping top of empty stack"};
        return buffer[top--]; 
    }

    T getTop() { 
        if (top < 0) throw std::out_of_range{"accesing top of empty stack"};
        return buffer[top]; 
    }
};

#endif 
