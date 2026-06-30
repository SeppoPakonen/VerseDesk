# Fixing Rating Selector and Total Conversion Normalization

I extended the total-conversion paste path so it applies the same TOON count normalization before loading alternatives. That keeps the GUI flow aligned with the existing SongModel paste logic and gives the parser one more chance to repair malformed `[...]` counts before the dialog rejects the clipboard payload.

While checking the lyric-rating toolbar, I found the dropdown only changed the column header because `SyncGrid()` and the row update path were reading the selector through the wrong accessor. The code was effectively stuck on the default magic metric, so the visible values never switched even though the label did.

I fixed the selector logic in `src/LyricsEditor/MainWindow.cpp` to read the drop list's selected data value directly. With that change, the grid column now recomputes from `magic_score`, `singability`, `punchiness`, `satisfaction`, or `rhyme_quality` as the toolbar selection changes.

I verified the updated build of `LyricsEditor` after the code changes. I also kept the existing `--test-song-toon` and `--test-total-conversion-toon` CLI checks in place so the TOON paths remain scriptable from the command line.
