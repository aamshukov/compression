//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
// based on Mark Nelson's https://github.com/marknelson/LZW/blob/master/lzw.h
// and on Fundamental Data Compression, Ida Menguy Pu
#ifndef __COMPRESSION_LZW_CODEC_H__
#define __COMPRESSION_LZW_CODEC_H__

#pragma once

BEGIN_NAMESPACE(compression::lzw)

// TODO:
//  0) remove limit for codes size - now it is up to KeyType/ValueType
//  1) add validation for codes so they do not excceed limit (MAX_CODE)
//  2) extend streams to accept vector of values
//  too lazy do it now :( ...
template <typename ElementType, typename IntegerType, typename KeyType, typename ValueType, typename InputStream, typename OutputStream>
class codec
{
    public:
        using integer_type = IntegerType;
        using element_type = ElementType;

        using key_type = KeyType;
        using value_type = ValueType;

        using model_type = std::shared_ptr<model<ElementType, IntegerType, KeyType, ValueType>>;

        using input_stream_type = std::shared_ptr<InputStream>;
        using output_stream_type = std::shared_ptr<OutputStream>;

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
                                   std::size_t& original_size);
};

template <typename ElementType, typename IntegerType, typename KeyType, typename ValueType, typename InputStream, typename OutputStream>
inline codec<ElementType, IntegerType, KeyType, ValueType, InputStream, OutputStream>::codec()
{
}

template <typename ElementType, typename IntegerType, typename KeyType, typename ValueType, typename InputStream, typename OutputStream>
inline bool codec<ElementType, IntegerType, KeyType, ValueType, InputStream, OutputStream>::encode(const model_type& model,
                                                                                                   const input_stream_type& input_stream,
                                                                                                   const output_stream_type& output_stream,
                                                                                                   std::size_t& original_size,
                                                                                                   std::size_t& encoded_size)
{
    //  1: word <-- ''
    //  2: while not EOF do
    //  3:      X <-- read_next_character()
    //  4:      if word + x is in the dictionary then
    //  5:          word <-- word + x
    //  6:      else
    //  7:          output the dictionary index for word
    //  8:          add word + x to the dictionary
    //  9:          word <-- x
    //  10:     end if
    //  11. end while
    //  12: output the dictionary index for word
    //
    original_size = 0;
    encoded_size = 0;

    bool result = true;

    std::size_t org_size = 0;
    std::size_t enc_size = 0;

    value_type next_code = static_cast<value_type>((*model).codes().size());

    std::basic_string<element_type> current_string;

    element_type symbol;

    while(!(*input_stream).eos())
    {
        if(!(*input_stream).read(symbol))
        {
            // if !result - report error and exit
            result = (*input_stream).eos();
            break;
        }

        org_size++;

        current_string = current_string + symbol;

        if((*model).codes().find(current_string) == (*model).codes().end())
        {
            (*model).codes()[current_string] = next_code++;

            current_string.erase(current_string.size() - 1);

            enc_size++;
            (*output_stream).write((*model).codes()[current_string]);

            current_string = symbol;
        }
    }

    if(!current_string.empty())
    {
        enc_size++;
        (*output_stream).write((*model).codes()[current_string]);
    }

    (*output_stream).flush();

    if(result)
    {
        original_size = org_size;
        encoded_size = enc_size;
    }

    return result;
}

template <typename ElementType, typename IntegerType, typename KeyType, typename ValueType, typename InputStream, typename OutputStream>
inline bool codec<ElementType, IntegerType, KeyType, ValueType, InputStream, OutputStream>::decode(const model_type& model,
                                                                                                   const input_stream_type& input_stream,
                                                                                                   const output_stream_type& output_stream,
                                                                                                   std::size_t& original_size)
{
    //  1:  read a token x from the compressed file
    //  2:  look up dictionary for element at x
    //  3:  output element
    //  4:  word <-- element
    //  5:  while not E0F do
    //  6:      read x
    //  7:      look up dictionary for element at x
    //  8:      if there is no entry yet for index x then
    //  9:          element <-- word + firstCharOfWord
    //  10:     end if
    //  11:     output element
    //  12:     add word + firstCharOfElement to the dictionary
    //  13:     word <-- element
    //  14: end while
    //
    original_size = 0;

    bool result = true;

    std::size_t org_size = 0;

    std::basic_string<element_type> previous_string;

    value_type next_code = static_cast<value_type>((*model).strings().size());

    value_type code;

    while(!(*input_stream).eos())
    {
        if(!(*input_stream).read(code))
        {
            // if !result - report error and exit
            result = (*input_stream).eos();
            break;
        }

        org_size++;

        if((*model).strings().find(code) == (*model).strings().end())
        {
                                                      // firstCharOfElement
            (*model).strings()[code] = previous_string + previous_string[0];
        }

        const auto& str((*model).strings()[code]);
        std::for_each(str.begin(), str.end(), [&output_stream](auto ch){(*output_stream).write(ch);});

        if(previous_string.size())
        {                                                       // firstCharOfElement
            (*model).strings()[next_code++] = previous_string + (*model).strings()[code][0];
        }

        previous_string = (*model).strings()[code];
    }

    if(result)
    {
        original_size = org_size;
    }

    return result;
}

END_NAMESPACE

#endif // __COMPRESSION_LZW_CODEC_H__
