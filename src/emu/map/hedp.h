// hedp.h - Header Entry (Delegate) package
//
// Author:  Tim Stark
// Date:    Jun 11, 2023

#pragma once

namespace map
{
    template <int dWidth, int aShift, typename Read>
    class HandlerReadDelegate : public HandlerReadAddress<dWidth, aShift>
    {
    public:
        using uintx_t = typename HandlerSize<dWidth>::uintx_t;
 
        HandlerReadDelegate(AddressSpace *space, uint32_t flags, const Read &delegate)
        : HandlerReadAddress<dWidth, aShift>(space, flags), delegate(delegate)
        { 
        }

        ~HandlerReadDelegate() = default;

        cstr_t getsName() const { return delegate.getName(); }

        uintx_t read(offs_t offset, uintx_t mask, CPUDevice *cpu) const override
        {
            return readi<Read>(offset, mask);
        }

    private:
        template <typename D>
        std::enable_if_t<
            std::is_same<D, read8d_t>::value ||
            std::is_same<D, read16d_t>::value ||
            std::is_same<D, read32d_t>::value ||
            std::is_same<D, read64d_t>::value,
            uintx_t> readi(offs_t offset, uintx_t mask) const
        {
            return delegate();
        }

        template <typename D>
        std::enable_if_t<
            std::is_same<D, read8do_t>::value ||
            std::is_same<D, read16do_t>::value ||
            std::is_same<D, read32do_t>::value ||
            std::is_same<D, read64do_t>::value,
            uintx_t> readi(offs_t offset, uintx_t mask) const
        {
            return delegate((offset - this->baseAddress) & this->maskAddress);
        }

        template <typename D>
        std::enable_if_t<
            std::is_same<D, read8dom_t>::value ||
            std::is_same<D, read16dom_t>::value ||
            std::is_same<D, read32dom_t>::value ||
            std::is_same<D, read64dom_t>::value,
            uintx_t> readi(offs_t offset, uintx_t mask) const
        {
            return delegate((offset - this->baseAddress) & this->maskAddress, mask);
        }

        Read delegate;
    };

    template <int dWidth, int aShift, typename Write>
    class HandlerWriteDelegate : public HandlerWriteAddress<dWidth, aShift>
    {
    public:
        using uintx_t = typename HandlerSize<dWidth>::uintx_t;
 
        HandlerWriteDelegate(AddressSpace *space, uint32_t flags, const Write &delegate)
        : HandlerWriteAddress<dWidth, aShift>(space, flags), delegate(delegate)
        {
        }

        ~HandlerWriteDelegate() = default;

        cstr_t getsName() const { return delegate.getName(); }

        void write(offs_t offset, uintx_t data, uintx_t mask, CPUDevice *cpu) const override
        {
            writei<Write>(offset, data, mask);
        }

    private:
        template <typename D>
        std::enable_if_t<
            std::is_same<D, write8d_t>::value ||
            std::is_same<D, write16d_t>::value ||
            std::is_same<D, write32d_t>::value ||
            std::is_same<D, write64d_t>::value,
            void> writei(offs_t offset, uintx_t data, uintx_t mask) const
        {
            delegate(data);
        }

        template <typename D>
        std::enable_if_t<
            std::is_same<D, write8do_t>::value ||
            std::is_same<D, write16do_t>::value ||
            std::is_same<D, write32do_t>::value ||
            std::is_same<D, write64do_t>::value,
            void> writei(offs_t offset, uintx_t data, uintx_t mask) const
        {
            delegate((offset - this->baseAddress) & this->maskAddress, data);
        }

        template <typename D>
        std::enable_if_t<
            std::is_same<D, write8dom_t>::value ||
            std::is_same<D, write16dom_t>::value ||
            std::is_same<D, write32dom_t>::value ||
            std::is_same<D, write64dom_t>::value,
            void> writei(offs_t offset, uintx_t data, uintx_t mask) const
        {
            delegate((offset - this->baseAddress) & this->maskAddress, data, mask);
        }
   
        Write delegate;
    }; 
}