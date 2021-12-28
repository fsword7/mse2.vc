// fw.h - Firmware inage management package
//
// Author:  Tim Stark
// Date:    12/27/21

#pragma once

// Per-region entry flags
#define FW_TYPE_MASK		0x0000'000F	 // Entry type
#define FW_BIT_WIDTH		0x0000'0030  // Bit width (0=8-bit, 1=16-but, 2=32-bit, 3=64-bit)
#define FW_ENDIAN_TYPE		0x0000'0040  // Endianess type (0=Little-endian, 1=Big-endian)
#define FW_INVERT_DATA		0x0000'0080	 // Inverted data
#define FW_REGION_DATA		0x0000'0100	 // Region data
#define FW_FILL_DATA		0x0000'8000  // Filled data
#define FW_FILL_VAL		    0x00FF'0000	 // Fill value (before loading)

// Per-image entry flag definitions



#define SET_WIDTH(x)		(((x) & 0x3) << 4)
#define SET_FILL(x)			((((x) & 0xFF) << 16) | FW_FILL_DATA)

#define	GET_WIDTH(x)		(((x) >> 4) & 0x3)
#define GET_FILL(x)			(((x) >> 16) & 0xFF)


// ROM entry type token definitions
#define FW_TYPE_END		    0			// End of ROM entry table
#define FW_TYPE_REGION		1			// Region entry
#define FW_TYPE_IMAGE		2			// ROM image space
#define FW_TYPE_FILL		3			// Filling ROM space
#define FW_TYPE_CONTAINER	4			// container package

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

#define FWENTRY_ISCONTAINER(entry)	((FW_GETFLAGS(entry) & FW_TYPE_MASK) == FW_TYPE_CONTAINER)
#define FWENTRY_ISREGION(entry)	    ((FW_GETFLAGS(entry) & FW_TYPE_MASK) == FW_TYPE_REGION)
#define FWENTRY_ISEND(entry)		((FW_GETFLAGS(entry) & FW_TYPE_MASK) == FW_TYPE_END)
#define FWENTRY_ISFILE(entry)		((FW_GETFLAGS(entry) & FW_TYPE_MASK) == FW_TYPE_IMAGE)
#define FWENTRY_ISREGIONEND(entry)	(FWENTRY_ISREGION(entry) || FWENTRY_ISEND(entry))

// ROM region entry
#define FWREGION_GETNAME(entry)	    ((entry).name)
#define FWREGION_GETHASH(entry)	    ((entry).hash)
#define FWREGION_GETFLAGS(entry)	((entry).flags)
#define FWREGION_GETOFFSET(entry)	((entry).offset)
#define FWREGION_GETLENGTH(entry)	((entry).length)
#define FWREGION_GETVALUE(entry)	((entry).value)
#define FWREGION_GETWIDTH(entry)	(8 << GET_WIDTH(FWREGION_GETFLAGS(entry)))
#define FWREGION_GETFILL(entry)	    GET_FILL(FWREGION_GETFLAGS(entry))

#define FWREGION_ISROMDATA(entry)			(((entry).flags & FW_REGION_DATA) == FW_REGION_IMAGE)
#define FWREGION_ISBIGENDIAN(entry)		    (((entry).flags & FW_ENDIANTYPE) == FW_REGION_BE)
#define FWREGION_ISLITTLEENDIAN(entry)		(((entry).flags & FW_ENDIANTYPE) == FW_REGION_LE)

#define FWREGION_HASFILLVALUE(entry)		((entry).flags & FW_FILL_DATA)

struct fwEntry_t
{
	ctag_t		*name;		// Region/file name
	ctag_t		*hash;		// Hash checksum string
	uint32_t	flags;		// ROM entry flags
	uint32_t	offset;		// offset
	uint32_t	length;		// length
	uint32_t	value;
};

typedef const fwEntry_t cfwEntry_t;
