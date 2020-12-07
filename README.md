# ROLI BLOCKS API

This module has been moved from its previous home in the JUCE repository. To
update to the new module, you'll need to update your Projucer files to use
`roli_blocks_basics` instead of `juce_blocks_basics`. Include paths and module
dependencies must be updated in a similar way.

The code in this module has also moved out of the `juce` namespace and into the
`roli` namespace.

This is a JUCE module which discovers any connected BLOCKS devices and provides
low-level access to their physical properties.

Its job is to provide:
- topology and status information about the list of connected devices
- callbacks for touch events, control button presses and rotary dial movements
- simple control over individual LEDs
- optional rule-based virtual device aggregation functionality
