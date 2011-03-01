#ifndef ZLCOMA_DIRECTORY_H
#define ZLCOMA_DIRECTORY_H

#include "Node.h"
#include <list>

class Config;

namespace Simulator
{

class ZLCOMA::DirectoryTop : public ZLCOMA::Node
{
protected:
    DirectoryTop(const std::string& name, ZLCOMA& parent, Clock& clock);
};

class ZLCOMA::DirectoryBottom : public ZLCOMA::Node
{
protected:
    DirectoryBottom(const std::string& name, ZLCOMA& parent, Clock& clock);
};

class ZLCOMA::Directory : public ZLCOMA::DirectoryBottom, public ZLCOMA::DirectoryTop
{
public:
    struct Line
    {
        bool         valid;
        MemAddr      tag;
        unsigned int tokens;     ///< Tokens in the caches in the group
    };

private:
    ArbitratedService<CyclicArbitratedPort> p_lines;      ///< Arbitrator for access to the lines
    std::vector<Line>   m_lines;      ///< The cache lines
    size_t              m_lineSize;   ///< The size of a cache-line
    size_t              m_assoc;      ///< Number of lines in a set
    size_t              m_sets;       ///< Number of sets
    size_t              m_numTokens;  ///< Total number of tokens per cache line
    CacheID             m_firstCache; ///< ID of first cache in the ring
    CacheID             m_lastCache;  ///< ID of last cache in the ring

    // Processes
    Process p_InBottom;
    Process p_InTop;

    Line* FindLine(MemAddr address);
    Line* AllocateLine(MemAddr address);
    bool  OnMessageReceivedBottom(Message* msg);
    bool  OnMessageReceivedTop(Message* msg);
    bool  IsBelow(CacheID id) const;

    bool OnBELRead(Message *);
    bool OnBELAcquireTokens(Message *);
    bool OnBELEviction(Message *);
    bool OnBELDirNotification(Message *);

    // Processes
    Result DoInBottom();
    Result DoOutBottom();
    Result DoInTop();
    Result DoOutTop();

public:
    const Line* FindLine(MemAddr address) const;

    Directory(const std::string& name, ZLCOMA& parent, Clock& clock, size_t numTokens, CacheID firstCache, CacheID lastCache, const Config& config);

    void Cmd_Help(std::ostream& out, const std::vector<std::string>& arguments) const;
    void Cmd_Read(std::ostream& out, const std::vector<std::string>& arguments) const;
};

}
#endif
