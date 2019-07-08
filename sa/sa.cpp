#include <core/pch.hpp>
#include <core/noncopyable.hpp>

#include <sa/sa.hpp>

USINGNAMESPACE(compression)


int main()
{
    //string_type str(L"mmiissiissiippii");
    //string_type str(L"mmiissiissiippii$");

    //string_type str(L"cabbagecabbagecabbagecabbagecabbagecabbagecabbagecabbagecabbagecabbagecabbagecabbagecabbagecabbagecabbagecabbagecabbagecabbagecabbagecabbagecabbagecabbagecabbagecabbagecabbagecabbagecabbagecabbagecabbagecabbagecabbagecabbagecabbagecabbagecabbage");
    string_type str(L"cabbage");
//    string_type str(L"caabage");
    //string_type str(L"rikki-tikki-tikka");

    //string_type str(L"baabaabac");


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

    const int count = 1;

    {
        auto start = std::chrono::high_resolution_clock::now();

        for(int i = 0; i < count; i++)
        {
            std::vector<int> naive_sa;
            sa::suffix_array<char_type, traits_type>::build(s, naive_sa, sa::suffix_array<char_type, traits_type>::algorithm::naive);
            sa::suffix_array<char_type, traits_type>::print(s, naive_sa);
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        auto seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start);

        std::wcout << std::endl << L"Naive time is " << milliseconds.count() << L"(ms), " << seconds.count() << L"(s)" << std::endl;
    }

    std::cout << std::endl;

    {
        auto start = std::chrono::high_resolution_clock::now();

        for(int i = 0; i < count; i++)
        {
            std::vector<int> sa_is;

            sa::suffix_array<char_type, traits_type>::build(s, sa_is);
            sa::suffix_array<char_type, traits_type>::print(s, sa_is);
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        auto seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start);

        std::wcout << std::endl << L"SA-IS time is " << milliseconds.count() << L"(ms), " << seconds.count() << L"(s)" << std::endl;
    }
}
