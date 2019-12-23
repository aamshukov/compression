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
    using integer_type = uint32_t;
    using model_type = lzw::binary_model<char_type, integer_type>;

    using input_stream_type = input_string_stream<char_type>;
    std::shared_ptr<input_stream_type> input_stream(std::make_shared<input_stream_type>(L"101010"));

    std::wcout << (*input_stream).data() << L':' << (*input_stream).data().size() << std::endl;

    using output_stream_type = output_string_stream<char_type>;
    std::shared_ptr<output_stream_type> output_stream(std::make_shared<output_stream_type>());

    lzw::codec<char_type, integer_type, input_stream_type, output_stream_type> lzwc;

    std::size_t original_size;
    std::size_t encoded_size;

    std::shared_ptr<model_type> model(std::make_shared<model_type>());

    bool rc1 = lzwc.encode(model, input_stream, output_stream, original_size, encoded_size);
    std::wcout << (*output_stream).data() << L':' << (*output_stream).data().size() << std::endl;

    std::shared_ptr<input_stream_type> input_stream2(std::make_shared<input_stream_type>((*output_stream).data()));
    std::shared_ptr<output_stream_type> output_stream2(std::make_shared<output_stream_type>());

    bool rc2 = lzwc.decode(model, input_stream2, output_stream2, original_size);
    std::wcout << (*output_stream2).data() << std::endl;

    std::wcout << (rc2 && ((*input_stream).data() == (*output_stream2).data()) ? L"ok" : L"error") << std::endl << std::endl;
}
