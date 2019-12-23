//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __COMPRESSION_LZW_MODEL_H__
#define __COMPRESSION_LZW_MODEL_H__

#pragma once

BEGIN_NAMESPACE(compression::lzw)
USINGNAMESPACE(compression)

template <typename ElementType, typename IntegerType>
interface model : private noncopyable
{
    using integer_type = IntegerType;
    using element_type = ElementType;

    virtual void update(const element_type& symbol) = 0;

    virtual void reset() = 0;
};

END_NAMESPACE

#endif // __COMPRESSION_LZW_MODEL_H__
