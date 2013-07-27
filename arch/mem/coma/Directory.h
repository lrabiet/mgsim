#ifndef COMA_DIRECTORY_H
#define COMA_DIRECTORY_H

#include "Node.h"
#include <sim/inspect.h>
#include <arch/BankSelector.h>

#include <queue>
#include <set>

class Config;

namespace Simulator
{


class COMA::DirectoryTop : public COMA::Node
{
protected:
    friend class OneLevelCOMA;
    friend class TwoLevelCOMA;
    friend class COMA::Directory;
    DirectoryTop(const std::string& name, COMA& parent, Clock& clock, size_t& numLines, Config& config);
    size_t GetNumLines() const override;
    size_t& m_numLines;
};

class COMA::DirectoryBottom : public COMA::Node
{
protected:
    friend class OneLevelCOMA;
    friend class TwoLevelCOMA;
    friend class COMA::Directory;
    DirectoryBottom(const std::string& name, COMA& parent, Clock& clock, Config& config);
};

class COMA::Directory : public COMA::Object, public Inspect::Interface<Inspect::Read>
{
public:
    struct Line
    {
        bool         valid;  ///< Valid line?
        MemAddr      tag;    ///< Tag of this line
        unsigned int tokens; ///< Tokens in this ring
    };

protected:
    friend class COMA;
    friend class OneLevelCOMA;
    friend class TwoLevelCOMA;
    COMA::DirectoryBottom m_bottom;
    COMA::DirectoryTop    m_top;

private:
    typedef COMA::Node::Message Message;

    IBankSelector&      m_selector;
    ArbitratedService<CyclicArbitratedPort> p_lines;      ///< Arbitrator for access to the lines
    size_t              m_assoc;      ///< Number of lines in a set
    size_t              m_sets;       ///< Number of sets
    std::vector<Line>   m_lines;      ///< The cache lines
    size_t              m_lineSize;   ///< The size of a cache-line
    size_t              m_maxNumLines; ///< Maximum number of lines to store
    NodeID              m_firstNode;  ///< ID of first cache in the ring
    NodeID              m_lastNode;   ///< ID of last cache in the ring

    // Processes
    Process p_InBottom;
    Process p_InTop;

    Line* FindLine(MemAddr address);
    Line* AllocateLine(MemAddr address);
    bool  OnMessageReceivedBottom(Message* msg);
    bool  OnMessageReceivedTop(Message* msg);
    bool  IsBelow(NodeID id) const;

    // Processes
    Result DoInBottom();
    Result DoOutBottom();
    Result DoInTop();
    Result DoOutTop();

public:
    Directory(const std::string& name, COMA& parent, Clock& clock, Config& config);

    // Connect directory to caches
    void ConnectRing(Node* first, Node* last);
    // Initialize after all caches have been connected
    void Initialize();

    void Cmd_Info(std::ostream& out, const std::vector<std::string>& arguments) const;
    void Cmd_Read(std::ostream& out, const std::vector<std::string>& arguments) const;
};

}
#endif
