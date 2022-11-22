//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __COMPRESSION_ARITHMETIC_ENGLISH_ABC_H__
#define __COMPRESSION_ARITHMETIC_ENGLISH_ABC_H__

#pragma once

BEGIN_NAMESPACE(compression::arithmetic)

template <typename ElementType = char_type>
class english_abc : public abc<ElementType>
{
    public:
        using index_type = abc<ElementType>::index_type;

        using element_type = abc<ElementType>::element_type;
        using elements_type = abc<ElementType>::elements_type;

    public:
                                english_abc();

        const elements_type&    data() const;

        index_type              index_by_symbol(const element_type& symbol) override;
        element_type            symbol_by_index(index_type index) override;
};

template <typename ElementType>
inline english_abc<ElementType>::english_abc()
{
}

template <typename ElementType>
inline const typename english_abc<ElementType>::elements_type& english_abc<ElementType>::data() const
{
    static elements_type abc = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };
    return abc;
}

template <typename ElementType>
inline typename english_abc<ElementType>::index_type english_abc<ElementType>::index_by_symbol(const element_type& symbol)
{
    index_type result = -1;

    if(symbol == L'a')
    {
        result = 0;
    }
    else if(symbol == 'b')
    {
        result = 1;
    }
    else if(symbol == 'c')
    {
        result = 2;
    }
    else if(symbol == 'd')
    {
        result = 3;
    }
    else if(symbol == 'e')
    {
        result = 4;
    }
    else if(symbol == 'f')
    {
        result = 5;
    }
    else if(symbol == 'g')
    {
        result = 6;
    }
    else if(symbol == 'h')
    {
        result = 7;
    }
    else if(symbol == 'i')
    {
        result = 8;
    }
    else if(symbol == 'j')
    {
        result = 9;
    }
    else if(symbol == 'k')
    {
        result = 10;
    }
    else if(symbol == 'l')
    {
        result = 11;
    }
    else if(symbol == 'm')
    {
        result = 12;
    }
    else if(symbol == 'n')
    {
        result = 13;
    }
    else if(symbol == 'o')
    {
        result = 14;
    }
    else if(symbol == 'p')
    {
        result = 15;
    }
    else if(symbol == 'q')
    {
        result = 16;
    }
    else if(symbol == 'r')
    {
        result = 17;
    }
    else if(symbol == 's')
    {
        result = 18;
    }
    else if(symbol == 't')
    {
        result = 19;
    }
    else if(symbol == 'u')
    {
        result = 20;
    }
    else if(symbol == 'v')
    {
        result = 21;
    }
    else if(symbol == 'w')
    {
        result = 22;
    }
    else if(symbol == 'x')
    {
        result = 23;
    }

    else if(symbol == 'y')
    {
        result = 24;
    }
    else if(symbol == 'z')
    {
        result = 25;
    }

    return result;
}

template <typename ElementType>
inline typename english_abc<ElementType>::element_type english_abc<ElementType>::symbol_by_index(index_type index)
{
    element_type result = data()[index];
    return result;
}

END_NAMESPACE

#endif // __COMPRESSION_ARITHMETIC_ENGLISH_ABC_H__
