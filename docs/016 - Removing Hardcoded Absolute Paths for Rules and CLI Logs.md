# Removing Hardcoded Absolute Paths for Rules and CLI Logs

I paired with the Captain to remove hardcoded absolute file paths and replace them with standard portable paths.

## Actions & Accomplishments
1. **CLI Log Path Update**:
   - Replaced the hardcoded absolute log path in `WriteToStdout` (inside `main.cpp`) with `ConfigFile("cli.log")`. This leverages U++'s standard portable configuration folder logic.
2. **Default Rules Path Resolution**:
   - Refactored `LoadDefaultRules()` in both `MainWindow.cpp` and `main.cpp` to resolve the default rules JSON files dynamically instead of using absolute paths.
   - The search checks relative paths under the executable folder, standard configuration folder `ConfigFile("rules")`, and climbs directories upward, assuring clean execution from anywhere.
