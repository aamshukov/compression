//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
// based on mathematicalmonk's videos
// https://www.youtube.com/playlist?list=PLE125425EC837021F
//
#ifndef __COMPRESSION_ARITHMETIC_H__
#define __COMPRESSION_ARITHMETIC_H__

#pragma once

BEGIN_NAMESPACE(compression)

template <typename ElementType, typename InputStream, typename OutputStream, typename IntegerType, IntegerType Precision = 32, IntegerType Scalling = 100000>
class arithmetic
{
    public:
        using integer_type = unsigned long long;

        using element_type = ElementType;
        using input_stream_type = InputStream;
        using output_stream_type = OutputStream;

    private:
        input_stream_type&  my_input_stream;
        output_stream_type& my_output_stream;

        integer_type        my_precision;
        integer_type        my_whole;
        integer_type        my_half;
        integer_type        my_quarter;
        integer_type        my_quarter3;
        integer_type        my_scalling;
        integer_type        my_zero_probabiliy;

    public:
                            arithmetic(input_stream_type& input_stream, output_stream_type& output_stream);

        bool                encode(std::size_t& original_size, std::size_t& encoded_size);
        bool                decode(std::size_t& original_size, std::size_t& decoded_size);
};

template <typename ElementType, typename InputStream, typename OutputStream, typename IntegerType, IntegerType Precision, IntegerType Scalling>
inline arithmetic<ElementType, InputStream, OutputStream, IntegerType, Precision, Scalling>::arithmetic(input_stream_type& input_stream, output_stream_type& output_stream)
    : my_input_stream(input_stream), my_output_stream(output_stream)
{
    my_precision = Precision;
    my_whole = static_cast<integer_type>(1) << my_precision;
    my_half = my_whole / 2;
    my_quarter = my_whole / 4;
    my_quarter3 = 3 * my_quarter;
    my_scalling = Scalling;
    my_zero_probabiliy = my_whole / 2;
}

template <typename ElementType, typename InputStream, typename OutputStream, typename IntegerType, IntegerType Precision, IntegerType Scalling>
inline bool arithmetic<ElementType, InputStream, OutputStream, IntegerType, Precision, Scalling>::encode(std::size_t& original_size, std::size_t& encoded_size)
{
    original_size = 0;
    encoded_size = 0;

    bool result = true;

    std::size_t org_size = 0;
    std::size_t enc_size = 0;

    while(!my_input_stream.eos())
    {
    }

    if(result)
    {
        original_size = org_size;
        encoded_size = enc_size;
    }

    return result;
}

template <typename ElementType, typename InputStream, typename OutputStream, typename IntegerType, IntegerType Precision, IntegerType Scalling>
inline bool arithmetic<ElementType, InputStream, OutputStream, IntegerType, Precision, Scalling>::decode(std::size_t& original_size, std::size_t& decoded_size)
{
    original_size = 0;
    decoded_size = 0;

    bool result = true;
    return result;
}

END_NAMESPACE

#endif // __COMPRESSION_ARITHMETIC_H__
