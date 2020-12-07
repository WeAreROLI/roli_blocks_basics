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

/**
    Represents the version number of a block device.

    @tags{Blocks}
*/
struct BlocksVersion
{
public:
    /** The main value in a version number x.0.0 */
    int major = 0;

    /** The secondary value in a version number 1.x.0 */
    int minor = 0;

    /** The tertiary value in a version number 1.0.x */
    int patch = 0;

    /** The release tag for this version, such as "beta", "alpha", "rc", etc */
    String releaseType;

    /** A numerical value associated with the release tag, such as "beta 4" */
    int releaseCount = 0;

    /** The associated git commit that generated this firmware version */
    String commit;

    /** Identify "forced" firmware builds **/
    bool forced = false;

    String toString (bool extended = false) const;

    /** Constructs a version number from an formatted String */
    BlocksVersion (const String&);

    /** Constructs a version number from another BlocksVersion */
    BlocksVersion (const BlocksVersion& other) = default;

    /** Creates an empty version number **/
    BlocksVersion() = default;

    /** Returns true if string format is valid */
    static bool isValidVersion (const String& versionString);

    bool operator == (const BlocksVersion&) const;
    bool operator != (const BlocksVersion&) const;
    bool operator <  (const BlocksVersion&) const;
    bool operator >  (const BlocksVersion&) const;
    bool operator <= (const BlocksVersion&) const;
    bool operator >= (const BlocksVersion&) const;

private:
    /** @internal */
    bool evaluate (const String& versionString);
    bool releaseTypeGreaterThan (const BlocksVersion& otherReleaseType) const;

    bool isGreaterThan (const BlocksVersion& other) const;
    bool isEqualTo (const BlocksVersion& other) const;
};

} // namespace juce
