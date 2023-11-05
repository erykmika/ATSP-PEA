#ifndef BNBSTACK_H
#define BNBSTACK_H

#include "BnBNode.h"

/*
    Klasa przechowujaca stos wskaznikow do wierzcholkow badanych w algorytmie BnB
*/
class BnBStack
{
    public:
        BnBStack();
        void push(BnBNode* node);
        BnBNode* peek() const;
        void pop();
        bool isEmpty() const;
        ~BnBStack();
    private:
        BnBNode* top;
};

#endif // BNBSTACK_H
