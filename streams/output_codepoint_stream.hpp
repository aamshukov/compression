//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __COMPRESSION_OUTPUT_CODEPOINT_STREAM_H__
#define __COMPRESSION_OUTPUT_CODEPOINT_STREAM_H__

#pragma once

BEGIN_NAMESPACE(compression)

template <typename D>
class output_codepoint_stream : public output_stream<codepoint>
{
    public:
        using data_provider_type = std::shared_ptr<D>;

    private:
        data_provider_type  my_data_provider;

    public:
                            output_codepoint_stream(const data_provider_type& data_provider);

        bool                eos() const override;
        bool                write(const codepoint& value) override;

};

template <typename D>
inline output_codepoint_stream<D>::output_codepoint_stream(const data_provider_type& data_provider)
                                 : my_data_provider(data_provider)
{
}

template <typename D>
bool output_codepoint_stream<D>::eos() const
{
    return (*my_data_provider).eof();
}

template <typename D>
inline bool output_codepoint_stream<D>::write(const codepoint& value)
{
    static char_type bits[] = { L'0', L'1'};

    bool result = (*my_data_provider).put(static_cast<codepoint>(bits[value]));

    return result;
}

END_NAMESPACE

#endif // __COMPRESSION_OUTPUT_CODEPOINT_STREAM_H__
