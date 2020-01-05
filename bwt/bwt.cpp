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
    std::string str("002322310232031330041211704000135324012010011016800100100231000211020000000213170041000000312110010003210010001010200001001206020046200102411001001000100000004100000310221000230320411010111001200100001013003020002013201311010100314000100010001111010002011302102021000201263001015201030020100001020102330302112030000113101101002010200020042012220012123200312110032001001020103000002010001011100010000310100101230200100140010000101111110310021000201010402201000301100261100203120010111210021140000530662");
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
    std::wcout << std::endl << std::endl;

    bwt_type::elements_type encoded_data;
    bwt_type::elements_type decoded_data;

    bwt_type::encode(data, encoded_data);
    bwt_type::print(encoded_data);

    std::wcout << std::endl << std::endl;

    bwt_type::decode(encoded_data, decoded_data);
    bwt_type::print(decoded_data);

    std::wcout << ((data == decoded_data) ? L"ok" : L"error") << std::endl << std::endl;
}
