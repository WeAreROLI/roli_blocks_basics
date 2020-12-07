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
    class BlockSerialReader
        : private MIDIDeviceConnection::Listener,
          private juce::Timer
    {
    public:
        //==============================================================================
        BlockSerialReader (MIDIDeviceConnection& deviceConnectionToUse) : deviceConnection (deviceConnectionToUse)
        {
            deviceConnection.addListener (this);
            startTimer (10);
        }

        ~BlockSerialReader() override
        {
            deviceConnection.removeListener (this);
        }

        bool hasSerial() const { return serial.isNotEmpty(); }

        juce::String getSerial() const { return serial; }

    private:
        MIDIDeviceConnection& deviceConnection;
        juce::String serial;

        bool shouldStop() { return hasSerial(); }

        //==============================================================================
        void timerCallback() override
        {
            if (shouldStop())
            {
                stopTimer();
                return;
            }

            sendRequest();
            startTimer (300);
        }

        void sendRequest()
        {
            const juce::uint8 dumpRequest[] = { 0xf0, 0x00, 0x21, 0x10, 0x78, 0x3f, 0xf7 };
            deviceConnection.sendMessageToDevice (dumpRequest, sizeof (dumpRequest));
        }

        void handleIncomingMidiMessage (const juce::MidiMessage& message) override
        {
            if (hasSerial())
                return;

            if (isResponse (message))
                parseResponse (message);
        }

        void connectionBeingDeleted (const MIDIDeviceConnection&) override
        {
            stopTimer();
        }

        bool isResponse (const juce::MidiMessage message)
        {
            const juce::uint8 roliDumpHeader[] = { 0xf0, 0x00, 0x21, 0x10, 0x78};
            return memcmp (message.getRawData(), roliDumpHeader, sizeof (roliDumpHeader)) == 0;
        }

        void parseResponse (const juce::MidiMessage& message)
        {
            int index = findMacAddressStart (message);

            if (index >= 0)
            {
                const int macSize = 17;
                const int offset = index + macSize;
                const int serialSize = 16;

                if (message.getRawDataSize() - offset < serialSize)
                {
                    jassertfalse;
                    return;
                }

                serial = juce::String ((const char*)message.getRawData() + offset, serialSize);
            }
        }

        int findMacAddressStart (const juce::MidiMessage& message)
        {
            const juce::uint8 macStart[] = { '4', '8', ':', 'B', '6', ':', '2', '0', ':' };
            return findSequence (macStart, sizeof (macStart), message);
        }

        int findSequence (const juce::uint8* sequence, int sequenceSize, const juce::MidiMessage& message)
        {
            for (int i = 0; i < message.getRawDataSize() - sequenceSize; i++)
            {
                if (memcmp (message.getRawData() + i, sequence, size_t (sequenceSize)) == 0)
                    return i;
            }

            return -1;
        }

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BlockSerialReader)
    };
}
