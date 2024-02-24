#ifndef __NFA_H__
#define __NFA_H__
#include <stdexcept>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "queue.h"

const char epsilon = 0;

class Nfa {
    struct State {
        // describes the transition to this state. 
        char trans;
        bool isAccept;
        std::shared_ptr<State> s1;
        std::shared_ptr<State> s2;
        std::weak_ptr<State> loop;
        State(char trans, bool isAccept, std::shared_ptr<State> s1 = nullptr, std::shared_ptr<State> s2 = nullptr):
            trans{trans}, isAccept{isAccept}, s1{s1}, s2{s2} {}

        static std::shared_ptr<State> startFactory(std::shared_ptr<State> s1 = nullptr, std::shared_ptr<State> s2 = nullptr) {
            return std::make_shared<State>(epsilon, false, s1, s2);
        }

    };

    std::shared_ptr<State> accept;
    std::shared_ptr<State> start;
    Nfa(std::shared_ptr<State> start, std::shared_ptr<State> accept);

    // given a set of current states returns all possible states reachable by one transition. 
    std::vector<State *> getNextStates(const std::vector<State *> &currStates, char c);
    // adds all states transitioned from state that are not epsilon to acc.
    void addStates(Nfa::State *state, std::vector<Nfa::State *> &acc, char c);

    // returns true if any of the states in finalStates is an accepting state. 
    bool isMatch(const std::vector<State *> &finalStates);

public:
    // creates a start state that has an epsilon transition to an accepting state.
    Nfa();

    // creates a start state that has a char a transition to an accepting state.
    Nfa(char a);
    
    // returns true if the word is accepted by the language described by the Nfa. 
    bool match(const std::string &word);

    friend std::ostream &operator<<(std::ostream &out, const Nfa &nfa);

    // the following static methods invalidate n1, and n2. 
    static Nfa unions(Nfa n1, Nfa n2) {
        auto start = State::startFactory(n1.start, n2.start);
        auto accept = std::make_shared<State>(epsilon, true);
        
        Nfa *arr[2] = {&n1, &n2};
        for (int i = 0; i < 2; ++i) {
            arr[i]->accept->isAccept = false;
            arr[i]->accept->s1 = accept;
        }

        return Nfa{start, accept};
    }

    static Nfa concat(Nfa n1, Nfa n2) {
        auto start = n1.start;
        auto accept = n2.accept;
        n1.accept->isAccept = false;
        n1.accept->s1 = n2.start->s1;
        n1.accept->s2 = n2.start->s2;

        return Nfa{start, accept};
    }

    static Nfa kleene(Nfa n1) {
        auto accept = std::make_shared<State>(epsilon, true);
        auto start = State::startFactory(n1.start, accept);
        n1.accept->isAccept = false;
        n1.accept->s1 = accept;
        n1.accept->loop = n1.start;

        return  Nfa{start, accept};
    } 

    static Nfa oneOrZero(Nfa n1) {
        auto accept = std::make_shared<State>(epsilon, true);
        auto start = State::startFactory(n1.start, accept);
        n1.accept->isAccept = false;
        n1.accept->s1 = accept;

        return Nfa{start, accept};
    }

    static Nfa atLeastOne(Nfa n1) {
        auto start = State::startFactory(n1.start);
        auto accept = std::make_shared<State>(epsilon, true);

        n1.accept->isAccept = false;
        n1.accept->s1 = accept;
        n1.accept->loop = n1.start;

        return  Nfa{start, accept};
    }
};

#endif 
