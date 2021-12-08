// driver.h - system driver package
//
// Author:  Tim Stark (fsword007@gmail.com)
// Date:    Dec 6, 2021

class Device;
class DeviceType;
class SystemConfig;

struct SystemFlag
{
    enum type : uint32_t
    {
        // System Flags
        CLASS_MASK  = 0x0000'000F,
        ORIENT_MASK = 0x0000'00F0,
        NOT_WORKING = 0x0000'0100,

        // Orientation flags
        FLIP_X  = 0x0000'0010,
        FLIP_Y  = 0x0000'0020,
        SWAP_XY = 0x0000'0040,

        // Machine class types
        CLASS_OTHER    = 0,
        CLASS_CONSOLE  = 1,
        CLASS_COMPUTER = 2,
        CLASS_TERMINAL = 3,
        CLASS_PRINTER  = 4
    };
};

struct SystemDriver
{
    typedef void (*sysConfigure)(SystemConfig &config, Device &owner);

    ctag_t              *name;
    ctag_t              *parent;
    ctag_t              *section;
    const DeviceType    &type;
    sysConfigure        configure;

    ctag_t              *description;
    ctag_t              *source; 
};


#define SYSTEM_NAME(Name) Name##_driver
#define SYSTEM_EXTERN(Name) extern const SystemDriver SYSTEM_NAME(Name)
#define SYSTEM_TRAITS_NAME(Name) Name##_system_traits

#define SYSTEM_TRAITS(Name, FullName)                   \
struct SYSTEM_TRAITS_NAME(Name) {                       \
    static constexpr tag_t shortName[] = #Name;         \
    static constexpr tag_t fullName[]  = FullName;      \
    static constexpr tag_t fileName[]  = __FILE__;      \
};                                                      \
constexpr tag_t SYSTEM_TRAITS_NAME(Name)::shortName[];  \
constexpr tag_t SYSTEM_TRAITS_NAME(Name)::fullName[];   \
constexpr tag_t SYSTEM_TRAITS_NAME(Name)::fileName[];

#define SYSTEM_TYPE(Name, Class)            \
systemCreator<Class,                        \
    (SYSTEM_TRAITS_NAME(Name)::shortName),  \
    (SYSTEM_TRAITS_NAME(Name)::fullName),   \
    (SYSTEM_TRAITS_NAME(Name)::fileName)>


#define CONSOLE(Name, Parent, Section, Type, Class, Configure, Reset, Company, Description, Flags) \
extern const SystemDriver SYSTEM_NAME(Name) = \
{                               \
    #Name,                      \
    #Parent,                    \
    #Section,                   \
    SYSTEM_TYPE(Name, Class),   \
    [] (SystemConfig &config, Device &owner) { static_cast<Class &>(owner).Configure(config); }, \
    ROM_NAME(Name),             \
    #Description,               \
    __FILE__                    \
};

#define COMPUTER(Name, Parent, Section, Type, Class, Configure, Reset, Company, Description, Flags) \
extern const SystemDriver SYSTEM_NAME(Name) = \
{                               \
    #Name,                      \
    #Parent,                    \
    #Section,                   \
    SYSTEM_TYPE(Name, Class),   \
    [] (SystemConfig &config, Device &owner) { static_cast<Class &>(owner).Configure(config); }, \
    ROM_NAME(Name),             \
    #Description,               \
    __FILE__                    \
};

#define TERMINAL(Name, Parent, Section, Type, Class, Configure, Reset, Company, Description, Flags) \
extern const SystemDriver SYSTEM_NAME(Name) = \
{                               \
    #Name,                      \
    #Parent,                    \
    #Section,                   \
    SYSTEM_TYPE(Name, Class),   \
    [] (SystemConfig &config, Device &owner) { static_cast<Class &>(owner).Configure(config); }, \
    ROM_NAME(Name),             \
    #Description,               \
    __FILE__                    \
};

#define PRINTER(Name, Parent, Section, Type, Class, Configure, Reset, Company, Description, Flags) \
extern const SystemDriver SYSTEM_NAME(Name) = \
{                               \
    #Name,                      \
    #Parent,                    \
    #Section,                   \
    SYSTEM_TYPE(Name, Class),   \
    [] (SystemConfig &config, Device &owner) { static_cast<Class &>(owner).Configure(config); }, \
    ROM_NAME(Name),             \
    #Description,               \
    __FILE__                    \
};
