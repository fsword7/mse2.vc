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
            struct is_transform_form1<Input, Result, Func, std::enable_if_t<std::is_convertible<std::invoke_result_t<Func, offs_t &, Input>, Result>::value> > : public std::true_type { };
        template <typename Input, typename Result, typename Func>
            struct is_transform_form1<Input, Result, Func, std::enable_if_t<std::is_convertible<std::invoke_result_t<Func, Input>, Result>::value> > : public std::true_type { };

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

}