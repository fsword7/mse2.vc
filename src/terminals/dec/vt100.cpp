// vt100.cpp - DEC VT100 terminal emulator
//
// Author:  Tim Stark
// Date:    12/8/2021

#include "emu/core.h"
#include "emu/map/map.h"
#include "emu/map/addrmap.h"
#include "emu/devsys.h"
#include "emu/devcpu.h"
#include "emu/devcb.h"

#include "dev/cpu/mcs80/mcs80.h"
#include "dev/bus/rs232/rs232.h"
#include "dev/chip/i8251.h"
#include "dev/video/dec/vt100.h"
#include "lib/util/xtal.h"

class vt100_Device : public SystemDevice
{
public:
    vt100_Device(const SystemConfig &config, const DeviceType &type, cstag_t &devName, uint64_t clock)
    : SystemDevice(config, type, devName, clock),
      cpu(*this, "i8080"),
      crt(*this, "VT100_Video"),
      usart(*this, "usart"),
      rs232(*this, "rs232"),
      ramData(*this, "ram")
    {

    }

    void vt100(SystemConfig &config);
    void vt100_init();

    void vt100_setMemoryMap(map::AddressList &map);
    void vt100_setIOPort(map::AddressList &map);
    
    uint32_t vt100_updateScreen(ScreenDevice &screen, bitmap16_t &bitmap, const rect_t &clip);
    uint8_t readData(offs_t addr);

    uint8_t read8flags();
    uint8_t read8modem();
    void write8nvr(uint8_t data);

private:
    RequiredDevice<i8080_cpuDevice> cpu;
    RequiredDevice<vt100video_t> crt;
    RequiredDevice<i8251_Device> usart;
    RequiredDevice<rs232_portDevice> rs232;

    RequiredSharedPointer<uint8_t> ramData;
};

uint32_t vt100_Device::vt100_updateScreen(ScreenDevice &screen, bitmap16_t &bitmap, const rect_t &clip)
{
    crt->updateVideo(bitmap, clip);
    return 0;
}

uint8_t vt100_Device::readData(offs_t addr)
{
    return ramData[addr];
}

uint8_t vt100_Device::read8flags()
{
    return 0;
}

uint8_t vt100_Device::read8modem()
{
    uint8_t result = 0;

    // Receiving signals from RS232 interface
    result |= rs232->read1cts() << 7;
    result |= rs232->read1si()  << 6;
    result |= rs232->read1ri()  << 5;
    result |= rs232->read1dcd() << 4;

    return result;
}

void vt100_Device::write8nvr(uint8_t data)
{
    // nvr->write1c1(!(data >> 1) & 1);
    // nvr->write1c2(!(data >> 2) & 1);
    // nvr->write1c3(!(data >> 3) & 1);
    // nvr->write(data & 2) ? 0 : !(data & 1));

    rs232->writw1spds((data >> 5) & 1);
}

void vt100_Device::vt100(SystemConfig &config)
{
    // fmt::printf("VT100 device configuration here.\n");

    i8080(config, cpu, "cpu", XTAL(24'883'200) / 9);
    cpu->setAddressMap(map::asProgram, &vt100_Device::vt100_setMemoryMap);
    cpu->setAddressMap(map::asIOPort, &vt100_Device::vt100_setIOPort);
    
    screen_t *screen = Screen(config, "scr", scrRaster, Color::amber());
    screen->setScreenArea(XTAL(24'073'400) * 2/3,  102*10, 0, 80*10,  262, 0, 25*10);
    screen->setScreenUpdate(FUNC(vt100_Device::vt100_updateScreen));

    VT100_VIDEO(config, crt, "crt", XTAL(24'073'400));
    // crt->bindReadRAMDataCallback().set(FUNC(vt100_Device::readData));

    I8251(config, usart, "usart", XTAL(24'883'200) / 9);
    RS232_PORT(config, rs232, "rs232", 0);

}

void vt100_Device::vt100_init()
{
}

void vt100_Device::vt100_setMemoryMap(map::AddressList &map)
{
    map.setUnmappedHigh();

    map(0x0000, 0x1FFF).rom();
    map(0x2000, 0x3FFF).ram().share("ram");
    map(0x8000, 0x9FFF).rom();
    map(0xA000, 0xBFFF).rom();
}

void vt100_Device::vt100_setIOPort(map::AddressList &map)
{
    map.setUnmappedHigh();

    map(0x00, 0x01).rw(usart, FUNC(i8251_Device::read8io), FUNC(i8251_Device::write8io));
    map(0x22, 0x22).r(FUNC(vt100_Device::read8modem));
    map(0x42, 0x42).r(FUNC(vt100_Device::read8flags));
    map(0x42, 0x42).w(crt, FUNC(vt100video_t::write8_brightness));
    map(0x62, 0x62).w(FUNC(vt100_Device::write8nvr));
    map(0xA2, 0xA2).w(crt, FUNC(vt100video_t::write8_dc012));
    map(0xC2, 0xC2).w(crt, FUNC(vt100video_t::write8_dc011));
}

static const fwEntry_t FW_NAME(vt100)[] =
{      
    FW_REGION("cpu", 0x10000, FW_ERASEFF),
    FW_LOAD("23-061e2-00.e56", 0x0000, 0x0800, 0, nullptr),
    FW_LOAD("23-032e2-00.e52", 0x0800, 0x0800, 0, nullptr),
    FW_LOAD("23-033e2-00.e45", 0x1000, 0x0800, 0, nullptr),
    FW_LOAD("23-034e2-00.e40", 0x1800, 0x0800, 0, nullptr),

    FW_REGION("chargen", 0x0800, 0),
    FW_LOAD("23-018e2-00.e4", 0x0000, 0x0800, 0, nullptr),

	FW_END
};

TERMINAL(vt100, nullptr, dec, vt100, vt100_Device, vt100, vt100_init, "DEC", "VT100 Terminal", SYSTEM_NOT_WORKING)