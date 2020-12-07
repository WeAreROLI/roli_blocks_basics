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

struct DeviceInfo
{
    // VS2015 requires a constructor to avoid aggregate initialization
    DeviceInfo (Block::UID buid, BlocksProtocol::TopologyIndex tidx,
                BlocksProtocol::BlockSerialNumber s, BlocksProtocol::VersionNumber v,
                BlocksProtocol::BlockName n, BlocksProtocol::BatteryLevel level,
                BlocksProtocol::BatteryCharging charging, Block::UID master)
        : uid (buid), index (tidx), serial (s), version (v), name (n),
          batteryLevel (level), batteryCharging (charging), masterUid (master),
          isMaster (uid == master)
    {
    }

    Block::UID uid {};
    BlocksProtocol::TopologyIndex index;
    BlocksProtocol::BlockSerialNumber serial;
    BlocksProtocol::VersionNumber version;
    BlocksProtocol::BlockName name;
    BlocksProtocol::BatteryLevel batteryLevel;
    BlocksProtocol::BatteryCharging batteryCharging;
    Block::UID masterUid;
    bool isMaster {};
};

} // namespace roli
