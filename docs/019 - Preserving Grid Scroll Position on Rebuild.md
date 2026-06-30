# Preserving Grid Scroll Position on Rebuild

I paired with the Captain to preserve the grid scroll position when reloading/syncing the grid tables.

## Actions & Accomplishments
1. **Scroll State Loss Diagnosis**:
   - Clearing (`Clear()`) and rebuilding the ArrayCtrl grid during a sync cycle causes the scroll position to reset back to index 0, which is disruptive when freezing/unfreezing or modifying rows.
2. **Scroll Offset Restoring**:
   - Added logic to `MainWindow::SyncGrid()`, `SyncRulesGrid()`, and `TotalConversionCompareDialog::SyncGrid()` to capture the vertical scrollbar offset using `int scroll = GetScroll()`.
   - Restored the offset immediately after rebuilding using `ScrollTo(scroll)`. This keeps the grid view completely static during redraws, maintaining visual stability.
