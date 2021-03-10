/*
  ==============================================================================

   Copyright (c) 2021 - ROLI Ltd

   Permission to use, copy, modify, and/or distribute this software for any
   purpose with or without fee is hereby granted, provided that the above
   copyright notice and this permission notice appear in all copies.

   THE SOFTWARE IS PROVIDED "AS IS" AND ROLI LTD DISCLAIMS ALL WARRANTIES WITH
   REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
   AND FITNESS. IN NO EVENT SHALL ROLI LTD BE LIABLE FOR ANY SPECIAL, DIRECT,
   INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
   LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
   OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
   PERFORMANCE OF THIS SOFTWARE.

  ==============================================================================
*/

#include "../BlockFinder.h"

// A simple JUCE app containing our BlockFinder. This is a quick way of
// setting up an event loop so we can receive Block topology change events.
class MyJUCEApp  : public juce::JUCEApplicationBase
{
public:
    MyJUCEApp()  {}
    ~MyJUCEApp() {}

    void initialise (const juce::String&) override {}
    void shutdown() override                       {}

    const juce::String getApplicationName() override           { return "BlockFinder"; }
    const juce::String getApplicationVersion() override        { return "1.0.0"; }
    bool moreThanOneInstanceAllowed() override                 { return true; }
    void anotherInstanceStarted (const juce::String&) override {}

    void suspended() override           {}
    void resumed() override             {}
    void systemRequestedQuit() override {}
    void unhandledException(const std::exception*, const juce::String&,
                            int lineNumber) override {}

private:
    // Our BLOCKS class.
    BlockFinder finder;
};

START_JUCE_APPLICATION (MyJUCEApp)
