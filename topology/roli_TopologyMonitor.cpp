/*
  ==============================================================================

    Copyright 2019 Luminary ROLI Ltd.

  ==============================================================================
*/

namespace roli
{
namespace
{
    bool allVersionsAreSynced (const Block::Array& blocks)
    {
        // Some firmware versions will temporarly missreport the version number after
        // an update. A short wait will give them time to update.
        static const auto minConnectionTime = juce::RelativeTime::seconds (2.0);
        const auto currentTime = juce::Time::getCurrentTime();

        for (const auto& block : blocks)
        {
            if (! block->isConnected())
                return false;

            if (block->versionNumber.isEmpty())
                return false;
            else if (currentTime < block->getConnectionTime() + minConnectionTime)
                return false;
        }

        return true;
    }
}

//==============================================================================
const juce::RelativeTime TopologyMonitor::defaultWaitTime = juce::RelativeTime::seconds (5.0);

TopologyMonitor::TopologyMonitor (TopologySource& topologySource)
    : topologySource (topologySource)
{
}

void TopologyMonitor::waitFor (ConditionFunc condition,
                               CallbackFunc callback,
                               const juce::RelativeTime& maxTime)
{
    if (condition == nullptr || callback == nullptr)
    {
        jassertfalse;
        return;
    }

    const auto timeout = juce::Time::getCurrentTime() + maxTime;

    waiters.push_back ({ std::move (condition),
                         std::move (callback),
                         std::move (timeout) });
    startPolling();
}

void TopologyMonitor::waitForStableVersions (Block::Array blocksToWaitFor,
                                             CallbackFunc callback,
                                             const juce::RelativeTime& maxWait)
{
    const auto condition = [blocks = std::move (blocksToWaitFor)] (const TopologySource&)
    {
        return allVersionsAreSynced (blocks);
    };

    waitFor (condition, std::move (callback), maxWait);
}

void TopologyMonitor::whenBlocksAreAvalible (CallbackFunc callback,
                                             const juce::RelativeTime& maxWait)
{
    const auto condition = [](const TopologySource& ts)
    {
        return ts.getBlocks().size() > 0;
    };

    waitFor (condition, std::move (callback), maxWait);
}

void TopologyMonitor::waitForBlocks (Block::Array expectedBlocks,
                                     CallbackFunc callback,
                                     const juce::RelativeTime& maxWait)
{
    const auto condition = [blocks = std::move (expectedBlocks)] (const TopologySource&)
    {
        for (const auto& block : blocks)
            if (! block->isConnected())
                return false;

        return true;
    };

    waitFor (condition, std::move (callback), maxWait);
}

namespace
{
    bool containsBlock (const juce::String& expectedSerial,
                        const Block::Array& blocks)
    {
        for (const auto block : blocks)
            if (block->serialNumber == expectedSerial)
                return true;

        return false;
    }
}

void TopologyMonitor::waitForBlocks (std::vector<juce::String> blockSerials,
                                     CallbackFunc callback,
                                     const juce::RelativeTime& maxWait)
{
    const auto condition = [serials = std::move (blockSerials)] (const TopologySource& ts)
    {
        const auto blocks = ts.getBlocks();

        for (const auto& expectedSerial : serials)
            if (! containsBlock (expectedSerial, blocks))
                return false;

        return true;
    };

    waitFor (condition, std::move (callback), maxWait);
}

//==============================================================================
void TopologyMonitor::startPolling()
{
    startTimer (500);
}

void TopologyMonitor::timerCallback()
{
    const auto removeIf = [this, timeNow = juce::Time::getCurrentTime()] (const auto& waiter)
    {
        if (waiter.condition (topologySource))
        {
            waiter.callback (true);
            return true;
        }
        else if (timeNow > waiter.timeout)
        {
            waiter.callback (false);
            return true;
        }

        return false;
    };

    for (auto i = int (waiters.size()); --i >= 0;)
        if (removeIf (waiters[size_t (i)]))
            waiters.erase (waiters.begin() + i);

    if (waiters.size() < 1)
        stopTimer();
}

//==============================================================================
bool TopologyMonitor::blockUntilVersionsSynced (const Block::Array& blocks,
                                                const juce::RelativeTime& maxWait)
{
    jassert (! juce::MessageManager::getInstanceWithoutCreating()->isThisTheMessageThread());

    const auto timeout = juce::Time::getCurrentTime() + maxWait;

    while (timeout > juce::Time::getCurrentTime())
    {
        if (allVersionsAreSynced (blocks))
            break;

        juce::Thread::sleep (100);
    }

    return allVersionsAreSynced (blocks);
}
}
