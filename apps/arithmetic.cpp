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

#include <arithmetic/binary_abc.hpp>
#include <arithmetic/binary_model.hpp>

USINGNAMESPACE(compression)


void test_binary_model_pf();
void test_binary_model();
void test_simple_model();

int main()
{
    test_binary_model_pf();
    test_binary_model();
    test_simple_model();
}

void test_binary_model_pf()
{
    const wchar_t* slices[] = { L"0000000000000010001000000000000000000000010000001000000000000000000000000000000000000000000000000000000000000000001000000000000000000000000001000000000000000000101000000000000000000010000000000000000000000100000100100000000000001000000001001000000000000000",
                                L"000010000000000000000000000000000000000100010000000000000000100000000000000010001000000001000001000010000100000000000000000000000000000000000000000010000000000000100000000000000000000000000000000000000000100000000000000010000010100000000000",
                                L"00000000000000000000000100000000000000000000001000000100000000010000000100001000000000000000000001000000000000000000000000000100100100000000010001100000000100000000000100000000000000000000000000000000000000000000001000000000",
                                L"0000000000000000000000010000000000100001001100000100000000000000010000000000000001000000000000000000001000100000010000000000000000000000000100000000000011000000010000000000000000000000100000000000000000000000",
                                L"000000010001000100000000000000000000100000000000000000000000000000000000000000001000000000100000000000000000000010000001000001000000000000000000000001000001000100010000011000000000100000000000",
                                L"00000000000000000001000000000000000100000000000100000000010000000000010000000010010001010000000000000000000000000000000000100001000000000000010000000001000000000000100000000000",
                                L"0000000110100001000000000000000000000000000001000000000000100000001000000000000000000000100100000000000001100000000100001000001000000000000100000000000000000000",
                                L"000010000000000001000000000000000000010100000000101000010000000000000000000001000000101000000000000010000000000101000000000000000000001100000000",
                                L"00000000001001000000000000001000000010000000000000000100000000000001101000000100001100000100010000100000000001000000001000000000",
                                L"0000101000000000001000000000000000000100000000000000000000100010001000000000000001001000000111000010011000000000",
                                L"010000001000100000001011000001010000000000000000000100000000100000000000101000000000000011000000",
                                L"00001010000101000000001000000000011010001000010000100001001000001000000000000001",
                                L"0000000110000000010100011011100000000000000110000110010010000000",
                                L"100000000100000111101000000010000111001100000100",
                                L"00111001110100010001110010000010",
                                L"0100011101100110" };

    using integer_type = uint32_t;
    using model_type = arithmetic::binary_model<char_type, integer_type>;

    std::size_t total = 0;

    for(auto i = 0; i < 16; i++)
    {
        using input_stream_type = input_string_stream<char_type>;
        std::shared_ptr<input_stream_type> input_stream(std::make_shared<input_stream_type>(slices[i]));

        std::wcout << (*input_stream).data() << L':' << (*input_stream).data().size() << std::endl;

        using output_stream_type = output_string_stream<char_type>;
        std::shared_ptr<output_stream_type> output_stream(std::make_shared<output_stream_type>());

        auto n = (*input_stream).data().size();
        auto p1 = static_cast<integer_type>((16.0 / n) * 32768);
        if(p1 >= 32768)
        {
            total += (*input_stream).data().size();
            continue;
        }
        std::vector<integer_type> probability = { 32768 - p1, p1 };

        auto abc(std::make_shared<arithmetic::simple_abc<>>());
        std::shared_ptr<model_type> model(std::make_shared<model_type>(probability, abc));

        arithmetic::codec<char_type, integer_type, input_stream_type, output_stream_type> ac;

        std::size_t original_size;
        std::size_t encoded_size;

        bool rc1 = ac.encode(model, input_stream, output_stream, original_size, encoded_size);
        std::wcout << (*output_stream).data() << L':' << (*output_stream).data().size() << std::endl;

        total += (*output_stream).data().size();

        std::shared_ptr<input_stream_type> input_stream2(std::make_shared<input_stream_type>((*output_stream).data()));
        std::shared_ptr<output_stream_type> output_stream2(std::make_shared<output_stream_type>());

        bool rc2 = ac.decode(model, input_stream2, output_stream2, original_size);
        std::wcout << (*output_stream2).data() << std::endl;

        std::wcout << (rc2 && ((*input_stream).data() == (*output_stream2).data()) ? L"ok" : L"error") << std::endl << std::endl;
    }

    std::wcout << total << std::endl << std::endl;
}

void test_binary_model()
{
    using input_stream_type = input_string_stream<char_type>;
    std::shared_ptr<input_stream_type> input_stream(std::make_shared<input_stream_type>
    (L"0000110001000001011000000000000001000000"));

    std::wcout << (*input_stream).data() << std::endl;

    using output_stream_type = output_string_stream<char_type>;
    std::shared_ptr<output_stream_type> output_stream(std::make_shared<output_stream_type>());

    using integer_type = uint32_t;

    using model_type = arithmetic::binary_model<char_type, integer_type>;

    std::vector<integer_type> probability = { 49152, 16384 };

    auto abc(std::make_shared<arithmetic::simple_abc<>>());
    std::shared_ptr<model_type> model(std::make_shared<model_type>(probability, abc));

    arithmetic::codec<char_type, integer_type, input_stream_type, output_stream_type> ac;

    std::size_t original_size;
    std::size_t encoded_size;

    bool rc1 = ac.encode(model, input_stream, output_stream, original_size, encoded_size);
    std::wcout << (*output_stream).data() << L':' << (*output_stream).data().size() << std::endl;

    std::shared_ptr<input_stream_type> input_stream2(std::make_shared<input_stream_type>((*output_stream).data()));
    std::shared_ptr<output_stream_type> output_stream2(std::make_shared<output_stream_type>());

    bool rc2 = ac.decode(model, input_stream2, output_stream2, original_size);
    std::wcout << (*output_stream2).data() << std::endl;

    std::wcout << (rc2 && ((*input_stream).data() == (*output_stream2).data()) ? L"ok" : L"error") << std::endl << std::endl;
}

void test_simple_model()
{
    using input_stream_type = input_string_stream<char_type>;
    std::shared_ptr<input_stream_type> input_stream(std::make_shared<input_stream_type>(L"2320")); // 011011000

    std::wcout << (*input_stream).data() << std::endl;

    using output_stream_type = output_string_stream<char_type>;
    std::shared_ptr<output_stream_type> output_stream(std::make_shared<output_stream_type>());

    using integer_type = uint32_t;

    using model_type = arithmetic::simple_model<char_type, integer_type>;

    std::vector<integer_type> probability = { 3276, 3276, 32768, 26215 };
    //std::vector<integer_type> probability = { 5, 5, 50, 40 }; // 0.0, 0.05, 0.1, 0.6

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

    std::wcout << (rc2 && ((*input_stream).data() == (*output_stream2).data()) ? L"ok" : L"error") << std::endl << std::endl;
}
