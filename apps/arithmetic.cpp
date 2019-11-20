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
    const wchar_t* slices[] = { L"000000100000000000000000000000000000000101000000000100000000000000010010001000100000000000000000000000000001000000000000100000000010000000000000000000000000000000000000000010001000000100000000000000010000000000000000000000000000000000000000001",
                                L"0000001000000000000000100000010001000000100000000000000100000000000000000000000010000000100000000000000100000000000000000000000010001000000000000000000001000000000000000000000000001000000000000000000000000000000000000000000000000101001",
                                L"00000000000000000000000000100000000000000000000000000000000010000000001100100000010010000000000000000010000000100000000000000000100000000000000000000000000000000000100000000000010000000000000001000000001010000000000000001",
                                L"00000000000000010100000000000000010000000011000000000000000000000000000000000000000000000010010000100000000000000100000100001000000100000000000000000000000001100000001000000000000000000000000001",
                                L"0000000000001001000000100000000000010000100000000000000000000000000000000000000000000000100000000000000000010010100000100001000100000000000010000000100000000000000000000000000000000100001",
                                L"000000000010000000000000000100000100010000001000000000000000100001000000100000000000000000000000000000000000000000000011100000000100000000000000000000010000000010000011",
                                L"0000100000000000000000000000000000000000001001010000000000000000000000100010000000000100000000010000000000000000000000000100010000101010000000100001001",
                                L"0000000000000000000000001000000000000000000000100001000000011000000000000000000101000000001000110000000001010010000000000000010001000000001",
                                L"01100000000000100001000000000000000000000000000100010000000000010100000000000001000001000000000000010000110000000000010011",
                                L"0000100000000000010001001000000100100000000000000000000000000000000100000010000001000100010010000101000001001",
                                L"0000000100001000010000001001010101000000000101000001000000010000000000000110010000000000000001",
                                L"000000001001000100000000000000010110000000010000001001011000000101000000100101",
                                L"110011010000100010001100000100000100000100000010000001000010001",
                                L"10000000110001010010010001100010001111000101",
                                L"0110111110110000011110001001001",
                                L"1111111111111111" };

    using integer_type = uint32_t;
    using model_type = arithmetic::binary_model<char_type, integer_type>;

    std::size_t total = 0;

    std::size_t r = 16;

    for(auto i = 0; i < r; i++)
    {
        using input_stream_type = input_string_stream<char_type>;
        std::shared_ptr<input_stream_type> input_stream(std::make_shared<input_stream_type>(slices[i]));

        std::wcout << (*input_stream).data() << L':' << (*input_stream).data().size() << std::endl;

        using output_stream_type = output_string_stream<char_type>;
        std::shared_ptr<output_stream_type> output_stream(std::make_shared<output_stream_type>());

        auto n = (*input_stream).data().size();
        auto p1 = static_cast<integer_type>(((double)r / n) * 32768);
        if(i >= r - 1)
        //if(p1 >= 32768)
        {
            //total += (*input_stream).data().size();
            continue;
        }
        std::vector<integer_type> probability = { 32768 - p1, p1 };

        auto abc(std::make_shared<arithmetic::simple_abc<>>());
        std::shared_ptr<model_type> model(std::make_shared<model_type>(probability, abc, (*input_stream).data().size(), r));

        arithmetic::codec<char_type, integer_type, input_stream_type, output_stream_type> ac;

        std::size_t original_size;
        std::size_t encoded_size;

        bool rc1 = ac.encode(model, input_stream, output_stream, original_size, encoded_size);
        std::wcout << (*output_stream).data() << L':' << (*output_stream).data().size() << std::endl;

        total += (*output_stream).data().size();

        std::shared_ptr<input_stream_type> input_stream2(std::make_shared<input_stream_type>((*output_stream).data()));
        std::shared_ptr<output_stream_type> output_stream2(std::make_shared<output_stream_type>());

        std::shared_ptr<model_type> model1(std::make_shared<model_type>(probability, abc, (*input_stream).data().size(), r));
        bool rc2 = ac.decode(model1, input_stream2, output_stream2, original_size);
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
    std::shared_ptr<model_type> model(std::make_shared<model_type>(probability, abc, (*input_stream).data().size(), 8));

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
