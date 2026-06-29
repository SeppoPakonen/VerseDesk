# Docks Toggling, Bold Formatting, Version History Tab, and Total Conversion Dialog Fixes

I paired with the Captain to refine UI usability, visual indicators, version control, and total conversion controls:

## Actions & Accomplishments
1. **Dock Windows Usability & Toggling**:
   - Programmed the dock layout lifecycle to hide/close all dockable panels when switching away from the "Lyrics Editor" tab.
   - Restores the layout automatically upon returning to the "Lyrics Editor" tab, maximizing workspace layout.
2. **Lyrics Bold indicator for Unfrozen Lines**:
   - Modified `MainWindow::SyncGrid()` to render lyrics cells in **bold** when the line is unfrozen (`!line.frozen`), and normal weight when frozen (locked).
   - This provides a clear visual focus on lines that still need work.
3. **Version History Tab & Version Restoring**:
   - Created a new main tab "Version History" containing a grid of previous undo/redo trace files from the project folder.
   - Quick-loads metadata from each backup JSON to display modification date, overall magic score, and author.
   - Double-clicking any version in the list or clicking the "Restore Selected" toolbar action loads that version in the editor while preserving the current version in undo trace history.
4. **Total Conversion Button Actions & Layout**:
   - Fixed the "Copy Task to Clipboard" and "Cancel" buttons in `TotalConversionDialog` to correctly exit the modal loop using `Break(IDOK)` and `Break(IDCANCEL)`.
   - Widened the Copy Task button (width 180) to fit its text perfectly without truncation.
