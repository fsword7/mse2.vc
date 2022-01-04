// delegate.h - delegate callback packages
//
// Author:  Tim Stark
// Date:    Dec 6, 2021

#pragma once

class Device;
class DeviceInterface;

class BindedObject
{
public:
    BindedObject() = default;
    virtual ~BindedObject() = default;
};

using GenericFunc = void(*)();
class GenericClass;

template <typename Class, typename Return, typename... Args>
struct DelegateTraits
{
    using static_ptr_func  = Return (*)(Class *, Args...);
    using static_ref_func  = Return (*)(Class &, Args...);
    using member_ptr_func  = Return (Class::*)(Args...);
    using member_ptr_cfunc = Return (Class::*)(Args...) const;
};

class MemberDelegate
{
public:
    MemberDelegate() = default;

    template <typename MemberFunctionType, typename MemberFunctionClass, typename MemberReturnType, typename StaticFuncType>
    MemberDelegate(MemberFunctionType mfp, MemberFunctionClass *, MemberReturnType *, StaticFuncType)
    {
        *reinterpret_cast<MemberFunctionType *>(this) = mfp;
    }

    bool isNull() const { return (func == 0 && thisDelta == 0); }

    template <typename FunctionClass>
    void update(FunctionClass &func, GenericClass *obj)
    {
        func = reinterpret_cast<FunctionClass>(convert(obj));
    }

private:
    GenericFunc convert(GenericClass *&obj) const
    {
        obj = reinterpret_cast<GenericClass *>(reinterpret_cast<uint8_t *>(obj) + thisDelta);

        if ((func & 1) == 0)
            return reinterpret_cast<GenericFunc>(func);
        uint8_t *base = *reinterpret_cast<uint8_t **>(obj);

        return *reinterpret_cast<GenericFunc>(base + func - 1);
    }

    uintptr_t   func = 0;
    int         thisDelta = 0;
};

template <class LateBindBase>
class DelegateLateBindHelper
{
public:
    DelegateLateBindHelper() = default;

    template <class FunctionClass>
    DelegateLateBindHelper(FunctionClass *)
    : binder(&DelegateLateBindHelper::bindLate<FunctionClass>)
    {
    }

    GenericClass *operator ()(LateBindBase &object)
    {
        return binder(object);
    }

    explicit operator bool() const { return bool(binder); }

private:
    using LateBindFunc = GenericClass*(*)(LateBindBase &object);

    template <class FunctionClass>
    static GenericClass *bindLate(LateBindBase &object)
    {
        fmt::printf("Attempting to bind member function (%llX)...\n", offs_t(&object));
        fmt::printf("Type info: FunctionClass = %s LateBindBase = %s\n",
            typeid(FunctionClass).name(), typeid(&object).name());

        return reinterpret_cast<GenericClass *>(&object);
        
        // FunctionClass *result = dynamic_cast<FunctionClass *>(&object);
        // if (result != nullptr)
        //     return reinterpret_cast<GenericClass *>(result);
        // Bad cast execption
        // fmt::printf("Binding exception: expecting type %s (actual type %s)\n",
        //     typeid(FunctionClass).name(), typeid(&object).name());
        // return nullptr;
    }

    LateBindFunc binder = nullptr;
};

template <class LateBindBase, typename Return, typename... Args>
class DelegateBase
{
public:
    template <typename FunctionClass>
        using traits = DelegateTraits<FunctionClass, Return, Args...>;
    using GenericStaticFunc = typename traits<GenericClass>::static_ptr_func;
    using GenericMemberFunc = GenericStaticFunc;

    using stdFuncType = std::function<Return (Args...)>;

    DelegateBase() = default;
    DelegateBase(stdFuncType func) : function(func) {}

    template <class FunctionClass>
    DelegateBase(typename traits<FunctionClass>::member_ptr_func func, FunctionClass *obj)
    : mfp(func, obj, static_cast<Return *>(nullptr), static_cast<GenericStaticFunc>(nullptr)),
      lateBinder(obj)
    {
        bind(obj);
    }

    template <class FunctionClass>
    DelegateBase(typename traits<FunctionClass>::member_ptr_cfunc func, FunctionClass *obj)
    : mfp(func, obj, static_cast<Return *>(nullptr), static_cast<GenericStaticFunc>(nullptr)),
      lateBinder(obj)
    {
        bind(obj);
    }

    template <class FunctionClass>
    DelegateBase(typename traits<FunctionClass>::static_ref_func func, FunctionClass *obj)
    : mfp(func, obj, static_cast<Return *>(nullptr), static_cast<GenericStaticFunc>(nullptr)),
      lateBinder(obj)
    {
        bind(obj);
    }

    bool isMemberFunction() const { return !mfp.isNull(); }
    bool isNull() const           { return (function == nullptr) && (stdfunc == nullptr) && mfp.isNull(); }
    bool hasObject() const        { return object != nullptr; }

    void bindLate(BindedObject &obj)
    {
        if (lateBinder)
            bind(lateBinder(obj));
    }

    Return operator ()(Args... args) const
    {
        assert(stdfunc != nullptr || function != nullptr);
        if (stdfunc != nullptr)
            return stdfunc(std::forward<Args>(args)...);
        else // if (function != nullptr)
            return (*function)(object, std::forward<Args>(args)...);
    }

protected:
    using LateBindFunc = DelegateLateBindHelper<LateBindBase>;

    template <typename FunctionClass>
    void bind(FunctionClass *obj)
    {
        object = reinterpret_cast<GenericClass *>(obj);
        if (object != nullptr && isMemberFunction())
            mfp.update(function, object);
    }

protected:
    GenericClass       *object = nullptr;
    GenericStaticFunc   function = nullptr;
    stdFuncType         stdfunc = nullptr;
    LateBindFunc        lateBinder;
    MemberDelegate      mfp;
};

template <typename Signature, class LateBindBase = BindedObject>
class Delegate;

template <class LateBindBase, typename Return, typename... Args>
class Delegate<Return (Args...), LateBindBase> : public DelegateBase<LateBindBase, Return, Args...>
{
private:
    using base = DelegateBase<LateBindBase, Return, Args...>;

protected:
    template <class FunctionClass> using traits = typename base::template traits<FunctionClass>;
    template <class FunctionClass> using member_ptr_func = typename traits<FunctionClass>::member_ptr_func;
    template <class FunctionClass> using member_ptr_cfunc = typename traits<FunctionClass>::member_ptr_cfunc;
    template <class FunctionClass> using static_ref_func = typename traits<FunctionClass>::static_ref_func;

    Delegate() : base() {}

    template <class FunctionClass>
    Delegate(member_ptr_func<FunctionClass> func, FunctionClass *obj)
    : base(func, obj) {}

    template <class FunctionClass>
    Delegate(member_ptr_cfunc<FunctionClass> func, FunctionClass *obj)
    : base(func, obj) {}
    
    template <class FunctionClass>
    Delegate(static_ref_func<FunctionClass> func, FunctionClass *obj)
    : base(func, obj) {}


};

template <typename Signature>
class NamedDelegate : public Delegate<Signature>
{
private:
    using base = Delegate<Signature>;

protected:
    template <class FunctionClass> using member_ptr_func =
        typename base::template member_ptr_func<FunctionClass>;
    template <class FunctionClass> using member_ptr_cfunc =
        typename base::template member_ptr_cfunc<FunctionClass>;
    template <class FunctionClass> using static_ref_func =
        typename base::template static_ref_func<FunctionClass>;

public:
    NamedDelegate() = default;

    template <class FunctionClass>
    NamedDelegate(member_ptr_func<FunctionClass> func, ctag_t *name, FunctionClass *obj)
    : base(func, obj), fncName(name) {}

    template <class FunctionClass>
    NamedDelegate(member_ptr_cfunc<FunctionClass> func, ctag_t *name, FunctionClass *obj)
    : base(func, obj), fncName(name) {}

    template <class FunctionClass>
    NamedDelegate(static_ref_func<FunctionClass> func, ctag_t *name, FunctionClass *obj)
    : base(func, obj), fncName(name) {}

    inline ctag_t *getName() const { return fncName; }

    NamedDelegate &operator = (const NamedDelegate &src) = default;

private:
    ctag_t *fncName = nullptr;
};

class DeviceDelegateHelper
{
public:
    DeviceDelegateHelper(Device &owner, ctag_t *name = nullptr)
    : base(owner), devName(name)
    { }

    BindedObject &getBoundObject() const
    {
        return reinterpret_cast<BindedObject &>(base.get());
    }

    void setName(ctag_t *name) { devName = name; }

private:
    std::reference_wrapper<Device> base;
    ctag_t *devName = nullptr;
};

template <typename Signature> class DeviceDelegate;

template <typename ReturnType, typename... Args>
class DeviceDelegate<ReturnType (Args...)>
: public NamedDelegate<ReturnType (Args...)>,
  public DeviceDelegateHelper
{
private:
    using nbase = NamedDelegate<ReturnType (Args...)>;

public:
    template <typename T>
	using supportCallback = std::bool_constant<std::is_constructible_v<DeviceDelegate, Device &, ctag_t *, T, ctag_t *>>;

    explicit DeviceDelegate(Device &owner) : nbase(), DeviceDelegateHelper(owner) {}
    DeviceDelegate(const nbase &src) : nbase(src) {}

    // DeviceDelegate &operator = (DeviceDelegate &src) = default;

    template <class D>
    DeviceDelegate(Device &dev, ctag_t *devName, ReturnType (D::*func)(Args...), ctag_t *fncName)
    : nbase(func, fncName, static_cast<D *>(nullptr)), DeviceDelegateHelper(dev, devName)
    { fmt::printf("Name: %s (%llX)\n", fncName, offs_t(&dev)); }

    template <class D>
    DeviceDelegate(Device &dev, ctag_t *devName, ReturnType (D::*func)(Args...) const, ctag_t *fncName)
    : nbase(func, fncName, static_cast<D *>(nullptr)), DeviceDelegateHelper(dev, devName)
    { }

    template <class D>
    DeviceDelegate(Device &dev, ctag_t *devName, ReturnType (*func)(D &, Args...), ctag_t *fncName)
    : nbase(func, fncName, static_cast<D *>(nullptr)), DeviceDelegateHelper(dev, devName)
    { }

	template <class D> void set(ReturnType (D::*func)(Args ...), ctag_t *name)
		{ nbase::operator = (nbase(func, name, static_cast<D *>(nullptr))); setName(nullptr); }
	template <class D> void set(ReturnType (D::*func)(Args ...) const, ctag_t *name)
		{ nbase::operator = (nbase(func, name, static_cast<D *>(nullptr))); setName(nullptr); }
	template <class D> void set(ReturnType (*func)(D &, Args ...), ctag_t *name)
		{ nbase::operator = (nbase(func, name, static_cast<D *>(nullptr))); setName(nullptr); }

    void resolve()
    {
        if (!nbase::isNull() && !nbase::hasObject())
            nbase::bindLate(getBoundObject());
    }
};

// FUNC macro definiton for delegate calls
#define FUNC(func) &func, #func

using read8d_t     = DeviceDelegate<uint8_t ()>;
using read8do_t    = DeviceDelegate<uint8_t (offs_t)>;
using read8dom_t   = DeviceDelegate<uint8_t (offs_t, uint8_t)>;

using read16d_t    = DeviceDelegate<uint16_t ()>;
using read16do_t   = DeviceDelegate<uint16_t (offs_t)>;
using read16dom_t  = DeviceDelegate<uint16_t (offs_t, uint16_t)>;

using read32d_t    = DeviceDelegate<uint32_t ()>;
using read32do_t   = DeviceDelegate<uint32_t (offs_t)>;
using read32dom_t  = DeviceDelegate<uint32_t (offs_t, uint32_t)>;

using read64d_t    = DeviceDelegate<uint64_t ()>;
using read64do_t   = DeviceDelegate<uint64_t (offs_t)>;
using read64dom_t  = DeviceDelegate<uint64_t (offs_t, uint64_t)>;


using write8d_t    = DeviceDelegate<void (uint8_t)>;
using write8do_t   = DeviceDelegate<void (offs_t, uint8_t)>;
using write8dom_t  = DeviceDelegate<void (offs_t, uint8_t, uint8_t)>;

using write16d_t   = DeviceDelegate<void (uint16_t)>;
using write16do_t  = DeviceDelegate<void (offs_t, uint16_t)>;
using write16dom_t = DeviceDelegate<void (offs_t, uint16_t, uint16_t)>;

using write32d_t   = DeviceDelegate<void (uint32_t)>;
using write32do_t  = DeviceDelegate<void (offs_t, uint32_t)>;
using write32dom_t = DeviceDelegate<void (offs_t, uint32_t, uint32_t)>;

using write64d_t   = DeviceDelegate<void (uint64_t)>;
using write64do_t  = DeviceDelegate<void (offs_t, uint64_t)>;
using write64dom_t = DeviceDelegate<void (offs_t, uint64_t, uint64_t)>;

// line read/write accesses
using readl_t      = DeviceDelegate<int ()>;
using writel_t     = DeviceDelegate<void (int)>;

// ********

template <typename D, typename T, typename Enable = void> struct device_class_rw { };

template <typename D, typename T, typename Ret, typename... Args>
struct device_class_rw<D, Ret (T::*)(Args...),
    std::enable_if_t<std::is_constructible<D, Device &, ctag_t *, Ret (T::*)(Args...), ctag_t *>::value> > { using type = T; };
template <typename D, typename T, typename Ret, typename... Args>
struct device_class_rw<D, Ret (T::*)(Args...) const,
    std::enable_if_t<std::is_constructible<D, Device &, ctag_t *, Ret (T::*)(Args...) const, ctag_t *>::value> > { using type = T; };
template <typename D, typename T, typename Ret, typename... Args>
struct device_class_rw<D, Ret (*)(T &, Args...),
    std::enable_if_t<std::is_constructible<D, Device &, ctag_t *, Ret (*)(T &, Args...), ctag_t *>::value> > { using type = T; };

template <typename D, typename T> using device_class_rw_t = typename device_class_rw<D, T>::type;

template <typename T, typename Enable = void> struct delegate_rw_type;

template <typename T> struct delegate_rw_type<T, std::void_t<device_class_rw_t<read8d_t, std::remove_reference_t<T> > > >
    { using type = read8d_t; using device_class = device_class_rw_t<type, std::remove_reference_t<T> >; };
template <typename T> struct delegate_rw_type<T, std::void_t<device_class_rw_t<read8do_t, std::remove_reference_t<T> > > >
    { using type = read8do_t; using device_class = device_class_rw_t<type, std::remove_reference_t<T> >; };
template <typename T> struct delegate_rw_type<T, std::void_t<device_class_rw_t<read8dom_t, std::remove_reference_t<T> > > >
    { using type = read8dom_t; using device_class = device_class_rw_t<type, std::remove_reference_t<T> >; };

template <typename T> struct delegate_rw_type<T, std::void_t<device_class_rw_t<read16d_t, std::remove_reference_t<T> > > >
    { using type = read16d_t; using device_class = device_class_rw_t<type, std::remove_reference_t<T> >; };
template <typename T> struct delegate_rw_type<T, std::void_t<device_class_rw_t<read16do_t, std::remove_reference_t<T> > > >
    { using type = read16do_t; using device_class = device_class_rw_t<type, std::remove_reference_t<T> >; };
template <typename T> struct delegate_rw_type<T, std::void_t<device_class_rw_t<read16dom_t, std::remove_reference_t<T> > > >
    { using type = read16dom_t; using device_class = device_class_rw_t<type, std::remove_reference_t<T> >; };

template <typename T> struct delegate_rw_type<T, std::void_t<device_class_rw_t<read32d_t, std::remove_reference_t<T> > > >
    { using type = read32d_t; using device_class = device_class_rw_t<type, std::remove_reference_t<T> >; };
template <typename T> struct delegate_rw_type<T, std::void_t<device_class_rw_t<read32do_t, std::remove_reference_t<T> > > >
    { using type = read32do_t; using device_class = device_class_rw_t<type, std::remove_reference_t<T> >; };
template <typename T> struct delegate_rw_type<T, std::void_t<device_class_rw_t<read32dom_t, std::remove_reference_t<T> > > >
    { using type = read32dom_t; using device_class = device_class_rw_t<type, std::remove_reference_t<T> >; };

template <typename T> struct delegate_rw_type<T, std::void_t<device_class_rw_t<read64d_t, std::remove_reference_t<T> > > >
    { using type = read64d_t; using device_class = device_class_rw_t<type, std::remove_reference_t<T> >; };
template <typename T> struct delegate_rw_type<T, std::void_t<device_class_rw_t<read64do_t, std::remove_reference_t<T> > > >
    { using type = read64do_t; using device_class = device_class_rw_t<type, std::remove_reference_t<T> >; };
template <typename T> struct delegate_rw_type<T, std::void_t<device_class_rw_t<read64dom_t, std::remove_reference_t<T> > > >
    { using type = read64dom_t; using device_class = device_class_rw_t<type, std::remove_reference_t<T> >; };


template <typename T> struct delegate_rw_type<T, std::void_t<device_class_rw_t<write8d_t, std::remove_reference_t<T> > > >
    { using type = write8d_t; using device_class = device_class_rw_t<type, std::remove_reference_t<T> >; };
template <typename T> struct delegate_rw_type<T, std::void_t<device_class_rw_t<write8do_t, std::remove_reference_t<T> > > >
    { using type = write8do_t; using device_class = device_class_rw_t<type, std::remove_reference_t<T> >; };
template <typename T> struct delegate_rw_type<T, std::void_t<device_class_rw_t<write8dom_t, std::remove_reference_t<T> > > >
    { using type = write8dom_t; using device_class = device_class_rw_t<type, std::remove_reference_t<T> >; };

template <typename T> struct delegate_rw_type<T, std::void_t<device_class_rw_t<write16d_t, std::remove_reference_t<T> > > >
    { using type = write16d_t; using device_class = device_class_rw_t<type, std::remove_reference_t<T> >; };
template <typename T> struct delegate_rw_type<T, std::void_t<device_class_rw_t<write16do_t, std::remove_reference_t<T> > > >
    { using type = write16do_t; using device_class = device_class_rw_t<type, std::remove_reference_t<T> >; };
template <typename T> struct delegate_rw_type<T, std::void_t<device_class_rw_t<write16dom_t, std::remove_reference_t<T> > > >
    { using type = write16dom_t; using device_class = device_class_rw_t<type, std::remove_reference_t<T> >; };

template <typename T> struct delegate_rw_type<T, std::void_t<device_class_rw_t<write32d_t, std::remove_reference_t<T> > > >
    { using type = write32d_t; using device_class = device_class_rw_t<type, std::remove_reference_t<T> >; };
template <typename T> struct delegate_rw_type<T, std::void_t<device_class_rw_t<write32do_t, std::remove_reference_t<T> > > >
    { using type = write32do_t; using device_class = device_class_rw_t<type, std::remove_reference_t<T> >; };
template <typename T> struct delegate_rw_type<T, std::void_t<device_class_rw_t<write32dom_t, std::remove_reference_t<T> > > >
    { using type = write32dom_t; using device_class = device_class_rw_t<type, std::remove_reference_t<T> >; };

template <typename T> struct delegate_rw_type<T, std::void_t<device_class_rw_t<write64d_t, std::remove_reference_t<T> > > >
    { using type = write64d_t; using device_class = device_class_rw_t<type, std::remove_reference_t<T> >; };
template <typename T> struct delegate_rw_type<T, std::void_t<device_class_rw_t<write64do_t, std::remove_reference_t<T> > > >
    { using type = write64do_t; using device_class = device_class_rw_t<type, std::remove_reference_t<T> >; };
template <typename T> struct delegate_rw_type<T, std::void_t<device_class_rw_t<write64dom_t, std::remove_reference_t<T> > > >
    { using type = write64dom_t; using device_class = device_class_rw_t<type, std::remove_reference_t<T> >; };

template <typename T> struct delegate_rw_type<T, std::void_t<device_class_rw_t<readl_t, std::remove_reference_t<T> > > >
    { using type = readl_t; using device_class = device_class_rw_t<type, std::remove_reference_t<T> >; };
template <typename T> struct delegate_rw_type<T, std::void_t<device_class_rw_t<writel_t, std::remove_reference_t<T> > > >
    { using type = writel_t; using device_class = device_class_rw_t<type, std::remove_reference_t<T> >; };


template <typename T> using delegate_rw_t = typename delegate_rw_type<T>::type;
template <typename T> using delegate_rw_device_class_t = typename delegate_rw_type<T>::device_class;


template <typename T>
inline delegate_rw_t<T> makeDelegate(Device &base, ctag_t *devName, T &&func, ctag_t *fncName)
{
    return delegate_rw_t<T>(base, devName, std::forward<T>(func), fncName);
}

template <typename T>
inline delegate_rw_t<T> makeDelegate(delegate_rw_device_class_t<T> &object, T &&func, ctag_t *fncName)
{
    return delegate_rw_t<T>(object, std::forward<T>(func), fncName);
}
