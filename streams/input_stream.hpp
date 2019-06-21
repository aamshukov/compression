//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __COMPRESSION_INPUT_STREAM_H__
#define __COMPRESSION_INPUT_STREAM_H__

#pragma once

BEGIN_NAMESPACE(compression)

template <typename T>
interface input_stream : public io_stream
{
    using element_type = T;

    virtual bool read(element_type& value) = 0;
};

END_NAMESPACE

#endif // __COMPRESSION_INPUT_STREAM_H__
