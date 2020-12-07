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

BitmapLEDProgram::BitmapLEDProgram (Block& b)  : Program (b) {}

/*
    The heap format for this program is just an array of 15x15 5:6:5 colours,
    and the program just copies them onto the screen each frame.
*/

void BitmapLEDProgram::setLED (juce::uint32 x, juce::uint32 y, LEDColour colour)
{
    if (auto ledGrid = block.getLEDGrid())
    {
        auto w = (juce::uint32) ledGrid->getNumColumns();
        auto h = (juce::uint32) ledGrid->getNumRows();

        if (x < w && y < h)
        {
            auto bit = (x + y * w) * 16;

            block.setDataBits (bit,      5, (juce::uint32) (colour.getRed()   >> 3));
            block.setDataBits (bit + 5,  6, (juce::uint32) (colour.getGreen() >> 2));
            block.setDataBits (bit + 11, 5, (juce::uint32) (colour.getBlue()  >> 3));
        }
    }
    else
    {
        jassertfalse;
    }
}

juce::String BitmapLEDProgram::getLittleFootProgram()
{
    juce::String program (R"littlefoot(

    #heapsize: 15 * 15 * 2

    void repaint()
    {
        for (int y = 0; y < NUM_ROWS; ++y)
        {
            for (int x = 0; x < NUM_COLUMNS; ++x)
            {
                int bit = (x + y * NUM_COLUMNS) * 16;

                fillPixel (makeARGB (255,
                                     getHeapBits (bit,      5) << 3,
                                     getHeapBits (bit + 5,  6) << 2,
                                     getHeapBits (bit + 11, 5) << 3), x, y);
            }
        }
    }

    )littlefoot");

    if (auto ledGrid = block.getLEDGrid())
        return program.replace ("NUM_COLUMNS", juce::String (ledGrid->getNumColumns()))
                      .replace ("NUM_ROWS",    juce::String (ledGrid->getNumRows()));

    jassertfalse;
    return {};
}

} // namespace roli
