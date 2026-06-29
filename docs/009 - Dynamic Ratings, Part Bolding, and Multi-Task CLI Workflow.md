# Dynamic Ratings, Part Bolding, and Multi-Task CLI Workflow

I paired with the Captain to implement dynamic toolbar ratings, bolding of section starts, clear selection highlights in tables, comprehensive metaphorical rules coloring, and separate copy/paste task automation.

## Actions & Accomplishments
1. **Dynamic Rating Selector DropList**: Added a rating category selector `DropList` in the toolbar. Selecting a rating dynamically updates the column label of the 4th column in the lyrics table and updates all cells with the selected line rating (e.g. Magic, Singability, Punchiness, Satisfaction, Rhyme Quality).
2. **First Line Bolding for Sections**: Configured the "Part" column cell to automatically render as **bold** for the first line of each section, providing clear visual boundaries between song sections.
3. **Selection Highlights Fixed**: Fixed table selection highlighting. Removed `.Paper()` and `.Ink()` overrides for normal cells, allowing U++ default highlight colors to apply when a row is selected.
4. **Comprehensive Rules Metaphorical Colors**: Expanded `GetMetaphoricalColor` with keywords mapping practically all rules categories and descriptions to a beautifully curated set of 8 emotional/thematic colors.
5. **Multi-Task Automation Pipeline**:
   - Added a new **Task** menu to the main menu bar, splitting songwriting into distinct sub-tasks: *Revise Lyrics*, *Initialize Colors & Ratings*, and *Evaluate Rules*.
   - Updated copy/paste commands to handle task-specific workflows (diff reviews vs direct metadata merges).
   - Extended the CLI interface to accept task names (`--dump-task [revise|init|eval]`, `--paste-result [revise|init|eval] <file>`) to allow separate headless automated workflows.
