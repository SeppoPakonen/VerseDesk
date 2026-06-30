# Fixing Metadata Dock Startup Crash

I introduced a startup crash while extending LyricsEditor with the new `low_friction` rating. The root cause was an off-by-one loop in `MetadataDock::MetadataDock()`: I had increased the field loop to 15 even though the metadata dock still only exposes 14 fields, so `ArrayCtrl::SetCtrl` walked past the end of the `fields[]` table during startup.

I fixed the loop back to 14 in `src/LyricsEditor/DockViews.cpp`. The separate 15-item loop in `LineEvalDock::LineEvalDock()` remains correct, because that dock now legitimately includes the new `Low Friction` control.

I rebuilt `LyricsEditor` after the fix and confirmed the package links cleanly again, which should clear the startup crash the Spearhead hit immediately after launch.
