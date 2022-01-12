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

        template <typename T, typename Impl>
        class TransformBase
        {
        public:
            Impl &setXOR(std::make_unsigned_t<T> val)
            { 
                xorValue ^= val;
                return static_cast<Impl &>(*this); 
            }
            
            Impl &setMask(std::make_unsigned_t<T> val)
            { 
                maskValue = flagMask ? val : (maskValue & val); 
                flagMask = false; 
                return static_cast<Impl &>(*this);
            }

            Impl &setInvert(std::make_unsigned_t<T> val)
            { 
                return setXOR(~std::make_unsigned_t<T>(0));
            }
        
            constexpr std::make_unsigned_t<T> getMask() const { return maskValue; }
            constexpr std::make_unsigned_t<T> getXOR() const { return xorValue & maskValue; }

        protected:
            TransformBase(std::make_unsigned_t<T> mask) : maskValue(mask) { }

        private:
            std::make_unsigned_t<T> maskValue;
            std::make_unsigned_t<T> xorValue = std::make_unsigned_t<T>(0);
            bool flagMask = true;
        };

        template <typename T, unsigned Count>
        class array : public std::array<T, Count>
        {
        private:
            template <unsigned... V>
            array(Device &owner, std::integer_sequence<unsigned, V...> const &)
            : std::array<T, Count>{{ { make_one<V>(owner) }... }}
            { }

            template <unsigned N> Device &make_one(Device &owner) { return owner; }

        public:
            using std::array<T, Count>::array;

            array(Device &owner) : array(owner, std::make_integer_sequence<unsigned, Count>()) { }

            void resolveAll()
            {
                for (T &elem : *this)
                    elem.resolve();
            }
        };

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

        class Creator
        {
        public:
            using ptr = std::unique_ptr<Creator>;
            
            virtual ~Creator() = default;

            virtual func_t create() = 0;

            std::make_unsigned_t<Result> getMask() const { return mask; }

        protected:
            Creator(std::make_unsigned_t<Result> mask) : mask(mask) { }

            std::make_unsigned_t<Result> mask;
        };

        template <typename T>
        class CreatorBase : public Creator
        {
        public:
            CreatorBase(T && builder) : Creator(builder.getMask()), builder(std::move(builder)) { }

            virtual func_t create() override
            {
                func_t newFunc = nullptr;
                builder.build(
                    [&newFunc](auto &&f)
                    {
                        newFunc = [cb = std::move(f)](offs_t offset, typename T::input_mask_t memMask)
                        {
                            return cb(offset, memMask);
                        };
                    }
                );
                return newFunc;
            }

        private:
            T builder;
        };

        class BuilderBase
        {
        protected:
            BuilderBase(readcb_t &cb, bool append)
            : target(cb), append(append)
            { }
            BuilderBase(BuilderBase &&) = default;
            BuilderBase(BuilderBase const &) = delete;
            ~BuilderBase() { assert(consumed); }

            void consume() { consumed = true; }

            template <typename T>
            void registerCreator()
            {
                if (consumed == false)
                {
                    if (append == false)
                        target.creators.clear();
                    consumed = true;
                    target.creators.emplace_back(std::make_unique<CreatorBase<T>>(std::move(static_cast<T &>(*this))));
                }
            }

            readcb_t &target;
            bool append;
            bool consumed = false;
        };

        template <typename Delegate>
        class DelegateBuilder
        : public BuilderBase,
          public TransformBase<mask_t<read_result_t<Result, Delegate>, Result>, DelegateBuilder<Delegate>>
        {
        public:
            using output_t = mask_t<read_result_t<Result, Delegate>, Result>;
            using input_mask_t = std::make_unsigned_t<Result>;

            template <typename T>
            DelegateBuilder(readcb_t &cb, bool append, Device &device, ctag_t *devName, T &&func, ctag_t *fncName)
            : BuilderBase(cb, append),
              TransformBase<output_t, DelegateBuilder<Delegate>>(defaultMask &delegate_traits<Delegate>::default_mask),
              delegate(device, devName, std::forward<T>(func), fncName)
            { }

            DelegateBuilder(DelegateBuilder &&that)
            : BuilderBase(std::move(that)),
              TransformBase<output_t, DelegateBuilder>(std::move(that)),
              delegate(std::move(that.delegate))
            {
                that.consume();
            }

            ~DelegateBuilder()
            {
                this->template registerCreator<DelegateBuilder>();
            }

            template <typename T>
            void build(T &&chain)
            {
                assert(this->consumed);
                delegate.resolve();
                chain(
                    [cb = std::move(this->delegate), exor = this->getXOR(), mask = this->getMask()](offs_t offset, input_mask_t memMask)
                    {
                        return (readcb_t::invokeRead<Result>(cb, offset, memMask & mask) ^ exor) & mask;
                    }
                );
            }

        private:
            Delegate delegate;
        };

        class Binder
        {
        public:
            Binder(readcb_t &cb) : target(cb) { }

            template <typename T> void set(T &&func, ctag_t *name)
            {
                setUsed();

                Device &dev = target.getOwner();
                Device &cdev = *dev.getSystemConfig().getConfigDevice();
                
                DelegateBuilder<delegate_type_t<T>>(target, append, cdev, "", std::forward<T>(func), name);
            }

        private:
            void setUsed() { assert(!used); used = true; }

            readcb_t &target;

            bool append = false;
            bool used = false;
        };

    public:
        template <unsigned Count>
        class array : public DeviceCallbackReadBase::array<readcb_t<Result, defaultMask>, Count>
        {
        public:
            using DeviceCallbackReadBase::array<readcb_t<Result, defaultMask>, Count>::array;

            void resolveAllSafe(Result defaultValue)
            {
                for (readcb_t<Result, defaultMask> &elem : *this)
                    elem.resolveSafe(defaultValue);
            }
        };

        readcb_t(Device &owner) : DeviceCallbackReadBase(owner) { }

        inline Binder bind() { return Binder(*this); }
        inline bool isNull() { return functions.empty(); }

        void resolveSafe(Result defaultValue)
        {
            resolve();
            if (!functions.empty())
                return;
            
            functions.emplace_back(
                [defaultValue](offs_t offset, std::make_unsigned_t<Result> memMask)
                {
                    return defaultValue;
                }
            );
        }

        void resolve()
        {
            assert(functions.empty());
            functions.reserve(creators.size());
            for (typename Creator::ptr const &creator : creators)
                functions.emplace_back(creator->create());
            creators.clear();
        }

        Result operator () (offs_t offset, std::make_unsigned_t<Result> memMask = defaultMask)
        {
            assert(creators.empty() && !functions.empty());

            typename std::vector<func_t>::const_iterator it(functions.begin());
            std::make_unsigned_t<Result> data((*it)(offset, memMask));
            while (functions.end() != ++it)
                data |= (*it)(offset, memMask);
            return data;
        }

        Result operator () ()
        {
            return this->operator ()(0, defaultMask);
        }

    private:
        std::vector<typename Creator::ptr> creators;
        std::vector<func_t> functions;
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

        class Creator
        {
        public:
            using ptr = std::unique_ptr<Creator>;
            
            virtual ~Creator() = default;

            virtual func_t create() = 0;

            std::make_unsigned_t<Input> getMask() const { return mask; }

        protected:
            Creator(std::make_unsigned_t<Input> mask) : mask(mask) { }

            std::make_unsigned_t<Input> mask;
        };

        template <typename T>
        class CreatorBase : public Creator
        {
        public:
            CreatorBase(T && builder) : Creator(builder.getMask()), builder(std::move(builder)) { }

            virtual func_t create() override
            {
                func_t newFunc = nullptr;
                builder.build(
                    [&newFunc](auto &&f)
                    {
                        newFunc = [cb = std::move(f)](offs_t offset, Input data, std::make_unsigned_t<Input> memMask)
                        {
                            cb(offset, data, memMask);
                        };
                    }
                );
                return newFunc;
            }

        private:
            T builder;
        };

        class BuilderBase
        {
        protected:
            BuilderBase(writecb_t &cb, bool append)
            : target(cb), append(append)
            { }
            BuilderBase(BuilderBase &&) = default;
            BuilderBase(BuilderBase const &) = delete;
            ~BuilderBase() { assert(consumed); }

            void consume() { consumed = true; }

            template <typename T>
            void registerCreator()
            {
                if (consumed == false)
                {
                    if (append == false)
                        target.creators.clear();
                    consumed = true;
                    target.creators.emplace_back(std::make_unique<CreatorBase<T>>(std::move(static_cast<T &>(*this))));
                }
            }

            writecb_t &target;
            bool append;
            bool consumed = false;
        };

        template <typename Delegate>
        class DelegateBuilder
        : public BuilderBase,
          public TransformBase<mask_t<Input, typename delegate_traits<Delegate>::input_t>, DelegateBuilder<Delegate>>
        {
        public:
            using input_t = intermediate_t<Input, typename delegate_traits<Delegate>::input_t>;

            template <typename T>
            DelegateBuilder(writecb_t &cb, bool append, Device &device, ctag_t *devName, T &&func, ctag_t *fncName)
            : BuilderBase(cb, append),
              TransformBase<mask_t<Input, typename delegate_traits<Delegate>::input_t>,
                DelegateBuilder>(defaultMask &delegate_traits<Delegate>::default_mask),
              delegate(device, devName, std::forward<T>(func), fncName)
            { }

            DelegateBuilder(DelegateBuilder &&that)
            : BuilderBase(std::move(that)),
              TransformBase<mask_t<Input, typename delegate_traits<Delegate>::input_t>, DelegateBuilder>(std::move(that)),
              delegate(std::move(that.delegate))
            {
                that.consume();
            }

            ~DelegateBuilder()
            {
                this->template registerCreator<DelegateBuilder>();
            }

            template <typename T>
            void build(T &&chain)
            {
                assert(this->consumed);
                delegate.resolve();
                chain(
                    [cb = std::move(this->delegate), exor = this->getXOR(), mask = this->getMask()]
                        (offs_t offset, Input data, input_t memMask)
                    {
                        writecb_t::invokeWrite<Input>(cb, offset, (data ^ exor) & mask, memMask & mask);
                    }
                );
            }

        private:
            Delegate delegate;
        };

        class Binder
        {
        public:
            Binder(writecb_t &cb) : target(cb) { }

            template <typename T> void set(T  &&func, ctag_t *name)
            {
                setUsed();

                Device &dev = target.getOwner();
                Device &cdev = *dev.getSystemConfig().getConfigDevice();

                DelegateBuilder<delegate_type_t<T>>(target, append, cdev, "", std::forward<T>(func), name);
            }

        private:
            void setUsed() { assert(!used); used = true; }

            writecb_t &target;

            bool append = false;
            bool used = false;
        };

    public:
        template <unsigned Count>
        class array : public DeviceCallbackWriteBase::array<writecb_t<Input, defaultMask>, Count>
        {
        public:
            using DeviceCallbackWriteBase::array<writecb_t<Input, defaultMask>, Count>::array;

            void resolveAllSafe()
            {
                for (writecb_t<Input, defaultMask> &elem : *this)
                    elem.resolveSafe();
            }
        };

        writecb_t(Device &owner) : DeviceCallbackWriteBase(owner) { }

        inline Binder bind() { return Binder(*this); }
        inline bool isNull() { return functions.empty(); }

        void resolveSafe()
        {
            resolve();
            if (!functions.empty())
                return;
            
            functions.emplace_back(
                [](offs_t offset, Input data, std::make_unsigned_t<Input> memMask)
                { }
            );
        }

        void resolve()
        {
            assert(functions.empty());
            functions.reserve(creators.size());
            for (typename Creator::ptr const &creator : creators)
                functions.emplace_back(creator->create());
            creators.clear();
        }

        void operator () (offs_t offset, Input data, std::make_unsigned_t<Input> memMask = defaultMask)
        {
            assert(creators.empty() && !functions.empty());

            typename std::vector<func_t>::const_iterator it(functions.begin());
            (*it)(offset, data, memMask);
            while (functions.end() != ++it)
                (*it)(offset, data, memMask);
        }

        void operator () (Input data)
        {
            this->operator ()(0, data, defaultMask);
        }

    private:
        std::vector<typename Creator::ptr> creators;
        std::vector<func_t> functions;
    };
   
}

using read8cb_t = emu::devices::readcb_t<uint8_t>;
using read16cb_t = emu::devices::readcb_t<uint16_t>;
using read32cb_t = emu::devices::readcb_t<uint32_t>;
using read64cb_t = emu::devices::readcb_t<uint64_t>;
using readlcb_t = emu::devices::readcb_t<uint8_t, 1u>;
using read1cb_t = emu::devices::readcb_t<uint8_t, 1u>;

using write8cb_t = emu::devices::writecb_t<uint8_t>;
using write16cb_t = emu::devices::writecb_t<uint16_t>;
using write32cb_t = emu::devices::writecb_t<uint32_t>;
using write64cb_t = emu::devices::writecb_t<uint64_t>;
using writelcb_t = emu::devices::writecb_t<uint8_t, 1u>;
using write1cb_t = emu::devices::writecb_t<uint8_t, 1u>;
