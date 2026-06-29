#include "TotalConversionDialog.h"

static int CountLeadingWhitespace(const String& s)
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

static String DeindentToonBlock(const String& text)
{
	String trimmed = TrimBoth(text);
	Vector<String> lines = Split(trimmed, '\n');
	int min_indent = INT_MAX;

	for (const String& line : lines) {
		String cur = line;
		if (cur.EndsWith("\r"))
			cur = cur.Left(cur.GetCount() - 1);
		if (TrimBoth(cur).IsEmpty())
			continue;
		int indent = CountLeadingWhitespace(cur);
		if (indent < min_indent)
			min_indent = indent;
	}

	if (min_indent <= 0 || min_indent == INT_MAX)
		return trimmed;

	String out;
	for (const String& line : lines) {
		String cur = line;
		if (cur.EndsWith("\r"))
			cur = cur.Left(cur.GetCount() - 1);
		if (cur.GetCount() >= min_indent)
			cur = cur.Mid(min_indent);
		out << cur << '\n';
	}

	return TrimBoth(out);
}

static String EscapeToonQuoted(const String& s)
{
	String out;
	out.Cat('"');
	for (int i = 0; i < s.GetLength(); i++) {
		char c = s[i];
		if (c == '\\' || c == '"')
			out.Cat('\\');
		out.Cat(c);
	}
	out.Cat('"');
	return out;
}

static bool ParseTabularScalarHeader(const String& line, String& head, String& count, String& field, int& header_indent)
{
	header_indent = CountLeadingWhitespace(line);
	String trimmed = line.Mid(header_indent);
	if (trimmed.IsEmpty() || trimmed.GetCount() < 3 || trimmed.Right(1) != ":")
		return false;

	int lb = trimmed.Find('[');
	int rb = trimmed.Find(']', lb + 1);
	if (lb <= 0 || rb <= lb + 1)
		return false;

	int lb_field = trimmed.Find('{', rb + 1);
	int rb_field = trimmed.Find('}', lb_field + 1);
	int colon = trimmed.ReverseFind(':');
	if (colon < 0 || colon != trimmed.GetCount() - 1)
		return false;

	head = TrimBoth(trimmed.Left(lb));
	count = TrimBoth(trimmed.Mid(lb + 1, rb - lb - 1));
	field.Clear();
	if (lb_field >= 0 && rb_field > lb_field) {
		field = TrimBoth(trimmed.Mid(lb_field + 1, rb_field - lb_field - 1));
	}

	if (head.IsEmpty() || count.IsEmpty() || field.IsEmpty())
		return false;

	return true;
}

static String NormalizeTotalConversionToon(const String& toon_text)
{
	Vector<String> raw_lines = Split(toon_text, '\n');
	String normalized;

	for (int i = 0; i < raw_lines.GetCount(); i++) {
		String raw = raw_lines[i];
		if (raw.EndsWith("\r"))
			raw = raw.Left(raw.GetCount() - 1);

		String head;
		String count;
		String field;
		int header_indent = 0;
		if (!ParseTabularScalarHeader(raw, head, count, field, header_indent)) {
			normalized << raw << '\n';
			continue;
		}

		int j = i + 1;
		int actual_count = 0;
		for (; j < raw_lines.GetCount(); j++) {
			String child = raw_lines[j];
			if (child.EndsWith("\r"))
				child = child.Left(child.GetCount() - 1);
			if (child.IsEmpty())
				continue;
			if (CountLeadingWhitespace(child) <= header_indent)
				break;
			actual_count++;
		}

		normalized << String(header_indent, ' ') << head << "[" << actual_count << "]{" << field << "}:\n";
		for (; j < raw_lines.GetCount(); j++) {
			String child = raw_lines[j];
			if (child.EndsWith("\r"))
				child = child.Left(child.GetCount() - 1);
			if (!child.IsEmpty() && CountLeadingWhitespace(child) <= header_indent)
				break;
			normalized << child << '\n';
		}
		i = j - 1;
	}

	return normalized;
}

bool LoadTotalConversionResponse(TotalConversionResponse& response, const String& toon_text)
{
	response.alternatives.Clear();

	if (LoadFromTOON(response, toon_text) || LoadFromJson(response, toon_text)) {
		return !response.alternatives.IsEmpty();
	}

	String deindented = DeindentToonBlock(toon_text);
	String normalized = NormalizeTotalConversionToon(deindented);
	if (normalized != deindented) {
		response.alternatives.Clear();
		if (LoadFromTOON(response, normalized) || LoadFromJson(response, normalized)) {
			return !response.alternatives.IsEmpty();
		}

		Value toon_value = ParseTOON(normalized, false);
		if (!toon_value.IsError()) {
			try {
				LoadFromJsonValue(response, toon_value);
				if (!response.alternatives.IsEmpty()) {
					return true;
				}
			}
			catch(...) {
			}
		}
	}

	if (deindented != toon_text) {
		response.alternatives.Clear();
		if (LoadFromTOON(response, deindented) || LoadFromJson(response, deindented)) {
			return !response.alternatives.IsEmpty();
		}

		Value toon_value = ParseTOON(deindented, false);
		if (!toon_value.IsError()) {
			try {
				LoadFromJsonValue(response, toon_value);
				if (!response.alternatives.IsEmpty()) {
					return true;
				}
			}
			catch(...) {
			}
		}
	}

	Value toon_value = ParseTOON(toon_text, false);
	if (!toon_value.IsError()) {
		try {
			LoadFromJsonValue(response, toon_value);
			if (!response.alternatives.IsEmpty()) {
				return true;
			}
		}
		catch(...) {
			// Fall through to the normalization fallback below.
		}
	}

	if (normalized != deindented) {
		response.alternatives.Clear();
		if (LoadFromTOON(response, normalized) || LoadFromJson(response, normalized)) {
			return !response.alternatives.IsEmpty();
		}

		toon_value = ParseTOON(normalized, false);
		if (!toon_value.IsError()) {
			try {
				LoadFromJsonValue(response, toon_value);
				if (!response.alternatives.IsEmpty()) {
					return true;
				}
			}
			catch(...) {
			}
		}

		String normalized_deindented = DeindentToonBlock(normalized);
		if (normalized_deindented != normalized) {
			response.alternatives.Clear();
			if (LoadFromTOON(response, normalized_deindented) || LoadFromJson(response, normalized_deindented)) {
				return !response.alternatives.IsEmpty();
			}

			toon_value = ParseTOON(normalized_deindented, false);
			if (!toon_value.IsError()) {
				try {
					LoadFromJsonValue(response, toon_value);
					if (!response.alternatives.IsEmpty()) {
						return true;
					}
				}
				catch(...) {
				}
			}
		}
	}

	return false;
}

TotalConversionDialog::TotalConversionDialog()
{
	Title("Total Conversion - Lyrics Editor").Sizeable().Zoomable();
	SetRect(0, 0, 500, 350);

	Label* lbl = new Label();
	lbl->SetLabel("Explain what you want to change (style, mood, theme, etc.):");
	Add(lbl->LeftPos(8, 400).TopPos(8, 20));

	Add(edit_instructions_.HSizePos(8, 8).VSizePos(32, 48));

	Add(btn_copy_task_.RightPos(136, 180).BottomPos(8, 30));
	btn_copy_task_.SetLabel("Copy Task to Clipboard");
	btn_copy_task_.WhenAction = THISBACK(OnCopyTask);

	Add(btn_cancel_.RightPos(8, 120).BottomPos(8, 30));
	btn_cancel_.SetLabel("Cancel");
	btn_cancel_.WhenAction = [=] { Break(IDCANCEL); };
}

void TotalConversionDialog::OnCopyTask()
{
	Break(IDOK);
}

// ---------------------------------------------------------------------------
// TotalConversionCompareDialog

TotalConversionCompareDialog::TotalConversionCompareDialog(const SongModel& old_song, const RuleSet& rules)
	: old_song_(old_song), rules_(rules)
{
	Title("Compare Conversion Alternatives - Lyrics Editor").Sizeable().Zoomable();
	SetRect(0, 0, 1000, 650);

	Add(grid_compare_.HSizePos(8, 8).VSizePos(8, 170));
	grid_compare_.WhenLeftClick = THISBACK(OnGridClick);
	grid_compare_.WhenLeftDouble = THISBACK(OnGridDoubleLeft);
	grid_compare_.WhenBar = THISBACK(OnGridMenu);

	Label* lbl = new Label();
	lbl->SetLabel("Explanation for rewrite / iteration:");
	Add(lbl->LeftPos(8, 400).BottomPos(136, 20));

	Add(edit_iteration_.HSizePos(8, 8).BottomPos(44, 90));

	Add(btn_copy_feedback_.LeftPos(8, 200).BottomPos(8, 30));
	btn_copy_feedback_.SetLabel("Copy Feedback to Clipboard");
	btn_copy_feedback_.WhenAction = THISBACK(OnCopyFeedback);

	Add(btn_paste_new_.LeftPos(216, 180).BottomPos(8, 30));
	btn_paste_new_.SetLabel("Paste New Alternatives");
	btn_paste_new_.WhenAction = THISBACK(OnPasteNew);

	Add(btn_accept_all_.RightPos(472, 120).BottomPos(8, 30));
	btn_accept_all_.SetLabel("Accept All");
	btn_accept_all_.WhenAction = THISBACK(OnAcceptAll);

	Add(btn_reject_all_.RightPos(284, 180).BottomPos(8, 30));
	btn_reject_all_.SetLabel("Keep Original (Reject All)");
	btn_reject_all_.WhenAction = THISBACK(OnRejectAll);

	Add(btn_apply_.RightPos(136, 140).BottomPos(8, 30));
	btn_apply_.SetLabel("Apply & Close");
	btn_apply_.WhenAction = THISBACK(OnApply);

	Add(btn_cancel_.RightPos(8, 120).BottomPos(8, 30));
	btn_cancel_.SetLabel("Cancel");
	btn_cancel_.WhenAction = THISBACK(OnCancel);
}

bool TotalConversionCompareDialog::LoadAlternatives(const String& toon_text)
{
	response_.alternatives.Clear();
	if (!LoadTotalConversionResponse(response_, toon_text)) {
		return false;
	}

	int alt_count = response_.alternatives.GetCount();
	int line_count = old_song_.lines.GetCount();

	selected_alt_idx_.SetCount(line_count);
	for (int i = 0; i < line_count; i++) {
		selected_alt_idx_[i] = 0; // Default to Alternative 1
	}

	grid_compare_.Reset();
	grid_compare_.ColumnSortThreeState();
	grid_compare_.AddColumn("Line #", 12).Sorting();
	grid_compare_.AddColumn("Old Lyrics", 45).Sorting();
	grid_compare_.AddColumn("Current Selected", 45).Sorting();

	for (int j = 0; j < alt_count; j++) {
		String header = response_.alternatives[j].name;
		if (!response_.alternatives[j].title.IsEmpty()) {
			header << " [" << response_.alternatives[j].title << "]";
		}
		grid_compare_.AddColumn(header, 40).Sorting();
	}

	// Bind header clicks to select alternative for all rows
	for (int j = 0; j < alt_count; j++) {
		grid_compare_.HeaderObject().Tab(3 + j).WhenLeftClick = [=] {
			for (int r = 0; r < old_song_.lines.GetCount(); r++) {
				selected_alt_idx_[r] = j;
			}
			SyncGrid();
		};
	}

	// Bind Old Lyrics header click to restore original for all rows
	grid_compare_.HeaderObject().Tab(1).WhenLeftClick = [=] {
		for (int r = 0; r < old_song_.lines.GetCount(); r++) {
			selected_alt_idx_[r] = -1;
		}
		SyncGrid();
	};

	SyncGrid();
	return true;
}

void TotalConversionCompareDialog::SyncGrid()
{
	int sel = grid_compare_.GetCursor();
	int scroll = grid_compare_.GetScroll();
	grid_compare_.Clear();

	int line_count = old_song_.lines.GetCount();
	int alt_count = response_.alternatives.GetCount();

	for (int i = 0; i < line_count; i++) {
		const LyricsLine& line = old_song_.lines[i];
		
		int sel_alt = selected_alt_idx_[i];
		String selected_text = line.text;
		if (sel_alt >= 0 && sel_alt < alt_count) {
			if (i < response_.alternatives[sel_alt].lines.GetCount()) {
				selected_text = response_.alternatives[sel_alt].lines[i].text;
			}
		}

		Color sel_bg = (sel_alt == -1) ? Color(240, 240, 240) : Color(220, 255, 220);
		Color sel_fg = (sel_alt == -1) ? Color(100, 100, 100) : Color(0, 128, 0);

		Vector<Value> row_data;
		row_data.Add(Format("%d", i + 1));
		row_data.Add(AttrText(line.text).NormalPaper(Color(255, 255, 255)));
		row_data.Add(AttrText(selected_text).NormalPaper(sel_bg).NormalInk(sel_fg).Bold());

		for (int j = 0; j < alt_count; j++) {
			String alt_text = "";
			if (i < response_.alternatives[j].lines.GetCount()) {
				alt_text = response_.alternatives[j].lines[i].text;
			}

			bool selected = (sel_alt == j);
			Color bg = selected ? Color(220, 255, 220) : Color(255, 255, 255);
			Color fg = selected ? Color(0, 128, 0) : Color(30, 30, 30);
			row_data.Add(AttrText(alt_text).NormalPaper(bg).NormalInk(fg));
		}

		grid_compare_.Add(row_data);
	}

	if (sel >= 0 && sel < grid_compare_.GetCount()) {
		grid_compare_.SetCursor(sel);
	}
	grid_compare_.ScrollTo(scroll);
}

void TotalConversionCompareDialog::OnGridClick()
{
	int col = grid_compare_.GetClickColumn();
	int row = grid_compare_.GetCursor();
	int alt_count = response_.alternatives.GetCount();

	if (row >= 0 && row < old_song_.lines.GetCount()) {
		if (col == 1) {
			selected_alt_idx_[row] = -1;
			SyncGrid();
		} else {
			int alt_idx = col - 3;
			if (alt_idx >= 0 && alt_idx < alt_count) {
				selected_alt_idx_[row] = alt_idx;
				SyncGrid();
			}
		}
	}
}

void TotalConversionCompareDialog::OnCopyFeedback()
{
	String instructions = (~edit_iteration_).ToString();
	if (instructions.IsEmpty()) {
		PromptOK("Ole hyvÃ¤ ja kirjoita selite ensin.");
		return;
	}

	String task;
	task << "Task: Total conversion iteration of the song '" << old_song_.title << "'.\n\n";
	task << "## Iteration Goal\n";
	task << instructions << "\n\n";
	task << "Please correct the song based on the original song and rules, and provide 1 to 5 updated alternatives matching the target schema.\n";
	task << "Every alternative must also rename the song with a clearly new title; do not reuse the original title.\n\n";
	task << "## Target TOON Schema\n";
	task << "```toon\n";
	task << "alternatives[1]:\n";
	task << "  - title: New Song Title\n";
	task << "    name: Alternative 1: <Short description>\n";
	task << "    lines[1]:\n";
	task << "      - text: Rewritten line text\n";
	task << "```\n\n";
	task << "## Current Song TOON Data\n";
	task << "```toon\n" << StoreAsTOON(old_song_) << "\n```\n";

	WriteClipboardText(task);
	PromptOK("Iteraatioprompt kopioitu leikepÃ¶ydÃ¤lle!");
}

static String ExtractStructuredClipboardText(const String& clip);
static String NormalizeToonClipboardBlock(const String& text);

void TotalConversionCompareDialog::OnPasteNew()
{
	String clip = ReadClipboardText();
	if (clip.IsEmpty()) {
		PromptOK("LeikepÃ¶ytÃ¤ on tyhjÃ¤.");
		return;
	}

	String toon_text = ExtractStructuredClipboardText(clip);

	if (LoadAlternatives(toon_text)) {
		PromptOK("Uudet vaihtoehdot ladattu onnistuneesti!");
	} else {
		PromptOK("Vaihtoehtojen lataus epäonnistui. Varmista, että leikepöydällä on validi TOON- tai JSON-rakenne.");
	}
}

static String ExtractStructuredClipboardText(const String& clip)
{
	String trimmed = TrimBoth(clip);
	String toon_text = trimmed;
	int start = clip.Find("```toon");
	if (start >= 0) {
		start += 7;
		int end = clip.ReverseFind("```");
		if (end >= 0) {
			toon_text = DeindentToonBlock(clip.Mid(start, end - start));
		}
	} else {
		start = clip.Find("```json");
		if (start >= 0) {
			start += 7;
			int end = clip.ReverseFind("```");
			if (end >= 0) {
				toon_text = TrimBoth(clip.Mid(start, end - start));
			}
		} else {
			if (trimmed.StartsWith("{")) {
				int start = clip.Find("{");
				int end = clip.ReverseFind("}");
				if (end >= start) {
					toon_text = TrimBoth(clip.Mid(start, end - start + 1));
				}
			} else {
				toon_text = DeindentToonBlock(trimmed);
			}
		}
	}
	return toon_text;
}

void TotalConversionCompareDialog::OnApply()
{
	Break(IDOK);
}

void TotalConversionCompareDialog::OnCancel()
{
	Break(IDCANCEL);
}

void TotalConversionCompareDialog::OnRejectAll()
{
	for (int r = 0; r < old_song_.lines.GetCount(); r++) {
		selected_alt_idx_[r] = -1;
	}
	SyncGrid();
}

void TotalConversionCompareDialog::OnAcceptAll()
{
	for (int r = 0; r < old_song_.lines.GetCount(); r++) {
		selected_alt_idx_[r] = 0;
	}
	SyncGrid();
}

void TotalConversionCompareDialog::OnGridDoubleLeft()
{
	int row = grid_compare_.GetCursor();
	if (row >= 0 && row < old_song_.lines.GetCount()) {
		if (selected_alt_idx_[row] >= 0) {
			selected_alt_idx_[row] = -1;
		} else {
			selected_alt_idx_[row] = 0;
		}
		SyncGrid();
	}
}

void TotalConversionCompareDialog::OnGridMenu(Bar& bar)
{
	int row = grid_compare_.GetCursor();
	if (row < 0 || row >= old_song_.lines.GetCount()) return;

	bar.Add("Use Original Line (Keep)", [=] {
		selected_alt_idx_[row] = -1;
		SyncGrid();
	});

	int alt_count = response_.alternatives.GetCount();
	for (int j = 0; j < alt_count; j++) {
		bar.Add("Use " + response_.alternatives[j].name, [=] {
			selected_alt_idx_[row] = j;
			SyncGrid();
		});
	}
}

SongModel TotalConversionCompareDialog::GetResultSong() const
{
	SongModel res = old_song_;
	int alt_count = response_.alternatives.GetCount();
	String chosen_title;
	for (int i = 0; i < res.lines.GetCount(); i++) {
		int alt = selected_alt_idx_[i];
		if (alt >= 0 && alt < alt_count) {
			if (chosen_title.IsEmpty() && !response_.alternatives[alt].title.IsEmpty()) {
				chosen_title = response_.alternatives[alt].title;
			}
			if (i < response_.alternatives[alt].lines.GetCount()) {
				res.lines[i].text = response_.alternatives[alt].lines[i].text;
			}
		}
	}
	if (!chosen_title.IsEmpty()) {
		res.title = chosen_title;
	}
	return res;
}

