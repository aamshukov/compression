//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __COMPRESSION_ARITHMETIC_BINARY_ABC_H__
#define __COMPRESSION_ARITHMETIC_BINARY_ABC_H__

#pragma once

BEGIN_NAMESPACE(compression::arithmetic)

template <typename ElementType = char_type>
class binary_abc : public abc<ElementType>
{
    public:
        using index_type = abc<ElementType>::index_type;

        using element_type = abc<ElementType>::element_type;
        using elements_type = abc<ElementType>::elements_type;

    public:
                                binary_abc();

        const elements_type&    data() const;

        index_type              index_by_symbol(const element_type& symbol) override;
        element_type            symbol_by_index(index_type index) override;
};

template <typename ElementType>
inline binary_abc<ElementType>::binary_abc()
{
}

template <typename ElementType>
inline const typename binary_abc<ElementType>::elements_type& binary_abc<ElementType>::data() const
{
    static elements_type abc = { L'0', L'1' };
    return abc;
}

template <typename ElementType>
inline typename binary_abc<ElementType>::index_type binary_abc<ElementType>::index_by_symbol(const element_type& symbol)
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

    return result;
}

template <typename ElementType>
inline typename binary_abc<ElementType>::element_type binary_abc<ElementType>::symbol_by_index(index_type index)
{
    element_type result = data()[index];
    return result;
}

END_NAMESPACE

#endif // __COMPRESSION_ARITHMETIC_BINARY_ABC_H__
