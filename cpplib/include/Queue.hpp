#pragma once

#define MAX_SIZE 20
namespace cpplib {
enum ErrorCode {
    success,
    underflow,
    overflow
};

template<class T>
class Queue {
public:
    Queue() : count(0), beg(0), end(0) {}

    ErrorCode pop();

    ErrorCode append(const T &);

    unsigned int size() const;

    T& top();   //get the head of the queue

    bool empty();

    bool full();

    void clear();

private:
    T list[MAX_SIZE + 1];
    unsigned int count;
    unsigned int beg;
    unsigned int end;
};

template<class T>
ErrorCode Queue<T>::pop() {
    if (count == 0) return underflow;
    beg++;
    if (beg == MAX_SIZE) beg = 0;
    count--;
    return success;
}

template<class T>
ErrorCode Queue<T>::append(const T& item) {
    if (count == MAX_SIZE) return overflow;
    list[end] = item;
    count++;
    end++;
    if (end == MAX_SIZE) end = 0;
    return success;
}

template<class T>
unsigned int Queue<T>::size() const {
    return count;
}

template<class T>
T& Queue<T>::top() {
    return list[beg];
}

template<class T>
bool Queue<T>::empty() {
    return !count;
}

template<class T>
bool Queue<T>::full() {
    return count == MAX_SIZE;
}

template<class T>
void Queue<T>::clear() {
    count = 0;
    beg = 0;
    end = 0;
}


}