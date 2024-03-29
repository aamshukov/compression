//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __COMPRESSION_ARITHMETIC_MODEL_H__
#define __COMPRESSION_ARITHMETIC_MODEL_H__

#pragma once

BEGIN_NAMESPACE(compression::arithmetic)
USINGNAMESPACE(compression)

template <typename ElementType, typename IntegerType>
interface model : private noncopyable
{
    using integer_type = IntegerType;
    using element_type = ElementType;

    using abc_type = std::shared_ptr<abc<element_type>>;

    using index_type = typename abc<element_type>::index_type;
    using size_type = std::size_t;

    virtual integer_type c_by_symbol(const element_type& symbol) = 0;
    virtual integer_type d_by_symbol(const element_type& symbol) = 0;

    virtual integer_type c_by_index(index_type index) = 0;
    virtual integer_type d_by_index(index_type index) = 0;

    virtual integer_type r() const = 0; // R = SUM(r(i)), i=0|n

    virtual abc_type abc() const = 0;

    virtual element_type symbol_by_index(index_type index) = 0;

    virtual void update(const element_type& symbol) = 0;

    virtual void reset() = 0;
};

END_NAMESPACE

#endif // __COMPRESSION_ARITHMETIC_MODEL_H__
