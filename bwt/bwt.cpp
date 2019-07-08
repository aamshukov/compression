#include <core/pch.hpp>
#include <core/noncopyable.hpp>

#include <sa/sa.hpp>
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

    std::string str("mmiissiissiippii");
    //std::string str("DRDOBBS");

    using bwt_type = bwt::bwt<byte, sa_traits>;

    bwt_type::elements_type data(str.begin(), str.end());
    data.push_back(0); // add virtual sentinel

    bwt_type::elements_type encoded_data;
    bwt_type::elements_type decoded_data;

    bwt_type::encode(data, encoded_data);
    bwt_type::print(encoded_data);

    bwt_type::decode(encoded_data, decoded_data);
    bwt_type::print(decoded_data);

    std::wcout << (data == decoded_data) << std::endl;
}
