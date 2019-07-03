#include <core/pch.hpp>
#include <core/noncopyable.hpp>

#include <sa/sa.hpp>
//
USINGNAMESPACE(compression)


int main()
{
    //string_type str(L"mmiissiissiippii");
    //string_type str(L"mmiissiissiippii$");

    string_type str(L"cabbage");
    //string_type str(L"caabage");
    //string_type str(L"rikki-tikki-tikka");

    std::vector<char_type> s(str.begin(), str.end());
    s.push_back(0); // add virtual sentinel

    struct sa_traits
    {
        static int abc_size()
        {
            return 255;
        }
    };

    using traits_type = sa_traits;

    std::vector<int> naive_sa;
    sa::suffix_array<char_type, traits_type>::build(s, naive_sa, sa::suffix_array<char_type, traits_type>::algorithm::naive);

    std::vector<int> sa_is;
    bool rc = sa::suffix_array<char_type, traits_type>::build(s, sa_is);

    std::wcout << (rc ? L"ok" : L"error") << std::endl << std::endl;
}
