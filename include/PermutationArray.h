#ifndef PERMUTATIONARRAY_H
#define PERMUTATIONARRAY_H


class PermutationArray
{
    public:
        PermutationArray();
        PermutationArray(int n);
        ~PermutationArray();
        static int factorial(int n);
        static void swap(int& a, int& b);
        int &operator[](int i);
        void nextPermutation();
    private:
        int size;
        int* arr;
};

#endif // PERMUTATIONARRAY_H
