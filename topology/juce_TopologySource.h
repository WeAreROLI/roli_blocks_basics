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

/** Base class for an entity that provides access to a blocks topology.

    @tags{Blocks}
*/
class TopologySource
{
public:
    //==============================================================================
    /** Destructor. */
    virtual ~TopologySource() = default;

    /** Returns the current topology that this object manages. */
    virtual BlockTopology getCurrentTopology() const = 0;

    /** Sets the TopologySource as active, occupying the midi port and trying to connect to the block devices */
    virtual void setActive (bool shouldBeActive) = 0;

    /** Returns true, if the TopologySource is currently trying to connect the block devices */
    virtual bool isActive() const = 0;

    /** Returns true if the topology is locked externally.*/
    virtual bool isLockedFromOutside() const = 0;

    //==============================================================================
    /** Used to receive callbacks for topology changes */
    struct Listener
    {
        virtual ~Listener() = default;

        /** Called for any change in topology - devices changed, connections changed, etc. */
        virtual void topologyChanged() {}

        /** Called when a new block is added to the topology. */
        virtual void blockAdded (const Block::Ptr) {}

        /** Called when a block is removed from the topology. */
        virtual void blockRemoved (const Block::Ptr) {}

        /** Called when a known block is updated.
            This could be because details have been received asynchronously. E.g. Block name.
         */
        virtual void blockUpdated (const Block::Ptr) {}
    };

    void addListener (Listener* l)       { listeners.add (l); }
    void removeListener (Listener* l)    { listeners.remove (l); }

    /** Invoke this to force touches-off on all physical devices. */
    virtual void cancelAllActiveTouches() noexcept {}

    /** Gets blocks from the current topology. */
    Block::Array getBlocks() const { return getCurrentTopology().blocks; }

    /**Gets a block with given uid from the current topology*/
    Block::Ptr getBlockWithUID (Block::UID uid) const { return getCurrentTopology().getBlockWithUID (uid); }

protected:
    //==============================================================================
    ListenerList<Listener> listeners;
};

} // namespace juce
