//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
// based on mathematicalmonk's videos
// https://www.youtube.com/playlist?list=PLE125425EC837021F
//
#ifndef __COMPRESSION_ARITHMETIC_MODEL_H__
#define __COMPRESSION_ARITHMETIC_MODEL_H__

#pragma once

BEGIN_NAMESPACE(compression::arithmetic)
USINGNAMESPACE(compression)

template <typename ElementType, typename IntegerType>
interface model : private noncopyable
{
    public:
        using integer_type = unsigned long long;
        using element_type = ElementType;
};

END_NAMESPACE

#endif // __COMPRESSION_ARITHMETIC_MODEL_H__
