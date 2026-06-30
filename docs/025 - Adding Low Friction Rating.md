# Adding Low Friction Rating

I extended LyricsEditor's line-evaluation model with a new `low_friction` rating so the toolbar and line evaluator can switch between six metrics instead of five. I kept the label neutral and product-focused: it measures whether a line is easy to parse, sing, and remember on first hearing.

I added the new field to `src/LyricsEditor/SongModel.h`, exposed it in `src/LyricsEditor/DockViews.cpp` and `src/LyricsEditor/DockViews.h`, and wired it into the main rating selector in `src/LyricsEditor/MainWindow.cpp`. The grid now shows stars for `Low Friction` just like the other rating modes.

I also updated the AI task prompt so the metadata-generation pass knows to fill in `low_friction`, and I copied the new value through the command-line merge path in `src/LyricsEditor/main.cpp` so it survives paste/import flows.

I rebuilt `LyricsEditor` after the change and verified the package still compiles cleanly with the new rating mode in place.
