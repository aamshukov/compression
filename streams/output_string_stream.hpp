//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __COMPRESSION_OUTPUT_STRING_STREAM_H__
#define __COMPRESSION_OUTPUT_STRING_STREAM_H__

#pragma once

BEGIN_NAMESPACE(compression)

template <typename T = char_type>
class output_string_stream : public output_stream<T>
{
    private:
        using datum_type = T;

        using data_type = std::basic_string<datum_type, std::char_traits<datum_type>, std::allocator<datum_type>>;
        using stream_type = std::basic_stringstream<datum_type, std::char_traits<datum_type>, std::allocator<datum_type>>;

    private:
        stream_type     my_stream;

    public:
                        output_string_stream(const data_type& string);

        bool            eos() const override;
        bool            write(const datum_type& value) override;
};

template <typename T>
inline output_string_stream<T>::output_string_stream(const data_type& string)
                              : my_stream(string)
{
}

template <typename T>
bool output_string_stream<T>::eos() const
{
    return my_stream.eof();
}

template <typename T>
inline bool output_string_stream<T>::write(const typename output_string_stream<T>::datum_type& value)
{
    my_stream.put(value);

    bool result = !(my_stream.fail() || my_stream.bad());

    return result;
}

END_NAMESPACE

#endif // __COMPRESSION_OUTPUT_STRING_STREAM_H__
