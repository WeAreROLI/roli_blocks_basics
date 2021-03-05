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

#pragma once

namespace roli::BlocksProtocol
{
enum BlockConfigId
{
    // MIDI
    midiStartChannel    = 0,
    midiEndChannel      = 1,
    midiUseMPE          = 2,
    pitchBendRange      = 3,
    octave              = 4,
    transpose           = 5,
    slideCC             = 6,
    slideMode           = 7,
    octaveTopology      = 8,
    midiChannelRange    = 9,
    MPEZone             = 40,
    // Touch
    velocitySensitivity = 10,
    glideSensitivity    = 11,
    slideSensitivity    = 12,
    pressureSensitivity = 13,
    liftSensitivity     = 14,
    fixedVelocity       = 15,
    fixedVelocityValue  = 16,
    pianoMode           = 17,
    glideLock           = 18,
    glideLockEnable     = 19,
    // Live
    mode                = 20,
    volume              = 21,
    scale               = 22,
    hideMode            = 23,
    chord               = 24,
    arpPattern          = 25,
    tempo               = 26,
    key                 = 27,
    autoTransposeToKey  = 28,
    // Tracking
    xTrackingMode       = 30,
    yTrackingMode       = 31,
    zTrackingMode       = 32,
    // Graphics
    gammaCorrection     = 33,
    globalKeyColour     = 34,
    rootKeyColour       = 35,
    brightness          = 36,
    // User
    user0               = 64,
    user1               = 65,
    user2               = 66,
    user3               = 67,
    user4               = 68,
    user5               = 69,
    user6               = 70,
    user7               = 71,
    user8               = 72,
    user9               = 73,
    user10              = 74,
    user11              = 75,
    user12              = 76,
    user13              = 77,
    user14              = 78,
    user15              = 79,
    user16              = 80,
    user17              = 81,
    user18              = 82,
    user19              = 83,
    user20              = 84,
    user21              = 85,
    user22              = 86,
    user23              = 87,
    user24              = 88,
    user25              = 89,
    user26              = 90,
    user27              = 91,
    user28              = 92,
    user29              = 93,
    user30              = 94,
    user31              = 95,
    // Mode Config 0
    mode0_colorType     = 100,
    mode0_midiMode      = 101,
    mode0_pitchEnable   = 102,
    mode0_pressureEnable = 103,
    mode0_showScale     = 104,
    mode0_globalColour  = 105,
    mode0_rootColour    = 106,
    // Mode Config 1
    mode1_colorType     = 107,
    mode1_midiMode      = 108,
    mode1_pitchEnable   = 109,
    mode1_pressureEnable = 110,
    mode1_showScale     = 111,
    mode1_globalColour  = 112,
    mode1_rootColour    = 113,
    // Mode Config 2
    mode2_colorType     = 114,
    mode2_midiMode      = 115,
    mode2_pitchEnable   = 116,
    mode2_pressureEnable = 117,
    mode2_showScale     = 118,
    mode2_globalColour  = 119,
    mode2_rootColour    = 120,
    // Mode Config 3
    mode3_colorType     = 121,
    mode3_midiMode      = 122,
    mode3_pitchEnable   = 123,
    mode3_pressureEnable = 124,
    mode3_showScale     = 125,
    mode3_globalColour  = 126,
    mode3_rootColour    = 127,
    numConfigItems
};
} // namespace juce
