# Creating LyricsEditor

I bootstrapped and implemented the `LyricsEditor` U++ application inside `src/`. The Curator requested a desktop lyrics editor supporting U++ docking layout patterns, rules evaluation, and integration with AI agent workflows.

## Actions and Rationale

1. **Bootstrap & Tooling**: I copied the U++ build compiler binary to a local `bin/` directory and added the appropriate entries to the gitignore rules so they remain untracked. I configured package search paths in `LyricsEditor.upp` to support U++ assembly lists.
2. **Model Definition**: I designed the `SongModel` and `RuleSet` serialization layouts using U++ Core `Jsonize` support. I added copy constructor and assignment operators utilizing `clone()` to handle U++ containers safely.
3. **UI Implementation**: I modified the copied `DockingTemplate2` source files. I created three custom dock windows (`MetadataDock`, `LineEvalDock`, and `AiReviewDock`) and set up the main area tabs for "Lyrics Editor" and "Rules".
4. **AI & Clipboard Workflow**: I implemented markdown task packaging for clipboard copying, along with JSON result parsing. I created a custom `DiffDialog` modal that lets the Curator inspect and comment on AI revisions before merging.
5. **Rules Configuration**: I stored default English and Finnish lyric rules and AI-tone mitigation (humanizing) rules in the `rules/` subdirectory, auto-loading them based on the song's language.

The compilation was successfully verified and linked, generating `bin/LyricsEditor.exe`.
