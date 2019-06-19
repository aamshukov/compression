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

#include <arithmetic/model.hpp>
#include <arithmetic/simple_model.hpp>
#include <arithmetic/codec.hpp>

USINGNAMESPACE(compression)


void test_byte_input();
//void test_codepoint_input();
//void test_bit_input();

int main()
{
    test_byte_input();
    //test_codepoint_input();
    //test_bit_input();
}

void test_byte_input()
{
    std::shared_ptr<file_data_provider> input_dp(std::make_shared<file_data_provider>(LR"(d:\tmp\data.txt)", L"rb"));
    
    using input_stream_type = input_byte_stream<file_data_provider>;
    std::shared_ptr<input_stream_type> input_stream(std::make_shared<input_stream_type>(input_dp));

    std::shared_ptr<file_data_provider> output_dp(std::make_shared<file_data_provider>(LR"(d:\tmp\data.dat)", L"wb"));
    using output_stream_type = output_bit_stream<file_data_provider>;
    std::shared_ptr<output_stream_type> output_stream(std::make_shared<output_stream_type>(output_dp));

    using integer_type = unsigned long long;

    using model_type = arithmetic::simple_model<byte, integer_type>;
    
    std::shared_ptr<model_type> model(std::make_shared<model_type>());

    arithmetic::codec<byte, input_stream_type, output_stream_type, integer_type> ac(input_stream, output_stream, model, 32, 1000);

    ac;
}

//void test_codepoint_input()
//{
//    icu_file_data_provider input_dp(LR"(d:\tmp\data.txt)", "UTF-8", "rb");
//    
//    using input_stream_type = input_codepoint_stream<icu_file_data_provider>;
//    input_stream_type input_stream(input_dp);
//
//    icu_file_data_provider output_dp(LR"(d:\tmp\data.dat)", "UTF-8", "wb");
//    using output_stream_type = output_codepoint_stream<icu_file_data_provider>;
//    output_stream_type output_stream(output_dp);
//
//    using integer_type = unsigned long long;
//
//    arithmetic::codec<byte, input_stream_type, output_stream_type, integer_type> ac(input_stream, output_stream, 32, 1000);
//
//    ac;
//}
//
//void test_bit_input()
//{
//    file_data_provider input_dp(LR"(d:\tmp\data.txt)", L"rb");
//    
//    using input_stream_type = input_bit_stream<file_data_provider>;
//    input_stream_type input_stream(input_dp);
//
//    file_data_provider output_dp(LR"(d:\tmp\data.dat)", L"wb");
//    using output_stream_type = output_bit_stream<file_data_provider>;
//    output_stream_type output_stream(output_dp);
//
//    using integer_type = unsigned long long;
//
//    arithmetic::codec<bit, input_stream_type, output_stream_type, integer_type> ac(input_stream, output_stream, 32, 1000);
//
//    ac;
//}
