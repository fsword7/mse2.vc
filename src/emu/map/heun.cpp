// heun.cpp - Handler Entry - Unmapped/NOP (No Operation)
//
// Author:  Tim Stark
// Date:    6/5/2023

#include "emu/core.h"
#include "emu/map/map.h"

namespace map
{
    template class HandlerReadUnmapped<0, 0>;
    template class HandlerReadUnmapped<1, 0>;
    template class HandlerReadUnmapped<1, 1>;
    template class HandlerReadUnmapped<2, 0>;
    template class HandlerReadUnmapped<2, 1>;
    template class HandlerReadUnmapped<2, 2>;
    template class HandlerReadUnmapped<3, 0>;
    template class HandlerReadUnmapped<3, 1>;
    template class HandlerReadUnmapped<3, 2>;
    template class HandlerReadUnmapped<3, 3>;

    template class HandlerWriteUnmapped<0, 0>;
    template class HandlerWriteUnmapped<1, 0>;
    template class HandlerWriteUnmapped<1, 1>;
    template class HandlerWriteUnmapped<2, 0>;
    template class HandlerWriteUnmapped<2, 1>;
    template class HandlerWriteUnmapped<2, 2>;
    template class HandlerWriteUnmapped<3, 0>;
    template class HandlerWriteUnmapped<3, 1>;
    template class HandlerWriteUnmapped<3, 2>;
    template class HandlerWriteUnmapped<3, 3>;


    template class HandlerReadNop<0, 0>;
    template class HandlerReadNop<1, 0>;
    template class HandlerReadNop<1, 1>;
    template class HandlerReadNop<2, 0>;
    template class HandlerReadNop<2, 1>;
    template class HandlerReadNop<2, 2>;
    template class HandlerReadNop<3, 0>;
    template class HandlerReadNop<3, 1>;
    template class HandlerReadNop<3, 2>;
    template class HandlerReadNop<3, 3>;

    template class HandlerWriteNop<0, 0>;
    template class HandlerWriteNop<1, 0>;
    template class HandlerWriteNop<1, 1>;
    template class HandlerWriteNop<2, 0>;
    template class HandlerWriteNop<2, 1>;
    template class HandlerWriteNop<2, 2>;
    template class HandlerWriteNop<3, 0>;
    template class HandlerWriteNop<3, 1>;
    template class HandlerWriteNop<3, 2>;
    template class HandlerWriteNop<3, 3>;
}