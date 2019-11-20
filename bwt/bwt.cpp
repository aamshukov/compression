#include <core/pch.hpp>
#include <core/noncopyable.hpp>

#include <suffixarray/suffixarray.hpp>
#include <bwt/bwt.hpp>

USINGNAMESPACE(compression)

int main()
{
    struct sa_traits
    {
        static int abc_size()
        {
            return 255;
        }
    };

    //std::string str("mmiissiissiippii");
    std::string str("0000000000000010001000000000000000000000010000001000000000000000000000000000000000000000000000000000000000000000001000000000000000000000000001000000000000000000101000000000000000000010000000000000000000000100000100100000000000001000000001001");
    //std::string str("23424132214321233132212413414343");
    //std::string str("23424132214321233132212413414343");
    //std::string str("abracadabra");
    //std::string str("DRDOBBS");


    //std::string str("2342413221432123");
    //std::string str("3132212413414314");
    //std::string str("4143212212413213");

    using bwt_type = bwt<byte, sa_traits>;

    bwt_type::elements_type data(str.begin(), str.end());
    data.push_back(0); // add virtual sentinel

    bwt_type::print(data);

    bwt_type::elements_type encoded_data;
    bwt_type::elements_type decoded_data;

    bwt_type::encode(data, encoded_data);
    bwt_type::print(encoded_data);

    bwt_type::decode(encoded_data, decoded_data);
    bwt_type::print(decoded_data);

    std::wcout << ((data == decoded_data) ? L"ok" : L"error") << std::endl << std::endl;
}
