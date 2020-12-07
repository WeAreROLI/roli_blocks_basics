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

#include "roli_blocks_basics.h"

#include <regex>

namespace roli
{
 #include "littlefoot/roli_LittleFootRemoteHeap.h"
}

#include "protocol/roli_BitPackingUtilities.h"
#include "protocol/roli_BlocksProtocolDefinitions.h"
#include "protocol/roli_HostPacketDecoder.h"
#include "protocol/roli_HostPacketBuilder.h"
#include "blocks/roli_BlockConfigManager.h"
#include "protocol/roli_BlockModels.h"
#include "blocks/roli_Block.cpp"
#include "blocks/roli_BlocksVersion.cpp"
#include "topology/roli_BlockGraph.cpp"
#include "topology/roli_PhysicalTopologySource.cpp"
#include "topology/roli_RuleBasedTopologySource.cpp"
#include "visualisers/roli_DrumPadLEDProgram.cpp"
#include "visualisers/roli_BitmapLEDProgram.cpp"
