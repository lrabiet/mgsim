#ifndef SIMPLEMEMORY_H
#define SIMPLEMEMORY_H

#include <queue>
#include <set>
#include "Memory.h"
#include "kernel.h"

namespace Simulator
{

class SimpleMemory : public IComponent, public IMemory, public IMemoryAdmin
{
public:
	struct Config
	{
        MemSize    size;
        BufferSize bufferSize;
        CycleNo    baseRequestTime;
        CycleNo    timePerLine;
        size_t     sizeOfLine;
	};

    class Request
    {
        void release()
        {
            if (refcount != NULL && --*refcount == 0) {
                delete[] (char*)data.data;
                delete refcount;
            }
        }

    public:
        unsigned long*      refcount;
        bool                write;
        CycleNo             done;
        MemAddr             address;
        MemData             data;
        IMemoryCallback*    callback;

        Request& operator =(const Request& req)
        {
            release();
            refcount  = req.refcount;
            write     = req.write;
            done      = req.done;
            address   = req.address;
            data      = req.data;
            callback  = req.callback;
            ++*refcount;
            return *this;
        }

        Request(const Request& req) : refcount(NULL) { *this = req; }
        Request() { refcount = new unsigned long(1); data.data = NULL; }
        ~Request() { release(); }
    };

    SimpleMemory(Object* parent, Kernel& kernel, const std::string& name, const Config& config);
    ~SimpleMemory();

    CycleNo getTotalWaitTime() const { return m_totalWaitTime; }

    // Component
    Result onCycleWritePhase(int stateIndex);

    // IMemory
    void registerListener  (IMemoryCallback& callback);
    void unregisterListener(IMemoryCallback& callback);
    Result read (IMemoryCallback& callback, MemAddr address, void* data, MemSize size, MemTag tag);
    Result write(IMemoryCallback& callback, MemAddr address, void* data, MemSize size, MemTag tag);

    // IMemoryAdmin
    void read (MemAddr address, void* data, MemSize size);
    void write(MemAddr address, void* data, MemSize size);
    bool idle()   const;

    const std::queue<Request>& getRequests() const {
        return m_requests;
    }

private:
    std::set<IMemoryCallback*>  m_caches;
    std::queue<Request>         m_requests;
    BufferSize                  m_requestsSize;
    char*                       m_memory;
    MemSize                     m_size;
    CycleNo					    m_baseRequestTime;
    CycleNo						m_timePerLine;
    size_t						m_sizeOfLine;

    // Statistics
    CycleNo                     m_totalWaitTime;
};

}
#endif

