#ifndef SOURCE_LOCATION_HPP_INCLUDED
#define SOURCE_LOCATION_HPP_INCLUDED

#include <cstddef>

struct SourceLocation
{
    std::size_t line, column;
};

#endif
