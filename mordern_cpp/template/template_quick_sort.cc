#include <iostream>
#include <type_traits>

// 列表定义
template <int... Ns>
struct IntList {};

// 列表连接
template <typename L1, typename L2>
struct Concat;

template <int... N1s, int... N2s>
struct Concat<IntList<N1s...>, IntList<N2s...>> {
    using type = IntList<N1s..., N2s...>;
};

// Partition 分割列表为小于Pivot和大于等于Pivot的部分
template <int Pivot, typename List>
struct Partition;

template <int Pivot>
struct Partition<Pivot, IntList<>> {
    using less = IntList<>;
    using greater_equal = IntList<>;
};

template <int Pivot, int Head, int... Tail>
struct Partition<Pivot, IntList<Head, Tail...>> {
private:
    using sub = Partition<Pivot, IntList<Tail...>>;

public:
    using less = std::conditional_t<(Head < Pivot), typename Concat<IntList<Head>, typename sub::less>::type, typename sub::less>;
    using greater_equal = std::conditional_t<(Head < Pivot), typename sub::greater_equal, typename Concat<IntList<Head>, typename sub::greater_equal>::type>;
};

// 快速排序模板
template <typename List>
struct QuickSort;

template <>
struct QuickSort<IntList<>> {
    using type = IntList<>;
};

template <int Pivot, int... Tail>
struct QuickSort<IntList<Pivot, Tail...>> {
private:
    using partition = Partition<Pivot, IntList<Tail...>>;
    using left_sorted = typename QuickSort<typename partition::less>::type;
    using right_sorted = typename QuickSort<typename partition::greater_equal>::type;

public:
    using type = typename Concat<typename Concat<left_sorted, IntList<Pivot>>::type, right_sorted>::type;
};

// 打印列表
template <typename List>
struct PrintList;

template <>
struct PrintList<IntList<>> {
    static void print() 
    {
        std::cout << "\n";
    }
};

template <int Head, int... Tail>
struct PrintList<IntList<Head, Tail...>> {
    static void print() 
    {
        std::cout << Head << " ";
        PrintList<IntList<Tail...>>::print();
    }
};

// 测试
int main() {
    using Unsorted = IntList<7, 2, 9, 4, 3, 8>;
    using Sorted = QuickSort<Unsorted>::type;

    PrintList<Sorted>::print();  // 输出：2 3 4 7 8 9
    return 0;
}
