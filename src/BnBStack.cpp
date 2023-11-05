#include "BnBStack.h"

// Inicjalizujemy stos - szczyt jako NULL
BnBStack::BnBStack()
{
    top = NULL;
}

// Dodajemy wskaznik do stosu, zmieniamy szczyt
void BnBStack::push(BnBNode* node)
{
    node->setNext(top);
    top = node;
}

// Zwracamy wskaznik do elementu na szczycie stosu
BnBNode* BnBStack::peek() const
{
    if(top==NULL) return NULL;
    return top;
}

// Czy stos jest pusty
bool BnBStack::isEmpty() const
{
    return top==NULL;
}

// Zamieniamy szczyt stosu na element pod szczytowym elementem
void BnBStack::pop()
{
    if(top==NULL) return;
    top = top->getNext();
}

// Dealokacja pamieci wskazanej przez wskazniki odbywa sie manualnie w metodzie klasy Graph
BnBStack::~BnBStack() {}
