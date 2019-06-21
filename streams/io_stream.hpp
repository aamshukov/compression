//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __COMPRESSION_IO_STREAM_H__
#define __COMPRESSION_IO_STREAM_H__

#pragma once

BEGIN_NAMESPACE(compression)

class io_stream : public stream
{
    public:
        using status_type = stream::status_type;

    protected:
        status_type my_status;

    public:
                    io_stream();

        status_type status() const override;
};

inline io_stream::io_stream()
                : my_status(0)
{
}

inline typename io_stream::status_type io_stream::status() const
{
    return my_status;
}

END_NAMESPACE

#endif // __COMPRESSION_IO_STREAM_H__
