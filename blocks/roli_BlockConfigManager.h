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

// This file provides interfaces for managing the internal configuration of Blocks
// and synchronises with the connected Block

using namespace BlocksProtocol;

using ConfigType = Block::ConfigMetaData::ConfigType;

/** Manages the configuration of blocks

    @tags{Blocks}
*/
struct BlockConfigManager
{
    /** Structure describing a configuration */
    struct ConfigDescription
    {
        juce::int32 id;
        juce::int32 value;
        juce::int32 min;
        juce::int32 max;
        bool isActive;
        const char* name;
        ConfigType type;
        const char* optionNames[configMaxOptions];
        const char* group;

        static_assert (configMaxOptions == Block::ConfigMetaData::numOptionNames, "Config options size and config metadata size should be the same");

        Block::ConfigMetaData toConfigMetaData() const
        {
            return Block::ConfigMetaData ((juce::uint32) id, value, { min, max }, isActive, name, type, (const char**) optionNames, group);
        }
    };

    BlockConfigManager (juce::Array<ConfigDescription> defaultConfigOverrides)
    {
        for (auto config : defaultConfig)
            configItems [config.id] = config;

        for (auto config : defaultConfigOverrides)
            configItems [config.id] = config;
    }

    void setDeviceIndex (TopologyIndex newDeviceIndex)                       { deviceIndex = newDeviceIndex; }
    void setDeviceComms (PhysicalTopologySource::DeviceConnection* newConn)  { deviceConnection = newConn; }

    static constexpr juce::uint32 numConfigItems = 96;

    static constexpr const char* midiSettingsGroup = "MIDI Settings";
    static constexpr const char* pitchGroup = "Pitch";
    static constexpr const char* playGroup = "Play mode";
    static constexpr const char* sensitivityGroup = "Sensitivity";
    static constexpr const char* rhythmGroup = "Rhythm";
    static constexpr const char* coloursGroup = "Colors";
    static constexpr const char* mode0Group = "Mode 1";
    static constexpr const char* mode1Group = "Mode 2";
    static constexpr const char* mode2Group = "Mode 3";
    static constexpr const char* mode3Group = "Mode 4";

    const ConfigDescription defaultConfig[numConfigItems] =
    {
        { midiStartChannel,     2,      1,      16,         false,  "MIDI Start Channel",   ConfigType::integer,    {},               midiSettingsGroup },
        { midiEndChannel,       16,     1,      16,         false,  "MIDI End Channel",     ConfigType::integer,    {},               midiSettingsGroup },
        { midiUseMPE,           1,      0,      2,          false,  "MIDI Mode",            ConfigType::options,    { "Multi Channel",
                                                                                                                  "MPE",
                                                                                                                  "Single Channel" }, midiSettingsGroup },
        { pitchBendRange,       48,     1,      96,         false,  "Pitch Bend Range",     ConfigType::integer,    {},               midiSettingsGroup },
        { midiChannelRange,     15,     1,      15,         false,  "No. MIDI Channels",    ConfigType::integer,    {},               midiSettingsGroup },
        { MPEZone,              0,      0,      1,          false,  "MPE Zone",             ConfigType::options,    { "Lower Zone",
                                                                                                                  "Upper Zone"},      midiSettingsGroup },
        { octave,               0,      -4,     6,          false,  "Octave",               ConfigType::integer,    {},               pitchGroup },
        { transpose,            0,      -11,    11,         false,  "Transpose",            ConfigType::integer,    {},               pitchGroup },
        { slideCC,              74,     0,      127,        false,  "Slide CC",             ConfigType::integer,    {},               playGroup },
        { slideMode,            0,      0,      2,          false,  "Slide Mode",           ConfigType::options,    { "Absolute",
                                                                                                                  "Relative Unipolar",
                                                                                                                  "Relative Bipolar" }, playGroup },
        { velocitySensitivity,  100,    0,      127,        false,  "Strike Sensitivity",   ConfigType::integer,    {},               sensitivityGroup },
        { glideSensitivity,     100,    0,      127,        false,  "Glide Sensitivity",    ConfigType::integer,    {},               sensitivityGroup },
        { slideSensitivity,     100,    0,      127,        false,  "Slide Sensitivity",    ConfigType::integer,    {},               sensitivityGroup },
        { pressureSensitivity,  100,    0,      127,        false,  "Pressure Sensitivity", ConfigType::integer,    {},               sensitivityGroup },
        { liftSensitivity,      100,    0,      127,        false,  "Lift Sensitivity",     ConfigType::integer,    {},               sensitivityGroup },
        { fixedVelocity,        0,      0,      1,          false,  "Fixed Velocity",       ConfigType::boolean,    {},               sensitivityGroup },
        { fixedVelocityValue,   127,    1,      127,        false,  "Fixed Velocity Value", ConfigType::integer,    {},               sensitivityGroup },
        { pianoMode,            0,      0,      1,          false,  "Piano Mode",           ConfigType::boolean,    {},               playGroup },
        { glideLock,            0,      0,      127,        false,  "Glide Rate",           ConfigType::integer,    {},               playGroup },
        { glideLockEnable,      0,      0,      1,          false,  "Glide Lock Enable",    ConfigType::boolean,    {},               playGroup },
        { mode,                 4,      1,      5,          false,  "Mode",                 ConfigType::integer,    {},               playGroup },
        { volume,               100,    0,      127,        false,  "Volume",               ConfigType::integer,    {},               playGroup },
        { scale,                0,      0,      18,         false,  "Scale",                ConfigType::integer,    {},               playGroup }, // NOTE: Should be options
        { hideMode,             0,      0,      1,          false,  "Hide Mode",            ConfigType::boolean,    {},               playGroup },
        { chord,                0,      0,      127,        false,  "Chord",                ConfigType::integer,    {},               playGroup }, // NOTE: Should be options
        { arpPattern,           0,      0,      127,        false,  "Arp Pattern",          ConfigType::integer,    {},               playGroup },
        { tempo,                120,    1,      300,        false,  "Tempo",                ConfigType::integer,    {},               rhythmGroup },
        { key,                  0,      0,      11,         false,  "Key",                  ConfigType::options,    { "C", "C#", "D", "D#",
                                                                                                                  "E", "F", "F#", "G",
                                                                                                                  "G#", "A", "A#", "B"}, playGroup },
        { autoTransposeToKey,   0,      0,      1,          false,  "Auto Transpose To Key",ConfigType::boolean,    {},               pitchGroup },
        { xTrackingMode,        1,      1,      4,          false,  "Glide Tracking",  ConfigType::options,    { "Multi-Channel",
                                                                                                                  "Last Played",
                                                                                                                  "Highest",
                                                                                                                  "Lowest",
                                                                                                                  "Disabled" },   playGroup },
        { yTrackingMode,        1,      1,      4,          false,  "Slide Tracking",  ConfigType::options,    { "Multi-Channel",
                                                                                                                  "Last Played",
                                                                                                                  "Highest",
                                                                                                                  "Lowest",
                                                                                                                  "Disabled" },   playGroup },
        { zTrackingMode,        1,      0,      4,          false,  "Pressure Tracking", ConfigType::options, { "Poly Aftertouch",
                                                                                                                  "Last Played",
                                                                                                                  "Highest",
                                                                                                                  "Lowest",
                                                                                                                  "Disabled",
                                                                                                                  "Hardest" },    playGroup },

        { gammaCorrection,      0,         0,         1,    false,  "Gamma Correction",     ConfigType::boolean,    {},             coloursGroup },
        { globalKeyColour, INT32_MIN, INT32_MIN, INT32_MAX, false,  "Global Key Color",     ConfigType::colour,     {},             coloursGroup },
        { rootKeyColour,   INT32_MIN, INT32_MIN, INT32_MAX, false,  "Root Key Color"  ,     ConfigType::colour,     {},             coloursGroup },
        { brightness,           100,    0,    100,          false,  "Brightness",           ConfigType::integer,    {},             coloursGroup },

        // These can be defined for unique usage for a given Littlefoot script
        { user0,                0,    0,      127,          false,  {},                     ConfigType::integer,    {},               {} },
        { user1,                0,    0,      127,          false,  {},                     ConfigType::integer,    {},               {} },
        { user2,                0,    0,      127,          false,  {},                     ConfigType::integer,    {},               {} },
        { user3,                0,    0,      127,          false,  {},                     ConfigType::integer,    {},               {} },
        { user4,                0,    0,      127,          false,  {},                     ConfigType::integer,    {},               {} },
        { user5,                0,    0,      127,          false,  {},                     ConfigType::integer,    {},               {} },
        { user6,                0,    0,      127,          false,  {},                     ConfigType::integer,    {},               {} },
        { user7,                0,    0,      127,          false,  {},                     ConfigType::integer,    {},               {} },
        { user8,                0,    0,      127,          false,  {},                     ConfigType::integer,    {},               {} },
        { user9,                0,    0,      127,          false,  {},                     ConfigType::integer,    {},               {} },
        { user10,               0,    0,      127,          false,  {},                     ConfigType::integer,    {},               {} },
        { user11,               0,    0,      127,          false,  {},                     ConfigType::integer,    {},               {} },
        { user12,               0,    0,      127,          false,  {},                     ConfigType::integer,    {},               {} },
        { user13,               0,    0,      127,          false,  {},                     ConfigType::integer,    {},               {} },
        { user14,               0,    0,      127,          false,  {},                     ConfigType::integer,    {},               {} },
        { user15,               0,    0,      127,          false,  {},                     ConfigType::integer,    {},               {} },
        { user16,               0,    0,      127,          false,  {},                     ConfigType::integer,    {},               {} },
        { user17,               0,    0,      127,          false,  {},                     ConfigType::integer,    {},               {} },
        { user18,               0,    0,      127,          false,  {},                     ConfigType::integer,    {},               {} },
        { user19,               0,    0,      127,          false,  {},                     ConfigType::integer,    {},               {} },
        { user20,               0,    0,      127,          false,  {},                     ConfigType::integer,    {},               {} },
        { user21,               0,    0,      127,          false,  {},                     ConfigType::integer,    {},               {} },
        { user22,               0,    0,      127,          false,  {},                     ConfigType::integer,    {},               {} },
        { user23,               0,    0,      127,          false,  {},                     ConfigType::integer,    {},               {} },
        { user24,               0,    0,      127,          false,  {},                     ConfigType::integer,    {},               {} },
        { user25,               0,    0,      127,          false,  {},                     ConfigType::integer,    {},               {} },
        { user26,               0,    0,      127,          false,  {},                     ConfigType::integer,    {},               {} },
        { user27,               0,    0,      127,          false,  {},                     ConfigType::integer,    {},               {} },
        { user28,               0,    0,      127,          false,  {},                     ConfigType::integer,    {},               {} },
        { user29,               0,    0,      127,          false,  {},                     ConfigType::integer,    {},               {} },
        { user30,               0,    0,      127,          false,  {},                     ConfigType::integer,    {},               {} },
        { user31,               0,    0,      127,          false,  {},                     ConfigType::integer,    {},               {} },
        { mode0_colorType,      0,    0,      4,            false,  "Color Scheme",         ConfigType::options,    {"Pro", "User", "Piano", "Stage",
                                                                                                                     "Rainbow"},      mode0Group },
        { mode0_midiMode,       0,    0,      1,            false,  "MIDI Settings",        ConfigType::options,    { "Single Channel",
                                                                                                                    "MPE" },          mode0Group },
        { mode0_pitchEnable,    0,    0,      1,            false,  "Enable Pitch Bend",    ConfigType::boolean,    {},               mode0Group },
        { mode0_pressureEnable, 0,    0,      1,            false,  "Enable Pressure",      ConfigType::boolean,    {},               mode0Group },
        { mode0_showScale,      0,    0,      1,            false,  "Show Scales",          ConfigType::boolean,    {},               mode0Group },
        { mode0_globalColour, INT32_MIN, INT32_MIN, INT32_MAX, false,  "Global Key Color",  ConfigType::colour,     {},               mode0Group },
        { mode0_rootColour,   INT32_MIN, INT32_MIN, INT32_MAX, false,  "Root Key Color",    ConfigType::colour,     {},               mode0Group },

        { mode1_colorType,      0,    0,      4,            false,  "Color Scheme",         ConfigType::options,    {"Pro", "User", "Piano", "Stage",
                                                                                                                     "Rainbow"},      mode1Group },
        { mode1_midiMode,       0,    0,      1,            false,  "MIDI Settings",        ConfigType::options,    { "Single Channel",
                                                                                                                    "MPE" },          mode1Group },
        { mode1_pitchEnable,    0,    0,      1,            false,  "Enable Pitch Bend",    ConfigType::boolean,    {},               mode1Group },
        { mode1_pressureEnable, 0,    0,      1,            false,  "Enable Pressure",      ConfigType::boolean,    {},               mode1Group },
        { mode1_showScale,      0,    0,      1,            false,  "Show Scales",          ConfigType::boolean,    {},               mode1Group },
        { mode1_globalColour, INT32_MIN, INT32_MIN, INT32_MAX, false,  "Global Key Color",  ConfigType::colour,     {},               mode1Group },
        { mode1_rootColour,   INT32_MIN, INT32_MIN, INT32_MAX, false,  "Root Key Color",    ConfigType::colour,     {},               mode1Group },

        { mode2_colorType,      0,    0,      4,            false,  "Color Scheme",         ConfigType::options,    {"Pro", "User", "Piano", "Stage",
                                                                                                                     "Rainbow"},      mode2Group },
        { mode2_midiMode,       0,    0,      1,            false,  "MIDI Settings",        ConfigType::options,    { "Single Channel",
                                                                                                                    "MPE" },          mode2Group },
        { mode2_pitchEnable,    0,    0,      1,            false,  "Enable Pitch Bend",    ConfigType::boolean,    {},               mode2Group },
        { mode2_pressureEnable, 0,    0,      1,            false,  "Enable Pressure",      ConfigType::boolean,    {},               mode2Group },
        { mode2_showScale,      0,    0,      1,            false,  "Show Scales",          ConfigType::boolean,    {},               mode2Group },
        { mode2_globalColour, INT32_MIN, INT32_MIN, INT32_MAX, false,  "Global Key Color",  ConfigType::colour,     {},               mode2Group },
        { mode2_rootColour,   INT32_MIN, INT32_MIN, INT32_MAX, false,  "Root Key Color",    ConfigType::colour,     {},               mode2Group },

        { mode3_colorType,      0,    0,      4,            false,  "Color Scheme",         ConfigType::options,    {"Pro", "User", "Piano", "Stage",
                                                                                                                     "Rainbow"},      mode3Group },
        { mode3_midiMode,       0,    0,      1,            false,  "MIDI Settings",        ConfigType::options,    { "Single Channel",
                                                                                                                    "MPE" },          mode3Group },
        { mode3_pitchEnable,    0,    0,      1,            false,  "Enable Pitch Bend",    ConfigType::boolean,    {},               mode3Group },
        { mode3_pressureEnable, 0,    0,      1,            false,  "Enable Pressure",      ConfigType::boolean,    {},               mode3Group },
        { mode3_showScale,      0,    0,      1,            false,  "Show Scales",          ConfigType::boolean,    {},               mode3Group },
        { mode3_globalColour, INT32_MIN, INT32_MIN, INT32_MAX, false,  "Global Key Color",  ConfigType::colour,     {},               mode3Group },
        { mode3_rootColour,   INT32_MIN, INT32_MIN, INT32_MAX, false,  "Root Key Color",    ConfigType::colour,     {},               mode3Group }
    };

    //==============================================================================
    juce::int32 getItemValue (BlockConfigId id)
    {
        if (auto* desc = getConfigDesc (id))
            return desc->value;

        return 0;
    }

    void setItemValue (BlockConfigId id, juce::int32 value)
    {
        if (auto* desc = getConfigDesc (id))
            desc->value = value;

        setBlockConfig (id, value);
    }

    juce::int32 getItemMin (BlockConfigId id)
    {
        if (auto* desc = getConfigDesc (id))
            return desc->min;

        return 0;
    }

    void setItemMin (BlockConfigId id, juce::int32 min)
    {
        if (auto* desc = getConfigDesc (id))
            desc->min = min;
    }

    juce::int32 getItemMax (BlockConfigId id)
    {
        if (auto* desc = getConfigDesc (id))
            return desc->max;

        return 0;
    }

    void setItemMax (BlockConfigId id, juce::int32 max)
    {
        if (auto* desc = getConfigDesc (id))
            desc->max = max;

        // Send updateConfig message to Block
    }

    bool getItemActive (BlockConfigId id)
    {
        if (auto* desc = getConfigDesc (id))
            return desc->isActive;

        return false;
    }

    void setItemActive (BlockConfigId id, bool isActive)
    {
        if (auto* desc = getConfigDesc (id))
            desc->isActive = isActive;

        // Send setConfigState message to Block
    }

    juce::String getOptionName (BlockConfigId id, juce::uint8 optionIndex)
    {
        if (auto* desc = getConfigDesc (id))
            return desc->optionNames[optionIndex];

        return {};
    }

    Block::ConfigMetaData getMetaData (BlockConfigId id)
    {
        if (auto* desc = getConfigDesc (id))
            return desc->toConfigMetaData();

        return { static_cast<juce::uint32> (id) };
    }

    void resetConfigListActiveStatus()
    {
        for (auto& [key, desc] : configItems)
            desc.isActive = false;
    }

    inline bool isUserConfigID (BlockConfigId id)
    {
        return id >= BlockConfigId::user0 && id <= BlockConfigId::user31;
    }

    //==============================================================================
    // Set Block Configuration
    void setBlockConfig (BlockConfigId id, juce::int32 value)
    {
        buildAndSendPacket ([id, value] (HostPacketBuilder<32>& p) { p.addConfigSetMessage (id, value); });
    }

    void requestBlockConfig (BlockConfigId id)
    {
        buildAndSendPacket ([id] (HostPacketBuilder<32>& p) { p.addRequestMessage (id); });
    }

    void requestFactoryConfigSync()
    {
        buildAndSendPacket ([] (HostPacketBuilder<32>& p) { p.addRequestFactorySyncMessage(); });
    }

    void requestUserConfigSync()
    {
        buildAndSendPacket ([] (HostPacketBuilder<32>& p) { p.addRequestUserSyncMessage(); });
    }

    void handleConfigUpdateMessage (juce::int32 id, juce::int32 value, juce::int32 min, juce::int32 max)
    {
        configItems[id].value = value;
        configItems[id].min = min;
        configItems[id].max = max;
        configItems[id].isActive = true;
    }

    void handleConfigSetMessage (juce::int32 id, juce::int32 value)
    {
        configItems[id].value = value;
    }

private:

    ConfigDescription* getConfigDesc (BlockConfigId id)
    {
        auto res = configItems.find (juce::int32 (id));

        return res != configItems.end() ? &res->second
                                        : nullptr;
    }

    template<typename PacketBuildFn>
    void buildAndSendPacket (PacketBuildFn buildFn)
    {
        if (deviceConnection == nullptr)
            return;

        HostPacketBuilder<32> packet;
        packet.writePacketSysexHeaderBytes (deviceIndex);
        buildFn (packet);
        packet.writePacketSysexFooter();
        deviceConnection->sendMessageToDevice (packet.getData(), (size_t) packet.size());
    }

    TopologyIndex deviceIndex {};
    PhysicalTopologySource::DeviceConnection* deviceConnection {};
    std::map <juce::int32, ConfigDescription> configItems;
};

} // namespace roli
