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

#include <lzw/model.hpp>
#include <lzw/binary_model.hpp>
#include <lzw/codec.hpp>

USINGNAMESPACE(compression)

int main()
{
    auto *test_data = L"01011100010100010010101000011111001011011010011011111010001010001110011001011101010101001111001010000010100000101100000101111111001101001100001110001101110010010111011000101001001000001111000000011111111100100010101010010101101011010011111010101111001000110111000100010110001100000011011001000101101100011011011010000100000000001001110111010100011101101010000101111110101010011100101011011110100010000100000000010101111001000010100110010000011011100001110010001010001111000100001101111101111100000110110101010001011010111110011111100011000010100110010101111110110011010110100100000000101010111111110011101001001100101101010100100100101011001110000000101111100010010110100110101100001100101101110111110111000100011010101110011101011101101011000111101000011011111101001010000101010110011001101000001111001100100111111110010001011011111100111001011000000000111001011001011111100110100010011111001101000100001110000100101100011111100011110011010101101000100100001101100100011101000111000101000010000111011010110111";

    using integer_type = uint32_t;
    using model_type = lzw::binary_model<char_type, integer_type, string_type, uint8_t>;

    {
        std::shared_ptr<model_type> model(std::make_shared<model_type>());

        (*model).codes().emplace(L"0", 0);
        (*model).codes().emplace(L"1", 1);

        using input_stream_type = input_string_stream<char_type>;
        std::shared_ptr<input_stream_type> input_stream(std::make_shared<input_stream_type>(test_data));

        std::wcout << (*input_stream).data() << L':' << (*input_stream).data().size() << std::endl;

        using output_stream_type = output_byte_stream<file_data_provider>;
        string_type file_name(LR"(d:\tmp\lzw.txt))");
        auto fdp(std::make_shared<file_data_provider>(file_name, L"r+b"));
        std::shared_ptr<output_stream_type> output_stream(std::make_shared<output_stream_type>(fdp));

        lzw::codec<char_type, integer_type, string_type, uint8_t, input_stream_type, output_stream_type> lzwc;

        std::size_t original_size;
        std::size_t encoded_size;

        bool rc1 = lzwc.encode(model, input_stream, output_stream, original_size, encoded_size);

        (*fdp).rewind();
        byte code;
        while((*fdp).get(code))
        {
            std::cout << std::to_string(code) << ' ';
        }
        std::wcout << std::endl;

        (*fdp).rewind();
    }
    {
        std::shared_ptr<model_type> model(std::make_shared<model_type>());

        (*model).strings().emplace(0, L"0");
        (*model).strings().emplace(1, L"1");

        using input_stream_type = input_byte_stream<file_data_provider>;
        string_type file_name(LR"(d:\tmp\lzw.txt))");
        auto fdp(std::make_shared<file_data_provider>(file_name, L"rb"));
        std::shared_ptr<input_stream_type> input_stream(std::make_shared<input_stream_type>(fdp));

        using output_stream_type = output_string_stream<char_type>;
        std::shared_ptr<output_stream_type> output_stream(std::make_shared<output_stream_type>());

        lzw::codec<char_type, integer_type, string_type, uint8_t, input_stream_type, output_stream_type> lzwc;

        std::size_t original_size;

        bool rc2 = lzwc.decode(model, input_stream, output_stream, original_size);
        std::wcout << (*output_stream).data() << L':' << (*output_stream).data().size() << std::endl;

        std::wcout << (rc2 && (test_data == (*output_stream).data()) ? L"ok" : L"error") << std::endl << std::endl;
    }
}
