#include "NFa.h"

Nfa::Nfa(): accept{new State{epsilon, true}}, start{State::startFactory(accept)} {}

Nfa::Nfa(char a): accept{new State{a, true}}, start{State::startFactory(accept)} {}

Nfa::Nfa(State *start, State *accept): accept{accept}, start{start} {
    if (start == nullptr || accept == nullptr)
        throw std::invalid_argument("start or accept can not be nullptr");
}

Nfa::~Nfa() { delete start; }

