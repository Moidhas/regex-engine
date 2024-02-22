#include "nfa.h"
#include <memory>

Nfa::Nfa(): accept{std::make_shared<State>(epsilon, true)}, start{State::startFactory(accept)} {}

Nfa::Nfa(char a): accept{new State{a, true}}, start{State::startFactory(accept)} {}

Nfa::Nfa(std::shared_ptr<State> start, std::shared_ptr<State> accept): accept{accept}, start{start} {
    if (start.use_count() == 0  || accept.use_count() == 0)
        throw std::invalid_argument("start or accept must be pointing to something.");
}

std::ostream &operator<<(std::ostream &out, const Nfa &nfa) {
    Queue<Nfa::State *> q;
    q.enqueue(nfa.start.get());

    while (!q.isEmpty()) {
        Nfa::State *curr = q.dequeue();
        std::string s = curr->trans == 0 ? "epsilon" : std::string{curr->trans};
        out << s << ", ";

        if (curr->s1) q.enqueue(curr->s1.get());
        if (curr->s2) q.enqueue(curr->s2.get());
        if (!curr->loop.expired()) out << "loop" << ", ";
    }

    return out;
}
