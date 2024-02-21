#include <cstdlib>
#include <iostream>
#include <sstream>
#include "queue.h"
#include "stack.h"
#include "Nfa.h"

# define TOKEN_LENGTH 16
const char concat = char(128);
const int nonValues = -3;
const int accepting = 257;
const int split = 257;

int ordonality(char c) {
    switch (c) {
        case '\\': return 2;
        case '+':
        case '?': 
        case '*': return 1;
        case concat: return 0;
        case '^':
        case '$': return -1;
        case '|': return -2;
        default: return nonValues;
    }
}

bool concatCond(char left, char right) {
    return left != '(' && ordonality(right) == nonValues && abs(ordonality(left)) != 2 && right != ')';
}

std::string concatString(const std::string &line) {
    Stack<char> rev;
    for (int i = line.size() - 1; i >= 0; --i) {
        rev.push(line[i]);
        if (i - 1 >= 0 && concatCond(line[i - 1], line[i])) {
            rev.push(concat);
        }
    }

    std::ostringstream oss;

    while (!rev.isEmpty()) {
        oss << rev.pop();
    }

    return oss.str();
}

std::string shuntingYard(const std::string &line) {
    Queue<char> q;
    Stack<char> s;
    for (char c: line) {
        if (ordonality(c) == nonValues && c != '(' && c != ')') {
            q.enqueue(c);
        } else {
            if (c == '(') {
               s.push(c);
            } else if (c == ')') {
                while(!s.isEmpty() && s.getTop() != '(') {
                    q.enqueue(s.pop());
                }
                s.pop();
            } else {
                while (!s.isEmpty() && ordonality(s.getTop()) >= ordonality(c)) {
                    q.enqueue(s.pop());
                }
                s.push(c);
            }
        } 
    }

    while (!s.isEmpty()) {
        q.enqueue(s.pop());
    }

    std::ostringstream oss;

    while (!q.isEmpty()) {
        oss << q.dequeue();
    }


    return oss.str();
}

int main(int argc, char *argv[]) {
    std::cout << Nfa{'a'} << std::endl;
}
