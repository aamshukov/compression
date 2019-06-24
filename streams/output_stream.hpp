//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __COMPRESSION_OUTPUT_STREAM_H__
#define __COMPRESSION_OUTPUT_STREAM_H__

#pragma once

BEGIN_NAMESPACE(compression)

template <typename T>
interface output_stream : public io_stream
{
    using element_type = T;

    virtual bool write(bit value) = 0;
    virtual bool write(const element_type& value) = 0;

    virtual void flush() = 0;
};

END_NAMESPACE

#endif // __COMPRESSION_OUTPUT_STREAM_H__
