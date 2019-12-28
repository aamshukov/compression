//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __COMPRESSION_LZW_MODEL_H__
#define __COMPRESSION_LZW_MODEL_H__

#pragma once

BEGIN_NAMESPACE(compression::lzw)
USINGNAMESPACE(compression)

template <typename ElementType, typename IntegerType, typename KeyType, typename ValueType>
interface model : private noncopyable
{
    using integer_type = IntegerType;
    using element_type = ElementType;

    using key_type = KeyType;
    using value_type = ValueType;

    using codes_type = std::unordered_map<key_type, value_type>;
    using strings_type = std::unordered_map<value_type, key_type>;

    virtual const codes_type&   codes() const = 0;
    virtual codes_type&         codes() = 0;

    virtual const strings_type& strings() const = 0;
    virtual strings_type&       strings() = 0;
};

END_NAMESPACE

#endif // __COMPRESSION_LZW_MODEL_H__
