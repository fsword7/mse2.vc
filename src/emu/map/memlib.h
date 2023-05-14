// memlib.h - Memory Library package
//
// Author:  Tim Stark
// Date:    May 11, 2023

namespace map
{
    class MemoryBlock
    {

    };

    class MemoryRegion
    {

    };

    class MemoryShare
    {

    };
    class MemoryBank
    {

    };

    using BlockList  = std::vector<MemoryBlock *>;
    using RegionList = std::map<str_t, MemoryRegion *>;
    using ShareList  = std::map<str_t, MemoryShare *>;
    using BankList   = std::map<str_t, MemoryBank *>;
}