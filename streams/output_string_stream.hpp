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
                        output_string_stream();

        data_type       data() const;

        bool            eos() const override;

        bool            write(bit value) override;
        bool            write(const datum_type& value) override;
};

template <typename T>
inline output_string_stream<T>::output_string_stream()
                              : my_stream(L"")
{
}

template <typename T>
inline typename output_string_stream<T>::data_type output_string_stream<T>::data() const
{
    return my_stream.str();
}

template <typename T>
bool output_string_stream<T>::eos() const
{
    return my_stream.eof();
}

template <typename T>
inline bool output_string_stream<T>::write(bit value)
{
    static char_type bits[] = { L'0', L'1'};

    my_stream.put(static_cast<datum_type>(bits[value]));

    bool result = !(my_stream.fail() || my_stream.bad());

    return result;
}

template <typename T>
inline bool output_string_stream<T>::write(const datum_type& value)
{
    my_stream.put(static_cast<datum_type>(value));

    bool result = !(my_stream.fail() || my_stream.bad());

    return result;
}

END_NAMESPACE

#endif // __COMPRESSION_OUTPUT_STRING_STREAM_H__
