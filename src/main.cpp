#include <iostream>
#include <sorting.h>

int main()
{
    int *arr = new int[10]{-3, 0, 1, -2, 0, 4, 5, -4, -5, 1};

    IMD::comb_sort(arr, arr + 10);

    for (size_t i(0); i < 10; ++i)
        std::cout << arr[i] << std::endl;

    delete[] arr;
    std::cin.get();
    std::cin.get();
    return 0;
}