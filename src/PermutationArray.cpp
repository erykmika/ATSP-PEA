#include "PermutationArray.h"

/*
    Konstruktor domyslny - wywolanie konstruktora tworzacego obiekt zawierajacy 10-elementowa
    tablice elementow.
*/
PermutationArray::PermutationArray(): PermutationArray(10) {}

/*
    Zakladamy, ze na potrzeby algorytmu ATSP bedziemy tworzyc permutacje wierzcholkow od 1 do n-1.
    W glownym algorytmie ATSP wszystkie cykle zaczynaja sie od wierzcholka 0.
*/
PermutationArray::PermutationArray(int n)
{
    size = n;
    arr = new int[size];
    for(int i=0; i<size; i++)
        arr[i] = i+1;
}

// Dealokacja pamieci
PermutationArray::~PermutationArray()
{
    delete [] arr;
}

// Obliczanie silni - liczba mozliwych permutacji, pomocniczna metoda statyczna
unsigned long long PermutationArray::factorial(int n)
{
    if(n==1) return 1;
    return (n * factorial(n-1));
}

// Zamiana elementow miejscami - pomocniczna metoda statyczna
void PermutationArray::swap(int& a, int& b)
{
    int temp = a;
    a = b;
    b = temp;
}

// Dostep do elementow wewnetrznej tablicy obiektu klasy za pomoca operatora []
int& PermutationArray::operator[](int i) { return arr[i]; }

// Glowna metoda realizujaca algorytm znajdowania leksykograficznego nastepnika obecnej permutacji w tablicy.
void PermutationArray::nextPermutation()
{
    int left_index = 0;
    int left_value = 0;

    /*
        Przechodzac po tablicy od prawej strony, znajdujemy pierwszy element, ktory nie jest w porzadku rosnacym
        w stosunku do poprzedniego elementu, np. 1-2-4-3 => 2
    */
    for(int j=size-2; j>=0; j--)
    {
        if(arr[j]<arr[j+1]){
            left_index = j;
            left_value = arr[j];
            break;
        }
    }

    int right_index = size-1;
    int right_value = size+1;

    /*
        Nastepnie przechodzimy ponownie od prawej strony i znajdujemy najmniejszy element wiekszy od wczesniej znalezionego elementu,
        np. 1-[2]-4-3 => 3
    */
    for(int j=right_index; j>left_index; j--)
    {
        if(arr[j] > left_value && arr[j] < right_value)
        {
            right_index = j;
            right_value = arr[j];
        }
    }

    // Zamieniamy elementy z poprzednich krokow miejscami np. 1-2-4-3 => 1-3-4-2
    swap(arr[left_index], arr[right_index]);

    /*
        Odwracamy czesc tablicy od indeksu [z pierwszego kroku + 1] do konca tablicy
        1-3-4-2 => 1-3-2-4. W ten sposob otrzymujemy poszukiwana nastepna permutacje.
    */
    int l = left_index + 1;
    int r = size-1;
    while(l<=r) swap(arr[l++], arr[r--]);
}
