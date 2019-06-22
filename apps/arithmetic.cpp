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

#include <arithmetic/abc.hpp>
#include <arithmetic/model.hpp>
#include <arithmetic/codec.hpp>

#include <arithmetic/simple_abc.hpp>
#include <arithmetic/simple_model.hpp>

USINGNAMESPACE(compression)


void test_string_input();

int main()
{
    test_string_input();
}

void test_string_input()
{
    using input_stream_type = input_string_stream<char_type>;
    std::shared_ptr<input_stream_type> input_stream(std::make_shared<input_stream_type>(L"2320")); // 011011000

    std::wcout << (*input_stream).data() << std::endl;

    using output_stream_type = output_string_stream<char_type>;
    std::shared_ptr<output_stream_type> output_stream(std::make_shared<output_stream_type>());

    using integer_type = uint32_t;

    using model_type = arithmetic::simple_model<char_type, integer_type>;

    std::vector<integer_type> probability = { 3276, 3276, 32768, 26216 };
    //std::vector<integer_type> probability = { 5, 5, 50, 40 };

    auto abc(std::make_shared<arithmetic::simple_abc<>>());
    std::shared_ptr<model_type> model(std::make_shared<model_type>(probability, abc));

    arithmetic::codec<char_type, integer_type, input_stream_type, output_stream_type> ac;

    std::size_t original_size;
    std::size_t encoded_size;

    bool rc1 = ac.encode(model, input_stream, output_stream, original_size, encoded_size);
    std::wcout << (*output_stream).data() << std::endl;

    std::shared_ptr<input_stream_type> input_stream2(std::make_shared<input_stream_type>((*output_stream).data()));
    std::shared_ptr<output_stream_type> output_stream2(std::make_shared<output_stream_type>());

    bool rc2 = ac.decode(model, input_stream2, output_stream2, original_size);
    std::wcout << (*output_stream2).data() << std::endl;

    std::wcout << (rc2 ? L"ok" : L"error") << std::endl;
}
