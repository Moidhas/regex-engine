#include "NFa.h"

Nfa::Nfa(): accept{new State{epsilon, true}}, start{State::startFactory(accept)} {}

Nfa::Nfa(char a): accept{new State{a, true}}, start{State::startFactory(accept)} {}

Nfa::Nfa(State *start, State *accept): accept{accept}, start{start} {
    if (start == nullptr || accept == nullptr)
        throw std::invalid_argument("start or accept can not be nullptr");
}

Nfa::~Nfa() { delete start; }

std::ostream &operator<<(std::ostream &out, const Nfa &nfa) {
    Queue<Nfa::State *> q;
    q.enqueue(nfa.start);

    while (!q.isEmpty()) {
        Nfa::State *curr = q.dequeue();
        std::string s = curr->trans == 0 ? "epsilon" : std::string{curr->trans};
        out << s << ", ";

        if (curr->s1) q.enqueue(curr->s1);
        if (curr->s2) q.enqueue(curr->s2);
    }

    return out;
}
