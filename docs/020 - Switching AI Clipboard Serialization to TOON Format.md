# Switching AI Clipboard Serialization to TOON Format

I paired with the Captain to replace the legacy JSON formatting for AI tasks with U++'s native compact TOON format.

## Actions & Accomplishments
1. **Prompt Instructions Refactoring**:
   - Refactored `MainWindow::BuildTaskPrompt()` to instruct the AI model to output and accept TOON structure blocks instead of JSON.
   - Updated the serialized song data block from JSON (`StoreAsJson`) to TOON format (`StoreAsTOON`).
2. **Robust TOON/JSON Pasting in MainWindow**:
   - Rewrote `MainWindow::PasteResultFromClipboardWith()` to detect both ````toon` and ````json` code block fences.
   - Deserializes results using `LoadFromTOON(new_song, toon_text)` with a fallback to `LoadFromJson(new_song, toon_text)` for backwards compatibility.
3. **Total Conversion Schema Refactoring**:
   - Modified `CopyTotalConversionTask()` to present the new Target TOON schema definition and serialize song data using TOON.
   - Refactored `PasteTotalConversionResult()` to extract TOON/JSON and pass it to `LoadAlternatives()`.
4. **Compare Dialog TOON Loader**:
   - Refactored `TotalConversionCompareDialog::LoadAlternatives()` to accept a `toon_text` parameter.
   - Implemented double-format loading using `LoadFromTOON` with a fallback to `LoadFromJson` to prevent import failures.
5. **CLI Parsing Updates**:
   - Updated the CLI parser in `main.cpp` to search result files for TOON blocks and load them using `LoadFromTOON()` (falling back to JSON).
