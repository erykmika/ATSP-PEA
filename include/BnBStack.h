#ifndef BNBSTACK_H
#define BNBSTACK_H

#include "BnBNode.h"

/*
    Klasa przechowujaca stos wskaznikow do wierzcholkow badanych w algorytmie BnB
*/
class BnBStack
{
    public:
        // Konstruktor
        BnBStack();
        // Dodaj wskaznik na stos
        void push(BnBNode* node);
        // Zwrocenie wskaznika na szczycie stosu
        BnBNode* peek() const;
        // Zdejmij wskaznik ze szczytu
        void pop();
        // Czy stos jest pusty
        bool isEmpty() const;
    private:
        // Wskaznik na szczyt stosu
        BnBNode* top;
};

// Dealokacja pamieci wskazanej przez wskazniki odbywa sie manualnie w metodzie klasy Graph

#endif // BNBSTACK_H
