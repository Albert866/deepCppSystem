/**
 * @file 1_template_arg_value.cpp
 * @brief 模板参数默认值
 * @author Albert
 * @version 1.0.0
 * @date 2024-12-14
 */
#include <array>
#include <iostream>
using namespace std;

template <typename T = int, size_t Size = 10>
class Array
{
private:
    T * ptr;

public:
    Array() {}
    Array(T arr[]);
    void print();
};

template <typename T, size_t Size>
Array<T, Size>::Array(T arr[])
{
    ptr = new T[Size];
    for (int i = 0; i < Size; i++)
        ptr[i] = arr[i];
}

template <typename T, size_t Size>
void Array<T, Size>::print()
{
    for (int i = 0; i < Size; i++)
        cout << " " << *(ptr + i);
    cout << endl;
}

int main()
{
    int arr[5] = { 1, 2, 3, 4, 5 };

    Array<> arrInt{ arr }; // ==> Array<int, 10> arrInt{ arr };
    arrInt.print();

    const int len = 100;
    Array<int, len> arrLong{ arr };
    arrLong.print();
}
