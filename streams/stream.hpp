//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __COMPRESSION_STREAM_H__
#define __COMPRESSION_STREAM_H__

#pragma once

BEGIN_NAMESPACE(compression)

interface stream : private noncopyable
{
    using status_type = int32_t;

    virtual bool eos() const = 0;
    virtual status_type status() const = 0;
};

END_NAMESPACE

#endif // __COMPRESSION_STREAM_H__
