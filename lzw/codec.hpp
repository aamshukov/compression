//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
//
#ifndef __COMPRESSION_LZW_CODEC_H__
#define __COMPRESSION_LZW_CODEC_H__

#pragma once

BEGIN_NAMESPACE(compression::lzw)

template <typename ElementType, typename IntegerType, typename InputStream, typename OutputStream>
class codec
{
    public:
        using integer_type = IntegerType;
        using element_type = ElementType;

        using input_stream_type = std::shared_ptr<InputStream>;
        using output_stream_type = std::shared_ptr<OutputStream>;

        using model_type = std::shared_ptr<model<element_type, integer_type>>;

    public:
                            codec();

        bool                encode(const model_type& model,
                                   const input_stream_type& input_stream,
                                   const output_stream_type& output_stream,
                                   std::size_t& original_size,
                                   std::size_t& encoded_size);

        bool                decode(const model_type& model,
                                   const input_stream_type& input_stream,
                                   const output_stream_type& output_stream,
                                   std::size_t original_size);
};

template <typename ElementType, typename IntegerType, typename InputStream, typename OutputStream>
inline codec<ElementType, IntegerType, InputStream, OutputStream>::codec()
{
}

template <typename ElementType, typename IntegerType, typename InputStream, typename OutputStream>
inline bool codec<ElementType, IntegerType, InputStream, OutputStream>::encode(const model_type& model,
                                                                               const input_stream_type& input_stream,
                                                                               const output_stream_type& output_stream,
                                                                               std::size_t& original_size,
                                                                               std::size_t& encoded_size)
{
    original_size = 0;
    encoded_size = 0;

    bool result = true;

    return result;
}

template <typename ElementType, typename IntegerType, typename InputStream, typename OutputStream>
inline bool codec<ElementType, IntegerType, InputStream, OutputStream>::decode(const model_type& model,
                                                                               const input_stream_type& input_stream,
                                                                               const output_stream_type& output_stream,
                                                                               std::size_t original_size)
{
    bool result = true;

    return result;
}

END_NAMESPACE

#endif // __COMPRESSION_LZW_CODEC_H__
