//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
// Suffix array construction by induced sorting (SA-IS)
// based on:
//  'Linear Suffix Array Construction by Almost Pure Induced-Sorting' Nong, G., Zhang, S. and Chan, W. Data Compression Conference, 2009
//
// ... and on awesome explanation https://zork.net/~st/jottings/sais.html (thanks!)
//
#ifndef __COMPRESSION_SUFFIX_ARRAY_H__
#define __COMPRESSION_SUFFIX_ARRAY_H__

#pragma once

BEGIN_NAMESPACE(compression::sa)
USINGNAMESPACE(compression)

template <typename ElementType, typename Traits>
class suffix_array : private noncopyable
{
    // virtual sentinel is always = 0
    public:
        using traits_type = Traits;

        using element_type = ElementType;
        using elements_type = std::vector<element_type>;

        using index_type = int;
        using indices_type = std::vector<index_type>;

        using size_type = int;

        using types_type = std::vector<bool>; // t

    public:
        enum class algorithm
        {
            naive = 1,
            sa_is = 2
        };

    private:
        struct bucket
        {
            size_type  size;

            index_type head;
            index_type tail;

            element_type element;

            bucket() : size(0), head(0), tail(0)
            {
            }
        };

        using bucket_type = bucket;
        using buckets_type = std::vector<bucket_type>;

    private:
        static bool is_lms(const types_type& type_map, index_type index);
        static bool are_lms_strings_equal(const elements_type& string, const types_type& type_map, index_type index_a, index_type index_b);

        static void build_type_map(const elements_type& strings, types_type& type_map);
        static void build_buckets(const elements_type& type_map, size_type abc_size, buckets_type& buckets);

        static void arrange_lms(const elements_type& string, const types_type& type_map, indices_type buckets_tails, indices_type& suffixes);

        static void induce_sort_ltype(const elements_type& string, const types_type& type_map, indices_type buckets_heads, indices_type& suffixes);
        static void induce_sort_stype(const elements_type& string, const types_type& type_map, indices_type buckets_tails, indices_type& suffixes);

        static void simplify_suffix_array(const elements_type& string,
                                          const types_type& type_map,
                                          const indices_type& suffixes,
                                          elements_type& new_string,
                                          indices_type& new_suffix_offsets,
                                          size_type& new_abc_size);

        static void build_suffix_array(const elements_type& new_string, size_type new_abc_size, indices_type& new_suffixes);
        static void build_suffix_array(const elements_type& string,
                                       indices_type buckets_tails,
                                       const indices_type& new_suffixes,
                                       const indices_type& new_suffix_offsets,
                                       indices_type& suffix_offsets);

    private:
        static bool build_naive(const elements_type& string, indices_type& suffixes);
        static bool build_sa_is(const elements_type& string, size_type abc_size, indices_type& suffixes);

    public:
        static bool build(const elements_type& string, indices_type& suffixes, algorithm = algorithm::sa_is);
        static void print(const elements_type& string, const indices_type& suffixes);
};

template <typename ElementType, typename Traits>
inline bool suffix_array<ElementType, Traits>::build(const typename suffix_array<ElementType, Traits>::elements_type& string,
                                                     typename suffix_array<ElementType, Traits>::indices_type& suffixes,
                                                     typename suffix_array<ElementType, Traits>::algorithm algorithm)
{
    bool result = true;

    if(algorithm == algorithm::naive)
    {
        result = build_naive(string, suffixes);
    }
    else if(algorithm == algorithm::sa_is)
    {
        result = build_sa_is(string, traits_type::abc_size(), suffixes);
    }

    return result;
}

template <typename ElementType, typename Traits>
bool suffix_array<ElementType, Traits>::build_naive(const typename suffix_array<ElementType, Traits>::elements_type& string, // S is the input string
                                                    typename suffix_array<ElementType, Traits>::indices_type& suffixes)      // SA is the output suffix array of S
{
    bool result = true;

    std::vector<elements_type> string_suffixes;

    size_type n = static_cast<size_type>(string.size()); // includes +1 for virtual sentinel

    for(index_type k = 0; k < n; k++)
    {
        elements_type suffix(string.begin() + k, string.end());
        string_suffixes.emplace_back(suffix);
    }

    std::sort(string_suffixes.begin(),
              string_suffixes.end(),
              [](const elements_type& suffix1, const elements_type& suffix2)
              {
                  return suffix1 < suffix2;
              });

    for(const auto& suffix : string_suffixes)
    {
        suffixes.emplace_back(static_cast<index_type>(string.size()) - static_cast<index_type>(suffix.size()));
    }

    return result;
}

template <typename ElementType, typename Traits>
void suffix_array<ElementType, Traits>::print(const typename suffix_array<ElementType, Traits>::elements_type& string,
                                              const typename suffix_array<ElementType, Traits>::indices_type& suffixes)
{
    for(auto element : string)
    {
        std::wcout << element << L' ';
    }

    std::wcout << std::endl;

    for(auto offset : suffixes)
    {
        std::wcout << std::setw(4) << offset << L":  ";

        for(index_type i = offset; i < static_cast<index_type>(string.size()); i++)
        {
            std::wcout << string[i] << L' ';
        }

        std::wcout << std::endl;
    }
}

template <typename ElementType, typename Traits>
bool suffix_array<ElementType, Traits>::build_sa_is(const typename suffix_array<ElementType, Traits>::elements_type& string, // S is the input string, must have virtual sentinel (0)
                                                    typename suffix_array<ElementType, Traits>::size_type abc_size,
                                                    typename suffix_array<ElementType, Traits>::indices_type& suffixes)      // SA is the output suffix array of S
{
    // input string s must be in this format:
    //  c ... c e
    //  chars   sentinel = 0
    // virtual sentinel is an empty suffix

    bool result = true;

    size_type n = static_cast<size_type>(string.size()); // includes +1 for virtual sentinel

    // build type map (populate t array)
    std::vector<bool> type_map;

    build_type_map(string, type_map);

    // build buckets and its heads/tails
    buckets_type buckets;

    build_buckets(string, abc_size, buckets);

    indices_type buckets_heads;

    for(const auto& bucket : buckets)
    {
        buckets_heads.emplace_back(bucket.head);
    }

    indices_type buckets_tails;

    for(const auto& bucket : buckets)
    {
        buckets_tails.emplace_back(bucket.tail);
    }

    // rearrange LMS suffixes, approximate positions
    indices_type lms_sufixes;

    arrange_lms(string, type_map, buckets_tails, lms_sufixes);

    // rearrange all other (non-LMS) suffixes, might move sa indices around
    induce_sort_ltype(string, type_map, buckets_heads, lms_sufixes);
    induce_sort_stype(string, type_map, buckets_tails, lms_sufixes);

    // simplify to S1 ...
    elements_type new_string;

    indices_type new_suffix_offsets;

    size_type new_abc_size;

    simplify_suffix_array(string, type_map, lms_sufixes, new_string, new_suffix_offsets, new_abc_size);

    // build suffix array
    indices_type new_suffixes;

    build_suffix_array(new_string, new_abc_size, new_suffixes);

    indices_type suffix_offsets;

    build_suffix_array(string, buckets_tails, new_suffixes, new_suffix_offsets, suffix_offsets);

    // rearrange all other (non-LMS) suffixes, might move sa indices around
    induce_sort_ltype(string, type_map, buckets_heads, suffix_offsets);
    induce_sort_stype(string, type_map, buckets_tails, suffix_offsets);

    // result
    suffixes.swap(suffix_offsets);

    return result;
}

template <typename ElementType, typename Traits>
inline bool suffix_array<ElementType, Traits>::is_lms(const typename suffix_array<ElementType, Traits>::types_type& type_map, index_type index)
{
    // true if the char at index = i is a left-most S-type (LMS)
    // if i == 0 -> true
    // from the paper:
    // 00 Index: 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16
    // 01 S:      m  m  i  i  s  s  i  i  s  s  i  i  p  p  i  i  $
    // 02 t:      L  L  S  S  L  L  S  S  L  L  S  S  L  L  L  L  S
    // 03 LMS:          *           *           *                 *
    //                                                            |
    //                                                      always true
    return index > 0 && type_map[index] && !type_map[index - 1];
}

template <typename ElementType, typename Traits>
inline bool suffix_array<ElementType, Traits>::are_lms_strings_equal(const typename suffix_array<ElementType, Traits>::elements_type& string,
                                                                     const typename suffix_array<ElementType, Traits>::types_type& type_map,
                                                                     index_type index_a, // first string start offset
                                                                     index_type index_b) // second string start offset
{
    // controversion:
    // An “LMS substring” is a portion of the input string starting at one LMS character and continuing up to (but not including) the next LMS character.
    //
    // Definition 2.2. (LMS-substring) A LMS-substring is (i) a substring S[i..j] with both S[i] and S[j] being LMS characters,
    // and there is no other LMS character in the substring, for i != j; or (ii) the sentinel itself.

    bool result = true;

    size_type n = static_cast<size_type>(string.size()); // includes +1 for virtual sentinel

    if(index_a == n - 1 || index_b == n - 1)
    {
        // no other substring is equal to the empty suffix
        result = false;
    }

    if(result)
    {
        index_type i = 0; // iterations

        for(;;)
        {
            bool is_a_lms = is_lms(type_map, i + index_a);
            bool is_b_lms = is_lms(type_map, i + index_b);

            // found start of the next LMS strings ...
            if(i > 0 && // at least one element progressed
               is_a_lms && is_b_lms)
            {
                result = true;
                break;
            }

            if(is_a_lms != is_b_lms) // mismatch
            {
                result = false;
                break;
            }

            if(string[i + index_a] != string[i + index_b]) // compare content, element by element
            {
                result = false;
                break;
            }

            i++; // advance to the next element
        }
    }

    return result;
}

template <typename ElementType, typename Traits>
inline void suffix_array<ElementType, Traits>::build_type_map(const typename suffix_array<ElementType, Traits>::elements_type& string,
                                                              typename suffix_array<ElementType, Traits>::types_type& type_map)
{
    // builds S/L-type map
    //  t: array [0 ... n] of booleans to represent the S-type and L-type vector,
    //  original paper has [0 ... n - 1] but in our case we have n + 1 elements, + 1 for virtual sentinel

    size_type n = static_cast<size_type>(string.size()); // includes +1 for virtual sentinel

    type_map.resize(n);

    type_map[n - 1] = true; // 1) The last suffix suf(S, n −1) (after the last character) consisting of only the single character $ or 0 (the sentinel) is defined as S-type.
                            // 2) The suffix containing only the last character (the last before virtual sentinel) must necessarily be larger than the empty suffix.

    if(n > 1) // real data, handles also 2)
    {
        for(index_type i = n - 1; i > 0; i--)
        {
            // properties:
            //  (i)  S[i] is S-type if (i.1)  S[i] < S[i + 1] or (i.2)  S[i] = S[i + 1] and suf(S, i + 1) is S-type
            //  (ii) S[i] is L-type if (ii.1) S[i] > S[i + 1] or (ii.2) S[i] = S[i + 1] and suf(S, i + 1) is L-type
            type_map[i - 1] = string[i - 1] == string[i] ? type_map[i] // ... or (i.2) or (ii.2) ...
                                                         : element_type(string[i - 1]) < element_type(string[i]); // ... if (i.1) if (ii.1) ...
        }
    }
}

template <typename ElementType, typename Traits>
void suffix_array<ElementType, Traits>::build_buckets(const typename suffix_array<ElementType, Traits>::elements_type& string,
                                                      typename suffix_array<ElementType, Traits>::size_type abc_size,
                                                      typename suffix_array<ElementType, Traits>::buckets_type& buckets)
{
    buckets.resize(abc_size);

    // collect stats
    for(const auto& element : string)
    {
        buckets[element].size++;
        buckets[element].element = element;
    }

    // calculate heads
    index_type offset = 0; // The empty suffix always winds up at the beginning of the suffix array in index 0, so the a bucket begins at index 1.

    for(auto& bucket : buckets)
    {
        if(bucket.size > 0)
        {
            bucket.head = offset;
            offset += bucket.size;
        }
    }

    // calculate tails
    offset = 0;

    for(auto& bucket : buckets)
    {
        if(bucket.size > 0)
        {
            offset += bucket.size;
            bucket.tail = offset - 1;
        }
    }
}

template <typename ElementType, typename Traits>
void suffix_array<ElementType, Traits>::arrange_lms(const typename suffix_array<ElementType, Traits>::elements_type& string,
                                                    const typename suffix_array<ElementType, Traits>::types_type& type_map,
                                                    typename suffix_array<ElementType, Traits>::indices_type buckets_tails, // not reference
                                                    typename suffix_array<ElementType, Traits>::indices_type& suffixes)
{
    // put all LMS suffixes into their appropriate bucket (at the end)
    //  05 Bucket: $    i                         m       p       s
    //  06 SA:     {16} {-1 -1 -1 -1 -1 10 06 02} {-1 -1} {-1 -1} {-1 -1 -1 -1}
    //                                  |  |  | first insertion
    //                                  |  | second insertion
    //                                  | third insertion

    size_type n = static_cast<size_type>(string.size()); // includes +1 for virtual sentinel

    suffixes.resize(n);

    std::fill(suffixes.begin(), suffixes.end(), -1);

    indices_type tails;

    tails.swap(buckets_tails);

    for(index_type i = 0; i < n - 1; i++) // -1 for excluding virtual sentinel
    {
        if(!is_lms(type_map, i))
        {
            continue; // not start of LMX suffix
        }

        index_type k = string[i]; // bucket index

        suffixes[tails[k]] = i;

        tails[k]--; // move tail index backward to insert the next element in front of the last inserted element
    }

    suffixes[0] = n - 1; // set the single sentinel LMS-substring, -1 for excluding virtual sentinel
}

template <typename ElementType, typename Traits>
void suffix_array<ElementType, Traits>::induce_sort_ltype(const typename suffix_array<ElementType, Traits>::elements_type& string,
                                                          const typename suffix_array<ElementType, Traits>::types_type& type_map,
                                                          typename suffix_array<ElementType, Traits>::indices_type buckets_heads, // not reference
                                                          typename suffix_array<ElementType, Traits>::indices_type& suffixes)
{
    // scanning through the suffix array from left-to-right arranging l-type suffixes ...

    indices_type heads;

    heads.swap(buckets_heads);

    size_type m = static_cast<size_type>(suffixes.size());

    for(index_type i = 0; i < m; i++)
    {
        if(suffixes[i] == -1)
        {
            continue; // skip non initialized entries
        }

        // get the index of the suffix that begins to the left of the suffix this entry points to
        index_type j = suffixes[i] - 1; // -1 is left of the current position

        if(j < 0)
        {
            continue;
        }

        if(type_map[j])
        {
            continue; // considering only l-type suffixes
        }

        index_type k = string[j]; // bucket index

        suffixes[heads[k]] = j;

        heads[k]++; // move head index forward where to insert the next element
    }
}

template <typename ElementType, typename Traits>
void suffix_array<ElementType, Traits>::induce_sort_stype(const typename suffix_array<ElementType, Traits>::elements_type& string,
                                                          const typename suffix_array<ElementType, Traits>::types_type& type_map,
                                                          typename suffix_array<ElementType, Traits>::indices_type buckets_tails, // not reference
                                                          typename suffix_array<ElementType, Traits>::indices_type& suffixes)
{
    // scanning through the suffix array from right-to-left arranging s-type suffixes ...

    indices_type tails;

    tails.swap(buckets_tails);

    for(index_type i = static_cast<size_type>(suffixes.size()) - 1; i >= 0; i--)
    {
        index_type j = suffixes[i] - 1;

        if(j < 0)
        {
            continue;
        }

        if(!type_map[j])
        {
            continue; // considering only s-type suffixes
        }

        index_type k = string[j]; // bucket index

        suffixes[tails[k]] = j;

        tails[k]--; // move head index backward where to insert the next element
    }
}

template <typename ElementType, typename Traits>
void suffix_array<ElementType, Traits>::simplify_suffix_array(const typename suffix_array<ElementType, Traits>::elements_type& string,
                                                              const typename suffix_array<ElementType, Traits>::types_type& type_map,
                                                              const typename suffix_array<ElementType, Traits>::indices_type& suffx_array,
                                                              typename suffix_array<ElementType, Traits>::elements_type& new_string,
                                                              typename suffix_array<ElementType, Traits>::indices_type& new_suffix_offsets,
                                                              typename suffix_array<ElementType, Traits>::size_type& new_abc_size)
{
    // ... Name each LMS-substring in S by its bucket index to get a new shortened string S1
    // ... Get lexicographical names of all (sorted) LMS-substrings and create S1
    
    size_type n = static_cast<size_type>(string.size()); // includes +1 for virtual sentinel

    // collect
    indices_type names(n); // new LMS names

    std::fill(names.begin(), names.end(), -1);

    index_type current_name = 0;

    names[suffx_array[0]] = current_name; // empty suffix

    index_type last_lms_suffix_offset = suffx_array[0];

    size_type m = static_cast<size_type>(suffx_array.size());

    for(index_type i = 1; i < m; i++) // from 1 as we already process the empty suffix at 0
    {
        index_type lms_suffix_offset = suffx_array[i];

        if(!is_lms(type_map, lms_suffix_offset))
        {
            continue; // not start of LMX suffix
        }

        // If this LMS suffix starts with a different LMS substring from the last suffix we looked at....
        if(!are_lms_strings_equal(string, type_map, last_lms_suffix_offset, lms_suffix_offset))
        {
            // ...then it gets a new name
            current_name++;
        }

        last_lms_suffix_offset = lms_suffix_offset; // remember

        names[lms_suffix_offset] = current_name; // store this LMX suffix in names
    }

    // cleanup
    for(index_type i = 0, n = static_cast<size_type>(names.size()); i < n; i++)
    {
        if(names[i] == -1)
        {
            continue;
        }

        new_string.emplace_back(element_type(names[i]));
        new_suffix_offsets.emplace_back(i);
    }

    new_abc_size = current_name + 1; // +1 because names starts from 0
}

template <typename ElementType, typename Traits>
void suffix_array<ElementType, Traits>::build_suffix_array(const typename suffix_array<ElementType, Traits>::elements_type& new_string,
                                                           typename suffix_array<ElementType, Traits>::size_type new_abc_size,
                                                           typename suffix_array<ElementType, Traits>::indices_type& new_suffixes)
{
    size_type n = static_cast<size_type>(new_string.size());

    if(new_abc_size == n)
    {
        new_suffixes.resize(n + 1);

        std::fill(new_suffixes.begin(), new_suffixes.end(), -1);

        new_suffixes[0] = n;

        for(index_type i = 0, n = static_cast<size_type>(new_string.size()); i < n; i++)
        {
            new_suffixes[new_string[i] + 1] = i;
        }
    }
    else
    {
        build_sa_is(new_string, new_abc_size, new_suffixes);
    }
}

template <typename ElementType, typename Traits>
void suffix_array<ElementType, Traits>::build_suffix_array(const typename suffix_array<ElementType, Traits>::elements_type& string,
                                                           typename suffix_array<ElementType, Traits>::indices_type buckets_tails, // not reference
                                                           const typename suffix_array<ElementType, Traits>::indices_type& new_suffixes,
                                                           const typename suffix_array<ElementType, Traits>::indices_type& new_suffix_offsets,
                                                           typename suffix_array<ElementType, Traits>::indices_type& suffix_offsets)
{
    size_type n = static_cast<size_type>(string.size()); // includes +1 for virtual sentinel

    suffix_offsets.resize(n);

    std::fill(suffix_offsets.begin(), suffix_offsets.end(), -1);

    indices_type tails;

    tails.swap(buckets_tails);

    // adding to the tail ...
    for(index_type i = static_cast<size_type>(new_suffixes.size()) - 1; i > 0; i--) // > 0
    {
        index_type string_index = new_suffix_offsets[new_suffixes[i]];
        index_type bucket_index = string[string_index];

        suffix_offsets[tails[bucket_index]] = string_index;

        tails[bucket_index]--; // move head index backward where to insert the next element
    }

    suffix_offsets[0] = n - 1; // set the single sentinel LMS-substring, -1 for excluding virtual sentinel
}

END_NAMESPACE

#endif // __COMPRESSION_SUFFIX_ARRAY_H__
