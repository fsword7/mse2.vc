// fileio.h - I/O file management
//
// Author:  Tim Stark
// Date:    1/2/2022

#include "emu/core.h"
#include "emu/fileio.h"

namespace emu
{
    ioFile::ioFile(uint32_t flags)
    {

    }

    bool ioFile::open(fs::path fname)
    {
        file.open(fname, std::ios::in|std::ios::binary);

        return file.is_open();
    }

    void ioFile::close()
    {
        file.close();
    }

    int ioFile::read(uint8_t *data, int length)
    {
        file.read((char *)data, length);
        return file.gcount();
    }

    int ioFile::write(uint8_t *data, int length)
    {
        file.write((char *)data, length);
        return file.gcount();
    }
}