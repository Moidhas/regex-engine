#include <stdexcept>
const char epsilon = 0;

class State {
public:
    char trans;
    bool isAccept;
    State *s1;
    State *s2;
    State(char trans, bool isAccept, State *s1 = nullptr, State *s2 = nullptr):
        trans{trans}, isAccept{isAccept}, s1{s1}, s2{s2} {
    }

    static State *startFactory(State *s1 = nullptr, State *s2 = nullptr) {
        return new State{epsilon, false, s1, s2};
    }
};

// Invariants: accept and start are not nullptr.
class Nfa {
    State *accept;
    State *start;

    static void cleanup(Nfa *n1, Nfa *n2) {
        Nfa *arr[2] = {n1, n2};
        for (int i = 0; i < 2; ++i) {
            if (arr[i]) {
                arr[i]->accept = nullptr;
                arr[i]->start = nullptr;
            }
        }
    }

public:
    Nfa(): accept{new State{epsilon, true}}, start{State::startFactory(accept)} {}
    Nfa(char a): accept{new State{a, true}}, start{State::startFactory(accept)} {}
    Nfa(State *start, State *accept): accept{accept}, start{start} {
        if (start == nullptr || accept == nullptr) throw std::invalid_argument("start or  accept can not be nullptr");
    }
    ~Nfa() {
        // TODO
    }

    static Nfa *unions(Nfa *n1, Nfa *n2) {
        if (n1 == nullptr || n2 == nullptr) return n1 ? n1 : n2;
        State *start = State::startFactory(n1->start, n2->start);
        State *accept = new State{epsilon, true};
        
        // allows for destruction of useless Nfas.
        Nfa *arr[2] = {n1, n2};
        for (int i = 0; i < 2; ++i) {
            arr[i]->accept->isAccept = false;
            arr[i]->accept->s1 = accept;
            arr[i]->accept = nullptr;
            arr[i]->start = nullptr;
        }

        return new Nfa{start, accept};
    }


    static Nfa *concat(Nfa *n1, Nfa *n2) {
        if (n1 == nullptr || n2 == nullptr) return n1 ? n1 : n2;
        State *start = n1->start;
        State *accept = n2->accept;
        n1->accept->isAccept = false;
        n1->accept->s1 = n2->start->s1;
        n1->accept->s2 = n2->start->s2;

        delete n2->start;

        cleanup(n1, n2);

        return new Nfa{start, accept};
    }

    static Nfa *kleen(Nfa *n1) {
        State *accept = new State{epsilon, true};
        State *start = State::startFactory(n1->start, accept);
        n1->accept->isAccept = false;
        n1->accept->s1 = accept;
        n1->accept->s2 = n1->start;
        cleanup(n1, nullptr);

        return new Nfa{start, accept};
    }
};

