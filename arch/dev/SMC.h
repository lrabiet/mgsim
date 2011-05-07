#ifndef SMC_H
#define SMC_H

#include "arch/IOBus.h"
#include "sim/storage.h"

class Config;
namespace Simulator
{
    class Processor;
    class ActiveROM;

    class SMC : public IIOBusClient, public Object
    {
    private:
        char *m_enumdata;
        size_t m_size;


        IIOBus& m_iobus;
        IODeviceID m_devid;

        const std::vector<std::pair<RegAddr, RegValue> >& m_regs;
        Processor& m_cpu;

        ActiveROM& m_rom;
        IODeviceID m_romid;

        bool       m_atboot;
        bool       m_enable_dca;
        SingleFlag m_start_dca;
        SingleFlag m_doboot;

    public:
        SMC(const std::string& name, Object& parent, IIOBus& iobus, IODeviceID devid, 
            const std::vector<std::pair<RegAddr, RegValue> >& regs,
            Processor& proc, 
            ActiveROM& rom, 
            Config& config);

        ~SMC();

        void Initialize(size_t numDevices);

        Process p_StartDCA;
        Process p_Boot;

        Result DoStartDCA();
        Result DoBoot();

        bool OnReadRequestReceived(IODeviceID from, MemAddr address, MemSize size);
        bool OnNotificationReceived(IOInterruptID which, Integer tag);
        
        void GetDeviceIdentity(IODeviceIdentification& id) const;
        std::string GetIODeviceName() const;
        

    };


}


#endif