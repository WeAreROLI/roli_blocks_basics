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
namespace BlocksProtocol
{

/**
    Parses data packets from a BLOCKS device, and translates them into callbacks
    on a handler object

    @tags{Blocks}
*/
template <typename Handler>
struct HostPacketDecoder
{
    static void processNextPacket (Handler& handler, TopologyIndex deviceIndex, const void* data, int size)
    {
        if (Packed7BitArrayReader::checksumIsOK (static_cast<const juce::uint8*> (data), (juce::uint32) size))
        {
            Packed7BitArrayReader reader (data, size - 1);

            if (reader.getRemainingBits() < (int) PacketTimestamp::bits)
            {
                jassertfalse; // not a valid message..
                return;
            }

            auto packetTimestamp = reader.read<PacketTimestamp>();
            deviceIndex &= 63; // top bit is used as a direction indicator

            for (;;)
            {
                auto nextMessageType = getMessageType (reader);

                if (nextMessageType == 0)
                    break;

                if (! processNextMessage (handler, reader, (MessageFromDevice) nextMessageType, deviceIndex, packetTimestamp))
                    break;
            }
        }
    }

    static juce::uint32 getMessageType (Packed7BitArrayReader& reader)
    {
        if (reader.getRemainingBits() < MessageType::bits)
            return 0;

        return reader.read<MessageType>().get();
    }

    static bool processNextMessage (Handler& handler, Packed7BitArrayReader& reader,
                                    MessageFromDevice messageType, TopologyIndex deviceIndex,
                                    PacketTimestamp packetTimestamp)
    {
        switch (messageType)
        {
            case MessageFromDevice::deviceTopology:           return handleTopology (handler, reader, true);
            case MessageFromDevice::deviceTopologyExtend:     return handleTopology (handler, reader, false);
            case MessageFromDevice::deviceTopologyEnd:        return handleTopologyEnd (handler, reader);
            case MessageFromDevice::deviceVersion:            return handleVersion (handler, reader);
            case MessageFromDevice::deviceName:               return handleName (handler, reader);
            case MessageFromDevice::touchStart:               return handleTouch (handler, reader, deviceIndex, packetTimestamp, true, false);
            case MessageFromDevice::touchMove:                return handleTouch (handler, reader, deviceIndex, packetTimestamp, false, false);
            case MessageFromDevice::touchEnd:                 return handleTouch (handler, reader, deviceIndex, packetTimestamp, false, true);
            case MessageFromDevice::touchStartWithVelocity:   return handleTouchWithVelocity (handler, reader, deviceIndex, packetTimestamp, true, false);
            case MessageFromDevice::touchMoveWithVelocity:    return handleTouchWithVelocity (handler, reader, deviceIndex, packetTimestamp, false, false);
            case MessageFromDevice::touchEndWithVelocity:     return handleTouchWithVelocity (handler, reader, deviceIndex, packetTimestamp, false, true);
            case MessageFromDevice::controlButtonDown:        return handleButtonDownOrUp (handler, reader, deviceIndex, packetTimestamp, true);
            case MessageFromDevice::controlButtonUp:          return handleButtonDownOrUp (handler, reader, deviceIndex, packetTimestamp, false);
            case MessageFromDevice::programEventMessage:      return handleCustomMessage (handler, reader, deviceIndex, packetTimestamp);
            case MessageFromDevice::packetACK:                return handlePacketACK (handler, reader, deviceIndex);
            case MessageFromDevice::firmwareUpdateACK:        return handleFirmwareUpdateACK (handler, reader, deviceIndex);
            case MessageFromDevice::configMessage:            return handleConfigMessage (handler, reader, deviceIndex);
            case MessageFromDevice::logMessage:               return handleLogMessage (handler, reader, deviceIndex);

            default:
                jassertfalse; // got an invalid message type, could be a corrupt packet, or a
                              // message type that the host doesn't expect to get
                return false;
        }
    }

    static bool handleTopology (Handler& handler, Packed7BitArrayReader& reader, bool newTopology)
    {
        if (reader.getRemainingBits() < (int) DeviceCount::bits + (int) ConnectionCount::bits)
        {
            jassertfalse; // not enough data available for this message type!
            return false;
        }

        auto deviceProtocolVersion = reader.read<ProtocolVersion>();

        if (deviceProtocolVersion > currentProtocolVersion)
        {
            jassertfalse;
            return false;
        }

        const juce::uint32 numDevices     = reader.read<DeviceCount>();
        const juce::uint32 numConnections = reader.read<ConnectionCount>();

        if ((juce::uint32) reader.getRemainingBits() < numDevices * BitSizes::topologyDeviceInfo
                                                       + numConnections * BitSizes::topologyConnectionInfo)
        {
            jassertfalse; // not enough data available for this message type!
            return false;
        }

        if (newTopology)
            handler.beginTopology ((int) numDevices, (int) numConnections);
        else
            handler.extendTopology ((int) numDevices, (int) numConnections);

        for (juce::uint32 i = 0; i < numDevices; ++i)
            handleTopologyDevice (handler, reader);

        for (juce::uint32 i = 0; i < numConnections; ++i)
            handleTopologyConnection (handler, reader);

        // Packet must be last in topology, otherwise wait for topology end message
        if (numDevices < maxBlocksInTopologyPacket && numConnections < maxConnectionsInTopologyPacket)
            handler.endTopology();

        return true;
    }

    static bool handleTopologyEnd (Handler& handler, Packed7BitArrayReader& reader)
    {
        auto deviceProtocolVersion = reader.read<ProtocolVersion>();

        if (deviceProtocolVersion > currentProtocolVersion)
        {
            jassertfalse;
            return false;
        }

        handler.endTopology();
        return true;
    }

    static void handleTopologyDevice (Handler& handler, Packed7BitArrayReader& reader)
    {
        DeviceStatus status;

        for (juce::uint32 i = 0; i < BlockSerialNumber::maxLength; ++i)
        {
            status.serialNumber.data[i] = (juce::uint8) reader.readBits (7);
            ++status.serialNumber.length;
        }

        status.index            = (TopologyIndex) reader.readBits (topologyIndexBits);
        status.batteryLevel     = reader.read<BatteryLevel>();
        status.batteryCharging  = reader.read<BatteryCharging>();

        handler.handleTopologyDevice (status);
    }

    static void handleTopologyConnection (Handler& handler, Packed7BitArrayReader& reader)
    {
        DeviceConnection connection;

        connection.device1 = (juce::uint8) reader.readBits (topologyIndexBits);
        connection.port1   = reader.read<ConnectorPort>();
        connection.device2 = (juce::uint8) reader.readBits (topologyIndexBits);
        connection.port2   = reader.read<ConnectorPort>();

        handler.handleTopologyConnection (connection);
    }

    static bool handleVersion (Handler& handler, Packed7BitArrayReader& reader)
    {
        DeviceVersion version;

        version.index = (TopologyIndex) reader.readBits (topologyIndexBits);
        version.version.length = (juce::uint8) reader.readBits (7);

        for (juce::uint32 i = 0; i < version.version.length; ++i)
            version.version.data[i] = (juce::uint8) reader.readBits (7);

        handler.handleVersion (version);
        return true;
    }

    static bool handleName (Handler& handler, Packed7BitArrayReader& reader)
    {
        DeviceName name;

        name.index = (TopologyIndex) reader.readBits (topologyIndexBits);
        name.name.length = (juce::uint8) reader.readBits (7);

        for (juce::uint32 i = 0; i < name.name.length; ++i)
            name.name.data[i] = (juce::uint8) reader.readBits (7);

        handler.handleName (name);
        return true;
    }

    static bool handleTouch (Handler& handler, Packed7BitArrayReader& reader, TopologyIndex deviceIndex,
                             PacketTimestamp packetTimestamp, bool isStart, bool isEnd)
    {
        if (reader.getRemainingBits() < (int) BitSizes::touchMessage - (int) MessageType::bits)
        {
            jassertfalse; // not enough data available for this message type!
            return false;
        }

        auto timeOffset  = reader.read<PacketTimestampOffset>();
        auto touchIndex  = reader.read<TouchIndex>();
        auto x           = reader.read<TouchPosition::Xcoord>();
        auto y           = reader.read<TouchPosition::Ycoord>();
        auto z           = reader.read<TouchPosition::Zcoord>();

        handleTouch (handler, deviceIndex, packetTimestamp.get() + timeOffset.get(),
                     touchIndex, { x, y, z }, { 0, 0, 0 }, isStart, isEnd);
        return true;
    }

    static bool handleTouchWithVelocity (Handler& handler, Packed7BitArrayReader& reader, TopologyIndex deviceIndex,
                                         PacketTimestamp packetTimestamp, bool isStart, bool isEnd)
    {
        if (reader.getRemainingBits() < (int) BitSizes::touchMessageWithVelocity - (int) MessageType::bits)
        {
            jassertfalse; // not enough data available for this message type!
            return false;
        }

        auto timeOffset  = reader.read<PacketTimestampOffset>();
        auto touchIndex  = reader.read<TouchIndex>();
        auto x           = reader.read<TouchPosition::Xcoord>();
        auto y           = reader.read<TouchPosition::Ycoord>();
        auto z           = reader.read<TouchPosition::Zcoord>();
        auto vx          = reader.read<TouchVelocity::VXcoord>();
        auto vy          = reader.read<TouchVelocity::VYcoord>();
        auto vz          = reader.read<TouchVelocity::VZcoord>();

        handleTouch (handler, deviceIndex, packetTimestamp.get() + timeOffset.get(),
                     touchIndex, { x, y, z }, { vx, vy, vz }, isStart, isEnd);
        return true;
    }

    static void handleTouch (Handler& handler, TopologyIndex deviceIndex, juce::uint32 timestamp, TouchIndex touchIndex,
                             TouchPosition position, TouchVelocity velocity, bool isStart, bool isEnd)
    {
        handler.handleTouchChange (deviceIndex, timestamp, touchIndex, position, velocity, isStart, isEnd);
    }

    static bool handleButtonDownOrUp (Handler& handler, Packed7BitArrayReader& reader, TopologyIndex deviceIndex,
                                      PacketTimestamp packetTimestamp, bool isDown)
    {
        if (reader.getRemainingBits() < (int) BitSizes::controlButtonMessage - (int) MessageType::bits)
        {
            jassertfalse; // not enough data available for this message type!
            return false;
        }

        auto timeOffset  = reader.read<PacketTimestampOffset>();
        auto buttonID    = reader.read<ControlButtonID>();

        handler.handleControlButtonUpDown (deviceIndex, packetTimestamp.get() + timeOffset.get(), buttonID, isDown);
        return true;
    }

    static bool handleCustomMessage (Handler& handler, Packed7BitArrayReader& reader,
                                     TopologyIndex deviceIndex, PacketTimestamp packetTimestamp)
    {
        if (reader.getRemainingBits() < BitSizes::programEventMessage - (int) MessageType::bits)
        {
            jassertfalse; // not enough data available for this message type!
            return false;
        }

        juce::int32 data[numProgramMessageInts] = {};

        for (juce::uint32 i = 0; i < numProgramMessageInts; ++i)
            data[i] = (juce::int32) reader.read<IntegerWithBitSize<32>>().get();

        handler.handleCustomMessage (deviceIndex, packetTimestamp.get(), data);
        return true;
    }

    static bool handlePacketACK (Handler& handler, Packed7BitArrayReader& reader, TopologyIndex deviceIndex)
    {
        if (reader.getRemainingBits() < BitSizes::packetACK - (int) MessageType::bits)
        {
            jassertfalse; // not enough data available for this message type!
            return false;
        }

        handler.handlePacketACK (deviceIndex, reader.read<PacketCounter>());
        return true;
    }

    static bool handleFirmwareUpdateACK (Handler& handler, Packed7BitArrayReader& reader, TopologyIndex deviceIndex)
    {
        if (reader.getRemainingBits() < FirmwareUpdateACKCode::bits)
        {
            jassertfalse; // not enough data available for this message type!
            return false;
        }

        auto ackCode   = reader.read<FirmwareUpdateACKCode>();
        auto ackDetail = reader.read<FirmwareUpdateACKDetail>();

        handler.handleFirmwareUpdateACK (deviceIndex, ackCode, ackDetail);
        return true;
    }

    static bool handleConfigMessage (Handler& handler, Packed7BitArrayReader& reader, TopologyIndex deviceIndex)
    {
        ConfigCommand type = reader.read<ConfigCommand>().get();

        if (type == updateConfig)
        {
            auto item  = (juce::int32) reader.read<IntegerWithBitSize<8>>().get();
            auto value = (juce::int32) reader.read<IntegerWithBitSize<32>>().get();
            auto min   = (juce::int32) reader.read<IntegerWithBitSize<32>>().get();
            auto max   = (juce::int32) reader.read<IntegerWithBitSize<32>>().get();

            handler.handleConfigUpdateMessage (deviceIndex, item, value, min, max);
            return true;
        }

        if (type == setConfig)
        {
            auto item  = (juce::int32) reader.read<IntegerWithBitSize<8>>().get();
            auto value = (juce::int32) reader.read<IntegerWithBitSize<32>>().get();

            handler.handleConfigSetMessage (deviceIndex, item, value);
            return true;
        }

        if (type == factorySyncEnd)
        {
            handler.handleConfigFactorySyncEndMessage (deviceIndex);
        }

        if (type == factorySyncReset)
        {
            handler.handleConfigFactorySyncResetMessage (deviceIndex);
        }

        return true;
    }

    static bool handleLogMessage (Handler& handler, Packed7BitArrayReader& reader, TopologyIndex deviceIndex)
    {
        juce::String message;

        while (reader.getRemainingBits() >= 7)
        {
            juce::uint32 c = reader.read<IntegerWithBitSize<7>>();
            message << (char) c;
        }

        handler.handleLogMessage (deviceIndex, message);
        return true;
    }
};

} // namespace BlocksProtocol
} // namespace roli
