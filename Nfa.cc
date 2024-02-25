#include "nfa.h"

Nfa::Nfa(): accept{std::make_shared<State>(epsilon, true)}, start{State::startFactory(accept)} {}

Nfa::Nfa(char a): accept{new State{a, true}}, start{State::startFactory(accept)} {}

Nfa::Nfa(std::shared_ptr<State> start, std::shared_ptr<State> accept): accept{accept}, start{start} {
    if (start.use_count() == 0  || accept.use_count() == 0)
        throw std::invalid_argument("start or accept must be pointing to something.");
}

bool Nfa::match(const std::string &word) {
    std::vector<Nfa::State *> currList{start.get()};
    if (word.size() == 0) {
        currList = getNextStates(currList, epsilon);
    }
    for(char c: word) {
         currList = getNextStates(currList,  c);
    }

    return isMatch(currList);
}

void Nfa::addStates(Nfa::State *state, std::vector<Nfa::State *> &acc, char c) {
    if (state == nullptr) return;
    if (state->trans == epsilon && ((state->s1 && state->s2) || (state->s1 && state->loop.lock().get()))) {
        addStates(state->s1.get(), acc, c);
        addStates(state->s2.get(), acc, c);
        addStates(state->loop.lock().get(), acc, c);
    } else if (state->trans == c || state->trans == epsilon) {
        acc.emplace_back(state);
    }
}

std::vector<Nfa::State *> Nfa::getNextStates(const std::vector<Nfa::State *> &currStates, char c) {
    std::vector<Nfa::State *> acc;
    for (auto state: currStates) {
        addStates(state->s1.get(), acc, c);
        addStates(state->s2.get(), acc, c);
        addStates(state->loop.lock().get(), acc, c);
    }

    return acc;
}

bool Nfa::isMatch(const std::vector<Nfa::State *> &finalStates) {
    for (auto state: finalStates) {
        if (state->isAccept) {
            return true;
        }
    }

    return false;
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
