//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __COMPRESSION_BURROWS_WHEELER_H__
#define __COMPRESSION_BURROWS_WHEELER_H__

#pragma once

BEGIN_NAMESPACE(compression)

template <typename ElementType, typename Traits>
class bwt : private noncopyable
{
    // virtual sentinel is always = 0
    public:
        using traits_type = Traits;

        using element_type = ElementType;
        using elements_type = std::vector<element_type>;

        using index_type = int;
        using indices_type = std::vector<index_type>;

        using size_type = int;

    public:
        static bool encode(const elements_type& raw_data, elements_type& encoded_data);
        static bool decode(const elements_type& encoded_data, elements_type& decoded_data);

        static void print(const elements_type& data, bool spaced = true);
};

template <typename ElementType, typename Traits>
inline bool bwt<ElementType, Traits>::encode(const typename bwt<ElementType, Traits>::elements_type& raw_data,
                                             typename bwt<ElementType, Traits>::elements_type& encoded_data)
{
    bool result = true;

    size_type n = static_cast<size_type>(raw_data.size()); // includes +1 for virtual sentinel

    encoded_data.resize(n);

    indices_type suffixes;

    algorithms::suffix_array<element_type, traits_type>::build(raw_data, suffixes);

    size_type m = static_cast<size_type>(suffixes.size());

    for(index_type i = 0; i < m; i++)
    {
        index_type suffix = suffixes[i];

        if(suffix > 0)
        {
            encoded_data[i] = raw_data[suffix - 1];
        }
        else
        {
            encoded_data[i] = 0; // 0 acts as $
        }
    }

    return result;
}

template <typename ElementType, typename Traits>
inline bool bwt<ElementType, Traits>::decode(const typename bwt<ElementType, Traits>::elements_type& encoded_data,
                                             typename bwt<ElementType, Traits>::elements_type& decoded_data)
{
    // https://www.student.cs.uwaterloo.ca/~cs240/f18/modules/module10.pdf
    //
    // BWT-decoding(C[0..n − 1])
    //  C: string of characters over alphabet Σc
    //  A <- array of size n
    //  for i = 0 to n − 1
    //      A[i] <- (C[i], i)
    //  Stably sort A by first entry
    //  S <- empty string
    //  for j = 0 to n
    //      if C[j] = $ break
    //  repeat
    //      j <- second entry of A[j]
    //      append C[j] to S
    //  until C[j] = $
    //  return S
    bool result = true;

    //  A <- array of size n
    std::vector<std::pair<element_type, index_type>> sorted_data;

    //  for i = 0 to n − 1
    //      A[i] <- (C[i], i)
    index_type k = 0;
    std::for_each(encoded_data.begin(),
                  encoded_data.end(),
                  [&sorted_data, &k](const element_type& element)
                  {
                      sorted_data.emplace_back(std::make_pair(element, k++));
                  });

    //  Stably (MUST BE STABLE) sort A by first entry
    std::stable_sort(sorted_data.begin(),
                     sorted_data.end(),
                     [](const auto& entry1, const auto& entry2)
                     {
                         return std::get<0>(entry1) < std::get<0>(entry2);
                     });

    //  S <- empty string
    size_type n = static_cast<size_type>(encoded_data.size()); // includes +1 for virtual sentinel

    decoded_data.resize(n);

    //  for j = 0 to n
    //      if C[j] = $ break
    index_type j = 0;

    for(; j < n; j++)
    {
        if(encoded_data[j] == element_type(0))
        {
            break;
        }
    }

    //  repeat
    //      j <- second entry of A[j]
    //      append C[j] to S
    //  until C[j] = $
    for(index_type i = 0; i < n; i++) // guarding against missing $
    {
        j = std::get<1>(sorted_data[j]);

        decoded_data[i] = encoded_data[j];

        if(encoded_data[j] == element_type(0))
        {
            break;
        }
    }

    return result;
}

template <typename ElementType, typename Traits>
void bwt<ElementType, Traits>::print(const typename bwt<ElementType, Traits>::elements_type& data, bool spaced)
{
    for(auto element : data)
    {
        std::wcout << char_type(element) << (spaced ? L" " : L"");
    }

    std::wcout << std::endl;
}

END_NAMESPACE

#endif // __COMPRESSION_BURROWS_WHEELER_H__
