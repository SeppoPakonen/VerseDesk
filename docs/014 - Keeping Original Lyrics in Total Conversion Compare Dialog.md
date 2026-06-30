# Keeping Original Lyrics in Total Conversion Compare Dialog

I paired with the Captain to add options for keeping the original lyric lines inside the Total Conversion alternatives compare and select interface.

## Actions & Accomplishments
1. **Keep Original Button (Reject All)**:
   - Added a new button "Keep Original (Reject All)" to the compare dialog.
   - When clicked, it sets the alternative selection index to `-1` for all rows, restoring the original lyrics for the entire song at once.
2. **Right-Click Option Selection (OnGridMenu)**:
   - Implemented a right-click context menu (`WhenBar`) in `grid_compare_`.
   - Offers "Use Original Line (Keep)" and a list of all alternatives (e.g. "Use Alternative 1", "Use Alternative 2") for the selected row, providing robust and granular control to select or reject alternatives on a line-by-line basis.
