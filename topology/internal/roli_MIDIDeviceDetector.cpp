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

namespace roli
{

struct MIDIDeviceDetector  : public PhysicalTopologySource::DeviceDetector
{
    MIDIDeviceDetector() {}

    juce::StringArray scanForDevices() override
    {
        juce::StringArray result;

        for (auto& pair : findDevices())
            result.add (pair.input.identifier + " & " + pair.output.identifier);

        return result;
    }

    PhysicalTopologySource::DeviceConnection* openDevice (int index) override
    {
        const auto allDevices = findDevices();

        if (allDevices.size() > index)
        {
            const auto pair = allDevices[index];
            auto dev = std::make_unique<MIDIDeviceConnection>();

            if (auto lock = createMidiPortLock (pair.input.name, pair.output.name))
            {
                lockedFromOutside = false;

                dev->setLockAgainstOtherProcesses (lock);
                dev->midiInput  = juce::MidiInput::openDevice  (pair.input.identifier, dev.get());
                dev->midiOutput = juce::MidiOutput::openDevice (pair.output.identifier);

                if (dev->midiInput != nullptr)
                {
                    dev->midiInput->start();
                    return dev.release();
                }
            }
            else
            {
                lockedFromOutside = true;
            }
        }

        return nullptr;
    }

    bool isLockedFromOutside() const override
    {
        return lockedFromOutside && ! findDevices().isEmpty();
    }

    static bool isBlocksMidiDeviceName (const juce::String& name)
    {
        return name.indexOf (" BLOCK") > 0 || name.indexOf (" Block") > 0;
    }

    static juce::String cleanBlocksDeviceName (juce::String name)
    {
        name = name.trim();

        if (name.endsWith (" IN)"))
            return name.dropLastCharacters (4);

        if (name.endsWith (" OUT)"))
            return name.dropLastCharacters (5);

        const int openBracketPosition = name.lastIndexOfChar ('[');
        if (openBracketPosition != -1 && name.endsWith ("]"))
            return name.dropLastCharacters (name.length() - openBracketPosition);

        return name;
    }

    struct MidiInputOutputPair
    {
        juce::MidiDeviceInfo input, output;
    };

    static juce::Array<MidiInputOutputPair> findDevices()
    {
        juce::Array<MidiInputOutputPair> result;

        auto midiInputs  = juce::MidiInput::getAvailableDevices();
        auto midiOutputs = juce::MidiOutput::getAvailableDevices();

        for (const auto& input : midiInputs)
        {
            if (isBlocksMidiDeviceName (input.name))
            {
                MidiInputOutputPair pair;
                pair.input = input;

                juce::String cleanedInputName = cleanBlocksDeviceName (input.name);

                int inputOccurences = 0;
                int outputOccurences = 0;

                for (const auto& p : result)
                    if (cleanBlocksDeviceName (p.input.name) == cleanedInputName)
                        ++inputOccurences;

                for (const auto& output : midiOutputs)
                {
                    if (cleanBlocksDeviceName (output.name) == cleanedInputName)
                    {
                        if (outputOccurences == inputOccurences)
                        {
                            pair.output = output;
                            break;
                        }

                        ++outputOccurences;
                    }
                }

                result.add (pair);
            }
        }

        return result;
    }

private:
    bool lockedFromOutside = true;

    /** For backwards compatibility, the block interprocess lock has to use the midi input name.
        The below is necessary because blocks of the same type might duplicate a port name, so
        must share an interprocess lock.
     */
    std::shared_ptr<juce::InterProcessLock> createMidiPortLock (const juce::String& midiInName, const juce::String& midiOutName)
    {
        const juce::String lockIdentifier = "blocks_sdk_"
                                            + juce::File::createLegalFileName (midiInName)
                                            + "_" + juce::File::createLegalFileName (midiOutName);

        const auto existingLock = midiPortLocks.find (lockIdentifier);

        if (existingLock != midiPortLocks.end())
            if (existingLock->second.use_count() > 0)
                return existingLock->second.lock();

        auto interprocessLock = std::make_shared<juce::InterProcessLock> (lockIdentifier);

        if (interprocessLock->enter (500))
        {
            midiPortLocks[lockIdentifier] = interprocessLock;
            return interprocessLock;
        }

        return nullptr;
    }

    std::map<juce::String, std::weak_ptr<juce::InterProcessLock>> midiPortLocks;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MIDIDeviceDetector)
};

} // namespace roli
