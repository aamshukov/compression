//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
// based on mathematicalmonk's videos
// https://www.youtube.com/playlist?list=PLE125425EC837021F
//
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

        using abc_type = model<element_type, integer_type>::abc_type;

        using probability_type = std::vector<integer_type>;

    private:
        probability_type    my_p;

        probability_type    my_c;
        probability_type    my_d;

        integer_type        my_r;

    public:
                            simple_model(const probability_type& p); // probability

        integer_type        c_by_symbol(const element_type& symbol) override;
        integer_type        d_by_symbol(const element_type& symbol) override;

        integer_type        c_by_index(std::size_t index) override;
        integer_type        d_by_index(std::size_t index) override;

        integer_type        r() const override;

        abc_type            abc() const override;

        void                update() override;
};

template <typename ElementType, typename IntegerType>
inline simple_model<ElementType, IntegerType>::simple_model(const probability_type& p)
    : my_p(p), my_r(0)
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
    for(auto r : p)
    {
        my_r += r;
    }
}

template <typename ElementType, typename IntegerType>
inline typename simple_model<ElementType, IntegerType>::integer_type simple_model<ElementType, IntegerType>::c_by_symbol(const element_type& symbol)
{
    integer_type result = -1; //??

    if(symbol == L'0')
    {
        result = my_c[0];
    }
    else if(symbol == L'1')
    {
        result = my_c[1];
    }
    else if(symbol == L'2')
    {
        result = my_c[2];
    }
    else if(symbol == L'3')
    {
        result = my_c[3];
    }

    return result;
}

template <typename ElementType, typename IntegerType>
inline typename simple_model<ElementType, IntegerType>::integer_type simple_model<ElementType, IntegerType>::d_by_symbol(const element_type& symbol)
{
    integer_type result = -1; //??

    if(symbol == L'0')
    {
        result = my_d[0];
    }
    else if(symbol == L'1')
    {
        result = my_d[1];
    }
    else if(symbol == L'2')
    {
        result = my_d[2];
    }
    else if(symbol == L'3')
    {
        result = my_d[3];
    }

    return result;
}

template <typename ElementType, typename IntegerType>
inline typename simple_model<ElementType, IntegerType>::integer_type simple_model<ElementType, IntegerType>::c_by_index(std::size_t index)
{
    return my_c[index];
}

template <typename ElementType, typename IntegerType>
inline typename simple_model<ElementType, IntegerType>::integer_type simple_model<ElementType, IntegerType>::d_by_index(std::size_t index)
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
    return { L'0', L'1', L'2', L'3' };
}

template <typename ElementType, typename IntegerType>
inline void simple_model<ElementType, IntegerType>::update()
{
    // static model, nothing to update
}

END_NAMESPACE

#endif // __COMPRESSION_ARITHMETIC_MODEL_H__
