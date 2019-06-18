//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __COMPRESSION_DATA_PROVIDER_H__
#define __COMPRESSION_DATA_PROVIDER_H__

#pragma once

BEGIN_NAMESPACE(compression)

template <typename T>
interface data_provider : private noncopyable
{
    using status_type = int32_t;

    virtual bool eof() const = 0;
    virtual status_type status() const = 0;

    virtual bool get(T& value) = 0;
    virtual bool put(const T& value) = 0;
};

END_NAMESPACE

#endif // __COMPRESSION_DATA_PROVIDER_H__
