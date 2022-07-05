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
    auto *test_data = L"00101111001110101111011101001001101110010110110011011110010111101110011110001000010110011101101111101001001111011010110100100100110010010000011111111110100010101000110101111010110111101000111000100110101111010001111101101010101001111110000111011011110001001111101010000101001101011010011";

    using integer_type = uint32_t;
    using model_type = lzw::binary_model<char_type, integer_type, string_type, uint8_t>;

    {
        std::shared_ptr<model_type> model(std::make_shared<model_type>());

        (*model).codes().emplace(L"0", 0);
        (*model).codes().emplace(L"1", 1);
        //(*model).codes().emplace(L"2", 2);
        //(*model).codes().emplace(L"3", 3);
        //(*model).codes().emplace(L"4", 4);
        //(*model).codes().emplace(L"5", 5);
        //(*model).codes().emplace(L"6", 6);
        //(*model).codes().emplace(L"7", 7);
        //(*model).codes().emplace(L"8", 8);
        //(*model).codes().emplace(L"9", 9);

        using input_stream_type = input_string_stream<char_type>;
        std::shared_ptr<input_stream_type> input_stream(std::make_shared<input_stream_type>(test_data));

        std::wcout << (*input_stream).data() << L':' << (*input_stream).data().size() << std::endl;

        using output_stream_type = output_byte_stream<file_data_provider>;
        string_type file_name(LR"(d:\tmp\lzw.txt)");
        auto fdp(std::make_shared<file_data_provider>(file_name, L"w+b"));
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

        (*model).codes().emplace(L"0", 0);
        (*model).codes().emplace(L"1", 1);
        //(*model).codes().emplace(L"2", 2);
        //(*model).codes().emplace(L"3", 3);
        //(*model).codes().emplace(L"4", 4);
        //(*model).codes().emplace(L"5", 5);
        //(*model).codes().emplace(L"6", 6);
        //(*model).codes().emplace(L"7", 7);
        //(*model).codes().emplace(L"8", 8);
        //(*model).codes().emplace(L"9", 9);

        using input_stream_type = input_byte_stream<file_data_provider>;
        string_type file_name(LR"(d:\tmp\lzw.txt)");
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
