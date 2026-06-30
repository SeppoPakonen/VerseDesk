# Accept All and Double Click Toggle in Total Conversion Compare

I paired with the Captain to add "Accept All" button and double-click toggle functionality to the Total Conversion comparison dialog.

## Actions & Accomplishments
1. **Accept All Button**:
   - Added an "Accept All" button to `TotalConversionCompareDialog`.
   - When clicked, it sets the alternative selection index to `0` (Alternative 1) for all rows in the song.
2. **Double-Click Row Toggle**:
   - Wired `grid_compare_.WhenLeftDouble` to `OnGridDoubleLeft()`.
   - Double-clicking a row toggles its state: if currently accepting an AI alternative, it resets to Keep Original (`-1`); if currently keeping the original, it toggles to Alternative 1 (`0`), matching the user's intuitive flow.
