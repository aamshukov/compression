//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __COMPRESSION_ARITHMETIC_ABC_H__
#define __COMPRESSION_ARITHMETIC_ABC_H__

#pragma once

BEGIN_NAMESPACE(compression)

template <typename ElementType>
interface abc : private noncopyable
{
    using index_type = std::size_t;

    using element_type = ElementType;
    using elements_type = std::vector<element_type>;

    virtual const elements_type& data() const = 0;

    virtual index_type index_by_symbol(const element_type& symbol) = 0;
    virtual element_type symbol_by_index(index_type index) = 0;
};

END_NAMESPACE

#endif // __COMPRESSION_ARITHMETIC_ABC_H__
