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

static Block::UID getBlockUIDFromSerialNumber (const juce::uint8* serial) noexcept
{
    Block::UID n = {};

    for (int i = 0; i < int (BlocksProtocol::BlockSerialNumber::maxLength); ++i)
        n += n * 127 + serial[i];

    return n;
}

static Block::UID getBlockUIDFromSerialNumber (const BlocksProtocol::BlockSerialNumber& serial) noexcept
{
    return getBlockUIDFromSerialNumber (serial.data);
}

static Block::UID getBlockUIDFromSerialNumber (const juce::String& serial) noexcept
{
    if (serial.length() < int (BlocksProtocol::BlockSerialNumber::maxLength))
    {
        jassertfalse;
        return getBlockUIDFromSerialNumber (serial.paddedRight ('0', BlocksProtocol::BlockSerialNumber::maxLength));
    }

    return getBlockUIDFromSerialNumber ((const juce::uint8*) serial.toRawUTF8());
}

Block::Block (const juce::String& serial)
   : serialNumber (serial), uid (getBlockUIDFromSerialNumber (serial))
{
}

Block::Block (const juce::String& serial, const juce::String& version, const juce::String& blockName)
   : serialNumber (serial), versionNumber (version), name (blockName), uid (getBlockUIDFromSerialNumber (serial))
{
}

Block::~Block() {}

bool Block::isControlBlock() const
{
    return isControlBlock (getType());
}

bool Block::isControlBlock (Block::Type type)
{
    return type == Block::Type::liveBlock
        || type == Block::Type::loopBlock
        || type == Block::Type::touchBlock
        || type == Block::Type::developerControlBlock;
}

void Block::addProgramLoadedListener (ProgramLoadedListener* listener)      { programLoadedListeners.add (listener); }
void Block::removeProgramLoadedListener (ProgramLoadedListener* listener)   { programLoadedListeners.remove (listener); }

void Block::addDataInputPortListener (DataInputPortListener* listener)      { dataInputPortListeners.add (listener); }
void Block::removeDataInputPortListener (DataInputPortListener* listener)   { dataInputPortListeners.remove (listener); }

void Block::addConfigItemListener (ConfigItemListener* listener)            { configItemListeners.add (listener); }
void Block::removeConfigItemListener (ConfigItemListener* listener)         { configItemListeners.remove (listener); }

void Block::addProgramEventListener (ProgramEventListener* listener)        { programEventListeners.add (listener); }
void Block::removeProgramEventListener (ProgramEventListener* listener)     { programEventListeners.remove (listener); }


bool Block::ConnectionPort::operator== (const ConnectionPort& other) const noexcept { return edge == other.edge && index == other.index; }
bool Block::ConnectionPort::operator!= (const ConnectionPort& other) const noexcept { return ! operator== (other); }

Block::Program::Program (Block& b) : block (b) {}

//==============================================================================
TouchSurface::TouchSurface (Block& b) : block (b) {}
TouchSurface::~TouchSurface() {}

TouchSurface::Listener::~Listener() {}

void TouchSurface::addListener (Listener* l)            { listeners.add (l); }
void TouchSurface::removeListener (Listener* l)         { listeners.remove (l); }

//==============================================================================
ControlButton::ControlButton (Block& b) : block (b) {}
ControlButton::~ControlButton() {}

ControlButton::Listener::~Listener() {}

void ControlButton::addListener (Listener* l)           { listeners.add (l); }
void ControlButton::removeListener (Listener* l)        { listeners.remove (l); }


//==============================================================================
LEDGrid::LEDGrid (Block& b) : block (b) {}
LEDGrid::~LEDGrid() {}

LEDGrid::Renderer::~Renderer() {}

//==============================================================================
LEDRow::LEDRow (Block& b) : block (b) {}
LEDRow::~LEDRow() {}

//==============================================================================
StatusLight::StatusLight (Block& b) : block (b) {}
StatusLight::~StatusLight() {}

} // namespace roli
