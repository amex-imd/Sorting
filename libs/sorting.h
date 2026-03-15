#ifndef __SORTING_IMD_
#define __SORTING_IMD_

#include <type_traits>
#include <iterator>
#include <algorithm>
#include <stack>
#include <utility>
#include <vector>

namespace IMD
{
    namespace DETAILS
    {
        template <typename InputIt, typename Comparator = std::less<typename std::iterator_traits<InputIt>::value_type>>
        InputIt Hoare_scheme(InputIt beg, InputIt end, Comparator cmp = Comparator())
        {
            auto len = std::distance(beg, end);
            auto mid = std::next(beg, len / 2);
            InputIt tmp = std::prev(end);

            if (cmp(*tmp, *beg))
                std::iter_swap(beg, tmp);
            if (cmp(*mid, *beg))
                std::iter_swap(beg, mid);
            if (cmp(*tmp, *mid))
                std::iter_swap(mid, tmp);

            auto pivot = *mid;
            InputIt left = beg;  // i
            InputIt right = tmp; // j

            while (left <= right)
            {
                while (cmp(*left, pivot))
                    ++left;
                while (cmp(pivot, *right))
                    --right;

                if (left <= right)
                {
                    std::iter_swap(left, right);
                    ++left;
                    --right;
                }
            }

            return right; // the position of the right pointer after the partiotion
        }

        template <typename InputIt, typename Comparator = std::less<typename std::iterator_traits<InputIt>::value_type>>
        InputIt Lomuto_scheme(InputIt beg, InputIt end, Comparator cmp = Comparator())
        {
            auto len = std::distance(beg, end);
            InputIt tmp = std::prev(end);
            auto pivot = *tmp;
            InputIt border = beg; // i

            for (InputIt it = beg; it != tmp; ++it)
            {
                if (cmp(*it, pivot))
                {
                    std::iter_swap(it, border);
                    ++border;
                }
            }
            std::iter_swap(tmp, border);
            return border; // the pivot position after the partition
        }
    }

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

    template <typename InputIt, typename Comparator = std::less<typename std::iterator_traits<InputIt>::value_type>>
    void selection_sort(InputIt beg, InputIt end, Comparator cmp = Comparator())
    {
        for (InputIt i = beg; i != end; ++i)
        {
            InputIt min_it = i;

            for (InputIt j = std::next(i); j != end; ++j)
            {
                if (cmp(*j, *min_it))
                    min_it = j;
            }

            std::iter_swap(i, min_it);
        }
    }

    template <typename InputIt, typename Comparator = std::less<typename std::iterator_traits<InputIt>::value_type>>
    void comb_sort(InputIt beg, InputIt end, Comparator cmp = Comparator(), double coefficient = 1.25)
    {
        size_t gap = std::distance(beg, end);
        bool flag = true;

        while (gap > 1 || flag)
        {
            gap = size_t(gap / coefficient);
            bool flag = false;

            if (gap == 0)
                gap = 1;

            for (InputIt it = beg; it != std::prev(end, gap); ++it)
            {
                auto tmp = std::next(it, gap);
                if (cmp(*tmp, *it))
                {
                    std::iter_swap(tmp, it);
                    bool flag = true;
                }
            }
        }
    }

    template <typename InputIt>
    void counting_sort(InputIt beg, InputIt end)
    {
        using value_type = typename std::iterator_traits<InputIt>::value_type;

        static_assert(std::is_integral_v<value_type>, "Counting sort requires integer types");

        auto min_max = std::minmax_element(beg, end);
        auto min_val = *min_max.first;
        auto max_val = *min_max.second;

        size_t size = max_val - min_val + 1;
        size_t *counter = new size_t[size]();
        try
        {
            for (auto it = beg; it != end; ++it)
                ++counter[static_cast<size_t>(*it - min_val)];

            auto it = beg;
            for (size_t i(0); i < size; ++i)
            {
                for (size_t j(0); j < counter[i]; ++j)
                    *it++ = static_cast<value_type>(i + min_val);
            }
        }
        catch (...)
        {
            delete[] counter;
            throw;
        }
    }
    template <typename InputIt, typename Comparator = std::less<typename std::iterator_traits<InputIt>::value_type>>
    void Hoare_recursive_sort(InputIt beg, InputIt end, Comparator cmp = Comparator())
    {
        if (beg == end || std::next(beg) == end)
            return;

        InputIt it = DETAILS::Hoare_scheme(beg, end, cmp);
        Hoare_recursive_sort(beg, std::next(it), cmp);
        Hoare_recursive_sort(std::next(it), end, cmp);
    }
    template <typename InputIt, typename Comparator = std::less<typename std::iterator_traits<InputIt>::value_type>>
    void Hoare_iterative_sort(InputIt beg, InputIt end, Comparator cmp = Comparator())
    {
        std::stack<std::pair<InputIt, InputIt>> st;
        st.emplace(beg, end);
        while (!st.empty())
        {
            auto [b, e] = st.top();
            st.pop();

            if (std::distance(b, e) <= 1)
                continue;

            InputIt it = DETAILS::Hoare_scheme(b, e, cmp);
            st.emplace(b, std::next(it));
            st.emplace(std::next(it), e);
        }
    }

    template <typename InputIt, typename Comparator = std::less<typename std::iterator_traits<InputIt>::value_type>>
    void Lomuto_recursive_sort(InputIt beg, InputIt end, Comparator cmp = Comparator())
    {
        if (beg == end || std::next(beg) == end)
            return;

        InputIt it = DETAILS::Lomuto_scheme(beg, end, cmp);
        Lomuto_recursive_sort(beg, it, cmp);
        Lomuto_recursive_sort(std::next(it), end, cmp);
    }
    template <typename InputIt, typename Comparator = std::less<typename std::iterator_traits<InputIt>::value_type>>
    void Lomuto_iterative_sort(InputIt beg, InputIt end, Comparator cmp = Comparator())
    {
        std::stack<std::pair<InputIt, InputIt>> st;
        st.emplace(beg, end);
        while (!st.empty())
        {
            auto [b, e] = st.top();
            st.pop();

            if (std::distance(b, e) <= 1)
                continue;

            InputIt it = DETAILS::Lomuto_scheme(b, e, cmp);
            st.emplace(b, it);
            st.emplace(std::next(it), e);
        }
    }

    template <typename InputIt, typename Comparator = std::less<typename std::iterator_traits<InputIt>::value_type>>
    void merge_sort(InputIt beg, InputIt end, Comparator cmp = Comparator())
    {
        using ValueType = typename std::iterator_traits<InputIt>::value_type;

        size_t len = std::distance(beg, end);
        if (len < 2)
            return;

        auto mid = beg + len / 2;
        merge_sort(beg, mid, cmp);
        merge_sort(mid, end, cmp);

        InputIt left = beg;
        InputIt right = mid;

        std::vector<ValueType> buffer;
        buffer.reserve(len);

        while (left != mid && right != end)
        {
            if (cmp(*left, *right))
            {
                buffer.push_back(std::move(*left));
                ++left;
            }
            else
            {
                buffer.push_back(std::move(*right));
                ++right;
            }
        }

        while (left != mid)
        {
            buffer.push_back(std::move(*left));
            ++left;
        }

        while (right != end)
        {
            buffer.push_back(std::move(*right));
            ++right;
        }

        std::copy(std::begin(buffer), std::end(buffer), beg);
    }
}

#endif