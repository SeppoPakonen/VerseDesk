# Adding Recent Project Files and Folder Memory

I added a `Recent Files` submenu under LyricsEditor's File menu and backed it with persistent state so the app remembers the 10 most recent project files. The list is stored in `AppState`, restored on startup, and each entry reopens the corresponding project directly from the menu.

I also taught the project and rules file dialogs to start in the current file's directory when possible, or fall back to the application's current directory. That keeps Open, Save, and Save As aligned with the user's last working location instead of bouncing back to a generic default.

I verified the change by rebuilding `LyricsEditor` after the patch. The code now uses U++-style vector copying for the saved recent-file list, so the app state load/save path stays compatible with the existing framework conventions.
