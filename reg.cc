#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include "queue.h"
#include "stack.h"
#include "nfa.h"

const char concat = char(128);
const int nonValues = -3;

int ordonality(char c) {
    switch (c) {
        case '+':
        case '?': 
        case '*': return 1;
        case concat: return 0;
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

Nfa regexToNfa(const std::string &pattern) {
    try {
        std::string regex = shuntingYard(concatString(pattern));
        Stack<Nfa> s;
        for (char c: regex) {
            if (c == '+') {
                s.push(Nfa::atLeastOne(s.pop()));
            } else if (c == '?') {
                s.push(Nfa::oneOrZero(s.pop()));
            } else if (c == '*') {
                s.push(Nfa::kleene(s.pop()));
            } else if (c == concat) {
                Nfa n2 = s.pop();
                Nfa n1 = s.pop();
                s.push(Nfa::concat(n1, n2));
            } else if (c == '|') {
                Nfa n2 = s.pop();
                Nfa n1 = s.pop();
                s.push(Nfa::unions(n1, n2));
            } else {
                s.push(Nfa{c});
            }
        }
        return s.pop();
    } catch (std::out_of_range r) {
        throw std::runtime_error{"regex pattern has an invalid format"};
    }
}

int main(int argc, char *argv[]) {
    std::cout << shuntingYard(concatString(argv[1])) << std::endl;
    try {
        Nfa nfa = regexToNfa(argv[1]);
        std::string b = nfa.match(argv[2]) ? "match" : "not matched";
        std::cout << b << std::endl;
    } catch (std::out_of_range r) {
        std::cerr << r.what() << std::endl; 
    }
}
