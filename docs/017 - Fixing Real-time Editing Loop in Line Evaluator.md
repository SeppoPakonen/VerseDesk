# Fixing Real-time Editing Loop in Line Evaluator

I paired with the Captain to resolve the editing/overwriting issue when typing in the Line Evaluator fields.

## Actions & Accomplishments
1. **Feedback Loop Resolution**:
   - Diagnosed that real-time typing in `LineEvalDock` fields (like comments and rhyme groups) triggered `WhenChanged`, which called `UpdateSelectedLineEval()`.
   - `UpdateSelectedLineEval()` previously called `SyncGrid()`, rebuilding the entire grid and resetting selection. This triggered a reload of the current line data back into the text boxes, overwriting new inputs to a single character.
2. **In-place Grid Cell Updating**:
   - Refactored `UpdateSelectedLineEval()` to update the active grid row cells in-place using `lyrics_grid_.Set(...)` instead of rebuilding the entire grid via `SyncGrid()`.
   - This avoids clearing the grid, prevents selection changes, and stops cursor/loading updates, allowing smooth character-by-character editing in real-time.
