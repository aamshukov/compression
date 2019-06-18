//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __COMPRESSION_IO_STREAM_H__
#define __COMPRESSION_IO_STREAM_H__

#pragma once

BEGIN_NAMESPACE(compression)

template <typename T>
class io_stream : public stream<T>
{
    public:
        using status_type = stream<T>::status_type;

    protected:
        status_type my_status;

    public:
                    io_stream();

        status_type status() const override;
};

template <typename T>
inline io_stream<T>::io_stream()
                   : my_status(0)
{
}

template <typename T>
inline typename io_stream<T>::status_type io_stream<T>::status() const
{
    return my_status;
}

END_NAMESPACE

#endif // __COMPRESSION_IO_STREAM_H__
