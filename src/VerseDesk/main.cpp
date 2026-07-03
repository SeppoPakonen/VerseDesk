#include "MainWindow.h"
#include "TotalConversionDialog.h"
#include <iostream>

#ifdef PLATFORM_WIN32
#include <windows.h>
#endif

using namespace Upp;

void WriteToStdout(const String& text)
{
	// Log to local file for debug/redirection fallback
	SaveFile(ConfigFile("cli.log"), text);

#ifdef PLATFORM_WIN32
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD written = 0;
	if (hOut && hOut != INVALID_HANDLE_VALUE) {
		DWORD flags;
		if (GetConsoleMode(hOut, &flags) || GetFileType(hOut) == FILE_TYPE_DISK || GetFileType(hOut) == FILE_TYPE_PIPE) {
			const char* data = ~text;
			int len = text.GetLength();
			if (::WriteFile(hOut, data, (DWORD)len, &written, NULL) && written > 0) {
				return;
			}
		}
	}
	if (AttachConsole(ATTACH_PARENT_PROCESS)) {
		hOut = CreateFile("CONOUT$", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
		if (hOut && hOut != INVALID_HANDLE_VALUE) {
			const char* data = ~text;
			int len = text.GetLength();
			::WriteFile(hOut, data, (DWORD)len, &written, NULL);
			CloseHandle(hOut);
		}
	}
#else
	std::cout << text.ToStd();
	std::cout.flush();
#endif
}

int CountLeadingWhitespace(const String& s)
{
	int i = 0;
	while (i < s.GetLength()) {
		char c = s[i];
		if (c != ' ' && c != '\t')
			break;
		i++;
	}
	return i;
}

bool ParseArrayHeaderLine(const String& line, String& prefix, String& suffix)
{
	int indent = CountLeadingWhitespace(line);
	String trimmed = line.Mid(indent);
	if (trimmed.IsEmpty())
		return false;

	int lb = trimmed.Find('[');
	if (lb <= 0)
		return false;

	int rb = trimmed.Find(']', lb + 1);
	if (rb <= lb + 1)
		return false;

	String count = TrimBoth(trimmed.Mid(lb + 1, rb - lb - 1));
	if (count.IsEmpty())
		return false;
	for (int i = 0; i < count.GetLength(); i++) {
		if (!IsDigit(count[i]))
			return false;
	}

	int p = rb + 1;
	if (p < trimmed.GetLength() && trimmed[p] == '{') {
		int rb_fields = trimmed.Find('}', p + 1);
		if (rb_fields <= p)
			return false;
		p = rb_fields + 1;
	}

	if (p >= trimmed.GetLength() || trimmed[p] != ':')
		return false;

	prefix = line.Left(indent + lb + 1);
	suffix = trimmed.Mid(rb);
	return true;
}

void NormalizeToonArrayCounts(Vector<String>& lines, int start, int base_indent)
{
	int i = start;
	while (i < lines.GetCount()) {
		String line = lines[i];
		if (TrimBoth(line).IsEmpty()) {
			i++;
			continue;
		}

		int indent = CountLeadingWhitespace(line);
		if (indent < base_indent)
			return;
		if (indent > base_indent) {
			i++;
			continue;
		}

		String prefix, suffix;
		if (ParseArrayHeaderLine(line, prefix, suffix)) {
			int child_indent = base_indent + 2;
			int next = i + 1;
			while (next < lines.GetCount() && TrimBoth(lines[next]).IsEmpty())
				next++;

			int child_end = next;
			if (next < lines.GetCount() && CountLeadingWhitespace(lines[next]) > base_indent) {
				child_end = next;
				while (child_end < lines.GetCount()) {
					String child = lines[child_end];
					if (TrimBoth(child).IsEmpty()) {
						child_end++;
						continue;
					}
					int child_line_indent = CountLeadingWhitespace(child);
					if (child_line_indent <= base_indent)
						break;
					child_end++;
				}
				NormalizeToonArrayCounts(lines, next, child_indent);
			}

			int item_count = 0;
			for (int j = next; j < child_end; j++) {
				String child = lines[j];
				if (TrimBoth(child).IsEmpty())
					continue;
				if (CountLeadingWhitespace(child) == child_indent)
					item_count++;
			}

			lines[i] = prefix + AsString(item_count) + suffix;
			i = child_end;
			continue;
		}

		int next = i + 1;
		while (next < lines.GetCount() && TrimBoth(lines[next]).IsEmpty())
			next++;
		if (next < lines.GetCount() && CountLeadingWhitespace(lines[next]) > base_indent) {
			NormalizeToonArrayCounts(lines, next, base_indent + 2);
			i = next;
			continue;
		}

		i++;
	}
}

String NormalizeToonArrayCounts(const String& text)
{
	Vector<String> lines = Split(TrimBoth(text), '\n');
	NormalizeToonArrayCounts(lines, 0, 0);
	String out;
	for (const String& line : lines)
		out << line << '\n';
	return TrimBoth(out);
}

GUI_APP_MAIN
{
	const Vector<String>& cmd = CommandLine();
	if (cmd.GetCount() > 0 && cmd[0] == "--test-total-conversion-toon") {
		if (cmd.GetCount() < 2 || cmd[1].StartsWith("--")) {
			WriteToStdout("Error: missing path for --test-total-conversion-toon\n");
			return;
		}

		String toon_text = LoadFile(cmd[1]);
		if (toon_text.IsEmpty()) {
			WriteToStdout("Error: failed to read TOON file: " + cmd[1] + "\n");
			return;
		}

		TotalConversionResponse response;
		if (!LoadTotalConversionResponse(response, toon_text)) {
			WriteToStdout("Error: failed to parse total conversion TOON\n");
			return;
		}

		String out;
		out << "Success: parsed " << response.alternatives.GetCount() << " alternatives\n";
		for (int i = 0; i < response.alternatives.GetCount(); i++) {
			out << "  [" << (i + 1) << "] " << response.alternatives[i].name
			    << " - " << response.alternatives[i].lines.GetCount() << " lines\n";
		}
		WriteToStdout(out);
		return;
	}

	if (cmd.GetCount() > 0 && cmd[0] == "--test-song-toon") {
		if (cmd.GetCount() < 2 || cmd[1].StartsWith("--")) {
			WriteToStdout("Error: missing path for --test-song-toon\n");
			return;
		}

		String toon_text = LoadFile(cmd[1]);
		if (toon_text.IsEmpty()) {
			WriteToStdout("Error: failed to read TOON file: " + cmd[1] + "\n");
			return;
		}

		toon_text = NormalizeToonArrayCounts(toon_text);

		Value val = ParseTOON(toon_text, false);
		if (val.IsError()) {
			WriteToStdout(String("Error: ParseTOON failed: ") + GetErrorText(val) + "\n");
			return;
		}

		SongModel song;
		try {
			LoadFromJsonValue(song, val);
		}
		catch(CParser::Error e) {
			WriteToStdout(String("Error: LoadFromJsonValue failed: ") + e + "\n");
			return;
		}
		catch(...) {
			WriteToStdout("Error: LoadFromJsonValue failed with unknown exception\n");
			return;
		}

		String out;
		out << "Success: parsed song '" << song.title << "' with " << song.lines.GetCount() << " lines\n";
		if (song.lines.GetCount() > 0) {
			out << "  First line: " << song.lines[0].text << "\n";
		}
		WriteToStdout(out);
		return;
	}

	if (cmd.GetCount() > 0 && !cmd[0].StartsWith("--")) {
		String proj_path = cmd[0];
		
		bool dump_task = false;
		bool paste_result = false;
		String result_path = "";
		String task_name = "revise"; // default
		
		for (int i = 1; i < cmd.GetCount(); i++) {
			if (cmd[i] == "--dump-task") {
				dump_task = true;
				if (i + 1 < cmd.GetCount() && !cmd[i + 1].StartsWith("--")) {
					task_name = cmd[i + 1];
					i++;
				}
			} else if (cmd[i] == "--paste-result") {
				paste_result = true;
				if (i + 2 < cmd.GetCount() && !cmd[i + 1].StartsWith("--")) {
					task_name = cmd[i + 1];
					result_path = cmd[i + 2];
					i += 2;
				} else if (i + 1 < cmd.GetCount()) {
					result_path = cmd[i + 1];
					i++;
				}
			}
		}
		
		if (dump_task) {
			SongModel song;
			if (!LoadFromJsonFile(song, proj_path)) {
				WriteToStdout("Error: Failed to load project file " + proj_path + "\n");
				return;
			}
			
			// Load rules based on language
			RuleSet rules;
			String filename = Format("rules_%s.json", song.language.IsEmpty() ? "fi" : song.language);
			String rules_path = AppendFileName(AppendFileName(GetFileDirectory(GetExeFilePath()), "rules"), filename);
			if (!FileExists(rules_path)) {
				rules_path = AppendFileName(ConfigFile("rules"), filename);
			}
			if (!FileExists(rules_path)) {
				rules_path = AppendFileName(GetFileDirectory(GetFileDirectory(GetExeFilePath())), "src/LyricsEditor/rules/" + filename);
			}
			if (!FileExists(rules_path)) {
				String dir = GetFileDirectory(GetExeFilePath());
				for (int i = 0; i < 5; i++) {
					String test = AppendFileName(dir, "src/LyricsEditor/rules/" + filename);
					if (FileExists(test)) {
						rules_path = test;
						break;
					}
					String up = GetFileDirectory(dir);
					if (up == dir) break;
					dir = up;
				}
			}
			LoadFromJsonFile(rules, rules_path);
			
			String task = BuildTaskPrompt(song, rules, task_name);
			WriteToStdout(task);
			return;
		}
		
		if (paste_result) {
			SongModel song;
			if (!LoadFromJsonFile(song, proj_path)) {
				WriteToStdout("Error: Failed to load project file " + proj_path + "\n");
				return;
			}
			
			String result_content = LoadFile(result_path);
			if (result_content.IsEmpty()) {
				WriteToStdout("Error: Result file is empty or missing: " + result_path + "\n");
				return;
			}
			
			// Extract TOON or JSON
			String toon_text = result_content;
			int start = result_content.Find("```toon");
			if (start >= 0) {
				start += 7;
				int end = result_content.Find("```", start);
				if (end >= 0) {
					toon_text = result_content.Mid(start, end - start);
				}
			} else {
				start = result_content.Find("```json");
				if (start >= 0) {
					start += 7;
					int end = result_content.Find("```", start);
					if (end >= 0) {
						toon_text = result_content.Mid(start, end - start);
					}
				} else {
					start = result_content.Find("{");
					if (start >= 0) {
						int end = result_content.ReverseFind("}");
						if (end >= start) {
							toon_text = result_content.Mid(start, end - start + 1);
						}
					}
				}
			}
			
			SongModel new_song;
			bool loaded = false;
			if (LoadFromTOON(new_song, toon_text)) {
				loaded = true;
			} else if (LoadFromJson(new_song, toon_text)) {
				loaded = true;
			}

			if (!loaded) {
				WriteToStdout("Error: Failed to parse TOON or JSON from result file.\n");
				return;
			}
			
			if (task_name == "init") {
				// Merge rating metadata, rhyme groups and color
				for (int i = 0; i < song.lines.GetCount() && i < new_song.lines.GetCount(); i++) {
					song.lines[i].color = new_song.lines[i].color;
					song.lines[i].magic_score = new_song.lines[i].magic_score;
					song.lines[i].singability = new_song.lines[i].singability;
					song.lines[i].punchiness = new_song.lines[i].punchiness;
					song.lines[i].satisfaction = new_song.lines[i].satisfaction;
					song.lines[i].rhyme_quality = new_song.lines[i].rhyme_quality;
					song.lines[i].low_friction = new_song.lines[i].low_friction;
					song.lines[i].rhyme_group_alliteration = new_song.lines[i].rhyme_group_alliteration;
					song.lines[i].rhyme_group_internal = new_song.lines[i].rhyme_group_internal;
					song.lines[i].rhyme_group_end = new_song.lines[i].rhyme_group_end;
					song.lines[i].correction_need = new_song.lines[i].correction_need;
					song.lines[i].correction_need_alliteration = new_song.lines[i].correction_need_alliteration;
					song.lines[i].correction_need_internal = new_song.lines[i].correction_need_internal;
					song.lines[i].correction_need_end = new_song.lines[i].correction_need_end;
				}
				song.ai_review = new_song.ai_review;
			} else if (task_name == "eval") {
				// Only merge rule scores and overall feedback
				song.ai_review = new_song.ai_review;
			} else {
				// Revise
				for (int i = 0; i < song.lines.GetCount() && i < new_song.lines.GetCount(); i++) {
					if (!song.lines[i].frozen) {
						song.lines[i] = new_song.lines[i];
					}
				}
				for (int i = song.lines.GetCount(); i < new_song.lines.GetCount(); i++) {
					song.lines.Add(new_song.lines[i]);
				}
				
				song.title = new_song.title;
				song.artist = new_song.artist;
				song.author = new_song.author;
				song.completion_year = new_song.completion_year;
				song.composition_year = new_song.composition_year;
				song.lyrics_year = new_song.lyrics_year;
				song.genre = new_song.genre;
				song.suno_style = new_song.suno_style;
				song.gender = new_song.gender;
				song.singer_count = new_song.singer_count;
				song.language = new_song.language;
				song.dialect = new_song.dialect;
				song.artist_style = new_song.artist_style;
				song.artist_image = new_song.artist_image;
				song.ai_review = new_song.ai_review;
			}
			
			if (StoreAsJsonFile(song, proj_path)) {
				// Also create undo trace
				if (!song.title.IsEmpty()) {
					String dir = AppendFileName(GetFileDirectory(proj_path), "undo_" + song.title);
					RealizeDirectory(dir);
					String time_str = FormatTime(GetSysTime(), "YYYYMMDD_hhmmss");
					String undo_file = AppendFileName(dir, time_str + "_" + GetFileTitle(proj_path) + ".json");
					StoreAsJsonFile(song, undo_file);
				}
				WriteToStdout("Success: Applied AI response (" + task_name + ") and saved project to " + proj_path + "\n");
			} else {
				WriteToStdout("Error: Failed to save updated project file " + proj_path + "\n");
			}
			return;
		}
	}

	MainWindow win;
	if (cmd.GetCount() > 0 && !cmd[0].StartsWith("--")) {
		win.SetInitialProject(cmd[0]);
	}
	win.Run();
}
