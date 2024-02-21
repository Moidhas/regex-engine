#ifndef __QUEUE_H_
#define __QUEUE_H_

template <typename T> class Node {
    T data;
    Node<T> *next;
public:
    Node(T data, Node<T> *next = nullptr): data{data}, next{next} {}
    void setNext(Node<T> *node) { next = node; }
    Node<T> *getNext() { return next; }
    T getValue() { return data; }
};

template <typename T> class Queue {
    Node<T> *head;
    Node<T> *tail;
public:
    Queue(): head{nullptr}, tail{nullptr} {}

    ~Queue() {
        while (head != nullptr) {
            Node<T> *nextNode = head->getNext();
            delete head;
            head = nextNode;
        }
        tail = nullptr;
    }

    void enqueue(T item) {
        Node<T> *node = new Node<T>{item};
        if (tail == nullptr) {
            tail = node;
            head = tail;
        } else {
            tail->setNext(node);
            tail = node;
        }
    }

    bool isEmpty() {
        return head == nullptr;
    }

    T dequeue() { 
        T value = head->getValue();
        Node<T> *next = head->getNext();
        if (head == tail) tail = next;
        delete head;
        head = next;
        return value;
    }
};

#endif // __QUEUE_H_
