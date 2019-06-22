//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __COMPRESSION_ARITHMETIC_SIMPLE_ABC_H__
#define __COMPRESSION_ARITHMETIC_SIMPLE_ABC_H__

#pragma once

BEGIN_NAMESPACE(compression::arithmetic)

template <typename ElementType = char_type>
class simple_abc : public abc<ElementType>
{
    public:
        using index_type = abc<ElementType>::index_type;

        using element_type = abc<ElementType>::element_type;
        using elements_type = abc<ElementType>::elements_type;

    public:
                                simple_abc();

        const elements_type&    data() const;

        index_type              index_by_symbol(const element_type& symbol) override;
        element_type            symbol_by_index(std::size_t index) override;
};

template <typename ElementType>
inline simple_abc<ElementType>::simple_abc()
{
}

template <typename ElementType>
inline const typename simple_abc<ElementType>::elements_type& simple_abc<ElementType>::data() const
{
    static elements_type abc = { '0', '1', '2', '3' };
    return abc;
}

template <typename ElementType>
inline typename simple_abc<ElementType>::index_type simple_abc<ElementType>::index_by_symbol(const element_type& symbol)
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
    else if(symbol == L'2')
    {
        result = 2;
    }
    else if(symbol == L'3')
    {
        result = 3;
    }

    return result;
}

template <typename ElementType>
inline typename simple_abc<ElementType>::element_type simple_abc<ElementType>::symbol_by_index(std::size_t index)
{
    element_type result = data()[index];
    return result;
}

END_NAMESPACE

#endif // __COMPRESSION_ARITHMETIC_SIMPLE_ABC_H__
