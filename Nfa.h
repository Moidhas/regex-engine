#ifndef __NFA_H__
#define __NFA_H__
#include <stdexcept>

const char epsilon = 0;

class Nfa {
    struct State {
        char trans;
        bool isAccept;
        State *s1;
        State *s2;
        State(char trans, bool isAccept, State *s1 = nullptr, State *s2 = nullptr):
            trans{trans}, isAccept{isAccept}, s1{s1}, s2{s2} {}

        ~State() {
            delete s1;
            s1 = nullptr;
            delete s2;
            s2 = nullptr;
        }

        static State *startFactory(State *s1 = nullptr, State *s2 = nullptr) {
            return new State{epsilon, false, s1, s2};
        }

    };

    State *accept;
    State *start;

    static inline void cleanup(Nfa *n1, Nfa *n2) {
        Nfa *arr[2] = {n1, n2};
        for (int i = 0; i < 2; ++i) {
            if (arr[i]) {
                arr[i]->accept = nullptr;
                arr[i]->start = nullptr;
            }
        }
    }

public:
    Nfa();
    Nfa(char a);
    Nfa(State *start, State *accept);
    ~Nfa(); 

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

#endif 
