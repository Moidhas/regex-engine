#ifndef __REG_H__
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include "queue.h"
#include "stack.h"
#include "nfa.h"

class Regex {
    // maps characters in pattern to numerical values to describe operator precedence. 
    int ordonality(char c);
    // boring method.
    bool concatCond(char left, char right);
    // adds a concatenation operator to pattern. 
    std::string concatString(const std::string &pattern);
    // changes regex pattern to postfix notation. 
    std::string shuntingYard(const std::string &pattern); 
    Nfa patternToNfa(const std::string &pattern); 
    Nfa nfa;
public: 
    Regex(const std::string &pattern);
    // only returns true if regex pattern matches line.
    bool match(const std::string &line);
};

#endif // !__REG_H__
