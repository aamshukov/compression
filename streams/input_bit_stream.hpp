//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __COMPRESSION_INPUT_BIT_STREAM_H__
#define __COMPRESSION_INPUT_BIT_STREAM_H__

#pragma once

BEGIN_NAMESPACE(compression)

template <typename D>
class input_bit_stream : public input_stream<bit>
{
    public:
        using data_provider_type = std::shared_ptr<D>;

    private:
        data_provider_type  my_data_provider;

        byte                my_cache; // current byte
        std::size_t         my_count; // bits count

    public:
                            input_bit_stream(const data_provider_type& data_provider);

        bool                eos() const override;
        bool                read(bit& value) override;
};

template <typename D>
inline input_bit_stream<D>::input_bit_stream(const data_provider_type& data_provider)
                          : my_data_provider(data_provider),
                            my_cache(0),
                            my_count(0)
{
}

template <typename D>
bool input_bit_stream<D>::eos() const
{
    return (*my_data_provider).eof();
}

template <typename D>
inline bool input_bit_stream<D>::read(bit& value)
{
    bool result = true;

    if(my_count == 0)
    {
        result = (*my_data_provider).get(my_cache);

        my_count = BYTE_SIZE;
    }

    if(result)
    {
        static byte masks[8] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

        my_count--;

        value = (my_cache & masks[my_count & 0x07]) == masks[my_count & 0x07];
    }

    return result;
}

END_NAMESPACE

#endif // __COMPRESSION_INPUT_BIT_STREAM_H__
