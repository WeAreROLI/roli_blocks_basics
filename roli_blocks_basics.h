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

/*******************************************************************************
 The block below describes the properties of this module, and is read by
 the Projucer to automatically generate project code that uses it.
 For details about the syntax and how to create or use a module, see the
 JUCE Module Format.md file.


 BEGIN_JUCE_MODULE_DECLARATION

  ID:                 roli_blocks_basics
  vendor:             ROLI Ltd
  version:            6.0.1
  name:               Provides low-level control over ROLI BLOCKS devices
  description:        JUCE wrapper for low-level control over ROLI BLOCKS devices.
  website:            http://developer.roli.com
  license:            ISC
  minimumCppStandard: 14

  dependencies:       juce_audio_devices

 END_JUCE_MODULE_DECLARATION

*******************************************************************************/


#pragma once

//==============================================================================
#include <juce_events/juce_events.h>
#include <juce_audio_devices/juce_audio_devices.h>

namespace roli
{
    class TouchSurface;
    class LEDGrid;
    class LEDRow;
    class StatusLight;
    class LightRing;
    class ControlButton;
}

#include "blocks/roli_Block.h"
#include "blocks/roli_TouchSurface.h"
#include "blocks/roli_LEDGrid.h"
#include "blocks/roli_LEDRow.h"
#include "blocks/roli_ControlButton.h"
#include "blocks/roli_TouchList.h"
#include "blocks/roli_StatusLight.h"
#include "blocks/roli_BlocksVersion.h"
#include "topology/roli_Topology.h"
#include "topology/roli_BlockGraph.h"
#include "topology/roli_TopologySource.h"
#include "topology/roli_PhysicalTopologySource.h"
#include "topology/roli_RuleBasedTopologySource.h"
#include "visualisers/roli_DrumPadLEDProgram.h"
#include "visualisers/roli_BitmapLEDProgram.h"

namespace roli
{
 #include "littlefoot/roli_LittleFootRunner.h"
 #include "littlefoot/roli_LittleFootCompiler.h"
}
