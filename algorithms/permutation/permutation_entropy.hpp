//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __ALGORITHMS_PERMUTATION_ENTROPY_H__
#define __ALGORITHMS_PERMUTATION_ENTROPY_H__

#pragma once

BEGIN_NAMESPACE(algorithms)

// α β ε λ ∅ ∈ ∉ Σ ∪ ⊕ τ

template <typename ElementType = int>
class permutation_entropy : private noncopyable
{
    public:
        using element_type = ElementType;
        using elements_type = std::vector<element_type>;

        using dimension_type = int;
        using time_delay_type = int;

        using index_type = int;
        using size_type = std::size_t;

        using matrix_type = std::vector<index_type>;

    public:
        static void partition_state_space(const elements_type& time_series,
                                          const dimension_type& dimension,
                                          const time_delay_type& delay,
                                          matrix_type& result_state_space);
};

template <typename ElementType>
void permutation_entropy<ElementType>::partition_state_space(const typename permutation_entropy<ElementType>::elements_type& time_series,
                                                             const typename permutation_entropy<ElementType>::dimension_type& dimension,
                                                             const typename permutation_entropy<ElementType>::time_delay_type& delay,
                                                             typename permutation_entropy<ElementType>::matrix_type& result_state_space)
{
    // https://www.aptech.com/blog/permutation-entropy
    //  time_series - { 4, 7, 9, 10, 6, 11, 3 }, T
    //  dimension   - The embedding dimension which controls the length of each of the new column vectors. 3 ≤ D ≤ 7
    //  delay       - The embedding time delay which controls the number of time periods between elements of each of the new column vectors. 1
    //  result - for (3, 1)
    //      | 0 0 1 1 1 |
    //      | 1 1 2 0 2 |
    //      | 2 2 0 2 0 |
    size_type cols = time_series.size() - (dimension - 1) * delay; // T - (D - 1) * τ
    size_type rows = dimension; // D

    matrix_type state_space;

    // column based indices
    for(index_type col = 0; col < cols; col++)
    {
        std::vector<element_type> values;

        for(index_type row = 0; row < rows; row++)
        {
            values.emplace_back(time_series[row * delay + col]);
        }

        index_type i = 0;

        std::vector<index_type> indices(dimension);

        std::generate(indices.begin(), indices.end(), [&]{ return i++; });

        std::sort(indices.begin(), indices.end(), [&values](index_type i1, index_type i2) {return values[i1] < values[i2];});

        std::for_each(indices.begin(), indices.end(), [&state_space](auto& index){ state_space.emplace_back(index); });
    }

    result_state_space.swap(state_space);
}

END_NAMESPACE

#endif // __ALGORITHMS_PERMUTATION_ENTROPY_H__
