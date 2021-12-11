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

    ObjectClass *object = nullptr;
};

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
        return false;
    }
};

template <typename DeviceClass> using RequiredDevice = DeviceFinder<DeviceClass, true>;
template <typename DeviceClass> using OptionalDevice = DeviceFinder<DeviceClass, false>;