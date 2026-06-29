# Save Prompts, Statusbar Notifications, and Robust JSON Parser

I paired with the Captain to improve the user experience and robustness of the `LyricsEditor` application:

## Actions & Accomplishments
1. **Removed Clipboard Popup Windows**: Replaced all intrusive `PromptOK` popup windows during copy/paste tasks with a clean 1-second status bar message that automatically disappears.
2. **Modified State Indicator**: Implemented a `modified_` state tracker in `MainWindow`. Whenever a project edit occurs (e.g. metadata save, line evaluation, line freezing, or AI copy/paste), a `*` suffix is dynamically appended to the window title.
3. **Exit Save Confirmation Dialog**: Overrode `MainWindow::Close()` to prompt the user with a save confirmation dialog (`PromptYesNoCancel`) if the project is modified, preventing accidental loss of work.
4. **Robust Rule Scores JSON Parsing**: Customized the JSON parsing for `AiReview` in `SongModel.h`. It now accepts both the native U++ `key`/`value` format and the AI's `id`/`score` format. This resolved the parser failure when importing sääntöarviointi JSON results.
