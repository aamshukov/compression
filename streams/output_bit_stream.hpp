//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __COMPRESSION_OUTPUT_BIT_STREAM_H__
#define __COMPRESSION_OUTPUT_BIT_STREAM_H__

#pragma once

BEGIN_NAMESPACE(compression)

template <typename D>
class output_bit_stream : public output_stream<bit>
{
    public:
        using data_provider_type = D;

    private:
        data_provider_type& my_data_provider;

        byte                my_cache; // current byte
        std::size_t         my_count; // bits count

    public:
                            output_bit_stream(data_provider_type& data_provider);

        bool                eos() const override;
        bool                write(const bit& value) override;
};

template <typename D>
inline output_bit_stream<D>::output_bit_stream(data_provider_type& data_provider)
                           : my_data_provider(data_provider),
                             my_cache(0),
                             my_count(0)
{
}

template <typename D>
bool output_bit_stream<D>::eos() const
{
    return my_data_provider.eof();
}

template <typename D>
inline bool output_bit_stream<D>::write(const bit& value)
{
    bool result = true;

    my_cache = my_cache << 1 | value;
    my_count++;

    if(my_count == BYTE_SIZE)
    {
        result = my_data_provider.put(my_cache);

        my_cache = 0;
        my_count = 0;
    }

    return result;
}

END_NAMESPACE

#endif // __COMPRESSION_OUTPUT_BIT_STREAM_H__
