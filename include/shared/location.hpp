#ifndef LOCATION_HPP_INCLUDED
#define LOCATION_HPP_INCLUDED

#include <cstddef>

typedef struct
{
    std::size_t line, column;
} Location;

#endif