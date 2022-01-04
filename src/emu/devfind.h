// devfind.h - Device finder package
//
// Author:  Tim Stark
// Date:    12/10/2021

#pragma once

class ObjectFinder
{
public:
    virtual ~ObjectFinder() = default;

    void setObjectName(ctag_t *name);

    virtual bool find() = 0;

    inline Device &getOwningDevice() const { return base; }
    inline Device &getDevice() const { return base; }
    inline ctag_t *getObjectName() const { return objName; }

protected:
    ObjectFinder(Device &owner, ctag_t *name);

    bool validate(bool found, bool required, ctag_t *name);

    void *findMemoryRegion(uint8_t width, size_t &size, bool required) const;
    void *findMemoryShared(uint8_t width, size_t &size, bool required) const;

    Device &base;
    ctag_t *objName;
    bool isResolved = false;
};

template <class ObjectClass, bool Required>
class ObjectFinderCommon : public ObjectFinder
{
public:
    inline ObjectClass *getObject() const { return object; }

    // Operator function calls
    inline operator ObjectClass * () const { return object; }
    inline ObjectClass *operator -> () const { assert(object != nullptr); return object; }

protected:
    ObjectFinderCommon(Device &owner, ctag_t *name)
    : ObjectFinder(owner, name)
    { }

    bool validate(ctag_t *name)
        { return ObjectFinder::validate(object != nullptr, Required, name); }

    ObjectClass *object = nullptr;
};

// ********

template <class DeviceClass, bool Required>
class DeviceFinder : public ObjectFinderCommon<DeviceClass, Required>
{
public:
    DeviceFinder(Device &owner, ctag_t *name)
    : ObjectFinderCommon<DeviceClass, Required>(owner, name)
    { }

    template <typename T>
    std::enable_if_t<std::is_convertible<T *, DeviceClass *>::value, T &> operator = (T &device)
    {
        assert(!this->isResolved);
        this->object = &device;
        return device;
    }

private:
    virtual bool find() override
    {
        assert(!this->isResolved);
        this->isResolved = true;

        if (Required && this->object == nullptr)
        {
            Device *const device = this->base.findDevice(this->objName);
            this->object = dynamic_cast<DeviceClass *>(device);
            if (device != nullptr && this->object == nullptr)
                fmt::printf("%s: Device '%s' found but is of incorrect type (actual type = %s)\n",
                    this->base.getDeviceName(), device->getDeviceName(), device->getShortName());
        }

        return this->validate("device");
    }
};

template <typename DeviceClass> using RequiredDevice = DeviceFinder<DeviceClass, true>;
template <typename DeviceClass> using OptionalDevice = DeviceFinder<DeviceClass, false>;

// ********

template <typename PointerType, bool Required>
class RegionPointerFinder : public ObjectFinderCommon<PointerType, Required>
{
public:
    RegionPointerFinder(Device &owner, ctag_t *name)
    : ObjectFinderCommon<PointerType, Required>(owner, name)
    { }

    inline PointerType &operator [] (int index) { return this->object[index]; }

    inline size_t getLength() const { return size; }
    inline size_t getBytes() const { return size * sizeof(PointerType); }

private:

    virtual bool find() override
    {
        assert(!this->isResolved);
        this->object = reinterpret_cast<PointerType *>
            (this->findMemoryRegion(sizeof(PointerType), size, Required));
        
        return this->validate("region memory");
    }
    
    size_t size = 0;
};

template <typename PointerType> using OptionalRegionPointer = RegionPointerFinder<PointerType, false>;
template <typename PointerType> using RequiredRegionPointer = RegionPointerFinder<PointerType, true>;

// ********

template <typename PointerType, bool Required>
class SharedPointerFinder : public ObjectFinderCommon<PointerType, Required>
{
public:
    SharedPointerFinder(Device &owner, ctag_t *name)
    : ObjectFinderCommon<PointerType, Required>(owner, name)
    { }

    inline PointerType &operator [] (int index) { return this->object[index]; }

    inline size_t getLength() const { return size; }
    inline size_t getBytes() const { return size * sizeof(PointerType); }

private:

    virtual bool find() override
    {
        assert(!this->isResolved);
        this->object = reinterpret_cast<PointerType *>
            (this->findMemoryShared(sizeof(PointerType), size, Required));
        
        return this->validate("shared memory");
    }
    
    size_t size = 0;
};

template <typename PointerType> using OptionalSharedPointer = SharedPointerFinder<PointerType, false>;
template <typename PointerType> using RequiredSharedPointer = SharedPointerFinder<PointerType, true>;
