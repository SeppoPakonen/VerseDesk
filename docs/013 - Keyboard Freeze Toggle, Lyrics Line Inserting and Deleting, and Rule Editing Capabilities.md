# Keyboard Freeze Toggle, Lyrics Line Inserting and Deleting, and Rule Editing Capabilities

I paired with the Captain to resolve line freeze behavior on single row cursor focus and add full adding, deleting, and editing support for both lyrics lines and style rules.

## Actions & Accomplishments
1. **Keyboard F2 / Freeze Toggle Support**:
   - Fixed the F2 key (mapped via menu action) to toggle line freeze state on the current single row cursor position if no multi-select range exists.
   - Refactored `MainWindow::OnGridCursor()`, `UpdateSelectedLineEval()`, and `FreezeSelectedLines()` to use the original line index from column 0, ensuring correct line mapping even when the grid is sorted.
2. **Lyric Lines Adding & Deleting**:
   - Added a right-click context menu to `lyrics_grid_` containing "Add Line Above", "Add Line Below", and "Delete Line".
   - Inserting a new line copies the section tag of the adjacent line, saves undo state, and automatically targets the correct insertion index when sorted.
3. **Lyrics Text Inline Editing**:
   - Set up `lyrics_grid_`'s "Lyrics Text" column with `EditString edit_lyrics_text` inline editor, allowing users to modify lyric content directly in the main grid.
4. **Style Rules Adding, Deleting, and Inline Editing**:
   - Added a right-click context menu to `rules_grid_` containing "Add Rule" and "Delete Rule".
   - Setup `rules_grid_` with inline editors (`edit_rule_id`, `edit_rule_cat`, `edit_rule_weight`, `edit_rule_desc`), saving edits directly to `rules_.rules` on row commit.
   - Stored the original rule index as an implicit row value, preventing data corruption when sorting rules.
