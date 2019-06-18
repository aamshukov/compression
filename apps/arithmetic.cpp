#include <core/pch.hpp>
#include <core/noncopyable.hpp>

#include <streams/stream.hpp>

#include <streams/io_stream.hpp>

#include <streams/input_stream.hpp>
#include <streams/output_stream.hpp>

#include <streams/data_provider.hpp>
#include <streams/file_data_provider.hpp>
#include <streams/icu_file_data_provider.hpp>

#include <streams/input_byte_stream.hpp>
#include <streams/output_byte_stream.hpp>

#include <streams/input_bit_stream.hpp>
#include <streams/output_bit_stream.hpp>

#include <streams/input_codepoint_stream.hpp>
#include <streams/output_codepoint_stream.hpp>

#include <streams/input_string_stream.hpp>
#include <streams/output_string_stream.hpp>

#include <arithmetic/arithmetic.hpp>

USINGNAMESPACE(compression)


void test_byte_input();
void test_codepoint_input();
void test_bit_input();

int main()
{
    test_byte_input();
    test_codepoint_input();
    test_bit_input();
}

void test_byte_input()
{
    file_data_provider input_dp(LR"(d:\tmp\data.txt)", L"rb");
    
    using input_stream_type = input_byte_stream<file_data_provider>;
    input_stream_type input_stream(input_dp);

    file_data_provider output_dp(LR"(d:\tmp\data.dat)", L"wb");
    using output_stream_type = output_bit_stream<file_data_provider>;
    output_stream_type output_stream(output_dp);

    using integer_type = unsigned long long;

    arithmetic<byte, input_stream_type, output_stream_type, integer_type, 32, 1000> ac(input_stream, output_stream);

    ac;
}

void test_codepoint_input()
{
    icu_file_data_provider input_dp(LR"(d:\tmp\data.txt)", "UTF-8", "rb");
    
    using input_stream_type = input_codepoint_stream<icu_file_data_provider>;
    input_stream_type input_stream(input_dp);

    icu_file_data_provider output_dp(LR"(d:\tmp\data.dat)", "UTF-8", "wb");
    using output_stream_type = output_codepoint_stream<icu_file_data_provider>;
    output_stream_type output_stream(output_dp);

    using integer_type = unsigned long long;

    arithmetic<byte, input_stream_type, output_stream_type, integer_type, 32, 1000> ac(input_stream, output_stream);

    ac;
}

void test_bit_input()
{
    file_data_provider input_dp(LR"(d:\tmp\data.txt)", L"rb");
    
    using input_stream_type = input_bit_stream<file_data_provider>;
    input_stream_type input_stream(input_dp);

    file_data_provider output_dp(LR"(d:\tmp\data.dat)", L"wb");
    using output_stream_type = output_bit_stream<file_data_provider>;
    output_stream_type output_stream(output_dp);

    using integer_type = unsigned long long;

    arithmetic<bit, input_stream_type, output_stream_type, integer_type, 32, 1000> ac(input_stream, output_stream);

    ac;
}
