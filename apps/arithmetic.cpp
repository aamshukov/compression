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

#include <arithmetic/decimal_abc.hpp>
#include <arithmetic/decimal_model.hpp>

#include <algorithms/permutation/permutation.hpp>
#include <iprecision.h>

USINGNAMESPACE(compression)
USINGNAMESPACE(algorithms)

void test_binary_model_pf();
void test_binary_model(const char_type* data = nullptr);
void test_decimal_model(const char_type* data = nullptr);
void test_simple_model();
void test_rle(const char_type* data = nullptr);
void test_permutation256();
void test_permutation256_n();

int main()
{
    test_rle();
    //test_binary_model();
    //test_decimal_model();
    test_permutation256_n();
    //test_binary_model();
    //test_binary_model_pf();
    //test_simple_model();
}

#define str2wstr(__str)                                                 \
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;   \
    std::wstring wide = converter.from_bytes(__str);

#define replace(__xx__)                                                 \
{                                                                       \
    std::size_t start = 0;                                              \
    string_type substr(__xx__);                                         \
    string_type stub(L"9");                                             \
    while((start = str.find(substr, start)) != std::string::npos)       \
    {                                                                   \
        str.replace(start, substr.length(), stub);                      \
        start += stub.length();                                         \
    }                                                                   \
}


template <typename T, std::size_t N> constexpr std::size_t array_size(T const (&)[N])
{
    return N;
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

        auto ii2 = (*output_stream).data(); // last 0s are not significant!!!
        if(ii2[ii2.size() - 1] == L'0')
           ii2 = ii2.substr(0, ii2.size() - 1);
           ii2 = ii2.substr(0, ii2.rfind(L'1') + 1);

        total += ii2.size();

        std::wcout << ii2 << L':' << ii2.size() << std::endl;
        std::shared_ptr<input_stream_type> input_stream2(std::make_shared<input_stream_type>(ii2));
        std::shared_ptr<output_stream_type> output_stream2(std::make_shared<output_stream_type>());

        std::shared_ptr<model_type> model1(std::make_shared<model_type>(probability, abc, (*input_stream).data().size(), r));
        bool rc2 = ac.decode(model1, input_stream2, output_stream2, original_size);
        std::wcout << (*output_stream2).data() << std::endl;

        std::wcout << (rc2 && ((*input_stream).data() == (*output_stream2).data()) ? L"ok" : L"error") << std::endl << std::endl;
    }

    std::wcout << total << std::endl << std::endl;
}

void test_binary_model(const char_type* data)
{
    if(data == nullptr)
    {
        data = L"0000101000010001011111100001010010010000110000010010100000000010100010000000110010110000001000001000100000001000001000000000100100011101001001000010001011100000010000001001000000010010101100001010100110101010101010100000000110000010100000000100001000001001101010000110100110101000101001000100100100100001000000000000010000000101001001010100000000010000011000011000000010000000100000001001001000000000001000000100000010011001000001110111001100101001000010100101010100000000000001000010001100100000001100";
    }

    string_type str(data);

    using input_stream_type = input_string_stream<char_type>;
    std::shared_ptr<input_stream_type> input_stream(std::make_shared<input_stream_type>(str.c_str()));

    std::wcout << (*input_stream).data() << L':' << (*input_stream).data().size() << std::endl << std::endl;

    using output_stream_type = output_string_stream<char_type>;
    std::shared_ptr<output_stream_type> output_stream(std::make_shared<output_stream_type>());

    using integer_type = uint32_t;

    using model_type = arithmetic::binary_model<char_type, integer_type>;

    auto x0count = 0;
    auto x1count = 0;

    std::for_each(str.begin(), str.end(), [&x0count](auto el){ if(el == L'0') x0count++; });
    std::for_each(str.begin(), str.end(), [&x1count](auto el){ if(el == L'1') x1count++; });

    auto p0 = std::max((integer_type)1, static_cast<integer_type>((static_cast<double>(x0count) / str.size()) * 32768));
    auto p1 = std::max((integer_type)1, static_cast<integer_type>((static_cast<double>(x1count) / str.size()) * 32768));

    p0 += 32768 - (p0 + p1);
    std::vector<integer_type> probability = { p0, p1 };

    auto abc(std::make_shared<arithmetic::binary_abc<>>());
    std::shared_ptr<model_type> model(std::make_shared<model_type>(probability, abc, (*input_stream).data().size(), 460));

    arithmetic::codec<char_type, integer_type, input_stream_type, output_stream_type> ac;

    std::size_t original_size;
    std::size_t encoded_size;

    bool rc1 = ac.encode(model, input_stream, output_stream, original_size, encoded_size);
    std::wcout << (*output_stream).data() << L':' << (*output_stream).data().size() << std::endl << std::endl;

    std::shared_ptr<input_stream_type> input_stream2(std::make_shared<input_stream_type>((*output_stream).data()));
    std::shared_ptr<output_stream_type> output_stream2(std::make_shared<output_stream_type>());

    bool rc2 = ac.decode(model, input_stream2, output_stream2, original_size);
    std::wcout << (*output_stream2).data() << std::endl;

    std::wcout << (rc2 && ((*input_stream).data() == (*output_stream2).data()) ? L"ok" : L"error") << std::endl << std::endl;
}

void test_decimal_model(const char_type* data)
{
    if(data == nullptr)
    {
        data = L"4214030020100100000000000010000200003000000210100101010111005100012000240020000100001100020030010000000000300000000000000002010002200000000210011000100000012014201010010200000220000000000002020000000001000001000000010100032000100200000000010000060003020010000100101000002000000000000040010020020000000020110000000120000000010020000000001000011000000000000001100002000000110110003300000000200010100000000200310012001000200000000300000000000000000010110100000010000000000058600240100000100014001000000000";
    }

    string_type str(data);

    replace(L"'10'")
    replace(L"'11'")
    replace(L"'12'")
    replace(L"'13'")
    replace(L"'14'")
    replace(L"'15'")
    replace(L"'16'")
    replace(L"'17'")
    replace(L"'18'")
    replace(L"'19'")

    using input_stream_type = input_string_stream<char_type>;
    std::shared_ptr<input_stream_type> input_stream(std::make_shared<input_stream_type>(str.c_str()));

    std::wcout << (*input_stream).data() << L':' << (*input_stream).data().size() << std::endl << std::endl;

    using output_stream_type = output_string_stream<char_type>;
    std::shared_ptr<output_stream_type> output_stream(std::make_shared<output_stream_type>());

    using integer_type = uint32_t;

    using model_type = arithmetic::decimal_model<char_type, integer_type>;

    auto x0count = 0;
    auto x1count = 0;
    auto x2count = 0;
    auto x3count = 0;
    auto x4count = 0;
    auto x5count = 0;
    auto x6count = 0;
    auto x7count = 0;
    auto x8count = 0;
    auto x9count = 0;

    std::for_each(str.begin(), str.end(), [&x0count](auto el){ if(el == L'0') x0count++; });
    std::for_each(str.begin(), str.end(), [&x1count](auto el){ if(el == L'1') x1count++; });
    std::for_each(str.begin(), str.end(), [&x2count](auto el){ if(el == L'2') x2count++; });
    std::for_each(str.begin(), str.end(), [&x3count](auto el){ if(el == L'3') x3count++; });
    std::for_each(str.begin(), str.end(), [&x4count](auto el){ if(el == L'4') x4count++; });
    std::for_each(str.begin(), str.end(), [&x5count](auto el){ if(el == L'5') x5count++; });
    std::for_each(str.begin(), str.end(), [&x6count](auto el){ if(el == L'6') x6count++; });
    std::for_each(str.begin(), str.end(), [&x7count](auto el){ if(el == L'7') x7count++; });
    std::for_each(str.begin(), str.end(), [&x8count](auto el){ if(el == L'8') x8count++; });
    std::for_each(str.begin(), str.end(), [&x9count](auto el){ if(el == L'9') x9count++; });
    
    auto p0 = std::max((integer_type)1, static_cast<integer_type>((static_cast<double>(x0count) / str.size()) * 32768));
    auto p1 = std::max((integer_type)1, static_cast<integer_type>((static_cast<double>(x1count) / str.size()) * 32768));
    auto p2 = std::max((integer_type)1, static_cast<integer_type>((static_cast<double>(x2count) / str.size()) * 32768));
    auto p3 = std::max((integer_type)1, static_cast<integer_type>((static_cast<double>(x3count) / str.size()) * 32768));
    auto p4 = std::max((integer_type)1, static_cast<integer_type>((static_cast<double>(x4count) / str.size()) * 32768));
    auto p5 = std::max((integer_type)1, static_cast<integer_type>((static_cast<double>(x5count) / str.size()) * 32768));
    auto p6 = std::max((integer_type)1, static_cast<integer_type>((static_cast<double>(x6count) / str.size()) * 32768));
    auto p7 = std::max((integer_type)1, static_cast<integer_type>((static_cast<double>(x7count) / str.size()) * 32768));
    auto p8 = std::max((integer_type)1, static_cast<integer_type>((static_cast<double>(x8count) / str.size()) * 32768));
    auto p9 = std::max((integer_type)1, static_cast<integer_type>((static_cast<double>(x9count) / str.size()) * 32768));

    p0 += 32768 - (p0 + p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9);
    //std::vector<integer_type> probability = { 17336, 7429, 4699, 1651, 571, 571, 317, 127, 63, 4 };
    std::vector<integer_type> probability = { p0, p1, p2, p3, p4, p5, p6, p7, p8, p9 };

    std::size_t original_size;
    std::size_t encoded_size;

    {
        auto abc(std::make_shared<arithmetic::decimal_abc<>>());
        std::shared_ptr<model_type> model(std::make_shared<model_type>(probability,
                                                                       abc,
                                                                       (*input_stream).data().size(),
                                                                       x0count, x1count, x2count, x3count, x4count, x5count, x6count, x7count, x8count, x9count));

        arithmetic::codec<char_type, integer_type, input_stream_type, output_stream_type> ac;

        bool rc1 = ac.encode(model, input_stream, output_stream, original_size, encoded_size);
        std::wcout << (*output_stream).data() << L':' << (*output_stream).data().size() << std::endl << std::endl;
    }
    {
        auto abc(std::make_shared<arithmetic::decimal_abc<>>());
        std::shared_ptr<model_type> model(std::make_shared<model_type>(probability,
                                                                       abc,
                                                                       (*input_stream).data().size(),
                                                                       x0count, x1count, x2count, x3count, x4count, x5count, x6count, x7count, x8count, x9count));

        arithmetic::codec<char_type, integer_type, input_stream_type, output_stream_type> ac;

        std::shared_ptr<input_stream_type> input_stream2(std::make_shared<input_stream_type>((*output_stream).data()));
        std::shared_ptr<output_stream_type> output_stream2(std::make_shared<output_stream_type>());

        bool rc2 = ac.decode(model, input_stream2, output_stream2, original_size);
        std::wcout << (*output_stream2).data() << std::endl;

        std::wcout << (rc2 && ((*input_stream).data() == (*output_stream2).data()) ? L"ok" : L"error") << std::endl << std::endl;

        if(!(rc2 && ((*input_stream).data() == (*output_stream2).data())))
        {
            throw "zopa";
        }
    }
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

void test_rle(const char_type* data)
{
    if(data == nullptr)
    {
        data = L"0000101000010001011111100001010010010000110000010010100000000010100010000000110010110000001000001000100000001000001000000000100100011101001001000010001011100000010000001001000000010010101100001010100110101010101010100000000110000010100000000100001000001001101010000110100110101000101001000100100100100001000000000000010000000101001001010100000000010000011000011000000010000000100000001001001000000000001000000100000010011001000001110111001100101001000010100101010100000000000001000010001100100000001100";
    }

    string_type str(data);

    using permutation_type = permutation<int, int_precision>;

    permutation_type::elements_type rle;

    int kn = str.size() - 1;
    for(;;)
    {
        auto x0count = 0;
        auto x1count = 0;
        auto x2count = 0;
        auto x3count = 0;
        auto x4count = 0;
        auto x5count = 0;
        auto x6count = 0;
        auto x7count = 0;
        auto x8count = 0;
        auto x9count = 0;

        while(kn > 0 && str[kn] == L'0')
            zeroes++, kn--;
        if(zeroes > 0)
            rle.emplace(rle.begin(), zeroes - 1);

        while(kn > 0 && str[kn] == L'1')
            ones++, kn--;
        if(ones > 0)
            rle.emplace(rle.begin(), ones - 1);

        if(kn == 0)
            break;
    }

    string_type rle_str;
    std::for_each(rle.begin(), rle.end(), [&rle_str](auto el)
    {
        rle_str += std::to_wstring(el);
    });

    std::wcout << L"RLE(" << rle_str.size() << L"):" << rle_str << std::endl;
}

void test_permutation256_n()
{
    std::srand((unsigned int)std::time(nullptr));

    int kk = 0;

    for(auto k = 0; k < 1000; k++)
    {
        test_permutation256();
        kk= k;
    }
    std::cout << "KK: " << kk << std::endl;
}

template <typename T>
void fibonacci(const T& n, std::pair<std::size_t, T>& result)
{
    T x = 0;
    T y = 1;
    T z = 0;

    std::size_t k = 0;

    while(x < n)
    {
        z = x + y;
        x = y;
        y = z;
        k++;
    }

    result.first = k;
    result.second = x;
}

//?? starting from 8th perm select huffman if it is less then non-huffman
//?? poprobovat' huff less than 9 codes

#define PRINT_STATS 1//??_DEBUG

#define BUILD_32    0
#define BUILD_64    0
#define BUILD_128   0
#define BUILD_256   1
#define BUILD_512   0

void test_permutation256()
{
    using permutation_type = permutation<int, int_precision>;

#if BUILD_32 == 1
    permutation_type::size_type size_to_read = 14;  // 118
    permutation_type::size_type size = 32;
    permutation_type::size_type sub_size = 8;
    permutation_type::size_type coefficient = 2;
#elif BUILD_64 == 1
    permutation_type::size_type size_to_read = 37;
    permutation_type::size_type size = 64;
    permutation_type::size_type sub_size = 8;
    permutation_type::size_type coefficient = 1;
#elif BUILD_128 == 1
    permutation_type::size_type size_to_read = 89;
    permutation_type::size_type size = 128;
    permutation_type::size_type sub_size = 16;
    permutation_type::size_type coefficient = 2;
#elif BUILD_256 == 1
    permutation_type::size_type size_to_read = 210;
    permutation_type::size_type size_in_bits = 1683;
    permutation_type::size_type sizes_in_bits[] = { 1683, 1556, 1430, 1303, 1184, 1064, 945, 829, 716, 605, 498, 394, 295, 202, 117, 44, 0 };
    permutation_type::size_type size = 256;         // 1     2     3     4     5     6    7    8    9   10   11   12   13   14   15  16
    permutation_type::size_type sub_size = 16;
    permutation_type::size_type coefficient = 1;
#elif BUILD_512 == 1
    permutation_type::size_type size_to_read = 484;
    permutation_type::size_type size = 512;
    permutation_type::size_type sub_size = 32;
    permutation_type::size_type coefficient = 2;
#endif

#if PRINT_STATS == 1
    std::cout << "Permutation size:     " << size << std::endl;
    std::cout << "Permutation sub-size: " << sub_size << std::endl;
#endif

#pragma warning(disable: 4996) // _CRT_SECURE_NO_WARNINGS
    int hfile = _open("d:\\tmp\\pf.dat", O_RDONLY|O_BINARY, 0);
    std::unique_ptr<byte> data(new byte[size_to_read]);
    memset(data.get(), 0, size_to_read);
    auto offset = std::rand() % RAND_MAX;
    _lseek(hfile, (offset % 2 == 0) ? (16000000 - offset) : offset, SEEK_SET);
    int count = read(hfile, data.get(), size_to_read);
    std::string str_data("0x");
    for(auto k = 0; k < count; k++)
    {
        int b = data.get()[k];
        std::stringstream str_stream;
        str_stream << std::setfill ('0') << std::setw(2) << std::hex << b;
        str_data += str_stream.str();
    }
    _close(hfile);
    auto szs = str_data.size();
#pragma warning(default: 4996) // _CRT_SECURE_NO_WARNINGS

    permutation_type::rank_type rank = 
    //int_precision("0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF") - 
    int_precision(str_data.c_str());

    std::pair<std::size_t, int_precision> rank_fib;
    fibonacci<int_precision>(rank, rank_fib);
    std::cout << rank.toString() << std::endl;
    std::cout << (rank_fib.second - rank).toString() << ':' << rank_fib.first << std::endl;
    //rank = rank_fib.second; //??
    //std::cout << (rank_fib.second - rank).toString() << ':' << rank_fib.first << std::endl;

    permutation_type::elements_type permutation;
    permutation_type::unrank(rank, size, permutation);


    //int n = 123;
    //int factor[100000];
    //memset(factor, 0, sizeof(factor));
    //for(int i = 2; i < 100000; i++)
    //{
    //    auto r = rank % i;
    //    if(long(r) == 0)
    //    {
    //        factor[i] = i;
    //        rank /= i;
    //    }
    //}

    //string_type huffman_codes[] = { L"1", L"01", L"000", L"0011", L"00100", L"001011", L"0010101", L"00101000", L"00101001" };
    string_type huffman_codes[] = { L"0", L"10", L"1110", L"1111", L"11000", L"11010", L"110010", L"110110", L"110111", L"11001100", L"11001101", L"11001110", L"11001111" };
    //string_type huffman_codes[] = { L"0", L"10", L"1110", L"1111", L"11000", L"11010", L"110010", L"110110", L"110111" };
    //string_type huffman_codes[] = { L"0", L"11", L"1000", L"1010", L"1011", L"10010", L"10011" };
    //string_type huffman_codes[] = { L"1", L"01", L"001", L"0001", L"0000" };


#if PRINT_STATS == 1
    for(auto element : permutation)
    {
        std::wcout << std::to_wstring(element) << L' ';
    }

    std::wcout << std::endl << std::endl;

    for(auto element : permutation)
    {
        std::wcout << (element < size/2 ? L'0' : L'1');
    }

    std::wcout << std::endl << std::endl;
#endif
    using slice_type = std::vector<int8_t>;
    using slices_type = std::vector<slice_type>;

    std::vector<int8_t> commpressed_slices_sizes;

    slices_type slices;
    permutation_type::elements_type positions;

    for(auto k = sub_size/coefficient - 1; k >= 0; k--) //?? /2
    {
        // build slice
        slice_type slice(size, false);
        permutation_type::elements_type positions0;
        permutation_type::elements_type sub_permutation;

        auto lb = k * sub_size;  // low bound
        auto hb = lb + sub_size; // high bound

        for(auto j = 0; j < size; j++)
        {
            auto el = permutation[j];

            if(el >= lb && el < hb)
            {
                slice[j] = 1;
                positions0.emplace_back(j);
                sub_permutation.emplace_back(hb - permutation[j] - 1); // -1 cause permutations are 0 based
            }
        }

        permutation_type::rank_type sub_rank = permutation_type::rank(sub_permutation);

        std::pair<std::size_t, int_precision> sub_rank_fib;
        fibonacci<int_precision>(sub_rank, sub_rank_fib);
        std::cout << sub_rank.toString() << std::endl;
        std::cout << (sub_rank_fib.second - sub_rank).toString() << ':' << sub_rank_fib.first << std::endl;

        std::wstringstream stats_content;

        //for(auto element : slice)
        //{
        //    stream << std::to_wstring(element);
        //}
        //stream << L":" << std::to_wstring(slice.size()) << std::endl;

        if(!positions.empty())
        {
            // mark elements to be deleted
            for(auto position : positions)
            {
                slice[position] = 2;
            }

            auto it(slice.begin());

            while(it != slice.end())
            {
                if(*it == 2)
                {
                    it = slice.erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }

#if PRINT_STATS == 1
        for(auto element : slice)
        {
            stats_content << std::to_wstring(element);
        }
        
        auto strr = sub_rank.toString();
        string_type sub_rank_ws(strr.begin() + 1, strr.end());
        stats_content << L":" << std::to_wstring(slice.size()) << L":" << sub_rank_ws << std::endl;
#endif

        if(slice[slice.size() - 1] == 0) // last 0s are not significant!!!
        {
            auto it = std::find(slice.rbegin(), slice.rend(), 1) + 1;
            auto sz = slice.rend() - it + 1;
            slice.resize(sz);
        }

        slices.emplace_back(slice);

#if PRINT_STATS == 1
        for(auto element : slice)
        {
            stats_content << std::to_wstring(element);
        }
        stats_content << L":" << std::to_wstring(slice.size()) << std::endl;
        stats_content.flush();
#endif
        positions.insert(positions.end(), positions0.begin(), positions0.end());

#if PRINT_STATS == 1
        std::wcout << stats_content.str();
#endif

        // collect RLE, backward because the last element is always 1
        permutation_type::elements_type rle;

        //ii2 = L"00000 1 000 1 0 1 00 1 000 1 0 1 0 11 00 1 00 1 0 1 0000 1 0 11 0 1 000 1 0000 1 0 111 0 1 00 11 00 1 0 111";
        //      4     1 3   1 1 1 2  1 3   1 1 1 1 2  2  1 2  1 1 1 4    1 1 2  1 1 3   1 4    1 1 3   1 1 2  2  2  1 1 3

        int kn = slice.size() - 1;
        for(;;)
        {
            auto zeroes = 0;
            auto ones = 0;

            while(kn > 0 && slice[kn] == 0)
                zeroes++, kn--;
            if(zeroes > 0)
                rle.emplace(rle.begin(), zeroes - 1); //?? -1

            while(kn > 0 && slice[kn] == 1)
                ones++, kn--;
            if(ones > 0)
                rle.emplace(rle.begin(), ones - 1); //?? -1

            if(kn == 0)
                break;
        }
#if PRINT_STATS == 1
        string_type rle_str;
        std::for_each(rle.begin(), rle.end(), [&rle_str, &huffman_codes](auto el)
        {
            if(el >= array_size(huffman_codes))
                rle_str += L"'";
            rle_str += std::to_wstring(el);
            if(el >= array_size(huffman_codes))
                rle_str += L"'";
        });
        std::wcout << L"RLE(" << rle_str.size() << L"):" << rle_str << std::endl;
#endif

    }

    auto total = 0;
    for(auto& slice : slices)
    {
        total += slice.size();
    }

#if PRINT_STATS == 1
    std::wcout << total << std::endl << std::endl;
#endif

    // ..................
    using integer_type = uint32_t;
    using model_type = arithmetic::binary_model<char_type, integer_type>;

    string_type rles;

    total = 0;

    std::size_t r = sub_size;

    for(auto i = 0; i < r/coefficient; i++) //?? /2
    {
        using input_stream_type = input_string_stream<char_type>;
        string_type slice;
        std::for_each(slices[i].begin(), slices[i].end(), [&slice](auto el){slice += std::to_wstring(el);});
        std::shared_ptr<input_stream_type> input_stream(std::make_shared<input_stream_type>(slice.data()));

#if PRINT_STATS == 1
        std::wcout << (*input_stream).data() << L':' << (*input_stream).data().size() << std::endl;
#endif

        using output_stream_type = output_string_stream<char_type>;
        std::shared_ptr<output_stream_type> output_stream(std::make_shared<output_stream_type>());

        auto n = (*input_stream).data().size();
        auto p1 = static_cast<integer_type>(((double)r / n) * 32768);
        if(i >= r/coefficient - 1) //?? /2
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
#if PRINT_STATS == 1
        std::wcout << (*output_stream).data() << L':' << (*output_stream).data().size() << std::endl;
#endif
        auto ii2 = (*output_stream).data(); // last 0s are not significant!!!
        if(ii2[ii2.size() - 1] == L'0')
            ii2 = ii2.substr(0, ii2.size() - 1);
            ii2 = ii2.substr(0, ii2.rfind(L'1') + 1);

        // prefixed 0s
        //auto it = std::find(ii2.begin(), ii2.end(), L'1');
        //auto offset = std::max(0, (int)std::distance(ii2.begin(), it));
        //offset = std::min(32, offset); // extra 5 bits
        //ii2 = ii2.substr(offset);

        total += ii2.size();

        commpressed_slices_sizes.emplace_back(ii2.size());

#if PRINT_STATS == 1
        std::wcout << ii2 << L':' << ii2.size() << std::endl;
#endif
        std::shared_ptr<input_stream_type> input_stream2(std::make_shared<input_stream_type>(ii2));
        std::shared_ptr<output_stream_type> output_stream2(std::make_shared<output_stream_type>());

        std::shared_ptr<model_type> model1(std::make_shared<model_type>(probability, abc, (*input_stream).data().size(), r));
        bool rc2 = ac.decode(model1, input_stream2, output_stream2, original_size);

        if(!rc2)
        {
            throw "zopa";
        }

        // collect RLE, backward because the last element is always 1
        permutation_type::elements_type rle;

        //ii2 = L"00000 1 000 1 0 1 00 1 000 1 0 1 0 11 00 1 00 1 0 1 0000 1 0 11 0 1 000 1 0000 1 0 111 0 1 00 11 00 1 0 111";
        //        4     1 3   1 1 1 2  1 3   1 1 1 1 2  2  1 2  1 1 1 4    1 1 2  1 1 3   1 4    1 1 3   1 1 2  2  2  1 1 3

        int k = ii2.size() - 1;
        for(;;)
        {
            auto zeroes = 0;
            auto ones = 0;

            while(k > 0 && ii2[k] == '0')
                zeroes++, k--;
            if(zeroes > 0)
                rle.emplace(rle.begin(), zeroes - 1); //?? -1 because we know RLE is never == 0

            while(k > 0 && ii2[k] == '1')
                ones++, k--;
            if(ones > 0)
                rle.emplace(rle.begin(), ones - 1); //?? -1 because we know RLE is never == 0

            if(k == 0)
                break;
        }
        string_type rle_str;
        string_type huf_str;
        std::for_each(rle.begin(), rle.end(), [&rle_str, &huf_str, &huffman_codes](auto el)
        {
            if(el >= array_size(huffman_codes))
                rle_str += L"'";
            rle_str += std::to_wstring(el);
            if(el >= array_size(huffman_codes))
                rle_str += L"'";

            if(el >= array_size(huffman_codes))
                huf_str += L'.';
            else
                huf_str += huffman_codes[el]; //?? - 1]; because we know RLE is never == 0
        });
#if PRINT_STATS == 1
        std::wcout << L"RLE(" << rle_str.size() << L"):" << rle_str << std::endl;
        std::wcout << L"HUF(" << huf_str.size() << L"):" << huf_str << std::endl;
#endif
        rles += rle_str;

        auto ar_huf_diff = (int(ii2.size()) - int(huf_str.size()));
        std::wcout << L"ARITH and HUFF difference: " << ar_huf_diff << std::endl;

#if PRINT_STATS == 1
        std::wcout << (*output_stream2).data() << std::endl;
        std::wcout << (rc2 && ((*input_stream).data() == (*output_stream2).data()) ? L"ok" : L"error") << std::endl;

        std::wcout << total
                   //<< L':' << (size_in_bits - total) 
                   //<< L':' << (size_in_bits - sizes_in_bits[i] - total)
                   << std::endl << std::endl;
#endif
    }

    std::wcout << std::endl;

    std::wcout << L"RLE(" << rles.size() << L"):" << rles << std::endl;
    string_type huf_str;
    std::for_each(rles.begin(), rles.end(), [&huf_str, &huffman_codes](auto el)
    {
        if(el >= '0' && (el - '0') < array_size(huffman_codes))
            huf_str += huffman_codes[(el - '0')];//?? - 1];
        else
            huf_str += L'.';
    });
    std::wcout << L"HUF(" << huf_str.size() << L"):" << huf_str << std::endl << std::endl << std::endl;

    test_decimal_model(rles.c_str());

    std::wcout << std::endl << total << std::endl << std::endl;

    // calculate permutations-waves?????
    for(int i = 0, n = commpressed_slices_sizes.size(); i < n; i++)
    {
        auto local_total = 0;

        for(auto j = i; j < commpressed_slices_sizes.size(); j++)
        //for(int j = commpressed_slices_sizes.size() - i - 1; j >= 0; j--)
        {
            local_total += commpressed_slices_sizes[j];
        }

        // 1683, 1556, 1430, 1303, 1184, 1064, 945, 829, 716, 605, 498, 394, 295, 202, 117, 44
        auto bits = sizes_in_bits[commpressed_slices_sizes.size() - i + 1];
        auto summary = bits + local_total + (720 - 44 * i) + 256; //?? 256 should be compressed
        std::wcout << L"WAVE " << i << L": " << summary << std::endl;
    }

    std::wcout << std::endl;
}
