/*
  ==============================================================================

   Copyright (c) 2020 - ROLI Ltd

   Permission to use, copy, modify, and/or distribute this software for any
   purpose with or without fee is hereby granted, provided that the above
   copyright notice and this permission notice appear in all copies.

   THE SOFTWARE IS PROVIDED “AS IS” AND ROLI LTD DISCLAIMS ALL WARRANTIES WITH
   REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
   AND FITNESS. IN NO EVENT SHALL ROLI LTD BE LIABLE FOR ANY SPECIAL, DIRECT,
   INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
   LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
   OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
   PERFORMANCE OF THIS SOFTWARE.

  ==============================================================================
*/

namespace juce
{

struct RuleBasedTopologySource::Internal  : public TopologySource::Listener,
                                            private AsyncUpdater
{
    Internal (RuleBasedTopologySource& da, TopologySource& bd)  : owner (da), detector (bd)
    {
        detector.addListener (this);
    }

    ~Internal() override
    {
        detector.removeListener (this);
    }

    void clearRules()
    {
        if (! rules.isEmpty())
        {
            rules.clear();
            triggerAsyncUpdate();
        }
    }

    void addRule (Rule* r)
    {
        if (r != nullptr)
        {
            rules.add (r);
            triggerAsyncUpdate();
        }
    }

    void topologyChanged() override
    {
        cancelPendingUpdate();
        regenerateTopology();
    }

    void handleAsyncUpdate() override
    {
        topologyChanged();
    }

    void regenerateTopology()
    {
        auto newTopology = detector.getCurrentTopology();

        for (auto rule : rules)
            rule->transformTopology (newTopology);

        if (topology != newTopology)
        {
            topology = newTopology;
            owner.listeners.call ([] (TopologySource::Listener& l) { l.topologyChanged(); });
        }
    }

    void setActive (bool shouldBeActive)
    {
        detector.setActive (shouldBeActive);
    }

    bool isActive() const
    {
        return detector.isActive();
    }

    RuleBasedTopologySource& owner;
    TopologySource& detector;

    BlockTopology topology;
    OwnedArray<Rule> rules;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Internal)
};

RuleBasedTopologySource::RuleBasedTopologySource (TopologySource& d)
{
    internal.reset (new Internal (*this, d));
}

RuleBasedTopologySource::~RuleBasedTopologySource()
{
    internal = nullptr;
}

BlockTopology RuleBasedTopologySource::getCurrentTopology() const             { return internal->topology; }

void RuleBasedTopologySource::clearRules()                                    { internal->clearRules(); }
void RuleBasedTopologySource::addRule (Rule* r)                               { internal->addRule (r); }

void RuleBasedTopologySource::setActive (bool shouldBeActive)
{
    internal->setActive (shouldBeActive);
}

bool RuleBasedTopologySource::isActive() const
{
    return internal->isActive();
}

} // namespace juce
