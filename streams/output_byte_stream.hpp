//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __COMPRESSION_OUTPUT_BYTE_STREAM_H__
#define __COMPRESSION_OUTPUT_BYTE_STREAM_H__

#pragma once

BEGIN_NAMESPACE(compression)

template <typename D>
class output_byte_stream : public output_stream<byte>
{
    public:
        using data_provider_type = std::shared_ptr<D>;

    private:
        data_provider_type  my_data_provider;

    public:
                            output_byte_stream(const data_provider_type& data_provider);

        bool                eos() const override;

        bool                write(bit value) override;
        bool                write(const byte& value) override;
};

template <typename D>
inline output_byte_stream<D>::output_byte_stream(const data_provider_type& data_provider)
                            : my_data_provider(data_provider)
{
}

template <typename D>
bool output_byte_stream<D>::eos() const
{
    return (*my_data_provider).eof();
}

template <typename D>
inline bool output_byte_stream<D>::write(bit value)
{
    bool result = (*my_data_provider).put(static_cast<byte>(value));
    return result;
}

template <typename D>
inline bool output_byte_stream<D>::write(const byte& value)
{
    bool result = (*my_data_provider).put(value);
    return result;
}

END_NAMESPACE

#endif // __COMPRESSION_OUTPUT_BYTE_STREAM_H__
