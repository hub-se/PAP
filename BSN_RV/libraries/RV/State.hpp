#ifndef STATE_HPP
#define STATE_HPP

#include <Arduino.h>
#include <List.hpp>
#include "Transition.hpp"
#include "Invariant.hpp"

class State {
  private:
    String name;
    Invariant* inv;


  public:
    List<Transition*> outgoingTransitions;
    List<Transition*> incomingTransitions;
    State(String name);
    ~State();
    void addIncomingTransition(Transition* t);
    void addOutgoingTransition(Transition* t);
    List<Transition*> getOutgoingTransitions();
    void removeIncomingTransition(Transition* t);
    void removeOutgoingTransition(Transition* t);
    String toString();
    bool checkInvariant(unsigned int clock);
    unsigned int nextInvariantChange(unsigned int clock);
    void setInvariant(Invariant* inv);

    void del();

    static int cnt; 
};



#endif
