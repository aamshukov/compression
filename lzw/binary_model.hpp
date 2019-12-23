//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __COMPRESSION_LZW_BINARY_MODEL_H__
#define __COMPRESSION_LZW_BINARY_MODEL_H__

#pragma once

BEGIN_NAMESPACE(compression::lzw)

template <typename ElementType, typename IntegerType>
class binary_model : public model<ElementType, IntegerType>
{
    public:
        using integer_type = model<ElementType, IntegerType>::integer_type;
        using element_type = model<ElementType, IntegerType>::element_type;

        using index_type = std::size_t;
        using size_type = std::size_t;

        using dictionary_type = std::unordered_map<string_type, string_type>;

    private:
        dictionary_type     my_dictionary;

    public:
                            binary_model();

        void                update(const element_type& symbol) override;
        void                reset() override;
};

template <typename ElementType, typename IntegerType>
inline binary_model<ElementType, IntegerType>::binary_model()
{
}

template <typename ElementType, typename IntegerType>
inline void binary_model<ElementType, IntegerType>::update(const element_type& symbol)
{
}

template <typename ElementType, typename IntegerType>
inline void binary_model<ElementType, IntegerType>::reset()
{
}

END_NAMESPACE

#endif // __COMPRESSION_LZW_BINARY_MODEL_H__
