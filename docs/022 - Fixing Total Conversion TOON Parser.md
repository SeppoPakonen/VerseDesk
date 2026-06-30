# Fixing Total Conversion TOON Parser

I traced the total-conversion failure to U++ TOON strict parsing. The AI output used `lines[34]{text}:` with scalar rows, and the strict parser reported `Tabular row width mismatch` even though the non-strict parser could read it.

I fixed the LyricsEditor path in `src/LyricsEditor/TotalConversionDialog.cpp` by trying strict TOON and JSON first, then falling back to `ParseTOON(..., false)` and loading the resulting `Value` through the existing JSONization path. I kept the normalization fallback in place as a second safety net.

I also updated the total-conversion prompt schema in `src/LyricsEditor/MainWindow.cpp` to ask for `lines[1]:` with `- text: ...` items instead of the tabular scalar form, and I added a `--test-total-conversion-toon` CLI switch in `src/LyricsEditor/main.cpp` so the parser can be exercised automatically from a file.

After the first fix, I found the compare dialog's `Paste New Alternatives` path was still only peeling JSON off the clipboard. I changed that GUI paste path to accept fenced TOON as well, so the same sample now works through the clipboard flow too.

I then tightened the clipboard heuristics so the raw paste path treats trimmed text as JSON only when the first non-whitespace character is `{`, uses `ReverseFind` for the closing fence, and otherwise falls through to TOON.

I also fixed the compare dialog's `Copy Feedback to Clipboard` output so it includes `## Current Song TOON Data` again, instead of only the iteration goal and schema.

I verified the fix by building `LyricsEditor` with `C:\Users\sblo\Dev\ai-upp\bin\build.exe -m MSVS26x64 --source-roots ".;../ai-upp" -j12 src/LyricsEditor/LyricsEditor.upp` and then running the new CLI test on the sample TOON. The parser accepted both alternatives and reported 34 lines each.
