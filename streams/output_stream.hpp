//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __COMPRESSION_OUTPUT_STREAM_H__
#define __COMPRESSION_OUTPUT_STREAM_H__

#pragma once

BEGIN_NAMESPACE(compression)

template <typename T>
interface output_stream : public io_stream<T>
{
    virtual bool write(const T& value) = 0;
};

END_NAMESPACE

#endif // __COMPRESSION_OUTPUT_STREAM_H__
