// devcb.h - device callback delegate package
//
// Author:  Tim Stark
// Date:    12/24/21

#pragma once

class Device;

namespace emu::devices
{
    class DeviceCallbackBase
    {
    public:
        DeviceCallbackBase(Device &owner) : base(owner) {}
        virtual ~DeviceCallbackBase() = default;

        template <typename T, typename U, typename Enable = void> struct intermediate;
        template <typename T, typename U> struct intermediate<T, U, std::enable_if_t<sizeof(T) >= sizeof(U)>> { using type = T; };
        template <typename T, typename U> struct intermediate<T, U, std::enable_if_t<sizeof(T) < sizeof(U)>> { using type = U; };
        template <typename T, typename U> using intermediate_t = typename intermediate<T, U>::type;
        template <typename T, typename U> using mask_t = std::make_unsigned_t<intermediate<T, U>>;

        template <typename Input, typename Result, typename Func, typename Enable = void>
            struct is_transform_form1 : public std::false_type { };
        template <typename Input, typename Result, typename Func, typename Enable = void>
            struct is_transform_form2 : public std::false_type { };
        template <typename Input, typename Result, typename Func, typename Enable = void>
            struct is_transform_form3 : public std::false_type { };

        template <typename Input, typename Result, typename Func>
            struct is_transform_form1<Input, Result, Func, std::enable_if_t<std::is_convertible<std::invoke_result_t<Func, offs_t &, Input, std::make_unsigned_t<Input> &>, Result>::value> > : public std::true_type { };
        template <typename Input, typename Result, typename Func>
            struct is_transform_form2<Input, Result, Func, std::enable_if_t<std::is_convertible<std::invoke_result_t<Func, offs_t &, Input>, Result>::value> > : public std::true_type { };
        template <typename Input, typename Result, typename Func>
            struct is_transform_form3<Input, Result, Func, std::enable_if_t<std::is_convertible<std::invoke_result_t<Func, Input>, Result>::value> > : public std::true_type { };

        template <typename Input, typename Result, typename Func>
            struct is_transform : public std::bool_constant<
                is_transform_form1<Input, Result, Func>::value ||
                is_transform_form2<Input, Result, Func>::value ||
                is_transform_form3<Input, Result, Func>::value> { };

        template <typename Input, typename Result, typename Func, typename Enable = void>
            struct transform_result;
        template <typename Input, typename Result, typename Func>
            struct transform_result<Input, Result, Func, std::enable_if_t<is_transform_form1<Input, Result, Func>::value> >
                { using type = std::invoke_result_t<Func, offs_t &, Input, std::make_unsigned_t<Input> &>; };
        template <typename Input, typename Result, typename Func>
            struct transform_result<Input, Result, Func, std::enable_if_t<is_transform_form2<Input, Result, Func>::value> >
                { using type = std::invoke_result_t<Func, offs_t &, Input>; };
        template <typename Input, typename Result, typename Func>
            struct transform_result<Input, Result, Func, std::enable_if_t<is_transform_form3<Input, Result, Func>::value> >
                { using type = std::invoke_result_t<Func, Input>; };
        template <typename Input, typename Result, typename Func>
            using transform_result_t = typename transform_result<Input, Result, Func>::type;
        
        template <typename T> using delegate_type_t = delegate_rw_t<T>;
        template <typename T> using delegate_device_class_t = delegate_rw_device_class_t<T>;

        inline Device &getOwner() { return base; }

    private:
        Device &base;
    };

    // ********

    class DeviceCallbackReadBase : public DeviceCallbackBase
    {
    protected:
        using DeviceCallbackBase::DeviceCallbackBase;
        ~DeviceCallbackReadBase() = default;

        template <typename Result, typename Func, typename Enable = void>
            struct is_read_form1 : std::false_type {};
        template <typename Result, typename Func, typename Enable = void>
            struct is_read_form2 : std::false_type {};
        template <typename Result, typename Func, typename Enable = void>
            struct is_read_form3 : std::false_type {};

        template <typename Result, typename Func>
            struct is_read_form1<Result, Func, std::enable_if_t<std::is_convertible<std::invoke_result_t<Func, offs_t, Result>, Result>::value> > : public std::true_type { };
        template <typename Result, typename Func>
            struct is_read_form2<Result, Func, std::enable_if_t<std::is_convertible<std::invoke_result_t<Func, offs_t>, Result>::value> > : public std::true_type { };
        template <typename Result, typename Func>
            struct is_read_form3<Result, Func, std::enable_if_t<std::is_convertible<std::invoke_result_t<Func>, Result>::value> > : public std::true_type { };

        template <typename Result, typename Func>
            struct is_read : public std::bool_constant<
                is_read_form1<Result, Func>::value ||
                is_read_form2<Result, Func>::value ||
                is_read_form3<Result, Func>::value> { };

        template <typename Result, typename Func, typename Enable = void> struct read_result;
        template <typename Result, typename Func> struct read_result<Result, Func, std::enable_if_t<is_read_form1<Result, Func>::value>>
            { using type = std::invoke_result_t<Func, offs_t, std::make_unsigned<Result>>; };
        template <typename Result, typename Func> struct read_result<Result, Func, std::enable_if_t<is_read_form2<Result, Func>::value>>
            { using type = std::invoke_result_t<Func, offs_t>; };
        template <typename Result, typename Func> struct read_result<Result, Func, std::enable_if_t<is_read_form3<Result, Func>::value>>
            { using type = std::invoke_result_t<Func>; };
        template <typename Result, typename Func> using read_result_t = typename read_result<Result, Func>::type;

        template <typename Result, typename T> static std::enable_if_t<is_read_form1<Result, T>::value, mask_t<read_result_t<Result, T>, Result>>
            invokeRead(T const &cb, offs_t offset, std::make_unsigned_t<Result> memMask)
            {
                return std::make_unsigned_t<read_result_t<Result, T>>(cb(offset, memMask));
            }

        template <typename Result, typename T> static std::enable_if_t<is_read_form2<Result, T>::value, mask_t<read_result_t<Result, T>, Result>>
            invokeRead(T const &cb, offs_t offset, std::make_unsigned_t<Result> memMask)
            {
                return std::make_unsigned_t<read_result_t<Result, T>>(cb(offset));
            }

        template <typename Result, typename T> static std::enable_if_t<is_read_form3<Result, T>::value, mask_t<read_result_t<Result, T>, Result>>
            invokeRead(T const &cb, offs_t offset, std::make_unsigned_t<Result> memMask)
            {
                return std::make_unsigned_t<read_result_t<Result, T>>(cb());
            }


        template <typename T, typename Enable = void>
            struct is_read_method : public std::false_type { };

        template <typename T> struct is_read_method<T, std::void_t<device_class_rw_t<read8d_t, std::remove_reference_t<T> > > >
            : public std::true_type { };
        template <typename T> struct is_read_method<T, std::void_t<device_class_rw_t<read8do_t, std::remove_reference_t<T> > > >
            : public std::true_type { };
        template <typename T> struct is_read_method<T, std::void_t<device_class_rw_t<read8dom_t, std::remove_reference_t<T> > > >
            : public std::true_type { };

        template <typename T> struct is_read_method<T, std::void_t<device_class_rw_t<read16d_t, std::remove_reference_t<T> > > >
            : public std::true_type { };
        template <typename T> struct is_read_method<T, std::void_t<device_class_rw_t<read16do_t, std::remove_reference_t<T> > > >
            : public std::true_type { };
        template <typename T> struct is_read_method<T, std::void_t<device_class_rw_t<read16dom_t, std::remove_reference_t<T> > > >
            : public std::true_type { };

        template <typename T> struct is_read_method<T, std::void_t<device_class_rw_t<read32d_t, std::remove_reference_t<T> > > >
            : public std::true_type { };
        template <typename T> struct is_read_method<T, std::void_t<device_class_rw_t<read32do_t, std::remove_reference_t<T> > > >
            : public std::true_type { };
        template <typename T> struct is_read_method<T, std::void_t<device_class_rw_t<read32dom_t, std::remove_reference_t<T> > > >
            : public std::true_type { };

        template <typename T> struct is_read_method<T, std::void_t<device_class_rw_t<read64d_t, std::remove_reference_t<T> > > >
            : public std::true_type { };
        template <typename T> struct is_read_method<T, std::void_t<device_class_rw_t<read64do_t, std::remove_reference_t<T> > > >
            : public std::true_type { };
        template <typename T> struct is_read_method<T, std::void_t<device_class_rw_t<read64dom_t, std::remove_reference_t<T> > > >
            : public std::true_type { };

        template <typename T, typename Dummy = void> struct delegate_traits;

        template <typename Dummy> struct delegate_traits<read8d_t, Dummy>
            { static constexpr uint8_t default_mask = ~uint8_t(0); };
        template <typename Dummy> struct delegate_traits<read8do_t, Dummy>
            { static constexpr uint8_t default_mask = ~uint8_t(0); };
        template <typename Dummy> struct delegate_traits<read8dom_t, Dummy>
            { static constexpr uint8_t default_mask = ~uint8_t(0); };

        template <typename Dummy> struct delegate_traits<read16d_t, Dummy>
            { static constexpr uint16_t default_mask = ~uint16_t(0); };
        template <typename Dummy> struct delegate_traits<read16do_t, Dummy>
            { static constexpr uint16_t default_mask = ~uint16_t(0); };
        template <typename Dummy> struct delegate_traits<read16dom_t, Dummy>
            { static constexpr uint16_t default_mask = ~uint16_t(0); };

        template <typename Dummy> struct delegate_traits<read32d_t, Dummy>
            { static constexpr uint32_t default_mask = ~uint32_t(0); };
        template <typename Dummy> struct delegate_traits<read32do_t, Dummy>
            { static constexpr uint32_t default_mask = ~uint32_t(0); };
        template <typename Dummy> struct delegate_traits<read32dom_t, Dummy>
            { static constexpr uint32_t default_mask = ~uint32_t(0); };

        template <typename Dummy> struct delegate_traits<read64d_t, Dummy>
            { static constexpr uint64_t default_mask = ~uint64_t(0); };
        template <typename Dummy> struct delegate_traits<read64do_t, Dummy>
            { static constexpr uint64_t default_mask = ~uint64_t(0); };
        template <typename Dummy> struct delegate_traits<read64dom_t, Dummy>
            { static constexpr uint64_t default_mask = ~uint64_t(0); };

    };

    template <typename Result, std::make_unsigned_t<Result> defaultMask =
        std::make_unsigned_t<Result>(~std::make_unsigned_t<Result>(0)) >
    class readcb_t : public DeviceCallbackReadBase
    {
    private:
        using func_t = std::function<Result (offs_t, std::make_unsigned_t<Result>) >;

    };

    // ********

    class DeviceCallbackWriteBase : public DeviceCallbackBase
    {
    protected:
        using DeviceCallbackBase::DeviceCallbackBase;
        ~DeviceCallbackWriteBase() = default;

        template <typename Input, typename Func, typename Enable = void>
            struct is_write_form1 : std::false_type {};
        template <typename Input, typename Func, typename Enable = void>
            struct is_write_form2 : std::false_type {};
        template <typename Input, typename Func, typename Enable = void>
            struct is_write_form3 : std::false_type {};

        template <typename Input, typename Func>
            struct is_write_form1<Input, Func, std::void_t<std::invoke_result_t<Func, offs_t, Input, std::make_unsigned_t<Input>> > > : public std::true_type { };
        template <typename Input, typename Func>
            struct is_write_form2<Input, Func, std::void_t<std::invoke_result_t<Func, offs_t, Input> > > : public std::true_type { };
        template <typename Input, typename Func>
            struct is_write_form3<Input, Func, std::void_t<std::invoke_result_t<Func> > > : public std::true_type { };

        template <typename Input, typename Func>
            struct is_write : public std::bool_constant<
                is_write_form1<Input, Func>::value ||
                is_write_form2<Input, Func>::value ||
                is_write_form3<Input, Func>::value> { };

        // template <typename Input, typename Func, typename Enable = void> struct write_result;
        // template <typename Input, typename Func> struct write_result<Input, Func, std::enable_if_t<is_write_form1<Input, Func>::value>>
        //     { using type = std::invoke_result_t<Func, offs_t, std::make_unsigned<Result>>; };
        // template <typename Input, typename Func> struct write_result<Input, Func, std::enable_if_t<is_write_form2<Input, Func>::value>>
        //     { using type = std::invoke_result_t<Func, offs_t>; };
        // template <typename Input, typename Func> struct write_result<Input, Func, std::enable_if_t<is_write_form3<Input, Func>::value>>
        //     { using type = std::invoke_result_t<Func>; };
        // template <typename Input, typename Func> using write_result_t = typename write_result<Input, Func>::type;

        template <typename Input, typename T> static std::enable_if_t<is_write_form1<Input, T>::value>
            invokeWrite(T const &cb, offs_t &offset, Input data, std::make_unsigned_t<Input> memMask)
            {
                return cb(offset, data, memMask);
            }

        template <typename Input, typename T> static std::enable_if_t<is_write_form2<Input, T>::value>
            invokeWrite(T const &cb, offs_t &offset, Input data, std::make_unsigned_t<Input> memMask)
            {
                return cb(offset, data);
            }

        template <typename Input, typename T> static std::enable_if_t<is_write_form3<Input, T>::value>
            invokeWrite(T const &cb, offs_t &offset, Input data, std::make_unsigned_t<Input> memMask)
            {
                return cb(data);
            }


        template <typename T, typename Enable = void>
            struct is_write_method : public std::false_type { };

        template <typename T> struct is_write_method<T, std::void_t<device_class_rw_t<write8d_t, std::remove_reference_t<T> > > >
            : public std::true_type { };
        template <typename T> struct is_write_method<T, std::void_t<device_class_rw_t<write8do_t, std::remove_reference_t<T> > > >
            : public std::true_type { };
        template <typename T> struct is_write_method<T, std::void_t<device_class_rw_t<write8dom_t, std::remove_reference_t<T> > > >
            : public std::true_type { };

        template <typename T> struct is_write_method<T, std::void_t<device_class_rw_t<write16d_t, std::remove_reference_t<T> > > >
            : public std::true_type { };
        template <typename T> struct is_write_method<T, std::void_t<device_class_rw_t<write16do_t, std::remove_reference_t<T> > > >
            : public std::true_type { };
        template <typename T> struct is_write_method<T, std::void_t<device_class_rw_t<write16dom_t, std::remove_reference_t<T> > > >
            : public std::true_type { };

        template <typename T> struct is_write_method<T, std::void_t<device_class_rw_t<write32d_t, std::remove_reference_t<T> > > >
            : public std::true_type { };
        template <typename T> struct is_write_method<T, std::void_t<device_class_rw_t<write32do_t, std::remove_reference_t<T> > > >
            : public std::true_type { };
        template <typename T> struct is_write_method<T, std::void_t<device_class_rw_t<write32dom_t, std::remove_reference_t<T> > > >
            : public std::true_type { };

        template <typename T> struct is_write_method<T, std::void_t<device_class_rw_t<write64d_t, std::remove_reference_t<T> > > >
            : public std::true_type { };
        template <typename T> struct is_write_method<T, std::void_t<device_class_rw_t<write64do_t, std::remove_reference_t<T> > > >
            : public std::true_type { };
        template <typename T> struct is_write_method<T, std::void_t<device_class_rw_t<write64dom_t, std::remove_reference_t<T> > > >
            : public std::true_type { };

        template <typename T, typename Dummy = void> struct delegate_traits;

        template <typename Dummy> struct delegate_traits<write8d_t, Dummy>
            { static constexpr uint8_t default_mask = ~uint8_t(0); };
        template <typename Dummy> struct delegate_traits<write8do_t, Dummy>
            { static constexpr uint8_t default_mask = ~uint8_t(0); };
        template <typename Dummy> struct delegate_traits<write8dom_t, Dummy>
            { static constexpr uint8_t default_mask = ~uint8_t(0); };

        template <typename Dummy> struct delegate_traits<write16d_t, Dummy>
            { static constexpr uint16_t default_mask = ~uint16_t(0); };
        template <typename Dummy> struct delegate_traits<write16do_t, Dummy>
            { static constexpr uint16_t default_mask = ~uint16_t(0); };
        template <typename Dummy> struct delegate_traits<write16dom_t, Dummy>
            { static constexpr uint16_t default_mask = ~uint16_t(0); };

        template <typename Dummy> struct delegate_traits<write32d_t, Dummy>
            { static constexpr uint32_t default_mask = ~uint32_t(0); };
        template <typename Dummy> struct delegate_traits<write32do_t, Dummy>
            { static constexpr uint32_t default_mask = ~uint32_t(0); };
        template <typename Dummy> struct delegate_traits<write32dom_t, Dummy>
            { static constexpr uint32_t default_mask = ~uint32_t(0); };

        template <typename Dummy> struct delegate_traits<write64d_t, Dummy>
            { static constexpr uint64_t default_mask = ~uint64_t(0); };
        template <typename Dummy> struct delegate_traits<write64do_t, Dummy>
            { static constexpr uint64_t default_mask = ~uint64_t(0); };
        template <typename Dummy> struct delegate_traits<write64dom_t, Dummy>
            { static constexpr uint64_t default_mask = ~uint64_t(0); };

    };

    template <typename Input, std::make_unsigned_t<Input> defaultMask =
        std::make_unsigned_t<Input>(~std::make_unsigned_t<Input>(0)) >
    class writecb_t : public DeviceCallbackWriteBase
    {
    private:
        using func_t = std::function<void (offs_t, Input, std::make_unsigned_t<Input>) >;
    };
   
}