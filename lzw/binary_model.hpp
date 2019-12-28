//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __COMPRESSION_LZW_BINARY_MODEL_H__
#define __COMPRESSION_LZW_BINARY_MODEL_H__

#pragma once

BEGIN_NAMESPACE(compression::lzw)

template <typename ElementType, typename IntegerType, typename KeyType, typename ValueType>
class binary_model : public model<ElementType, IntegerType, KeyType, ValueType>
{
    public:
        using integer_type = model<ElementType, IntegerType, KeyType, ValueType>::integer_type;
        using element_type = model<ElementType, IntegerType, KeyType, ValueType>::element_type;

        using codes_type = model<ElementType, IntegerType, KeyType, ValueType>::codes_type;
        using strings_type = model<ElementType, IntegerType, KeyType, ValueType>::strings_type;

    private:
        codes_type              my_codes;
        strings_type            my_strings;

    public:
                                binary_model();

        const codes_type&       codes() const override;
        codes_type&             codes() override;

        const strings_type&     strings() const override;
        strings_type&           strings() override;
};

template <typename ElementType, typename IntegerType, typename KeyType, typename ValueType>
inline binary_model<ElementType, IntegerType, KeyType, ValueType>::binary_model()
{
}

template <typename ElementType, typename IntegerType, typename KeyType, typename ValueType>
inline const typename binary_model<ElementType, IntegerType, KeyType, ValueType>::codes_type&
    binary_model<ElementType, IntegerType, KeyType, ValueType>::codes() const
{
    return my_codes;
}

template <typename ElementType, typename IntegerType, typename KeyType, typename ValueType>
inline typename binary_model<ElementType, IntegerType, KeyType, ValueType>::codes_type&
    binary_model<ElementType, IntegerType, KeyType, ValueType>::codes()
{
    return my_codes;
}

template <typename ElementType, typename IntegerType, typename KeyType, typename ValueType>
inline const typename binary_model<ElementType, IntegerType, KeyType, ValueType>::strings_type&
    binary_model<ElementType, IntegerType, KeyType, ValueType>::strings() const
{
    return my_strings;
}

template <typename ElementType, typename IntegerType, typename KeyType, typename ValueType>
inline typename binary_model<ElementType, IntegerType, KeyType, ValueType>::strings_type&
    binary_model<ElementType, IntegerType, KeyType, ValueType>::strings()
{
    return my_strings;
}

END_NAMESPACE

#endif // __COMPRESSION_LZW_BINARY_MODEL_H__
