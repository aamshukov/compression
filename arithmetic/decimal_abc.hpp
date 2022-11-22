//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __COMPRESSION_ARITHMETIC_DECIMAL_ABC_H__
#define __COMPRESSION_ARITHMETIC_DECIMAL_ABC_H__

#pragma once

BEGIN_NAMESPACE(compression::arithmetic)

template <typename ElementType = char_type>
class decimal_abc : public abc<ElementType>
{
    public:
        using index_type = abc<ElementType>::index_type;

        using element_type = abc<ElementType>::element_type;
        using elements_type = abc<ElementType>::elements_type;

    public:
                                decimal_abc();

        const elements_type&    data() const;

        index_type              index_by_symbol(const element_type& symbol) override;
        element_type            symbol_by_index(index_type index) override;
};

template <typename ElementType>
inline decimal_abc<ElementType>::decimal_abc()
{
}

template <typename ElementType>
inline const typename decimal_abc<ElementType>::elements_type& decimal_abc<ElementType>::data() const
{
    static elements_type abc = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
    return abc;
}

template <typename ElementType>
inline typename decimal_abc<ElementType>::index_type decimal_abc<ElementType>::index_by_symbol(const element_type& symbol)
{
    index_type result = -1;

    if(symbol == L'0')
    {
        result = 0;
    }
    else if(symbol == '1')
    {
        result = 1;
    }
    else if(symbol == '2')
    {
        result = 2;
    }
    else if(symbol == '3')
    {
        result = 3;
    }
    else if(symbol == '4')
    {
        result = 4;
    }
    else if(symbol == '5')
    {
        result = 5;
    }
    else if(symbol == '6')
    {
        result = 6;
    }
    else if(symbol == '7')
    {
        result = 7;
    }
    else if(symbol == '8')
    {
        result = 8;
    }
    else if(symbol == '9')
    {
        result = 9;
    }

    return result;
}

template <typename ElementType>
inline typename decimal_abc<ElementType>::element_type decimal_abc<ElementType>::symbol_by_index(index_type index)
{
    element_type result = data()[index];
    return result;
}

END_NAMESPACE

#endif // __COMPRESSION_ARITHMETIC_DECIMAL_ABC_H__
