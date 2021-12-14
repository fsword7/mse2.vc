// hedw0.cpp - Write Handler Entry - Dispatch
//
// Author:  Tim Stark
// Date:    12/14/2021

#include "emu/core.h"
#include "emu/map/he.h"
#include "emu/map/heun.h"
#include "emu/map/space.h"
#include "emu/map/hedw.h"

namespace map
{
    template class HandlerWriteDispatch<17, 0, 0>;
    template class HandlerWriteDispatch<18, 0, 0>;
    template class HandlerWriteDispatch<19, 0, 0>;
    template class HandlerWriteDispatch<20, 0, 0>;
    template class HandlerWriteDispatch<21, 0, 0>;
    template class HandlerWriteDispatch<22, 0, 0>;
    template class HandlerWriteDispatch<23, 0, 0>;
    template class HandlerWriteDispatch<24, 0, 0>;

    template class HandlerWriteDispatch<17, 1, 0>;
    template class HandlerWriteDispatch<18, 1, 0>;
    template class HandlerWriteDispatch<19, 1, 0>;
    template class HandlerWriteDispatch<20, 1, 0>;
    template class HandlerWriteDispatch<21, 1, 0>;
    template class HandlerWriteDispatch<22, 1, 0>;
    template class HandlerWriteDispatch<23, 1, 0>;
    template class HandlerWriteDispatch<24, 1, 0>;

    template class HandlerWriteDispatch<17, 1, 1>;
    template class HandlerWriteDispatch<18, 1, 1>;
    template class HandlerWriteDispatch<19, 1, 1>;
    template class HandlerWriteDispatch<20, 1, 1>;
    template class HandlerWriteDispatch<21, 1, 1>;
    template class HandlerWriteDispatch<22, 1, 1>;
    template class HandlerWriteDispatch<23, 1, 1>;
    template class HandlerWriteDispatch<24, 1, 1>;

    template class HandlerWriteDispatch<17, 2, 0>;
    template class HandlerWriteDispatch<18, 2, 0>;
    template class HandlerWriteDispatch<19, 2, 0>;
    template class HandlerWriteDispatch<20, 2, 0>;
    template class HandlerWriteDispatch<21, 2, 0>;
    template class HandlerWriteDispatch<22, 2, 0>;
    template class HandlerWriteDispatch<23, 2, 0>;
    template class HandlerWriteDispatch<24, 2, 0>;

    template class HandlerWriteDispatch<17, 2, 1>;
    template class HandlerWriteDispatch<18, 2, 1>;
    template class HandlerWriteDispatch<19, 2, 1>;
    template class HandlerWriteDispatch<20, 2, 1>;
    template class HandlerWriteDispatch<21, 2, 1>;
    template class HandlerWriteDispatch<22, 2, 1>;
    template class HandlerWriteDispatch<23, 2, 1>;
    template class HandlerWriteDispatch<24, 2, 1>;

    template class HandlerWriteDispatch<17, 2, 2>;
    template class HandlerWriteDispatch<18, 2, 2>;
    template class HandlerWriteDispatch<19, 2, 2>;
    template class HandlerWriteDispatch<20, 2, 2>;
    template class HandlerWriteDispatch<21, 2, 2>;
    template class HandlerWriteDispatch<22, 2, 2>;
    template class HandlerWriteDispatch<23, 2, 2>;
    template class HandlerWriteDispatch<24, 2, 2>;

    template class HandlerWriteDispatch<17, 3, 0>;
    template class HandlerWriteDispatch<18, 3, 0>;
    template class HandlerWriteDispatch<19, 3, 0>;
    template class HandlerWriteDispatch<20, 3, 0>;
    template class HandlerWriteDispatch<21, 3, 0>;
    template class HandlerWriteDispatch<22, 3, 0>;
    template class HandlerWriteDispatch<23, 3, 0>;
    template class HandlerWriteDispatch<24, 3, 0>;

    template class HandlerWriteDispatch<17, 3, 1>;
    template class HandlerWriteDispatch<18, 3, 1>;
    template class HandlerWriteDispatch<19, 3, 1>;
    template class HandlerWriteDispatch<20, 3, 1>;
    template class HandlerWriteDispatch<21, 3, 1>;
    template class HandlerWriteDispatch<22, 3, 1>;
    template class HandlerWriteDispatch<23, 3, 1>;
    template class HandlerWriteDispatch<24, 3, 1>;

    template class HandlerWriteDispatch<17, 3, 2>;
    template class HandlerWriteDispatch<18, 3, 2>;
    template class HandlerWriteDispatch<19, 3, 2>;
    template class HandlerWriteDispatch<20, 3, 2>;
    template class HandlerWriteDispatch<21, 3, 2>;
    template class HandlerWriteDispatch<22, 3, 2>;
    template class HandlerWriteDispatch<23, 3, 2>;
    template class HandlerWriteDispatch<24, 3, 2>;

    template class HandlerWriteDispatch<17, 3, 3>;
    template class HandlerWriteDispatch<18, 3, 3>;
    template class HandlerWriteDispatch<19, 3, 3>;
    template class HandlerWriteDispatch<20, 3, 3>;
    template class HandlerWriteDispatch<21, 3, 3>;
    template class HandlerWriteDispatch<22, 3, 3>;
    template class HandlerWriteDispatch<23, 3, 3>;
    template class HandlerWriteDispatch<24, 3, 3>;
}