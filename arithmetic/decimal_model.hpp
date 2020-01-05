//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __COMPRESSION_ARITHMETIC_DECIMAL_MODEL_H__
#define __COMPRESSION_ARITHMETIC_DECIMAL_MODEL_H__

#pragma once

BEGIN_NAMESPACE(compression::arithmetic)

template <typename ElementType, typename IntegerType>
class decimal_model : public model<ElementType, IntegerType>
{
    public:
        using integer_type = model<ElementType, IntegerType>::integer_type;
        using element_type = model<ElementType, IntegerType>::element_type;

        using abc_type = model<ElementType, IntegerType>::abc_type;

        using index_type = std::size_t;
        using size_type = std::size_t;

        using probability_type = std::vector<integer_type>;

    private:
        probability_type    my_p;

        probability_type    my_c;
        probability_type    my_d;

        integer_type        my_r;

        abc_type            my_abc;

        size_type           my_len;

        size_type           my_0count;
        size_type           my_1count;
        size_type           my_2count;
        size_type           my_3count;
        size_type           my_4count;
        size_type           my_5count;
        size_type           my_6count;
        size_type           my_7count;
        size_type           my_8count;
        size_type           my_9count;

    private:
        void                update_model(const probability_type& p);

    public:
                            decimal_model(const probability_type& p, // probability
                                         const abc_type& abc,
                                         size_type input_length,
                                         size_type x0count,
                                         size_type x1count,
                                         size_type x2count,
                                         size_type x3count,
                                         size_type x4count,
                                         size_type x5count,
                                         size_type x6count,
                                         size_type x7count,
                                         size_type x8count,
                                         size_type x9count);

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
inline decimal_model<ElementType, IntegerType>::decimal_model(const probability_type& p,
                                                              const abc_type& abc,
                                                              size_type input_length,
                                                              size_type x0count,
                                                              size_type x1count,
                                                              size_type x2count,
                                                              size_type x3count,
                                                              size_type x4count,
                                                              size_type x5count,
                                                              size_type x6count,
                                                              size_type x7count,
                                                              size_type x8count,
                                                              size_type x9count)
    : my_r(0), my_abc(abc), my_len(input_length),
      my_0count(x0count),
      my_1count(x1count),
      my_2count(x2count),
      my_3count(x3count),
      my_4count(x4count),
      my_5count(x5count),
      my_6count(x6count),
      my_7count(x7count),
      my_8count(x8count),
      my_9count(x9count)
{
    update_model(p);
}

template <typename ElementType, typename IntegerType>
inline typename decimal_model<ElementType, IntegerType>::integer_type decimal_model<ElementType, IntegerType>::c_by_symbol(const element_type& symbol)
{
    integer_type result = my_c[(*abc()).index_by_symbol(symbol)];
    return result;
}

template <typename ElementType, typename IntegerType>
inline typename decimal_model<ElementType, IntegerType>::integer_type decimal_model<ElementType, IntegerType>::d_by_symbol(const element_type& symbol)
{
    integer_type result = my_d[(*abc()).index_by_symbol(symbol)];
    return result;
}

template <typename ElementType, typename IntegerType>
inline typename decimal_model<ElementType, IntegerType>::integer_type decimal_model<ElementType, IntegerType>::c_by_index(index_type index)
{
    return my_c[index];
}

template <typename ElementType, typename IntegerType>
inline typename decimal_model<ElementType, IntegerType>::integer_type decimal_model<ElementType, IntegerType>::d_by_index(index_type index)
{
    return my_d[index];
}

template <typename ElementType, typename IntegerType>
inline typename decimal_model<ElementType, IntegerType>::integer_type decimal_model<ElementType, IntegerType>::r() const
{
    return my_r;
}

template <typename ElementType, typename IntegerType>
typename decimal_model<ElementType, IntegerType>::abc_type typename decimal_model<ElementType, IntegerType>::abc() const
{
    return my_abc;
}

template <typename ElementType, typename IntegerType>
inline typename decimal_model<ElementType, IntegerType>::element_type decimal_model<ElementType, IntegerType>::symbol_by_index(index_type index)
{
    element_type result = (*abc()).symbol_by_index(index);
    return result;
}

template <typename ElementType, typename IntegerType>
inline void decimal_model<ElementType, IntegerType>::update(const element_type& symbol)
{
    auto p0 = static_cast<integer_type>((static_cast<double>(my_0count) / my_len) * 32768);
    auto p1 = static_cast<integer_type>((static_cast<double>(my_1count) / my_len) * 32768);
    auto p2 = static_cast<integer_type>((static_cast<double>(my_2count) / my_len) * 32768);
    auto p3 = static_cast<integer_type>((static_cast<double>(my_3count) / my_len) * 32768);
    auto p4 = static_cast<integer_type>((static_cast<double>(my_4count) / my_len) * 32768);
    auto p5 = static_cast<integer_type>((static_cast<double>(my_5count) / my_len) * 32768);
    auto p6 = static_cast<integer_type>((static_cast<double>(my_6count) / my_len) * 32768);
    auto p7 = static_cast<integer_type>((static_cast<double>(my_7count) / my_len) * 32768);
    auto p8 = static_cast<integer_type>((static_cast<double>(my_8count) / my_len) * 32768);
    auto p9 = static_cast<integer_type>((static_cast<double>(my_9count) / my_len) * 32768);

    if(p0 >= 32768 || p1 >= 32768 || p2 >= 32768 || p3 >= 32768 || p4 >= 32768 || p5 >= 32768 || p6 >= 32768 || p7 >= 32768 || p8 >= 32768 || p9 >= 32768)
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
    else if(symbol == '4')
    {
        my_4count--;
    }
    else if(symbol == '5')
    {
        my_5count--;
    }
    else if(symbol == '6')
    {
        my_6count--;
    }
    else if(symbol == '7')
    {
        my_7count--;
    }
    else if(symbol == '8')
    {
        my_8count--;
    }
    else if(symbol == '9')
    {
        my_9count--;
    }

    my_len--;

    update_model({ p0, p1, p2, p3, p4, p5, p6, p7, p8, p9 });
}

template <typename ElementType, typename IntegerType>
inline void decimal_model<ElementType, IntegerType>::reset()
{
    update_model(my_p);
}

template <typename ElementType, typename IntegerType>
inline void decimal_model<ElementType, IntegerType>::update_model(const probability_type& p)
{
    // c(0) = 0, c(j) = r(0) + ... + r(j-1) for j = 1, ..., n
    my_c.resize(p.size());

    my_c[0] = 0;

    for(std::size_t j = 1, n = p.size(); j < n; j++)
    {
        my_c[j] = p[0]; // r(0)

        for(std::size_t k = 1; k < j; k++) // up to j-1
        {
            my_c[j] += p[k];
        }
    }

    // d(j) = c(j) + r(j) for j = 1, ..., n
    my_d.resize(p.size());

    for(std::size_t j = 0, n = p.size(); j < n; j++)
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

#endif // __COMPRESSION_ARITHMETIC_DECIMAL_MODEL_H__
