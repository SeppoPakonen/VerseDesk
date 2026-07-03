# Requiring New Song Titles in Total Conversion

I tightened the total conversion flow so it no longer treats the old song title as the default outcome. The prompt now explicitly requires every alternative to rename the song, and the TOON example includes a dedicated `title` field alongside the alternative label.

I also extended `ConversionAlternative` with a `title` field, propagated it through the compare dialog, and let the result song inherit the selected alternative's title. That keeps the UI and saved song aligned with the new naming instead of leaving the original title in place.

While touching the clipboard path, I made the fenced TOON and JSON extraction use the last closing fence so pasted AI responses are parsed more reliably.

The build itself was blocked at link time because `bin/LyricsEditor.exe` was already running and holding the file open.
