//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __COMPRESSION_INPUT_CODEPOINT_STREAM_H__
#define __COMPRESSION_INPUT_CODEPOINT_STREAM_H__

#pragma once

BEGIN_NAMESPACE(compression)

template <typename D>
class input_codepoint_stream : public input_stream<codepoint>
{
    public:
        using data_provider_type = std::shared_ptr<D>;

    private:
        data_provider_type  my_data_provider;

    public:
                            input_codepoint_stream(const data_provider_type& data_provider);

        bool                eos() const override;
        bool                read(codepoint& value) override;
};

template <typename D>
inline input_codepoint_stream<D>::input_codepoint_stream(const data_provider_type& data_provider)
                                : my_data_provider(data_provider)
{
}

template <typename D>
bool input_codepoint_stream<D>::eos() const
{
    return (*my_data_provider).eof();
}

template <typename D>
inline bool input_codepoint_stream<D>::read(codepoint& value)
{
    bool result = (*my_data_provider).get(value);
    return result;
}

END_NAMESPACE

#endif // __COMPRESSION_INPUT_CODEPOINT_STREAM_H__
