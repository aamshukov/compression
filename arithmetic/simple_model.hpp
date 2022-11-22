//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __COMPRESSION_ARITHMETIC_SIMPLE_MODEL_H__
#define __COMPRESSION_ARITHMETIC_SIMPLE_MODEL_H__

#pragma once

BEGIN_NAMESPACE(compression::arithmetic)

template <typename ElementType, typename IntegerType>
class simple_model : public model<ElementType, IntegerType>
{
    public:
        using integer_type = model<ElementType, IntegerType>::integer_type;
        using element_type = model<ElementType, IntegerType>::element_type;

        using abc_type = model<ElementType, IntegerType>::abc_type;

        using index_type = model<ElementType, IntegerType>::index_type;
        using size_type = model<ElementType, IntegerType>::size_type;

        using probability_type = std::vector<integer_type>;

    private:
        probability_type    my_p;
        integer_type        my_space; // 32768 ...

        probability_type    my_c;
        probability_type    my_d;

        integer_type        my_r;

        abc_type            my_abc;

        size_type           my_len;
        size_type           my_0count;
        size_type           my_1count;
        size_type           my_2count;
        size_type           my_3count;

    private:
        void                update_model(const probability_type& probability);

    public:
                            simple_model(const probability_type& probability,
                                         const integer_type& space,
                                         const abc_type& abc,
                                         size_type input_length,
                                         size_type xi0s_count,
                                         size_type xi1s_count,
                                         size_type xi2s_count,
                                         size_type xi3s_count);

        integer_type        c_by_symbol(const element_type& symbol) override;
        integer_type        d_by_symbol(const element_type& symbol) override;

        integer_type        c_by_index(index_type index) override;
        integer_type        d_by_index(index_type index) override;

        integer_type        r() const override;

        abc_type            abc() const override;

        element_type        symbol_by_index(index_type index) override;

        void                update(const element_type& symbol) override;
        void                reset() override;
};

template <typename ElementType, typename IntegerType>
inline simple_model<ElementType, IntegerType>::simple_model(const probability_type& probability,
                                                            const integer_type& space,
                                                            const abc_type& abc,
                                                            size_type input_length,
                                                            size_type xi0s_count,
                                                            size_type xi1s_count,
                                                            size_type xi2s_count,
                                                            size_type xi3s_count)
    : my_space(space), my_r(0), my_abc(abc), my_len(input_length), my_0count(xi0s_count), my_1count(xi1s_count), my_2count(xi2s_count), my_3count(xi3s_count)
{
    update_model(probability);
}

template <typename ElementType, typename IntegerType>
inline typename simple_model<ElementType, IntegerType>::integer_type simple_model<ElementType, IntegerType>::c_by_symbol(const element_type& symbol)
{
    integer_type result = my_c[(*abc()).index_by_symbol(symbol)];
    return result;
}

template <typename ElementType, typename IntegerType>
inline typename simple_model<ElementType, IntegerType>::integer_type simple_model<ElementType, IntegerType>::d_by_symbol(const element_type& symbol)
{
    integer_type result = my_d[(*abc()).index_by_symbol(symbol)];
    return result;
}

template <typename ElementType, typename IntegerType>
inline typename simple_model<ElementType, IntegerType>::integer_type simple_model<ElementType, IntegerType>::c_by_index(index_type index)
{
    return my_c[index];
}

template <typename ElementType, typename IntegerType>
inline typename simple_model<ElementType, IntegerType>::integer_type simple_model<ElementType, IntegerType>::d_by_index(index_type index)
{
    return my_d[index];
}

template <typename ElementType, typename IntegerType>
inline typename simple_model<ElementType, IntegerType>::integer_type simple_model<ElementType, IntegerType>::r() const
{
    return my_r;
}

template <typename ElementType, typename IntegerType>
typename simple_model<ElementType, IntegerType>::abc_type typename simple_model<ElementType, IntegerType>::abc() const
{
    return my_abc;
}

template <typename ElementType, typename IntegerType>
inline typename simple_model<ElementType, IntegerType>::element_type simple_model<ElementType, IntegerType>::symbol_by_index(index_type index)
{
    element_type result = (*abc()).symbol_by_index(index);
    return result;
}

template <typename ElementType, typename IntegerType>
inline void simple_model<ElementType, IntegerType>::update(const element_type& symbol)
{
    auto p0 = static_cast<integer_type>((static_cast<double>(my_0count) / my_len) * my_space);
    auto p1 = static_cast<integer_type>((static_cast<double>(my_1count) / my_len) * my_space);
    auto p2 = static_cast<integer_type>((static_cast<double>(my_2count) / my_len) * my_space);
    auto p3 = static_cast<integer_type>((static_cast<double>(my_3count) / my_len) * my_space);

    if(p0 > my_space || p1 > my_space || p2 > my_space || p3 > my_space)
    {
        return; //??
    }

    if(symbol == '0')
    {
        my_0count--;
    }
    else if(symbol == '1')
    {
        my_1count--;
    }
    else if(symbol == '2')
    {
        my_2count--;
    }
    else if(symbol == '3')
    {
        my_3count--;
    }
    else
        my_len--;

    my_len--;

    update_model({ p0, p1, p2, p3 });
}

template <typename ElementType, typename IntegerType>
inline void simple_model<ElementType, IntegerType>::reset()
{
    update_model(my_p);
}

template <typename ElementType, typename IntegerType>
inline void simple_model<ElementType, IntegerType>::update_model(const probability_type& p)
{
    // c(0) = 0, c(j) = r(0) + ... + r(j-1) for j = 1, ..., n
    my_c.resize(p.size());

    my_c[0] = 0;

    for(index_type j = 1, n = p.size(); j < n; j++)
    {
        my_c[j] = p[0]; // r(0)

        for(index_type k = 1; k <= j - 1; k++) // up to j-1
        {
            my_c[j] += p[k];
        }
    }

    // d(j) = c(j) + r(j) for j = 1, ..., n
    my_d.resize(p.size());

    for(index_type j = 0, n = p.size(); j < n; j++)
    {
        my_d[j] = my_c[j] + p[j];
    }

    // R
    my_r = 0;

    for(auto r : p)
    {
        my_r += r;
    }

    // P
    my_p = p; //??
}

END_NAMESPACE

#endif // __COMPRESSION_ARITHMETIC_MODEL_H__
