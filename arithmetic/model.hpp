//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
// based on mathematicalmonk's videos
// https://www.youtube.com/playlist?list=PLE125425EC837021F
//
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

    using abc_type = std::vector<element_type>;

    virtual integer_type c_by_symbol(const element_type& symbol) = 0;
    virtual integer_type d_by_symbol(const element_type& symbol) = 0;

    virtual integer_type c_by_index(std::size_t index) = 0;
    virtual integer_type d_by_index(std::size_t index) = 0;

    virtual integer_type r() const = 0; // R = SUM(r(i)), i=0|n

    virtual abc_type abc() const = 0;

    virtual void update() = 0;
};

END_NAMESPACE

#endif // __COMPRESSION_ARITHMETIC_MODEL_H__
