//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __COMPRESSION_INPUT_STRING_STREAM_H__
#define __COMPRESSION_INPUT_STRING_STREAM_H__

#pragma once

BEGIN_NAMESPACE(compression)

template <typename T = char_type>
class input_string_stream : public input_stream<T>
{
    public:
        using datum_type = T;

        using data_type = std::basic_string<datum_type, std::char_traits<datum_type>, std::allocator<datum_type>>;
        using stream_type = std::basic_stringstream<datum_type, std::char_traits<datum_type>, std::allocator<datum_type>>;

    private:
        stream_type     my_stream;

    public:
                        input_string_stream(const data_type& string);

        data_type       data() const;

        bool            eos() const override;
        bool            read(datum_type& value) override;
};

template <typename T>
inline input_string_stream<T>::input_string_stream(const data_type& string)
                             : my_stream(string)
{
}

template <typename T>
inline typename input_string_stream<T>::data_type input_string_stream<T>::data() const
{
    return my_stream.str();
}

template <typename T>
inline bool input_string_stream<T>::eos() const
{
    return my_stream.eof();
}

template <typename T>
inline bool input_string_stream<T>::read(typename input_string_stream<T>::datum_type& value)
{
    my_stream.get(value);

    bool result = !(my_stream.fail() || my_stream.bad());

    return result;
}

END_NAMESPACE

#endif // __COMPRESSION_INPUT_STRING_STREAM_H__
