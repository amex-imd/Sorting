#ifndef __SORTING_IMD_
#define __SORTING_IMD_

#include <type_traits>
#include <iterator>
#include <algorithm>

namespace IMD
{
    template <typename InputIt, typename Comparator = std::less<typename std::iterator_traits<InputIt>::value_type>>
    void bubble_sort(InputIt beg, InputIt end, Comparator cmp = Comparator())
    {
        while (true)
        {
            bool flag = false;
            for (InputIt it = beg; it != std::prev(end); ++it)
            {
                if (cmp(*std::next(it), *it))
                {
                    std::iter_swap(it, std::next(it));
                    flag = true;
                }
            }

            if (!flag)
                break;
        }
    }

    template <typename InputIt, typename Comparator = std::less<typename std::iterator_traits<InputIt>::value_type>>
    void insertion_sort(InputIt beg, InputIt end, Comparator cmp = Comparator())
    {
        for (InputIt i = std::next(beg); i != end; ++i)
        {
            auto key = std::move(*i);
            InputIt j = i;

            while (j != beg && cmp(key, *std::prev(j)))
            {
                *j = std::move(*std::prev(j));
                --j;
            }
            *j = std::move(key);
        }
    }
}

#endif