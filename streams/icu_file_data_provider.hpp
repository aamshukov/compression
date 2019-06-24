//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __COMPRESSION_ICU_FILE_DATA_PROVIDER_H__
#define __COMPRESSION_ICU_FILE_DATA_PROVIDER_H__

#pragma once

#include <unicode/uchar.h>
#include <unicode/unistr.h>
#include <unicode/ustdio.h>

BEGIN_NAMESPACE(compression)

class icu_file_data_provider : public data_provider<codepoint>
{
    public:
        const codepoint invalid_codepoint = 0xFFFFFFFF;

        using status_type = data_provider::status_type;

    private:
        using file_type = UFILE*;

    public:
        file_type   my_file;
        status_type my_status;
        string_type my_file_name;

    public:
                    icu_file_data_provider(const string_type& file_name, const char* encoding, const char* mode);
        virtual    ~icu_file_data_provider();

        status_type status() const override;

        bool        eof() const override;

        bool        get(codepoint& value) override;
        bool        put(const codepoint& value) override;

        void        flush() override;
};

inline icu_file_data_provider::icu_file_data_provider(const string_type& file_name, const char* encoding, const char* mode)
                             : my_file(nullptr),
                               my_status(0),
                               my_file_name(file_name)
{
    UFILE* ufile(u_fopen_u(reinterpret_cast<const UChar*>(my_file_name.c_str()), mode, nullptr, encoding));

    my_status = ufile == nullptr;

    if(my_status == 0 && ufile != nullptr)
    {
        my_status = fseek(u_fgetfile(ufile), 0L, SEEK_SET);

        if(my_status == 0)
        {
            my_file = ufile;
        }
    }
}

inline icu_file_data_provider::~icu_file_data_provider()
{
    if(my_file != nullptr)
    {
        u_fflush(my_file);
        u_fclose(my_file);

        my_file = nullptr;
        my_status = 0;
    }
}

inline bool icu_file_data_provider::eof() const
{
    return u_feof(my_file);
}

inline typename icu_file_data_provider::status_type icu_file_data_provider::status() const
{
    return my_status;
}

inline bool icu_file_data_provider::get(codepoint& value)
{
    bool result = false;

    if(my_status == 0 && my_file != nullptr)
    {
        value = u_fgetcx(my_file);

        result = value != invalid_codepoint;

        my_status = value == invalid_codepoint ? invalid_codepoint : 0;
    }

    return result;
}

inline bool icu_file_data_provider::put(const codepoint& value)
{
    bool result = false;

    if(my_status == 0 && my_file != nullptr)
    {
        UChar32 ch = u_fputc(value, my_file);

        result = ch == value;

        my_status = ch; // EOF in case of error
    }

    return result;
}

inline void icu_file_data_provider::flush()
{
    if(my_status == 0 && my_file != nullptr)
    {
        u_fflush(my_file);
    }
}

END_NAMESPACE

#endif // __COMPRESSION_ICU_FILE_DATA_PROVIDER_H__
