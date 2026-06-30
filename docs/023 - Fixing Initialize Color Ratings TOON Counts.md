# Fixing Initialize Color Ratings TOON Counts

I traced the `Initialize Colors & Ratings` paste failure to a count mismatch in the AI's SongModel TOON. The content itself was sound, but `rule_scores[60]{key,value}:` declared 60 rows while the payload actually carried 61, so strict parsing left one row behind and the root parser tripped over the leftover indentation.

I fixed `src/LyricsEditor/MainWindow.cpp` by adding a lenient TOON count normalizer before the `init` paste path loads `SongModel`. It now rewrites array header counts to the actual number of direct child rows, which lets the AI's metadata blocks parse even when the declared count is off by one.

I also added the same normalization to the new `--test-song-toon` CLI switch in `src/LyricsEditor/main.cpp`, so I can exercise the SongModel paste path automatically from a file and verify that the parser accepts the full color-ratings payload.

I verified the fix by building `LyricsEditor` with `C:\Users\sblo\Dev\ai-upp\bin\build.exe -m MSVS26x64 --source-roots ".;../ai-upp" -j12 src/LyricsEditor/LyricsEditor.upp` and then running the new CLI test on the exact sample. With normalization in place, the song loads with 34 lines instead of failing on the stray `rule_scores` row.
