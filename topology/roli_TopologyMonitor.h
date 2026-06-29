/*
  ==============================================================================

    Copyright 2019 Luminary ROLI Ltd.

  ==============================================================================
*/
#pragma once
#include "roli_TopologySource.h"

namespace roli
{
class TopologyMonitor  : private juce::Timer
{
public:
    //==============================================================================
    using ConditionFunc = std::function<bool (const TopologySource&)>;
    using CallbackFunc = std::function<void (const bool conditionMet)>;

    //==============================================================================
    TopologyMonitor (TopologySource& topologySource);

    //==============================================================================
    void waitFor (ConditionFunc,
                  CallbackFunc,
                  const juce::RelativeTime&);

    void waitForStableVersions (Block::Array,
                                CallbackFunc,
                                const juce::RelativeTime& maxWait = defaultWaitTime);

    void whenBlocksAreAvalible (CallbackFunc,
                                const juce::RelativeTime& maxWait = defaultWaitTime);

    void waitForBlocks (Block::Array,
                        CallbackFunc,
                        const juce::RelativeTime& maxWait = defaultWaitTime);

    void waitForBlocks (std::vector<juce::String> blockSerials,
                        CallbackFunc,
                        const juce::RelativeTime& maxWait = defaultWaitTime);

    //==============================================================================
    static bool blockUntilVersionsSynced (const Block::Array&,
                                          const juce::RelativeTime& maxWait = defaultWaitTime);

private:
    //==============================================================================
    struct Waiter
    {
        ConditionFunc condition;
        CallbackFunc callback;
        juce::Time timeout;
    };

    static const juce::RelativeTime defaultWaitTime;

    std::vector<Waiter> waiters;
    TopologySource& topologySource;

    void startPolling();
    void timerCallback() override;
};
}
