# Sortable Grids, Part Attributes Dock, and Total Conversion Workflow

I paired with the Captain to add major interactive and metadata editing features to `LyricsEditor`:

## Actions & Accomplishments
1. **Sortable Grids**:
   - Enabled column header sorting on the Lyrics grid, Rules grid, and AI Review rules grid.
   - Configured custom numerical sorting for line numbers (`#`), scores (`Score (0-10)`), weights, and the custom asterisk-based `Magic` ratings.
2. **Part Attributes Dock Panel**:
   - Created a new dockable panel `Part Attributes` (`SectionsDock`) that lists all unique sections (parts) detected in the song.
   - Designed inline editing for two section attributes (`Attr 1` and `Attr 2`). Modifying them dynamically merges the values and updates all lines belonging to that section.
3. **Diff Dialog Exit fixes**:
   - Resolved the buttons "Close / Apply" and "Write Task & Paste New" not closing the diff comparison dialog by replacing `Accept()` with the standard `Break(IDOK)` call.
4. **Floating Point Magic Score**:
   - Upgraded `overall_magic` in `SongModel.h` and the docks to be a `double` (e.g. `9.5/10`) to support fine-grained rating feedback.
5. **Total Conversion Workflow**:
   - Designed `TotalConversionDialog` to capture user instructions before copying the rewrite task to the clipboard.
   - Implemented `TotalConversionCompareDialog` allowing side-by-side comparison of 1 to 5 AI-generated alternatives.
   - Configured column header clicks to select a whole alternative, double-clicks to choose individual lines, and added iteration input area with direct feedback copying and new result pasting.
