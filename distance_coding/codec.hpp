//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
// Algoritm by Edgar Binder
//  Usenet group comp.compression by Edgar Binder in 2000.
// R. Bastys
//  "Fibonacci Coding Within the Burrows-Wheeler Compression Scheme"
//
// Ватолин Д., Ратушняк А., Смирнов М., Юкин В.
//  Методы сжатия данных. Устройство архиваторов, сжатие изображений и видео. - М.: ДИАЛОГ-МИФИ, 2003. - 384 с.
//
// Donald Adjeroh, Tim Bell, Amar Mukherjee
//  THE BURROWSWHEELER TRANSFORM:
//  Data Compression, Suffix Arrays, and Pattern Matching
#ifndef __COMPRESSION_DISTANCE_CODING_CODEC_H__
#define __COMPRESSION_DISTANCE_CODING_CODEC_H__

#pragma once

BEGIN_NAMESPACE(compression::distance_coding)

template <typename ElementType, typename IntegerType>
class codec
{
    public:
        using integer_type = IntegerType;
        using element_type = ElementType;

        using abc_type = std::shared_ptr<abc<element_type>>;

        using original_data_type = string_type;
        using encoded_data_type = std::vector<integer_type>;

    private:
        enum class state
        {
            unknown = 0,
            processed
        };

    public:
                            codec();

        bool                encode(const abc_type& abc,
                                   const original_data_type& input_data,
                                   encoded_data_type& output_data,
                                   integer_type& original_size,
                                   integer_type& encoded_size);

        bool                decode(const abc_type& abc,
                                   const encoded_data_type& input_data,
                                   original_data_type& output_data,
                                   integer_type original_size);
};

template <typename ElementType, typename IntegerType>
inline codec<ElementType, IntegerType>::codec()
{
}

template <typename ElementType, typename IntegerType>
inline bool codec<ElementType, IntegerType>::encode(const abc_type& abc,
                                                    const original_data_type& input_data,
                                                    encoded_data_type& output_data,
                                                    integer_type& original_size,
                                                    integer_type& encoded_size)
{
    // s* = abc + s + eof
    // processed symbol is lower case letter:                 a
    // non-processed, encircled, symbol is upper case symbol: A
    // currently processing symbol is in parens:             (a)
    // if no more symbols - output 0, that means need to add 1 to all encoded precedents
    //
    // abc  | data      | eof
    // acrs | rccrsaaaa | #
    //
    // abc:
    //  a:  (a) c r s   R C C R S (a) A A A    distance = 6 (number of upper case symbols 5 + 1)
    //  c:  a (c) r s   R (c) C R S a A A A    distance = 2 (number of upper case symbols 1 + 1)
    //  r:  a c (r) s   (r) c C R S a A A A    distance = 1 (number of upper case symbols 0 + 1)
    //  s:  a c r (s)   r c C R (s) a A A A    distance = 3 (number of upper case symbols 2 + 1)
    //
    // input:
    //  r:  a c r s   (r) c C (r) s a A A A    distance = 2 (number of upper case symbols 1 + 1)
    //  c:  a c r s   r (c) (c) r s a A A A    skip because it is part of a run
    //  c:  a c r s   r c (c) r s a A A A      distance = 0 (no more 'c')
    //  r:  a c r s   r c c (r) s a A A A      distance = 0 (no more 'r')
    //  s:  a c r s   r c c r (s) a A A A      distance = 0 (no more 's')
    //  a:  a c r s   r c c r s (a) (a) A A    skip because it is part of a run
    //  a:  a c r s   r c c r s a (a) (a) A    skip because it is part of a run
    //  a:  a c r s   r c c r s a a (a) (a)    skip because it is part of a run
    //                                         6 2 1 3 2 0 0 0
    assert(!(*abc).data().empty());
    assert(!input_data.empty());

    original_size = 0;
    encoded_size = 0;

    bool result = true;

    integer_type abc_size = static_cast<integer_type>((*abc).data().size());
    integer_type input_size = static_cast<integer_type>(input_data.size());

    std::vector<state> states(input_size);

    // phase I (calculate first occurence of each symbol from the abc)
    for(integer_type i = 0; i < abc_size; i++)
    {
        auto ch = (*abc).data()[i];

        integer_type precedents = 0; // current symbol precedents - how many unknown (encircled) positions counted + 1

        for(integer_type k = 0; k < input_size; k++)
        {
            if(input_data[k] == ch)
            {
                states[k] = state::processed;

                if(!(i + 1 == abc_size && input_data[0] == ch)) // special case when the last abc symbol == the first input symbol, абдкр рдакраааабб
                    output_data.emplace_back(precedents + 1);                                                                     

                break;
            }
            else
            {
                if(states[k] == state::unknown)
                {
                    precedents++;
                }
            }
        }
    }

    // phase II (encode input)
    for(integer_type i = 0; i < input_size; i++)
    {
        auto ch = input_data[i];

        integer_type j = i + 1;

        while(j < input_size && input_data[j] == ch) // process run
        {
            states[j] = state::processed;
            j++;
        }

        if(j == input_size)
        {
            break; // end of input
        }

        i = j - 1; // after the next symbol is not equal - rollback

        integer_type precedents = 0; // current symbol precedents - how many unknown (encircled) positions counted + 1
        integer_type previous_precedents = static_cast<integer_type>(output_data.size());

        for(integer_type k = i; k < input_size; k++)
        {
            if(input_data[k] == ch)
            {
                if(states[k] == state::unknown)
                {
                    states[k] = state::processed;
                    output_data.emplace_back(precedents + 1);
                    break;
                }
            }
            else
            {
                if(states[k] == state::unknown)
                {
                    precedents++;
                }
            }
        }

        if(static_cast<integer_type>(output_data.size()) == previous_precedents)
        {
            output_data.emplace_back(0); // no more such symbols, write 0
        }
    }

    if(result)
    {
        original_size = input_size;
        encoded_size = static_cast<integer_type>(output_data.size());
    }

    return result;
}

template <typename ElementType, typename IntegerType>
inline bool codec<ElementType, IntegerType>::decode(const abc_type& abc,
                                                    const encoded_data_type& input_data,
                                                    original_data_type& output_data,
                                                    integer_type original_size)
{
    assert(!(*abc).data().empty());

    bool result = true;

    integer_type org_size = 0;

    // phase I (restore first occurence of each symbol from the abc)


    // phase II (decode input)

    result = org_size == original_size;
    return result;
}

END_NAMESPACE

#endif // __COMPRESSION_DISTANCE_CODING_CODEC_H__
