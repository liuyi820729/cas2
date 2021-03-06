/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2009  Fabian Schmitthenner

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

#ifndef CAS_HASH_H
#define CAS_HASH_H
#include <stdint.h>

#include "hashes.h"
#include <ostream>

namespace CAS {

class Hash
{
  private:
    uint32_t hash;
    Hash (uint32_t h) : hash (h) { }
  public:
    Hash ()
    : hash (0)
    { }
    Hash (hashes::Hashes type, uint32_t data)
    : hash (type ^ data)
    { }
    
    Hash (hashes::Hashes type)
    : hash (type)
    { }
    
    friend bool operator == (Hash, Hash);
    friend bool operator < (Hash, Hash);
    friend Hash operator ^ (Hash, Hash);
    friend Hash operator ~ (Hash);
    friend Hash operator ^ (Hash h1, int h2);
    friend std::ostream &operator << (std::ostream &, Hash);
};

inline bool operator == (Hash h1, Hash h2)
{
  return h1.hash == h2.hash;
}

inline bool operator != (Hash h1, Hash h2)
{
  return ! (h1 == h2 );
}

inline bool operator < (Hash h1, Hash h2)
{
  return h1.hash < h2.hash;
}

inline Hash operator ^ (Hash h1, Hash h2)
{
  return Hash (h1.hash ^ h2.hash);
}

inline Hash operator ^ (Hash h1, int h2)
{
  return Hash (h1.hash ^ h2);
}

inline std::ostream &operator << (std::ostream &o, Hash h)
{
  o << h.hash;
  return o;
}

inline Hash operator ~ (Hash h)
{
  return ~h.hash;
}

};

#endif // HASH_H
