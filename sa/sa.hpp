//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __COMPRESSION_SUFFIX_ARRAY_H__
#define __COMPRESSION_SUFFIX_ARRAY_H__

#pragma once

BEGIN_NAMESPACE(compression::sa)
USINGNAMESPACE(compression)

template <typename ElementType>
class sa : private noncopyable
{
    using element_type = ElementType;
    using elements_type = std::vector<element_type>;
};

END_NAMESPACE

#endif // __COMPRESSION_SUFFIX_ARRAY_H__
