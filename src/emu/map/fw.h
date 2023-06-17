// fw.h - Fireware Image Managerment package
//
// Author:  Tim Stark
// Date:    Jun 17, 2023

#pragma once

// Per-region entry flag definitions
#define FW_TYPE_MASK		0x0000'000F	 // Entry type
#define FW_BIT_WIDTH		0x0000'0030  // Bit width (0=8-bit, 1=16-but, 2=32-bit, 3=64-bit)
#define FW_ENDIAN_TYPE		0x0000'0040  // Endianess type (0=Little-endian, 1=Big-endian)
#define FW_DATA_TYPE		0x0000'0080  // Data type (0 = Image, 1 = Disk)
#define FW_REGION_DATA		0x0000'0100	 // Region data
#define FW_INVERTED_DATA	0x0000'0200	 // Inverted data
#define FW_FILL_DATA		0x0000'8000  // Fill/erase region space before loading
#define FW_FILL_VAL		    0x00FF'0000	 // Filling value (before loading)

// Per-image entry flag definitions
#define FW_READONLY_BIT		0x0000'0010  // Read-only bit (0 = read/write access, 1 = read-only access)
#define FW_REQUIRED_BIT		0x0000'0020  // Required bit (0 = Optional, 1 = Required)
#define FW_REVERSE_BIT		0x0000'0040  // Reverse the byte order of loading image
#define FW_INHERIT_BIT		0x0000'0080  // Inherit flags from the previous image
#define FW_GROUP_MASK		0x0000'0F00  // load data in group of size + 1
#define FW_SKIP_MASK		0x0000'F000  // Skip bytes each group
#define FW_BITWIDTH_MASK	0x000F'0000  // Width of data in image space
#define FW_BITSHIFT_MASK	0x00F0'0000  // left-shift count for bits in image space
#define FW_BIOSFLAGS_MASK   0xFF00'0000  // BIOS flags

#define FW_INHERIT_FLAGS	(FW_GROUP_MASK|FW_GROUP_SKIP_MASK|FW_BITWIDTH_MASK|FW_BITSHIFT_MASK|FW_BIOSFLAGS_MASK)

#define SET_WIDTH(x)		(((x) & 0x3) << 4)
#define SET_FILL(x)			((((x) & 0xFF) << 16) | FW_FILL_DATA)
#define SET_BIOS(x)			(((x) & 0xFF) << 16)

#define	GET_WIDTH(x)		(((x) >> 4) & 0x3)
#define GET_FILL(x)			(((x) >> 16) & 0xFF)
#define GET_BIOS(x)			(((x) >> 16) & 0xFF)

// Per-image entry flags
#define GET_GROUP(x)		(((x) >> 8) & 0x0F)
#define GET_SKIP(x)         (((x) >> 12) & 0x0F)
#define GET_BITWIDTH(x)		(((x) >> 16) & 0x0F)
#define GET_BITSHIFT(x)		(((x) >> 20) & 0x0F)
#define GET_BIOSFLAGS(x)	(((x) >> 24) & 0xFF)

// ROM entry type token definitions
#define FW_TYPE_END		    	0			// End of ROM entry table
#define FW_TYPE_REGION			1			// Region entry
#define FW_TYPE_IMAGE			2			// ROM image space
#define FW_TYPE_FILL			3			// Filling ROM space
#define FW_TYPE_COPY			4			// Copy from another region/offset
#define FW_TYPE_CONTINUE		5			// Continue loading from the previous image
#define FW_TYPE_IGNORE			6			// Ignore data from loading
#define FW_TYPE_CONTAINER		7			// container package
#define FW_TYPE_CARTRIDGE		8			// cartridge
#define FW_TYPE_SYSTEM_BIOS		9			// system bios
#define FW_TYPE_DEFAULT_BIOS	10			// default bios

// Per-region ROM bit width definitions
#define FW_REGION_8BIT		0x0000'0000	// 8-bit data width
#define FW_REGION_16BIT	    0x0000'0010	// 16-bit data width
#define FW_REGION_32BIT	    0x0000'0020	// 32-bit data width
#define FW_REGION_64BIT	    0x0000'0030	// 64-bit data width

// Per-region Endianess flag definitions
#define FW_REGION_LE		0x0000'0000	// Little-endian
#define FW_REGION_BE		0x0000'0040	// Big-endian

// Per-region invert flag definitions
#define FW_REGION_NORMAL	0x0000'0000	// Normal data
#define FW_REGION_INVERT	0x0000'0080	// Inverted data

// Per-region data type
#define FW_REGION_IMAGE     0x0000'0000 // Image data

// Per-region erase (filling) value definitions
#define FW_ERASEVAL(x)		SET_FILL(x)
#define FW_ERASE00			SET_FILL(0x00)
#define FW_ERASEFF			SET_FILL(0xFF)

#define FW_CONTAINER(name) \
	{ name, nullptr, FW_TYPE_CONTAINER, 0, 0, 0 }

#define FW_SYSTEM_BIOS(value, name, desc) \
	{ name, desc, FW_TYPE_SYSTEM_BIOS | SET_BIOS(value), 0, 0, 0 }

#define FW_DEFAULT_BIOS(name) \
	{ name, nullptr, FW_TYPE_DEFAULT_BIOS, 0, 0, 0 }

#define FW_REGION(name, length, flags) \
	{ name, nullptr, FW_TYPE_REGION | (flags), 0, length, 0 }

#define FW_LOAD(name, offset, length, flags, hash) \
	{ name, hash, FW_TYPE_IMAGE | (flags), offset, length, 0 }

#define FW_NAME(name)	fw_##name
#define FW_END			{ nullptr, nullptr, FW_TYPE_END, 0, 0, 0 }

#define FW_GETFLAGS(entry)			((entry)->flags)
#define FW_GETNAME(entry)			((entry)->name)
#define FW_GETLENGTH(entry)		    ((entry)->length)
#define FW_GETOFFSET(entry)		    ((entry)->offset)

#define FWENTRY_ISCONTAINER(entry)	  ((FW_GETFLAGS(entry) & FW_TYPE_MASK) == FW_TYPE_CONTAINER)
#define FWENTRY_ISSYSTEM_BIOS(entry)  ((FW_GETFLAGS(entry) & FW_TYPE_MASK) == FW_TYPE_SYSTEM_BIOS)
#define FWENTRY_ISDEFAULT_BIOS(entry) ((FW_GETFLAGS(entry) & FW_TYPE_MASK) == FW_TYPE_DEFAULT_BIOS)
#define FWENTRY_ISREGION(entry)	      ((FW_GETFLAGS(entry) & FW_TYPE_MASK) == FW_TYPE_REGION)
#define FWENTRY_ISEND(entry)		  ((FW_GETFLAGS(entry) & FW_TYPE_MASK) == FW_TYPE_END)
#define FWENTRY_ISFILE(entry)		  ((FW_GETFLAGS(entry) & FW_TYPE_MASK) == FW_TYPE_IMAGE)
#define FWENTRY_ISREGIONEND(entry)	  (FWENTRY_ISREGION(entry) || FWENTRY_ISEND(entry))

// Per-region entry macro definitons
#define FWREGION_GETNAME(entry)	    ((entry).name)
#define FWREGION_GETHASH(entry)	    ((entry).hash)
#define FWREGION_GETFLAGS(entry)	((entry).flags)
#define FWREGION_GETOFFSET(entry)	((entry).offset)
#define FWREGION_GETLENGTH(entry)	((entry).length)
#define FWREGION_GETVALUE(entry)	((entry).value)
#define FWREGION_GETWIDTH(entry)	(8 << GET_WIDTH(FWREGION_GETFLAGS(entry)))
#define FWREGION_GETFILL(entry)	    GET_FILL(FWREGION_GETFLAGS(entry))

#define FWREGION_ISROMDATA(entry)			(((entry).flags & FW_REGION_DATA) == FW_REGION_IMAGE)
#define FWREGION_ISBIGENDIAN(entry)		    (((entry).flags & FW_ENDIAN_TYPE) == FW_REGION_BE)
#define FWREGION_ISLITTLEENDIAN(entry)		(((entry).flags & FW_ENDIAN_TYPE) == FW_REGION_LE)

#define FWREGION_HASFILLVALUE(entry)		((entry).flags & FW_FILL_DATA)

// Per-image entry macro definitons
#define FWIMAGE_ISREQUIRED(entry)		((entry)->flags & FW_REQUIRED_BIT)
#define FWIMAGE_INHERITED(entry) 		((entry)->flags & FW_INHERIT_BIT)
#define FWIMAGE_ISREVERSED(entry)		((entry)->flags & FW_REVERSE_BIT)
#define FWIMAGE_GETGROUPSIZE(entry)		GET_GROUP(FW_GETFLAGS(entry))
#define FWIMAGE_GETSKIP(entry)			GET_SKIP(FW_GETFLAGS(entry))
#define FWIMAGE_GETBITWIDTH(entry)		GET_BITWIDTH(FW_GETFLAGS(entry))
#define FWIMAGE_GETBITSHIFT(entry)		GET_BITSHIFT(FW_GETFLAGS(entry))
#define FWIMAGE_GETBIOSFLAGS(entry)		GET_BIOSFLAGS(FW_GETFLAGS(entry))

struct fwEntry_t
{
	cchar_t		*name;		// Region/file name
	cchar_t		*hash;		// Hash checksum string
	uint32_t	flags;		// ROM entry flags
	uint32_t	offset;		// offset
	uint32_t	length;		// length
	uint32_t	value;
};

using cfwEntry_t = const fwEntry_t;
