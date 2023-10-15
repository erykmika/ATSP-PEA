#include "PermutationArray.h"

PermutationArray::PermutationArray(): PermutationArray(10) {}

PermutationArray::PermutationArray(int n)
{
    size = n;
    arr = new int[size];
    for(int i=0; i<size; i++)
        arr[i] = i+1;
}

PermutationArray::~PermutationArray()
{
    delete [] arr;
}

int PermutationArray::factorial(int n)
{
    if(n==1) return 1;
    return (n * factorial(n-1));
}

void PermutationArray::swap(int& a, int& b)
{
    int temp = a;
    a = b;
    b = temp;
}

int& PermutationArray::operator[](int i) { return arr[i]; }

void PermutationArray::nextPermutation()
{
    int left_index = 0;
    int left_value = 0;

    for(int j=size-2; j>=0; j--)
    {
        if(arr[j]<arr[j+1]){
            left_index = j;
            left_value = arr[j];
            break;
        }
    }

    int right_index = size-1;
    int right_value = size;

    for(int j=right_index; j>left_index; j--)
    {
        if(arr[j] > left_value && arr[j] < right_value)
        {
            right_index = j;
            right_value = arr[j];
        }
    }

    swap(arr[left_index], arr[right_index]);

    int l = left_index + 1;
    int r = size-1;
    while(l<=r) swap(arr[l++], arr[r--]);
}
