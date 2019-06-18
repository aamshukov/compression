//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __COMPRESSION_FILE_DATA_PROVIDER_H__
#define __COMPRESSION_FILE_DATA_PROVIDER_H__

#pragma once

BEGIN_NAMESPACE(compression)

class file_data_provider : public data_provider<byte>
{
    public:
        using status_type = data_provider::status_type;

    private:
        using file_type = FILE*;

    public:
        file_type   my_file;
        errno_t     my_status;
        string_type my_file_name;

    public:
                    file_data_provider(const string_type& file_name);
        virtual    ~file_data_provider();

        bool        eof() const override;

        status_type status() const override;

        bool        get(byte& value) override;
        bool        put(const byte& value) override;
};

inline file_data_provider::file_data_provider(const string_type& file_name)
                         : my_file(nullptr),
                           my_status(0),
                           my_file_name(file_name)
{
    FILE* file(nullptr);

    my_status = _wfopen_s(&file, file_name.c_str(), L"rb+");

    if(my_status == 0 && file != nullptr)
    {
        my_status = fseek(file, 0L, SEEK_SET);

        if(my_status == 0)
        {
            my_file = file;
        }
    }
}

inline file_data_provider::~file_data_provider()
{
    if(my_file != nullptr)
    {
        fflush(my_file);
        fclose(my_file);

        my_file = nullptr;
        my_status = 0;
    }
}

inline bool file_data_provider::eof() const
{
    return feof(my_file);
}

inline typename file_data_provider::status_type file_data_provider::status() const
{
    return my_status;
}

inline bool file_data_provider::get(byte& value)
{
    bool result = false;

    if(my_status == 0 && my_file != nullptr)
    {
        result = fread(&value, sizeof(byte), 1, my_file) == 1;

        my_status = ferror(my_file);
    }

    return result;
}

inline bool file_data_provider::put(const byte& value)
{
    bool result = false;

    if(my_status == 0 && my_file != nullptr)
    {
        result = fwrite(&value, 1, sizeof(byte), my_file) == sizeof(byte);

        my_status = ferror(my_file);
    }

    return result;
}

END_NAMESPACE

#endif // __COMPRESSION_FILE_DATA_PROVIDER_H__
