# Fixing Value Cast Exceptions for Null or Double Ratings

I paired with the Captain to resolve a type mismatch exception (`double_Null` or `Null` cast to `int`) that occurred when editing evaluation scores in the Line Evaluator.

## Actions & Accomplishments
1. **Type Cast Exception Diagnosis**:
   - Identified that U++'s DropList and EditField controls can return `Null` values, which are internally represented as `double_Null` (NaN/infinity) for floating-point mapping.
   - When implicitly converted to `int` via `Value::operator int()`, U++ threw a type mismatch exception since the value was outside valid `int` bounds.
2. **Safe Integer Value Helper**:
   - Added a safe `GetValueInt(const Value& v)` inline function in `SongModel.h`.
   - The helper safely checks for `Null` and converts `double`, `int`, or `String` values to integers without throwing exceptions.
3. **Application Integration**:
   - Integrated `GetValueInt()` in `LineEvalDock::Save()` for all DropList rating fields, preventing exceptions on empty or typing actions.
   - Used `GetValueInt()` inside `MainWindow::UpdateSelectedLineEval()` and `OnRulesGridUpdateRow()` to secure DropList rating values and EditInt values.
