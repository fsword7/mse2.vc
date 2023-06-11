// hedp.cpp - Header Entry (Delegate) package
//
// Author:  Tim Stark
// Date:    Jun 11, 2023

#include "emu/core.h"
#include "emu/map/he.h"
#include "emu/map/hea.h"
#include "emu/map/hedp.h"

namespace map
{
    template class HandlerReadDelegate<0, 0, read8d_t>;
    template class HandlerReadDelegate<1, 0, read16d_t>;
    template class HandlerReadDelegate<1, 1, read16d_t>;
    template class HandlerReadDelegate<2, 0, read32d_t>;
    template class HandlerReadDelegate<2, 1, read32d_t>;
    template class HandlerReadDelegate<2, 2, read32d_t>;
    template class HandlerReadDelegate<3, 0, read64d_t>;
    template class HandlerReadDelegate<3, 1, read64d_t>;
    template class HandlerReadDelegate<3, 2, read64d_t>;
    template class HandlerReadDelegate<3, 3, read64d_t>;

    template class HandlerReadDelegate<0, 0, read8do_t>;
    template class HandlerReadDelegate<1, 0, read16do_t>;
    template class HandlerReadDelegate<1, 1, read16do_t>;
    template class HandlerReadDelegate<2, 0, read32do_t>;
    template class HandlerReadDelegate<2, 1, read32do_t>;
    template class HandlerReadDelegate<2, 2, read32do_t>;
    template class HandlerReadDelegate<3, 0, read64do_t>;
    template class HandlerReadDelegate<3, 1, read64do_t>;
    template class HandlerReadDelegate<3, 2, read64do_t>;
    template class HandlerReadDelegate<3, 3, read64do_t>;

    template class HandlerReadDelegate<0, 0, read8dom_t>;
    template class HandlerReadDelegate<1, 0, read16dom_t>;
    template class HandlerReadDelegate<1, 1, read16dom_t>;
    template class HandlerReadDelegate<2, 0, read32dom_t>;
    template class HandlerReadDelegate<2, 1, read32dom_t>;
    template class HandlerReadDelegate<2, 2, read32dom_t>;
    template class HandlerReadDelegate<3, 0, read64dom_t>;
    template class HandlerReadDelegate<3, 1, read64dom_t>;
    template class HandlerReadDelegate<3, 2, read64dom_t>;
    template class HandlerReadDelegate<3, 3, read64dom_t>;


    template class HandlerWriteDelegate<0, 0, write8d_t>;
    template class HandlerWriteDelegate<1, 0, write16d_t>;
    template class HandlerWriteDelegate<1, 1, write16d_t>;
    template class HandlerWriteDelegate<2, 0, write32d_t>;
    template class HandlerWriteDelegate<2, 1, write32d_t>;
    template class HandlerWriteDelegate<2, 2, write32d_t>;
    template class HandlerWriteDelegate<3, 0, write64d_t>;
    template class HandlerWriteDelegate<3, 1, write64d_t>;
    template class HandlerWriteDelegate<3, 2, write64d_t>;
    template class HandlerWriteDelegate<3, 3, write64d_t>;

    template class HandlerWriteDelegate<0, 0, write8do_t>;
    template class HandlerWriteDelegate<1, 0, write16do_t>;
    template class HandlerWriteDelegate<1, 1, write16do_t>;
    template class HandlerWriteDelegate<2, 0, write32do_t>;
    template class HandlerWriteDelegate<2, 1, write32do_t>;
    template class HandlerWriteDelegate<2, 2, write32do_t>;
    template class HandlerWriteDelegate<3, 0, write64do_t>;
    template class HandlerWriteDelegate<3, 1, write64do_t>;
    template class HandlerWriteDelegate<3, 2, write64do_t>;
    template class HandlerWriteDelegate<3, 3, write64do_t>;

    template class HandlerWriteDelegate<0, 0, write8dom_t>;
    template class HandlerWriteDelegate<1, 0, write16dom_t>;
    template class HandlerWriteDelegate<1, 1, write16dom_t>;
    template class HandlerWriteDelegate<2, 0, write32dom_t>;
    template class HandlerWriteDelegate<2, 1, write32dom_t>;
    template class HandlerWriteDelegate<2, 2, write32dom_t>;
    template class HandlerWriteDelegate<3, 0, write64dom_t>;
    template class HandlerWriteDelegate<3, 1, write64dom_t>;
    template class HandlerWriteDelegate<3, 2, write64dom_t>;
    template class HandlerWriteDelegate<3, 3, write64dom_t>;
}