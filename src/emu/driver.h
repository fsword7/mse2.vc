// driver.h - system driver package
//
// Date:    May 7, 2023
// Author:  Tim Stark

#pragma once

struct SystemDriver
{
    using sysConfigure = void (*)(SystemConfig &config, Device &owner);

    cchar_t *name;
    cchar_t *parent;
    cchar_t *section;
    const DeviceType &type;
    sysConfigure configure;

    cchar_t *description;
    cchar_t *source;
};

using cSystemDriver = const SystemDriver;

#define SYSTEM_NAME(Name) Name##_driver
#define SYSTEM_EXTERN(Name) extern const SystemDriver SYSTEM_NAME(Name)
#define SYSTEM_TRAITS_NAME(Name) Name##_system_traits

#define SYSTEM_TRAITS(Name, FullName)                     \
struct SYSTEM_TRAITS_NAME(Name) {                         \
    static constexpr cchar_t shortName[] = #Name;         \
    static constexpr cchar_t fullName[]  = FullName;      \
    static constexpr cchar_t fileName[]  = __FILE__;      \
};                                                        \
constexpr cchar_t SYSTEM_TRAITS_NAME(Name)::shortName[];  \
constexpr cchar_t SYSTEM_TRAITS_NAME(Name)::fullName[];   \
constexpr cchar_t SYSTEM_TRAITS_NAME(Name)::fileName[];

#define SYSTEM_TYPE(Name, Class)            \
systemCreator<Class,                        \
    (SYSTEM_TRAITS_NAME(Name)::shortName),  \
    (SYSTEM_TRAITS_NAME(Name)::fullName),   \
    (SYSTEM_TRAITS_NAME(Name)::fileName)>


// #define COMPUTER(Name, Parent, Section, Type, Class, Configure, Reset, Company, Description, Flags) \
// SYSTEM_TRAITS(Name, Description) \
// extern const SystemDriver SYSTEM_NAME(Name) = \
// {                                \
//     #Name,                       \
//     #Parent,                     \
//     #Section,                    \
//     SYSTEM_TYPE(Name, Class),    \
//     [] (SystemConfig &config, Device &owner) { static_cast<Class &>(owner).Configure(config); }, \
//     FW_NAME(Name),               \
//     #Description,                \
//     __FILE__                     \
// };

#define COMPUTER(Name, Parent, Section, Type, Class, Configure, Reset, Company, Description, Flags) \
SYSTEM_TRAITS(Name, Description) \
extern const SystemDriver SYSTEM_NAME(Name) = \
{                                \
    #Name,                       \
    #Parent,                     \
    #Section,                    \
    SYSTEM_TYPE(Name, Class),    \
    [] (SystemConfig &config, Device &owner) { static_cast<Class &>(owner).Configure(config); }, \
    #Description,                \
    __FILE__                     \
};
