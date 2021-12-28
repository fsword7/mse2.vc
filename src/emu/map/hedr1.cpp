// hedr0.cpp - Read Handler Entry - Dispatch
//
// Author:  Tim Stark
// Date:    12/14/2021

#include "emu/core.h"
#include "emu/map/he.h"
#include "emu/map/heun.h"
#include "emu/map/maccess.h"
#include "emu/map/space.h"
#include "emu/map/hedr.h"

namespace map
{
    template class HandlerReadDispatch< 9, 0, 0>;
    template class HandlerReadDispatch<10, 0, 0>;
    template class HandlerReadDispatch<11, 0, 0>;
    template class HandlerReadDispatch<12, 0, 0>;
    template class HandlerReadDispatch<13, 0, 0>;
    template class HandlerReadDispatch<14, 0, 0>;
    template class HandlerReadDispatch<15, 0, 0>;
    template class HandlerReadDispatch<16, 0, 0>;

    template class HandlerReadDispatch< 9, 1, 0>;
    template class HandlerReadDispatch<10, 1, 0>;
    template class HandlerReadDispatch<11, 1, 0>;
    template class HandlerReadDispatch<12, 1, 0>;
    template class HandlerReadDispatch<13, 1, 0>;
    template class HandlerReadDispatch<14, 1, 0>;
    template class HandlerReadDispatch<15, 1, 0>;
    template class HandlerReadDispatch<16, 1, 0>;

    template class HandlerReadDispatch< 9, 1, 1>;
    template class HandlerReadDispatch<10, 1, 1>;
    template class HandlerReadDispatch<11, 1, 1>;
    template class HandlerReadDispatch<12, 1, 1>;
    template class HandlerReadDispatch<13, 1, 1>;
    template class HandlerReadDispatch<14, 1, 1>;
    template class HandlerReadDispatch<15, 1, 1>;
    template class HandlerReadDispatch<16, 1, 1>;

    template class HandlerReadDispatch< 9, 2, 0>;
    template class HandlerReadDispatch<10, 2, 0>;
    template class HandlerReadDispatch<11, 2, 0>;
    template class HandlerReadDispatch<12, 2, 0>;
    template class HandlerReadDispatch<13, 2, 0>;
    template class HandlerReadDispatch<14, 2, 0>;
    template class HandlerReadDispatch<15, 2, 0>;
    template class HandlerReadDispatch<16, 2, 0>;

    template class HandlerReadDispatch< 9, 2, 1>;
    template class HandlerReadDispatch<10, 2, 1>;
    template class HandlerReadDispatch<11, 2, 1>;
    template class HandlerReadDispatch<12, 2, 1>;
    template class HandlerReadDispatch<13, 2, 1>;
    template class HandlerReadDispatch<14, 2, 1>;
    template class HandlerReadDispatch<15, 2, 1>;
    template class HandlerReadDispatch<16, 2, 1>;

    template class HandlerReadDispatch< 9, 2, 2>;
    template class HandlerReadDispatch<10, 2, 2>;
    template class HandlerReadDispatch<11, 2, 2>;
    template class HandlerReadDispatch<12, 2, 2>;
    template class HandlerReadDispatch<13, 2, 2>;
    template class HandlerReadDispatch<14, 2, 2>;
    template class HandlerReadDispatch<15, 2, 2>;
    template class HandlerReadDispatch<16, 2, 2>;

    template class HandlerReadDispatch< 9, 3, 0>;
    template class HandlerReadDispatch<10, 3, 0>;
    template class HandlerReadDispatch<11, 3, 0>;
    template class HandlerReadDispatch<12, 3, 0>;
    template class HandlerReadDispatch<13, 3, 0>;
    template class HandlerReadDispatch<14, 3, 0>;
    template class HandlerReadDispatch<15, 3, 0>;
    template class HandlerReadDispatch<16, 3, 0>;

    template class HandlerReadDispatch< 9, 3, 1>;
    template class HandlerReadDispatch<10, 3, 1>;
    template class HandlerReadDispatch<11, 3, 1>;
    template class HandlerReadDispatch<12, 3, 1>;
    template class HandlerReadDispatch<13, 3, 1>;
    template class HandlerReadDispatch<14, 3, 1>;
    template class HandlerReadDispatch<15, 3, 1>;
    template class HandlerReadDispatch<16, 3, 1>;

    template class HandlerReadDispatch< 9, 3, 2>;
    template class HandlerReadDispatch<10, 3, 2>;
    template class HandlerReadDispatch<11, 3, 2>;
    template class HandlerReadDispatch<12, 3, 2>;
    template class HandlerReadDispatch<13, 3, 2>;
    template class HandlerReadDispatch<14, 3, 2>;
    template class HandlerReadDispatch<15, 3, 2>;
    template class HandlerReadDispatch<16, 3, 2>;

    template class HandlerReadDispatch< 9, 3, 3>;
    template class HandlerReadDispatch<10, 3, 3>;
    template class HandlerReadDispatch<11, 3, 3>;
    template class HandlerReadDispatch<12, 3, 3>;
    template class HandlerReadDispatch<13, 3, 3>;
    template class HandlerReadDispatch<14, 3, 3>;
    template class HandlerReadDispatch<15, 3, 3>;
    template class HandlerReadDispatch<16, 3, 3>;
}