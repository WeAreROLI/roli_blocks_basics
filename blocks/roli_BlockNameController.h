/*
  ==============================================================================

    Copyright 2017 Luminary ROLI Ltd.

    Provides a bunch of functions for renaming a block.
    Block names are a slightly fragile thing. The API relies on a specific string appearing in their name
    This class provides functions to let you change the name, whilst ensuring that you comply with these rules

  ==============================================================================
*/

#pragma once

namespace roli
{
struct BlockNameController
{
    /**
     * Get the characters that are allowed in a Block name
     * @retval const char* The chars allowed
     */
    static constexpr const char* getCharactersAllowedInBlockNames()
    {
        return "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 #$(),.-_";
    }

    /**
     * Get the max length of a block name
     * @retval int the max length of the block name
     */
    static constexpr int getMaxBlockNameLength()
    {
        return 30;
    }

    /**
     * Get the max length of name that a user can input
     * @retval int the max length of name that a user can input
     */
    static constexpr int getMaxNumberOfCharsUserCanEnterForBlockName()
    {
        return getMaxBlockNameLength() - 7;      // 25 + ' Block\0' = getMaxBlockNameLength
    }

     /**
     * Check if this is a valid Blocks API device name
     * @retval const char* The chars allowed
     */
    static bool isBlocksMidiDeviceName (const juce::String& name)
    {
        return name.indexOf (" BLOCK") > 0 || name.indexOf (" Block") > 0 
            || name.containsIgnoreCase ("roli piano") || name.containsIgnoreCase ("airwave pedal");
    }

    /**
     * Set the name of the device
     * @param block The block to check the firmware version of. Must not be nullptr
     * @param name The new name. Pass an empty string to reset the blocks name back to its default
     * @note This function will block, and must be called on the mssage thread.
     * @retval bool True if we set the name properly, false if there was na error
     */
    static bool setBlockName (const Block::Ptr& block, juce::String name, const bool powerCycleNow)
    {
        if (block == nullptr)
            return false;

        if (! doesNameConformToAPIRulesOfBlockNaming (name))        // If not, please call the provided functions first...
            return false;

        block->setName (name);

        if (powerCycleNow)
            block->blockReset();

        return true;
    }

    /**
     * Convert a user input name to a properly constructed name
     * @param name The name / string the user entered
     * @param block The block we are targetting
     * @retval String The final safe name to pass to the naming function
     */
    static juce::String convertStringToBlockName (const Block::Ptr& block, const juce::String& name)
    {
        return name.isEmpty() ? getFactoryNameForBlock (block) : makeNameSafeForUseAsBlockName (name, block->getType());
    }

    /**
     * Determine if the specified name conforms to the API rules for a block name
     * @param name The name specified
     * @retval bool True if it conforms, false otherwise
     */
    static bool doesNameConformToAPIRulesOfBlockNaming (const juce::String& name)
    {
        return name.length() < getMaxBlockNameLength() && isBlocksMidiDeviceName (name);
    }

    /**
     * Given a block type, get us a string version of it
     * @param type The type of the block
     * @retval String The default name (essentially, what the block comes from the factory with
     */
    static juce::String getBlockTypeAsString (const Block::Type type)
    {
        switch (type)
        {
            case Block::Type::lightPadBlock:         return lightpadName;
            case Block::Type::seaboardBlock:         return seaboardName;
            case Block::Type::liveBlock:             return liveName;
            case Block::Type::loopBlock:             return loopName;
            case Block::Type::touchBlock:            return touchName;
            case Block::Type::developerControlBlock: return devConName;
            case Block::Type::lumiKeysBlock:         return roliPianoMName;
            case Block::Type::roliPiano49:           return roliPiano49Name;
            case Block::Type::airwave:               return airwaveName;
          #ifndef IGNORE_NEW_BLOCKS
            case Block::Type::lightpadProBlock:      return lightpadProName;
          #endif
            case Block::Type::unknown:               return {};
        }

        jassertfalse;
        return {};
    }

    static Block::Type getBlockTypeFromString (const juce::String& value)
    {
        if (value.containsIgnoreCase (lightpadName))
            return Block::Type::lightPadBlock;

        if (value.containsIgnoreCase (seaboardName))
            return Block::Type::seaboardBlock;

        if (value.containsIgnoreCase (liveName))
            return Block::Type::liveBlock;

        if (value.containsIgnoreCase (loopName))
            return Block::Type::loopBlock;

        if (value.containsIgnoreCase (touchName))
            return Block::Type::touchBlock;

        if (value.containsIgnoreCase (devConName))
            return Block::Type::developerControlBlock;

        if (value.containsIgnoreCase (roliPiano49Name))
            return Block::Type::roliPiano49;

        if (value.containsIgnoreCase (lumiKeysName) || value.containsIgnoreCase (roliPianoMName))
            return Block::Type::lumiKeysBlock;

        if (value.containsIgnoreCase (airwaveName))
            return Block::Type::airwave;
      #ifndef IGNORE_NEW_BLOCKS
        if (value.containsIgnoreCase (lightpadProName))
            return Block::Type::lightpadProBlock;
      #endif
        return Block::Type::unknown;
    }

    /**
      * Get original Block type as a string
      */
    static juce::String getLegacyBlockTypeAsString (const Block::Type type)
    {
        if (type == Block::Type::lumiKeysBlock)
            return lumiKeysName;

        return getBlockTypeAsString (type);
    }

    /**
     * Given a block, give us a description of it
     * @param block The block to get the description of
     * @retval String The default name (essentially, what the block comes from the factory with
     */
    static juce::String getDescriptionOfBlock (const Block::Ptr& block)
    {
        return block == nullptr ? "" : getDescriptionOfBlock (*block);
    }

    /**
     * Given a block, give us a description of it
     * @param block The block to get the description of
     * @retval String The default name (essentially, what the block comes from the factory with
     */
    static juce::String getDescriptionOfBlock (const Block& block)
    {
        const auto type = block.getType();

        auto desc = getBlockTypeAsString (type);

        if (type != Block::Type::airwave && type != Block::Type::roliPiano49)
            desc += " Block";

        if (block.modelVariant.isNotEmpty())
            desc += " " + block.modelVariant;

        return desc;
    }

    /**
     * Get the identifier of a block (normally the last 4 digits of the serial code
     * @param block The block to check
     * @retval juce::String A valid identifier for this block. Dont rely on this for anything specific, its information only...
     */
    static juce::String getSerialIdentifier (const Block::Ptr& block)
    {
        return getSerialIdentifier (*block);
    }

    static juce::String getSerialIdentifier (const Block& block)
    {
        return block.serialNumber.getLastCharacters (4);
    }

    /**
     * Get a default name for a block
     * @param block The block to get the default name for
     * @retval String The default name (essentially, what the block comes from the factory with)
     */
    static juce::String getFactoryNameForBlock (const Block::Ptr& block)
    {
        return block == nullptr ? "" : getFactoryNameForBlock (*block);
    }

    /**
     * Get a default name for a block
     * @param block The block to get the default name for
     * @retval String The default name (essentially, what the block comes from the factory with)
     */
    static juce::String getFactoryNameForBlock (const Block& block)
    {
        return getDescriptionOfBlock (block) + " " + getSerialIdentifier (block);
    }

    /**
     * Get the block name
     * @param block The block to get the default name for
     * @retval String The name of the block (either default or valid)
     */
    static juce::String getNameOfBlock (const Block::Ptr& block)
    {
        return block == nullptr ? "" : getNameOfBlock (*block);
    }

     /**
     * Get the block name
     * @param block The block to get the default name for
     * @retval String The name of the block (either default or valid)
     */
    static juce::String getNameOfBlock (const Block& block)
    {
        return block.name.isEmpty() ? getFactoryNameForBlock (block) : block.name;
    }

    static juce::String getDefaultNameSuffix (Block::Type type)
    {
        if (type == Block::Type::airwave) return " ROLI Airwave";
        if (type == Block::Type::roliPiano49) return " ROLI Piano";
        return " Block";
    }

    /**
     * Make sure that the given name is safe as a name
     * @param name The name the user has specified for use on bluetooth
     * @retval String the sanitised name safe to pass to the target
     */
    static juce::String makeNameSafeForUseAsBlockName (const juce::String& name, Block::Type type)
    {
        if (doesNameConformToAPIRulesOfBlockNaming (name))
            return name;

        // They typed in 'block' but without the right capitalisation, lets try to correct the spelling so that it works...
        int blockNameIndex  = name.indexOfIgnoreCase (" block");
        if (blockNameIndex > 0)
        {
            auto newName = name.replace (" block", " Block", true);
            if (doesNameConformToAPIRulesOfBlockNaming (newName))
                return newName;
        }

        // This is not very subtle - we could have a much better renaming scheme, which will let the users
        // enter something too long and which we cut down in a sensible way...
        return name.substring (0, getMaxNumberOfCharsUserCanEnterForBlockName()) + getDefaultNameSuffix (type);
    }

    /**
     * Get the identifying name of a block in a firmware file
     * @param block The block to check
     * @retval juce::String The firmware file identifier name
     */
    static juce::String getFirmwareFileIdentifierForBlock (const Block::Ptr& block)
    {
        switch (block->getType())
        {
            case Block::Type::liveBlock:
            case Block::Type::loopBlock:
            case Block::Type::touchBlock:
            case Block::Type::developerControlBlock:
                return "control";
            case Block::Type::lightPadBlock:
                return "padmini";
            case Block::Type::seaboardBlock:
                return "seaboard";
            case Block::Type::lumiKeysBlock:
                return "lightkey";
            case Block::Type::roliPiano49:
                return "roliPiano49";
            case Block::Type::airwave:
                return "airwave";
          #ifndef IGNORE_NEW_BLOCKS
            case Block::Type::lightpadProBlock:
                return "lightpadPro";
          #endif
            case Block::Type::unknown :
                break;
        }

        jassertfalse;
        return "unknown";
    }

private:
    static constexpr const char* lightpadName = "Lightpad";
    static constexpr const char* seaboardName = "Seaboard";
    static constexpr const char* lumiKeysName = "LUMI Keys";
    static constexpr const char* roliPianoMName = "ROLI Piano M";
    static constexpr const char* roliPiano49Name = "ROLI Piano";
    static constexpr const char* airwaveName = "ROLI Airwave";
    static constexpr const char* lightpadProName = "Lightpad Pro";
    static constexpr const char* liveName = "Live";
    static constexpr const char* loopName = "Loop";
    static constexpr const char* touchName = "Touch";
    static constexpr const char* devConName = "Developer Control";
};
}
