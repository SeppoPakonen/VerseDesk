#include "MainWindow.h"
#include "DiffDialog.h"
#include "TotalConversionDialog.h"

static int HexCharVal(char c) {
	if (c >= '0' && c <= '9') return c - '0';
	if (c >= 'a' && c <= 'f') return c - 'a' + 10;
	if (c >= 'A' && c <= 'F') return c - 'A' + 10;
	return 0;
}

static Color ParseHexColor(const String& hex)
{
	if (hex.StartsWith("#") && hex.GetLength() == 7) {
		int r = (HexCharVal(hex[1]) << 4) | HexCharVal(hex[2]);
		int g = (HexCharVal(hex[3]) << 4) | HexCharVal(hex[4]);
		int b = (HexCharVal(hex[5]) << 4) | HexCharVal(hex[6]);
		return Color(r, g, b);
	}
	return Null;
}

static Color GetMetaphoricalColor(const String& category, const String& text)
{
	String s = ToLower(category + " " + text);
	if (s.Find("love") >= 0 || s.Find("rakkaus") >= 0 || s.Find("romance") >= 0 || s.Find("desire") >= 0 || s.Find("passion") >= 0 || s.Find("intimacy") >= 0 || s.Find("kiss") >= 0 || s.Find("suudella") >= 0 || s.Find("himo") >= 0)
		return Color(255, 0, 0);
	if (s.Find("sad") >= 0 || s.Find("suru") >= 0 || s.Find("melanchol") >= 0 || s.Find("regret") >= 0 || s.Find("trace") >= 0 || s.Find("evidence") >= 0 || s.Find("symptom") >= 0 || s.Find("melankolia") >= 0 || s.Find("menetys") >= 0 || s.Find("ikävä") >= 0 || s.Find("kyynel") >= 0 || s.Find("vihje") >= 0 || s.Find("merkki") >= 0 || s.Find("katoaa") >= 0)
		return Color(0, 0, 255);
	if (s.Find("death") >= 0 || s.Find("kuolema") >= 0 || s.Find("dead") >= 0 || s.Find("void") >= 0 || s.Find("darkness") >= 0 || s.Find("pimeys") >= 0)
		return Color(0, 0, 0);
	if (s.Find("joy") >= 0 || s.Find("ilo") >= 0 || s.Find("party") >= 0 || s.Find("dance") >= 0 || s.Find("humor") >= 0 || s.Find("irony") >= 0 || s.Find("critic") >= 0 || s.Find("vitsi") >= 0 || s.Find("nauru") >= 0 || s.Find("hymy") >= 0 || s.Find("tanssi") >= 0 || s.Find("bileet") >= 0 || s.Find("naurettava") >= 0 || s.Find("koominen") >= 0)
		return Color(255, 165, 0);
	if (s.Find("rhyme") >= 0 || s.Find("riimi") >= 0 || s.Find("alliter") >= 0 || s.Find("alkusoin") >= 0 || s.Find("alkusointu") >= 0 || s.Find("sisäsointu") >= 0 || s.Find("sound") >= 0 || s.Find("ääni") >= 0 || s.Find("slang") >= 0 || s.Find("vocabulary") >= 0 || s.Find("urban") >= 0 || s.Find("sana") >= 0 || s.Find("kieli") >= 0)
		return Color(0, 180, 0);
	if (s.Find("meter") >= 0 || s.Find("syllable") >= 0 || s.Find("tavu") >= 0 || s.Find("rytmi") >= 0 || s.Find("rakenne") >= 0 || s.Find("kertosäe") >= 0 || s.Find("chorus") >= 0 || s.Find("verse") >= 0 || s.Find("säkeistö") >= 0 || s.Find("structure") >= 0 || s.Find("payoff") >= 0 || s.Find("setup") >= 0 || s.Find("callback") >= 0)
		return Color(0, 180, 180);
	if (s.Find("human") >= 0 || s.Find("mitigat") >= 0 || s.Find("tekoäly") >= 0 || s.Find("ai-tone") >= 0 || s.Find("hallucinat") >= 0 || s.Find("sensur") >= 0 || s.Find("mitigointi") >= 0)
		return Color(128, 0, 128);
	if (s.Find("conflict") >= 0 || s.Find("angst") >= 0 || s.Find("viha") >= 0 || s.Find("anger") >= 0 || s.Find("rage") >= 0 || s.Find("taistelu") >= 0 || s.Find("pressure") >= 0 || s.Find("paine") >= 0 || s.Find("rebellion") >= 0 || s.Find("kapina") >= 0 || s.Find("opponent") >= 0 || s.Find("vastustaja") >= 0 || s.Find("este") >= 0 || s.Find("obstacle") >= 0)
		return Color(139, 0, 0);
	if (s.Find("detail") >= 0 || s.Find("image") >= 0 || s.Find("concrete") >= 0 || s.Find("object") >= 0 || s.Find("yksityiskohta") >= 0 || s.Find("esine") >= 0 || s.Find("scene") >= 0 || s.Find("kuva") >= 0)
		return Color(197, 160, 89);
	return Color(128, 128, 128);
}


static String FormatRatingStars(int value)
{
	if (value <= 0)
		return "0";
	String out;
	for (int i = 0; i < value; i++)
		out << "*";
	return out;
}

String BuildTaskPrompt(const SongModel& song, const RuleSet& rules, const String& task_name)
{
	String task;
	if (task_name == "init") {
		task << "Task: Initialize songwriting metadata (colors, ratings, rhyme groups, and correction flags) for the song '" << song.title << "'.\n\n";
		task << "## Rules / Style Constraints\n";
		task << "Review the songwriting rules to understand the style targets:\n";
		for (const SongRule& r : rules.rules) {
			task << "- Rule [" << r.id << "] (" << r.category << "): " << r.text << "\n";
		}
		task << "\n";
		task << "## Metaphorical Colors Guidelines\n";
		task << "Assign a hex color code (e.g. \"#FF0000\") to each line matching its tone:\n";
		task << "- Love, intimacy, passion: red (\"#FF0000\")\n";
		task << "- Sadness, regret, melancholy: blue (\"#0000FF\")\n";
		task << "- Death, void, finality: black (\"#000000\")\n";
		task << "- Anger, tension, aggression: dark red / crimson (\"#8B0000\")\n";
		task << "- Joy, excitement, party, neon vibe: gold (\"#FFD700\") or orange (\"#FFA500\")\n";
		task << "- Calmness, transition, neutral description: grey (\"#808080\") or light blue (\"#ADD8E6\")\n\n";

		task << "## Assignment Instructions\n";
		task << "For each line in 'lines', fill in the following fields:\n";
		task << "1. 'color': Assign a metaphorical hex color based on the line's emotional tone.\n";
		task << "2. Quality ratings (on a -5 to 10 scale, 0 to 10 for magic/low friction):\n";
		task << "   - 'magic_score': assess if the line has that special songwriting spark.\n";
		task << "   - 'singability': assess flow, vocalization, rhythm.\n";
		task << "   - 'punchiness': assess impact, surprise, hook factor.\n";
		task << "   - 'satisfaction': assess emotional payoff.\n";
		task << "   - 'rhyme_quality': quality of rhymes with context.\n";
		task << "   - 'low_friction': assess whether the line is immediately easy to parse, sing, and follow without strain.\n";
		task << "3. Rhyme groups (use matching short codes e.g. \"A\", \"B\", \"C\" to group rhyming lines):\n";
		task << "   - 'rhyme_group_alliteration': lines starting with the same sounds (alkusointu).\n";
		task << "   - 'rhyme_group_internal': lines with internal rhymes (sisäsointu).\n";
		task << "   - 'rhyme_group_end': lines with end rhymes (loppusointu).\n";
		task << "4. Correction Need (0 if fine, 1-5 if needs work):\n";
		task << "   - 'correction_need': overall necessity to rewrite the line.\n";
		task << "   - 'correction_need_alliteration', 'correction_need_internal', 'correction_need_end'.\n\n";
 
		task << "Return ONLY a single TOON block representing the updated SongModel. Do not modify the lyrics text itself.\n\n";
	} else if (task_name == "eval") {
		task << "Task: Adherence Evaluation for the song '" << song.title << "'.\n\n";
		task << "## Rules / Style Constraints\n";
		task << "Evaluate the song's adherence to each of these rules on a 0-10 scale:\n";
		for (const SongRule& r : rules.rules) {
			task << "- Rule [" << r.id << "] (" << r.category << "): " << r.text << "\n";
		}
		task << "\n";
		task << "## Instructions\n";
		task << "1. Review the lyrics and assess how well they follow each rule.\n";
		task << "2. Fill in the 'ai_review' section:\n";
		task << "   - 'feedback': write a detailed summary feedback explaining which rules are broken/followed and advice.\n";
		task << "   - 'overall_magic': rate the song's overall magic score (0 to 10).\n";
		task << "   - 'rule_scores': an array containing the score for each rule (e.g. {\"id\": \"1\", \"score\": 8}).\n";
		task << "3. Return ONLY a single TOON block representing the updated SongModel. Do not modify the lyrics lines at all.\n\n";
	} else {
		task << "Task: Revise the lyrics of the song '" << song.title << "'.\n\n";
		task << "## Rules / Style Constraints\n";
		task << "You must follow these rules. Assess how well each rule is followed on a 0-10 scale:\n";
		for (const SongRule& r : rules.rules) {
			task << "- Rule [" << r.id << "] (" << r.category << ", Weight " << r.weight << "): " << r.text << "\n";
		}
		task << "\n";
		task << "## Song Metadata\n";
		task << "- Artist Name: " << song.artist << "\n";
		task << "- Author: " << song.author << "\n";
		task << "- Genre: " << song.genre << "\n";
		task << "- Suno Style: " << song.suno_style << "\n";
		task << "- Vocals Gender: " << song.gender << "\n";
		task << "- Dialect / Language style: " << song.dialect << "\n";
		task << "- Artist image/persona description: " << song.artist_image << "\n";
		task << "- Artist general style description: " << song.artist_style << "\n\n";
		
		task << "## Editing Directives\n";
		task << "1. Frozen lines ('\"frozen\": true') MUST NOT be modified or replaced.\n";
		task << "2. Only lines with non-zero 'correction_need' fields (or specific rhyme correction needs) should be modified.\n";
		task << "3. Pay attention to the 'comment' fields of individual lines for instructions.\n";
		task << "4. Re-evaluate the overall song and provide textual feedback in the 'feedback' section of 'ai_review'.\n";
		task << "5. Return ONLY a single TOON block representing the updated SongModel. Do not include markdown around it except standard fenced blocks.\n\n";
		
		task << "## Metaphorical Colors for Lines\n";
		task << "Each line in 'lines' has a 'color' field, which represents the emotional/thematic tone of that line. You must supply this color as a hex code (e.g. \"#FF0000\"). Use these guidelines:\n";
		task << "- Love, intimacy, passion: red (\"#FF0000\")\n";
		task << "- Sadness, regret, melancholy: blue (\"#0000FF\")\n";
		task << "- Death, void, finality: black (\"#000000\")\n";
		task << "- Anger, tension, aggression: dark red / crimson (\"#8B0000\")\n";
		task << "- Joy, excitement, party, neon vibe: gold (\"#FFD700\") or orange (\"#FFA500\")\n";
		task << "- Calmness, transition, neutral description: grey (\"#808080\") or light blue (\"#ADD8E6\")\n";
		task << "Please assign the best metaphorical color hex code to the 'color' field of each updated line.\n\n";
	}

	task << "## Current Song TOON Data\n";
	task << "```toon\n" << StoreAsTOON(song) << "\n```\n";
	return task;
}

void MainWindow::OnRatingSelectorChanged()
{
	int sel = ~rating_selector_;
	static const char* labels[] = { "Magic", "Singability", "Punchiness", "Satisfaction", "Rhyme Q", "Low Friction" };
	if (sel >= 0 && sel < 6) {
		lyrics_grid_.HeaderObject().Tab(4).SetText(labels[sel]);
		lyrics_grid_.HeaderObject().Refresh();
	}
	SyncGrid();
}


// ---------------------------------------------------------------------------
// Constructor

MainWindow::MainWindow()
{
	Title("LyricsEditor").Sizeable().Zoomable();

	AddFrame(menu_);
	menu_.Set(THISBACK(MainMenu));

	AddFrame(toolbar_);
	toolbar_.Set(THISBACK(UpdateToolBar));
	
	AddFrame(statusbar_);

	// Tab control inside main window
	tab_lyrics_.Add(lyrics_grid_.SizePos());
	tab_rules_.Add(rules_grid_.SizePos());

	// Setup lyrics grid columns
	lyrics_grid_.ColumnSortThreeState();
	lyrics_grid_.AddColumn("#", 30).SortingBy([](const Value& a, const Value& b) {
		return atoi(a.ToString()) < atoi(b.ToString()) ? -1 : (atoi(a.ToString()) > atoi(b.ToString()) ? 1 : 0);
	});
	lyrics_grid_.AddColumn("Part", 120).Sorting();
	lyrics_grid_.AddColumn("Lyrics Text", 400).Edit(edit_lyrics_text).Sorting();
	lyrics_grid_.AddColumn("Freeze", 60).Sorting();
	lyrics_grid_.AddColumn("Magic", 60).SortingBy([](const Value& a, const Value& b) {
		String sa = a.ToString();
		String sb = b.ToString();
		int val_a = (sa == "0") ? 0 : sa.GetLength();
		int val_b = (sb == "0") ? 0 : sb.GetLength();
		return val_a < val_b ? -1 : (val_a > val_b ? 1 : 0);
	});
	lyrics_grid_.AddColumn("Alku-R", 60).Sorting();
	lyrics_grid_.AddColumn("Sisä-R", 60).Sorting();
	lyrics_grid_.AddColumn("Loppu-R", 60).Sorting();
	lyrics_grid_.AddColumn("Corr", 50).SortingBy([](const Value& a, const Value& b) {
		return atoi(a.ToString()) < atoi(b.ToString()) ? -1 : (atoi(a.ToString()) > atoi(b.ToString()) ? 1 : 0);
	});
	
	lyrics_grid_.WhenSel = THISBACK(OnGridCursor);
	lyrics_grid_.WhenLeftDouble = THISBACK(FreezeSelectedLines);
	lyrics_grid_.WhenUpdateRow = THISBACK(OnLyricsGridUpdateRow);
	lyrics_grid_.WhenBar = THISBACK(LyricsGridMenu);
	lyrics_grid_.SetLineCy(EditField::GetStdHeight() + DPI(4));

	// Setup rules grid columns
	rules_grid_.ColumnSortThreeState();
	rules_grid_.AddColumn("Rule ID", 80).Edit(edit_rule_id).SortingBy([](const Value& a, const Value& b) {
		String sa = a.ToString();
		String sb = b.ToString();
		bool a_is_num = IsDigit(sa[0]);
		bool b_is_num = IsDigit(sb[0]);
		if (a_is_num && b_is_num) {
			return atoi(~sa) < atoi(~sb) ? -1 : (atoi(~sa) > atoi(~sb) ? 1 : 0);
		}
		return sa < sb ? -1 : (sa > sb ? 1 : 0);
	});
	rules_grid_.AddColumn("Category", 120).Edit(edit_rule_cat).Sorting();
	rules_grid_.AddColumn("Weight", 80).Edit(edit_rule_weight).SortingBy([](const Value& a, const Value& b) {
		return atoi(a.ToString()) < atoi(b.ToString()) ? -1 : (atoi(a.ToString()) > atoi(b.ToString()) ? 1 : 0);
	});
	rules_grid_.AddColumn("Rule Description", 600).Edit(edit_rule_desc).Sorting();

	rules_grid_.WhenUpdateRow = THISBACK(OnRulesGridUpdateRow);
	rules_grid_.WhenBar = THISBACK(RulesGridMenu);
	rules_grid_.SetLineCy(EditField::GetStdHeight() + DPI(4));

	main_tabs_.Add(tab_lyrics_.SizePos(), "Lyrics Editor");
	main_tabs_.Add(tab_rules_.SizePos(), "Rules");
	main_tabs_.Add(tab_history_.SizePos(), "Version History");
	main_tabs_.Add(debug_tab_.SizePos(), "Log");
	main_tabs_.WhenSet = [=] { OnMainTabChanged(); };

	tab_history_.Add(history_grid_.SizePos());
	history_grid_.ColumnSortThreeState();
	history_grid_.AddColumn("Date & Time", 150).Sorting();
	history_grid_.AddColumn("Magic Score", 80).SortingBy([](const Value& a, const Value& b) {
		return atof(a.ToString()) < atof(b.ToString()) ? -1 : (atof(a.ToString()) > atof(b.ToString()) ? 1 : 0);
	});
	history_grid_.AddColumn("Author", 100).Sorting();
	history_grid_.AddColumn("File", 250).Sorting();
	history_grid_.WhenLeftDouble = THISBACK(RestoreHistoryVersion);

	Add(main_tabs_.SizePos());

	// Wire Dockable change events
	dock_metadata_.WhenChanged = THISBACK(SyncUI);
	dock_line_eval_.WhenChanged = THISBACK(UpdateSelectedLineEval);
	dock_sections_.WhenChanged = [=] {
		modified_ = true;
		SaveUndoState();
		SyncGrid();
		SyncTitle();
	};

	// Setup rating selector dropdown
	rating_selector_.Add(0, "Magic");
	rating_selector_.Add(1, "Singability");
	rating_selector_.Add(2, "Punchiness");
	rating_selector_.Add(3, "Satisfaction");
	rating_selector_.Add(4, "Rhyme Quality");
	rating_selector_.Add(5, "Low Friction");
	rating_selector_.SetData(0);
	rating_selector_.WhenAction = THISBACK(OnRatingSelectorChanged);
}

// ---------------------------------------------------------------------------
// DockInit

void MainWindow::DockInit()
{
	log_.WhenRecord = [=](const AppLogRecord& r) { debug_tab_.AddRecord(r); };

	InitDockers();
	OnResetDockLayout();
	CacheDefaultLayout();
	InitRegistry();
	LoadAppState();
	
	if (!LoadUserLayout())
		Log("layout: using default docking positions");
		
	loaded_ = true;

	// Load last open files or defaults
	if (!current_project_path_.IsEmpty() && FileExists(current_project_path_)) {
		OpenProjectFile(current_project_path_);
	}
	if (!current_rules_path_.IsEmpty() && FileExists(current_rules_path_)) {
		OpenRulesFile(current_rules_path_);
	} else {
		LoadDefaultRules("fi"); // default to Finnish rules
	}

	Log("registry config: " + registry_.GetConfigDir());
	toolbar_.Set(THISBACK(UpdateToolBar));
}

void MainWindow::Close()
{
	if (modified_) {
		int ret = PromptYesNoCancel("Tiedostoa on muutettu. Haluatko tallentaa muutokset?");
		if (ret == 1) {
			SaveProject();
			if (modified_) return; // Save cancelled or failed, don't exit
		} else if (ret == -1) {
			return; // Cancel exit
		}
	}

	if (loaded_) {
		SaveUserLayout();
		SaveAppState();
		registry_.Save();
		Log("registry: saved");
	}
	TopWindow::Close();
}

// ---------------------------------------------------------------------------
// Docker registration

void MainWindow::InitDockers()
{
	dock_metadata_.Title("Metadata").SizeHint(Size(280, 400));
	Register(dock_metadata_);

	dock_line_eval_.Title("Line Evaluator").SizeHint(Size(280, 400));
	Register(dock_line_eval_);

	dock_ai_review_.Title("AI Review").SizeHint(Size(320, 400));
	Register(dock_ai_review_);

	dock_sections_.Title("Part Attributes").SizeHint(Size(280, 400));
	Register(dock_sections_);
}

void MainWindow::OnResetDockLayout()
{
	for (DockableCtrl* dc : GetDockableCtrls())
		DockWindow::Close(*dc);

	DockLeft(dock_metadata_);
	DockRight(dock_sections_);
	DockRight(dock_line_eval_);
	DockBottom(dock_ai_review_);

	Log("layout: reset default docks");
}

void MainWindow::CacheDefaultLayout()
{
	StringStream out;
	SerializeWindow(out);
	default_layout_data_ = out.GetResult();
}

void MainWindow::SetDefaultLayout()
{
	if (default_layout_data_.IsEmpty()) return;
	for (DockableCtrl* dc : GetDockableCtrls())
		DockWindow::Close(*dc);
	StringStream in(default_layout_data_);
	SerializeWindow(in);
}

void MainWindow::SetDockVisible(DockableCtrl& dc, bool visible)
{
	if (!IsOpen()) { dc.Show(visible); return; }
	if (visible)  { if (dc.IsHidden()) RestoreDockerPos(dc, false); }
	else         { if (!dc.IsHidden()) DockWindow::Close(dc); }
}

// ---------------------------------------------------------------------------
// AppRegistry Integration

static String sTabLayoutKey(int tab)
{
	switch (tab) {
	case 1:  return "dock.layout.rules";
	case 2:  return "dock.layout.debug";
	default: return "dock.layout.lyrics";
	}
}

void MainWindow::InitRegistry()
{
	registry_.Vendor("AiUpp").AppId("LyricsEditor").Profile("default").SetLog(&log_);
	registry_.Load();
	Log("registry: loaded");
}

bool MainWindow::LoadUserLayout()
{
	String data;
	if (!registry_.LoadBlob(sTabLayoutKey(current_tab_), data)) {
		return false;
	}
	StringStream in(data);
	SerializeWindow(in);
	return true;
}

void MainWindow::SaveUserLayout()
{
	if (!IsOpen()) return;
	StringStream out;
	SerializeWindow(out);
	String data = out.GetResult();
	registry_.SaveBlob(sTabLayoutKey(current_tab_), data);
}

void MainWindow::LoadAppState()
{
	AppState st;
	if (!registry_.LoadJson("app.state", st)) {
		return;
	}
	// Restore active tab
	int tab = -1;
	for (int i = 0; i < main_tabs_.GetCount(); i++) {
		if (main_tabs_.GetItem(i).GetText() == st.active_tab_name) {
			tab = i;
			break;
		}
	}
	if (tab >= 0) {
		main_tabs_.Set(tab);
		current_tab_ = tab;
	}

	// Retrieve last paths from registry values
	current_project_path_ = registry_.Get("project_file", "");
	current_rules_path_ = registry_.Get("rules_file", "");
	recent_project_files_ <<= st.recent_project_files;
}

void MainWindow::SaveAppState()
{
	AppState st;
	int tab = main_tabs_.Get();
	st.active_tab_name = main_tabs_.GetItem(tab).GetText();
	st.active_tab_index = tab;
	st.editor_mode = 0; // standard view
	st.recent_project_files <<= recent_project_files_;
	registry_.SaveJson("app.state", st);

	registry_.Set("project_file", current_project_path_);
	registry_.Set("rules_file", current_rules_path_);
}

void MainWindow::AddRecentProjectFile(const String& path)
{
	if (path.IsEmpty())
		return;

	for (int i = 0; i < recent_project_files_.GetCount(); i++) {
		if (recent_project_files_[i] == path) {
			recent_project_files_.Remove(i);
			break;
		}
	}

	recent_project_files_.Insert(0, path);
	while (recent_project_files_.GetCount() > 10) {
		recent_project_files_.Drop();
	}
}

String MainWindow::GetProjectDialogDir() const
{
	if (!current_project_path_.IsEmpty())
		return GetFileDirectory(current_project_path_);
	return GetCurrentDirectory();
}

String MainWindow::GetRulesDialogDir() const
{
	if (!current_rules_path_.IsEmpty())
		return GetFileDirectory(current_rules_path_);
	return GetCurrentDirectory();
}

// ---------------------------------------------------------------------------
// Actions and Sync

void MainWindow::SyncTitle()
{
	String title = "Lyrics Editor - ";
	if (!current_project_path_.IsEmpty()) {
		title << current_project_path_;
	} else {
		title << "[New Project]";
	}
	if (modified_) {
		title << " *";
	}
	Title(title);
}

void MainWindow::SyncGrid()
{
	SyncTitle();

	int sel = lyrics_grid_.GetCursor();
	int scroll = lyrics_grid_.GetScroll();
	lyrics_grid_.Clear();
	
	String prev_section = "";
	int selected_metric = GetValueInt(~rating_selector_);

	for (int i = 0; i < song_.lines.GetCount(); i++) {
		const LyricsLine& line = song_.lines[i];
		bool is_first_line_of_section = (line.section != prev_section);
		prev_section = line.section;
		
		Color meta_c = ParseHexColor(line.color);
		Color base_bg = Color(255, 255, 255);
		if (!IsNull(meta_c)) {
			base_bg = Blend(meta_c, Color(255, 255, 255));
		} else if (line.correction_need >= 3) {
			base_bg = Color(255, 230, 230); // light pink warning
		} else if (!line.section.IsEmpty()) {
			int hash = 0;
			for (int ch = 0; ch < line.section.GetCount(); ch++) {
				hash = hash * 31 + line.section[ch];
			}
			int choice = abs(hash) % 5;
			switch (choice) {
				case 0: base_bg = Color(245, 250, 245); break; // soft green
				case 1: base_bg = Color(253, 248, 240); break; // soft apricot
				case 2: base_bg = Color(250, 245, 253); break; // soft purple
				case 3: base_bg = Color(240, 250, 253); break; // soft blue
				default: base_bg = Color(255, 255, 255); break;
			}
		}

		Color bg = line.frozen ? Blend(base_bg, Color(220, 220, 220)) : base_bg;
		Color fg = line.frozen ? Color(0, 102, 204) : Color(30, 30, 30);
		
		auto make_cell = [&](const String& txt) {
			return AttrText(txt).NormalPaper(bg).NormalInk(fg);
		};
		auto make_bold_cell = [&](const String& txt, bool bold) {
			return AttrText(txt).NormalPaper(bg).NormalInk(fg).Bold(bold);
		};
		
		String metric_val;
		switch (selected_metric) {
			case 0: metric_val = FormatRatingStars(line.magic_score); break;
			case 1: metric_val = FormatRatingStars(line.singability); break;
			case 2: metric_val = FormatRatingStars(line.punchiness); break;
			case 3: metric_val = FormatRatingStars(line.satisfaction); break;
			case 4: metric_val = FormatRatingStars(line.rhyme_quality); break;
			case 5: metric_val = FormatRatingStars(line.low_friction); break;
			default: metric_val = ""; break;
		}
		
		String status_str = line.frozen ? "Locked" : "Edit";
		
		lyrics_grid_.Add(
			make_cell(Format("%d", i + 1)),
			make_bold_cell(line.section + (line.attributes.IsEmpty() ? "" : ", " + line.attributes), is_first_line_of_section),
			AttrText(line.text).NormalPaper(bg).NormalInk(fg).Bold(!line.frozen),
			make_cell(status_str),
			make_cell(metric_val),
			make_cell(line.rhyme_group_alliteration),
			make_cell(line.rhyme_group_internal),
			make_cell(line.rhyme_group_end),
			make_cell(Format("%d", line.correction_need))
		);
	}
	
	if (sel >= 0 && sel < lyrics_grid_.GetCount())
		lyrics_grid_.SetCursor(sel);
	lyrics_grid_.ScrollTo(scroll);
}

void MainWindow::SyncRulesGrid()
{
	int sel = rules_grid_.GetCursor();
	int scroll = rules_grid_.GetScroll();
	rules_grid_.Clear();
	for (int i = 0; i < rules_.rules.GetCount(); i++) {
		const SongRule& rule = rules_.rules[i];
		Color meta_c = GetMetaphoricalColor(rule.category, rule.text);
		Color bg = Blend(meta_c, Color(255, 255, 255));
		Color fg = Color(0, 0, 0);
		
		auto make_cell = [&](const String& txt) {
			return AttrText(txt).NormalPaper(bg).NormalInk(fg);
		};
		
		rules_grid_.Add(
			make_cell(rule.id),
			make_cell(rule.category),
			make_cell(Format("%d", rule.weight)),
			make_cell(rule.text),
			i
		);
	}
	if (sel >= 0 && sel < rules_grid_.GetCount()) {
		rules_grid_.SetCursor(sel);
	}
	rules_grid_.ScrollTo(scroll);
}

void MainWindow::OnGridCursor()
{
	int row = lyrics_grid_.GetCursor();
	if (row >= 0 && row < lyrics_grid_.GetCount()) {
		int orig_idx = atoi(lyrics_grid_.Get(row, 0).ToString()) - 1;
		if (orig_idx >= 0 && orig_idx < song_.lines.GetCount()) {
			dock_line_eval_.Load(song_.lines[orig_idx]);
			return;
		}
	}
	dock_line_eval_.Clear();
}

void MainWindow::UpdateSelectedLineEval()
{
	int row = lyrics_grid_.GetCursor();
	if (row >= 0 && row < lyrics_grid_.GetCount()) {
		int orig_idx = atoi(lyrics_grid_.Get(row, 0).ToString()) - 1;
		if (orig_idx >= 0 && orig_idx < song_.lines.GetCount()) {
			dock_line_eval_.Save(song_.lines[orig_idx]);
			SaveUndoState();

			LyricsLine& line = song_.lines[orig_idx];
			int selected_metric = GetValueInt(~rating_selector_);
			String metric_val;
			switch (selected_metric) {
				case 0: metric_val = FormatRatingStars(line.magic_score); break;
				case 1: metric_val = FormatRatingStars(line.singability); break;
				case 2: metric_val = FormatRatingStars(line.punchiness); break;
				case 3: metric_val = FormatRatingStars(line.satisfaction); break;
				case 4: metric_val = FormatRatingStars(line.rhyme_quality); break;
				case 5: metric_val = FormatRatingStars(line.low_friction); break;
			}

			Color meta_c = ParseHexColor(line.color);
			Color base_bg = Color(255, 255, 255);
			if (!IsNull(meta_c)) {
				base_bg = Blend(meta_c, Color(255, 255, 255));
			} else if (line.correction_need >= 3) {
				base_bg = Color(255, 230, 230);
			}

			Color bg = line.frozen ? Blend(base_bg, Color(220, 220, 220)) : base_bg;
			Color fg = line.frozen ? Color(0, 102, 204) : Color(30, 30, 30);

			auto make_cell = [&](const String& txt) {
				return AttrText(txt).NormalPaper(bg).NormalInk(fg);
			};

			lyrics_grid_.Set(row, 2, AttrText(line.text).NormalPaper(bg).NormalInk(fg).Bold(!line.frozen));
			lyrics_grid_.Set(row, 3, make_cell(line.frozen ? "Locked" : "Edit"));
			lyrics_grid_.Set(row, 4, make_cell(metric_val));
			lyrics_grid_.Set(row, 5, make_cell(line.rhyme_group_alliteration));
			lyrics_grid_.Set(row, 6, make_cell(line.rhyme_group_internal));
			lyrics_grid_.Set(row, 7, make_cell(line.rhyme_group_end));
			lyrics_grid_.Set(row, 8, make_cell(Format("%d", line.correction_need)));
		}
	}
}

void MainWindow::FreezeSelectedLines()
{
	bool changed = false;
	bool has_selection = false;
	for (int r = 0; r < lyrics_grid_.GetCount(); r++) {
		if (lyrics_grid_.IsSelected(r)) {
			has_selection = true;
			int orig_idx = atoi(lyrics_grid_.Get(r, 0).ToString()) - 1;
			if (orig_idx >= 0 && orig_idx < song_.lines.GetCount()) {
				song_.lines[orig_idx].frozen = !song_.lines[orig_idx].frozen;
				changed = true;
			}
		}
	}
	if (!has_selection) {
		int row = lyrics_grid_.GetCursor();
		if (row >= 0 && row < lyrics_grid_.GetCount()) {
			int orig_idx = atoi(lyrics_grid_.Get(row, 0).ToString()) - 1;
			if (orig_idx >= 0 && orig_idx < song_.lines.GetCount()) {
				song_.lines[orig_idx].frozen = !song_.lines[orig_idx].frozen;
				changed = true;
			}
		}
	}
	if (changed) {
		SaveUndoState();
		SyncGrid();
		OnGridCursor();
	}
}

void MainWindow::SyncUI()
{
	dock_metadata_.Save(song_);
	SaveUndoState();
	SyncGrid();
}

void MainWindow::SaveUndoState()
{
	modified_ = true;
	SyncTitle();

	if (current_project_path_.IsEmpty() || song_.title.IsEmpty())
		return;

	String dir = AppendFileName(GetFileDirectory(current_project_path_), "undo_" + song_.title);
	RealizeDirectory(dir);

	String time_str = FormatTime(GetSysTime(), "YYYYMMDD_hhmmss");
	String undo_file = AppendFileName(dir, time_str + "_" + GetFileTitle(current_project_path_) + ".json");
	
	StoreAsJsonFile(song_, undo_file);
	Log("Undo history trace written: " + undo_file);
}

// ---------------------------------------------------------------------------
// File Operations

void MainWindow::OpenProject()
{
	FileSel fs;
	fs.Type("JSON Song Files", "*.json");
	fs.ActiveDir(GetProjectDialogDir());
	if (fs.ExecuteOpen("Open Song Project")) {
		OpenProjectFile(fs.Get());
	}
}

void MainWindow::OpenProjectFile(const String& path)
{
	if (!FileExists(path)) return;
	song_ = SongModel();
	
	if (LoadFromJsonFile(song_, path)) {
		current_project_path_ = path;
		AddRecentProjectFile(path);
		modified_ = false;
		dock_metadata_.Load(song_);
		dock_sections_.Load(song_);
		SyncGrid();
		dock_ai_review_.Load(song_.ai_review, rules_);
		Log("Opened project: " + path);
		
		// Auto-load matching default rules if available
		LoadDefaultRules(song_.language);
	} else {
		PromptOK("Lataus epäonnistui: Virheellinen JSON tiedosto.");
	}
}

void MainWindow::SaveProject()
{
	if (current_project_path_.IsEmpty()) {
		SaveProjectAs();
	} else {
		SaveProjectFile(current_project_path_);
	}
}

void MainWindow::SaveProjectAs()
{
	FileSel fs;
	fs.Type("JSON Song Files", "*.json");
	fs.ActiveDir(GetProjectDialogDir());
	if (fs.ExecuteSaveAs("Save Song Project As")) {
		SaveProjectFile(fs.Get());
	}
}

void MainWindow::SaveProjectFile(const String& path)
{
	dock_metadata_.Save(song_);
	if (StoreAsJsonFile(song_, path)) {
		current_project_path_ = path;
		AddRecentProjectFile(path);
		Log("Saved project: " + path);
		statusbar_.Set("Saved " + path);
		SetTimeCallback(1000, THISBACK(ClearStatusBar));
		SaveUndoState();
		modified_ = false;
		SyncTitle();
		SyncGrid(); // Refresh title
	} else {
		PromptOK("Tallennus epäonnistui.");
	}
}

void MainWindow::OpenRules()
{
	FileSel fs;
	fs.Type("JSON Rule Files", "*.json");
	fs.ActiveDir(GetRulesDialogDir());
	if (fs.ExecuteOpen("Open Rule Set")) {
		OpenRulesFile(fs.Get());
	}
}

void MainWindow::OpenRulesFile(const String& path)
{
	if (!FileExists(path)) return;
	rules_ = RuleSet();
	
	if (LoadFromJsonFile(rules_, path)) {
		current_rules_path_ = path;
		SyncRulesGrid();
		dock_ai_review_.Load(song_.ai_review, rules_);
		Log("Loaded rule set: " + path);
	} else {
		PromptOK("Sääntöjen lataus epäonnistui.");
	}
}

void MainWindow::SaveRules()
{
	if (current_rules_path_.IsEmpty()) {
		SaveRulesAs();
	} else {
		SaveRulesFile(current_rules_path_);
	}
}

void MainWindow::SaveRulesAs()
{
	FileSel fs;
	fs.Type("JSON Rule Files", "*.json");
	fs.ActiveDir(GetRulesDialogDir());
	if (fs.ExecuteSaveAs("Save Rule Set As")) {
		SaveRulesFile(fs.Get());
	}
}

void MainWindow::SaveRulesFile(const String& path)
{
	if (StoreAsJsonFile(rules_, path)) {
		current_rules_path_ = path;
		Log("Saved rule set: " + path);
	} else {
		PromptOK("Sääntöjen tallennus epäonnistui.");
	}
}

void MainWindow::LoadDefaultRules(const String& lang)
{
	String filename = Format("rules_%s.json", lang.IsEmpty() ? "fi" : lang);
	String path = AppendFileName(AppendFileName(GetFileDirectory(GetExeFilePath()), "rules"), filename);
	if (!FileExists(path)) {
		path = AppendFileName(ConfigFile("rules"), filename);
	}
	if (!FileExists(path)) {
		path = AppendFileName(GetFileDirectory(GetFileDirectory(GetExeFilePath())), "src/LyricsEditor/rules/" + filename);
	}
	if (!FileExists(path)) {
		String dir = GetFileDirectory(GetExeFilePath());
		for (int i = 0; i < 5; i++) {
			String test = AppendFileName(dir, "src/LyricsEditor/rules/" + filename);
			if (FileExists(test)) {
				path = test;
				break;
			}
			String up = GetFileDirectory(dir);
			if (up == dir) break;
			dir = up;
		}
	}

	if (FileExists(path)) {
		OpenRulesFile(path);
	} else {
		Log("Default rules not found for language: " + lang);
	}
}

// ---------------------------------------------------------------------------
// Clipboard and AI Review

void MainWindow::CopyTaskToClipboardWith(String task_name)
{
	if (song_.title.IsEmpty()) {
		PromptOK("Luo tai avaa biisiprojekti ensin.");
		return;
	}

	String task = BuildTaskPrompt(song_, rules_, task_name);
	WriteClipboardText(task);
	Log("Copy Task (" + task_name + ") to Clipboard: Done.");
	
	statusbar_.Set("Kopioitu: " + task_name);
	SetTimeCallback(1000, THISBACK(ClearStatusBar));
}

void MainWindow::PasteResultFromClipboardWith(String task_name)
{
	String clip = ReadClipboardText();
	if (clip.IsEmpty()) {
		PromptOK("Leikepöytä on tyhjä.");
		return;
	}

	// Extract TOON or JSON
	String trimmed = TrimBoth(clip);
	String toon_text = trimmed;
	int start = clip.Find("```toon");
	if (start >= 0) {
		start += 7;
		int end = clip.ReverseFind("```");
		if (end >= 0) {
			toon_text = TrimBoth(clip.Mid(start, end - start));
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
				Vector<String> lines = Split(trimmed, '\n');
				int min_indent = INT_MAX;
				for (const String& line : lines) {
					String cur = line;
					if (cur.EndsWith("\r"))
						cur = cur.Left(cur.GetCount() - 1);
					if (TrimBoth(cur).IsEmpty())
						continue;
					int indent = 0;
					while (indent < cur.GetCount()) {
						char c = cur[indent];
						if (c != ' ' && c != '\t')
							break;
						indent++;
					}
					if (indent < min_indent)
						min_indent = indent;
				}
				if (min_indent > 0 && min_indent < INT_MAX) {
					String out;
					for (const String& line : lines) {
						String cur = line;
						if (cur.EndsWith("\r"))
							cur = cur.Left(cur.GetCount() - 1);
						if (cur.GetCount() >= min_indent)
							cur = cur.Mid(min_indent);
						out << cur << '\n';
					}
					toon_text = TrimBoth(out);
				} else {
					toon_text = trimmed;
				}
			}
		}
	}

	toon_text = NormalizeToonArrayCounts(toon_text);

	SongModel new_song;
	bool loaded = false;
	if (LoadFromTOON(new_song, toon_text)) {
		loaded = true;
	} else if (LoadFromJson(new_song, toon_text)) {
		loaded = true;
	}

	if (loaded) {
		if (task_name == "init") {
			// Merge rating metadata, rhyme groups and color
			for (int i = 0; i < song_.lines.GetCount() && i < new_song.lines.GetCount(); i++) {
				song_.lines[i].color = new_song.lines[i].color;
				song_.lines[i].magic_score = new_song.lines[i].magic_score;
				song_.lines[i].singability = new_song.lines[i].singability;
				song_.lines[i].punchiness = new_song.lines[i].punchiness;
				song_.lines[i].satisfaction = new_song.lines[i].satisfaction;
				song_.lines[i].rhyme_quality = new_song.lines[i].rhyme_quality;
				song_.lines[i].low_friction = new_song.lines[i].low_friction;
				song_.lines[i].rhyme_group_alliteration = new_song.lines[i].rhyme_group_alliteration;
				song_.lines[i].rhyme_group_internal = new_song.lines[i].rhyme_group_internal;
				song_.lines[i].rhyme_group_end = new_song.lines[i].rhyme_group_end;
				song_.lines[i].correction_need = new_song.lines[i].correction_need;
				song_.lines[i].correction_need_alliteration = new_song.lines[i].correction_need_alliteration;
				song_.lines[i].correction_need_internal = new_song.lines[i].correction_need_internal;
				song_.lines[i].correction_need_end = new_song.lines[i].correction_need_end;
			}
			song_.ai_review = new_song.ai_review;
			SaveUndoState();
			SyncGrid();
			dock_metadata_.Load(song_);
			dock_sections_.Load(song_);
			int row = lyrics_grid_.GetCursor();
			if (row >= 0 && row < song_.lines.GetCount()) {
				dock_line_eval_.Load(song_.lines[row]);
			}
			dock_ai_review_.Load(song_.ai_review, rules_);
			Log("Applied AI initialization metadata changes.");
			
			statusbar_.Set("Alustus valmis!");
			SetTimeCallback(1000, THISBACK(ClearStatusBar));
		}
		else if (task_name == "eval") {
			// Only merge AI review
			song_.ai_review = new_song.ai_review;
			SaveUndoState();
			SyncGrid();
			dock_metadata_.Load(song_);
			dock_sections_.Load(song_);
			dock_ai_review_.Load(song_.ai_review, rules_);
			Log("Applied AI rules adherence evaluation.");
			
			statusbar_.Set("Sääntöarviointi valmis!");
			SetTimeCallback(1000, THISBACK(ClearStatusBar));
		}
		else {
			// Revise
			DiffDialog dlg(song_, new_song);
			if (dlg.Execute() == IDOK) {
				song_ = dlg.GetResultSong();
				SaveUndoState();
				SyncGrid();
				dock_metadata_.Load(song_);
				dock_sections_.Load(song_);
				dock_ai_review_.Load(song_.ai_review, rules_);
				
				if (dlg.task_requested) {
					String feedback = dlg.GetFeedbackText();
					String task = ReadClipboardText();
					task << "\n## ITERATIVE USER FEEDBACK (Re-refine based on this):\n";
					task << feedback << "\n\n";
					task << "Please correct the song and output the updated TOON.\n";
					WriteClipboardText(task);
					Log("Iteration task generated and copied to clipboard.");
					
					statusbar_.Set("Iteraatiotehtävä kopioitu!");
					SetTimeCallback(1000, THISBACK(ClearStatusBar));
				} else {
					Log("Pasted and applied AI revision changes.");
					
					statusbar_.Set("Muutokset päivitetty!");
					SetTimeCallback(1000, THISBACK(ClearStatusBar));
				}
			}
		}
	} else {
		PromptOK("Lataus epäonnistui. Varmista että leikepöydällä on validi TOON tai JSON -rakenne.");
	}
}

void MainWindow::CopyTaskToClipboard()
{
	CopyTaskToClipboardWith("revise");
}

void MainWindow::PasteResultFromClipboard()
{
	PasteResultFromClipboardWith("revise");
}

void MainWindow::PasteLyrics()
{
	if (song_.lines.GetCount() > 0) {
		if (!PromptYesNo("Taulukko ei ole tyhjä. Haluatko korvata olemassa olevat sanat uusin sanoituksin?"))
			return;
	}

	String clip = ReadClipboardText();
	if (clip.IsEmpty()) {
		PromptOK("Leikepöytä on tyhjä.");
		return;
	}

	song_.lines.Clear();
	
	String current_section = "Intro";
	String current_attrs = "";

	Vector<String> raw_lines = Split(clip, '\n');
	for (int i = 0; i < raw_lines.GetCount(); i++) {
		String line = TrimBoth(raw_lines[i]);
		if (line.IsEmpty()) continue;

		if (line.StartsWith("[") && line.EndsWith("]")) {
			String content = line.Mid(1, line.GetLength() - 2);
			Vector<String> parts = Split(content, ',');
			if (parts.GetCount() > 0) {
				current_section = TrimBoth(parts[0]);
				current_attrs = "";
				for (int p = 1; p < parts.GetCount(); p++) {
					if (!current_attrs.IsEmpty()) current_attrs << ", ";
					current_attrs << TrimBoth(parts[p]);
				}
			}
		} else {
			LyricsLine ll;
			ll.text = line;
			ll.section = current_section;
			ll.attributes = current_attrs;
			song_.lines.Add(ll);
		}
	}

	SaveUndoState();
	SyncGrid();
	Log("Pasted lyrics from clipboard: " + Format("%d", song_.lines.GetCount()) + " lines parsed.");
}

void MainWindow::CopyLyricsToClipboard()
{
	if (song_.lines.IsEmpty()) {
		PromptOK("Biisissä ei ole sanoja.");
		return;
	}

	String text;
	String last_sec = "";
	String last_attrs = "";

	for (int i = 0; i < song_.lines.GetCount(); i++) {
		const LyricsLine& line = song_.lines[i];
		if (line.section != last_sec || line.attributes != last_attrs) {
			if (i > 0) text << "\n";
			text << "[" << line.section;
			if (!line.attributes.IsEmpty()) text << ", " << line.attributes;
			text << "]\n";
			last_sec = line.section;
			last_attrs = line.attributes;
		}
		text << line.text << "\n";
	}

	WriteClipboardText(text);
	Log("Lyrics copied to clipboard.");
	PromptOK("Sanat kopioitu leikepöydälle muotoiltuna!");
}

// ---------------------------------------------------------------------------
// Tab Coordination

void MainWindow::UpdateToolBar(Bar& bar)
{
	int tab = main_tabs_.Get();
	if (tab == 0)      ToolBarTabLyrics(bar);
	else if (tab == 1) ToolBarTabRules(bar);
	else if (tab == 2) {
		bar.Add("Refresh History", CtrlImg::open(), [=] { SyncHistoryGrid(); });
		bar.Add("Restore Selected", CtrlImg::go_back(), THISBACK(RestoreHistoryVersion));
	}
	else               ToolBarTabDebug(bar);
}

void MainWindow::OnMainTabChanged()
{
	int next = main_tabs_.Get();
	if (!loaded_ || next == current_tab_) return;

	if (next != 0) {
		SaveUserLayout();
		for (DockableCtrl* dc : GetDockableCtrls())
			DockWindow::Close(*dc);
	} else {
		if (!LoadUserLayout())
			SetDefaultLayout();
	}

	current_tab_ = next;

	if (next == 2) {
		SyncHistoryGrid();
	}

	toolbar_.Set(THISBACK(UpdateToolBar));
}

void MainWindow::OnSaveLayoutAs()
{
	String name;
	if (EditText(name, "Save Layout", "Name:") && !name.IsEmpty())
		SaveLayout(name);
}

void MainWindow::OnLoadLayout()
{
	DockManager();
}

void MainWindow::CopyTotalConversionTask()
{
	if (song_.lines.IsEmpty()) {
		PromptOK("Biisissä ei ole sanoja. Lisää ne ensin.");
		return;
	}

	TotalConversionDialog dlg;
	if (dlg.Execute() == IDOK) {
		String instructions = (~dlg.edit_instructions_).ToString();
		if (instructions.IsEmpty()) {
			PromptOK("Ohjeistus ei voi olla tyhjä.");
			return;
		}

		String task;
		task << "Task: Total conversion of the song '" << song_.title << "'.\n\n";
		task << "## User Request / Conversion Goal\n";
		task << instructions << "\n\n";

		task << "## Song Metadata\n";
		task << "- Artist Name: " << song_.artist << "\n";
		task << "- Author: " << song_.author << "\n";
		task << "- Genre: " << song_.genre << "\n";
		task << "- Suno Style: " << song_.suno_style << "\n";
		task << "- Vocals Gender: " << song_.gender << "\n";
		task << "- Dialect / Language style: " << song_.dialect << "\n";
		task << "- Artist image/persona description: " << song_.artist_image << "\n";
		task << "- Artist general style description: " << song_.artist_style << "\n\n";

		task << "## Songwriting Rules to Follow\n";
		for (const SongRule& r : rules_.rules) {
			task << "- Rule [" << r.id << "] (" << r.category << "): " << r.text << "\n";
		}
		task << "\n";

		task << "## Editing Instructions\n";
		task << "1. Rewrite the lyrics to satisfy the conversion goal and songwriting rules.\n";
		task << "2. Keep the exact same structure (same sections/parts and same line count).\n";
		task << "3. Frozen lines ('\"frozen\": true') MUST NOT be modified under any circumstances.\n";
		task << "4. Rename the song as part of the conversion. Every alternative MUST include a new song title that is clearly different from the original title.\n";
		task << "5. Provide between 1 to 5 different complete alternatives depending on instructions complexity. You decide the amount.\n";
		task << "6. Return ONLY a single TOON block conforming to the schema below. Do not output anything except the TOON inside a fenced code block.\n\n";

		task << "## Target TOON Schema\n";
		task << "```toon\n";
		task << "alternatives[1]:\n";
		task << "  - title: New Song Title\n";
		task << "    name: Alternative 1: <Short description>\n";
		task << "    lines[1]:\n";
		task << "      - text: Rewritten line text\n";
		task << "```\n\n";

		task << "## Current Song TOON Data\n";
		task << "```toon\n" << StoreAsTOON(song_) << "\n```\n";

		WriteClipboardText(task);
		Log("Total Conversion task generated and copied to clipboard.");

		statusbar_.Set("Total Conversion tehtävä kopioitu!");
		SetTimeCallback(1000, THISBACK(ClearStatusBar));
	}
}

void MainWindow::PasteTotalConversionResult()
{
	String clip = ReadClipboardText();
	if (clip.IsEmpty()) {
		PromptOK("Leikepöytä on tyhjä.");
		return;
	}

	// Extract TOON or JSON
	String toon_text = clip;
	int start = clip.Find("```toon");
	if (start >= 0) {
		start += 7;
		int end = clip.ReverseFind("```");
		if (end >= 0) {
			toon_text = clip.Mid(start, end - start);
		}
	} else {
		start = clip.Find("```json");
		if (start >= 0) {
			start += 7;
			int end = clip.ReverseFind("```");
			if (end >= 0) {
				toon_text = clip.Mid(start, end - start);
			}
		} else {
			start = clip.Find("{");
			if (start >= 0) {
				int end = clip.ReverseFind("}");
				if (end >= start) {
					toon_text = clip.Mid(start, end - start + 1);
				}
			}
		}
	}

	toon_text = NormalizeToonArrayCounts(toon_text);

	TotalConversionCompareDialog cmp(song_, rules_);
	if (cmp.LoadAlternatives(toon_text)) {
		if (cmp.Execute() == IDOK) {
			song_ = cmp.GetResultSong();
			SaveUndoState();
			SyncGrid();
			dock_metadata_.Load(song_);
			dock_sections_.Load(song_);
			dock_ai_review_.Load(song_.ai_review, rules_);
			SyncTitle();
			Log("Applied total conversion alternatives selection.");
			
			statusbar_.Set("Käännös sovellettu!");
			SetTimeCallback(1000, THISBACK(ClearStatusBar));
		}
	} else {
		PromptOK("Vaihtoehtojen lataus epäonnistui. Varmista, että leikepöydällä on validi 'alternatives'-TOON- tai JSON-rakenne.");
	}
}

void MainWindow::SyncHistoryGrid()
{
	history_grid_.Clear();
	if (current_project_path_.IsEmpty() || song_.title.IsEmpty())
		return;

	String dir = AppendFileName(GetFileDirectory(current_project_path_), "undo_" + song_.title);
	if (!DirectoryExists(dir))
		return;

	FindFile ff;
	if (ff.Search(AppendFileName(dir, "*.json"))) {
		do {
			if (ff.IsFile()) {
				String filename = ff.GetName();
				String path = ff.GetPath();

				String date_str = "";
				if (filename.GetLength() >= 15 && filename[8] == '_') {
					date_str = filename.Mid(0, 4) + "-" + filename.Mid(4, 2) + "-" + filename.Mid(6, 2) + " " +
					           filename.Mid(9, 2) + ":" + filename.Mid(11, 2) + ":" + filename.Mid(13, 2);
				} else {
					date_str = Format(ff.GetLastWriteTime());
				}

				SongModel temp;
				double magic = 0.0;
				String author = "";
				if (LoadFromJsonFile(temp, path)) {
					magic = temp.ai_review.overall_magic;
					author = temp.author;
				}

				history_grid_.Add(date_str, Format("%.1f", magic), author, filename);
			}
		} while (ff.Next());
	}

	history_grid_.Sort(0, [](const Value& a, const Value& b) {
		String sa = a.ToString();
		String sb = b.ToString();
		return sa < sb ? 1 : (sa > sb ? -1 : 0);
	});
}

void MainWindow::RestoreHistoryVersion()
{
	int row = history_grid_.GetCursor();
	if (row < 0) return;

	String filename = history_grid_.Get(row, 3).ToString();
	String dir = AppendFileName(GetFileDirectory(current_project_path_), "undo_" + song_.title);
	String path = AppendFileName(dir, filename);

	if (!FileExists(path)) {
		PromptOK("Tiedostoa ei löydy: " + path);
		return;
	}

	if (PromptYesNo("Haluatko palauttaa valitun version? Nykyinen tila tallennetaan peruutushistoriaan.")) {
		SaveUndoState();

		if (LoadFromJsonFile(song_, path)) {
			modified_ = true;
			SyncGrid();
			dock_metadata_.Load(song_);
			dock_sections_.Load(song_);
			dock_ai_review_.Load(song_.ai_review, rules_);
			SyncTitle();
			SyncHistoryGrid();
			Log("Restored project version from trace: " + filename);
			statusbar_.Set("Versio palautettu!");
			SetTimeCallback(1000, THISBACK(ClearStatusBar));
		} else {
			PromptOK("Version palautus epäonnistui: Virheellinen tiedostosisältö.");
		}
	}
}

void MainWindow::AddLyricsLine(bool below)
{
	int row = lyrics_grid_.GetCursor();
	int insert_idx = song_.lines.GetCount();
	String current_section = "Verse 1";
	if (row >= 0 && row < lyrics_grid_.GetCount()) {
		int orig_idx = atoi(lyrics_grid_.Get(row, 0).ToString()) - 1;
		if (orig_idx >= 0 && orig_idx < song_.lines.GetCount()) {
			insert_idx = below ? (orig_idx + 1) : orig_idx;
			current_section = song_.lines[orig_idx].section;
		}
	}
	
	LyricsLine line;
	line.text = "New lyrics line";
	line.section = current_section;
	line.frozen = false;
	
	if (insert_idx >= song_.lines.GetCount()) {
		song_.lines.Add(line);
	} else {
		song_.lines.Insert(insert_idx, line);
	}
	
	SaveUndoState();
	SyncGrid();
	
	for (int r = 0; r < lyrics_grid_.GetCount(); r++) {
		int orig = atoi(lyrics_grid_.Get(r, 0).ToString()) - 1;
		if (orig == insert_idx) {
			lyrics_grid_.SetCursor(r);
			break;
		}
	}
	
	dock_sections_.Load(song_);
	Log("Added new lyric line.");
}

void MainWindow::DeleteSelectedLyricsLine()
{
	int row = lyrics_grid_.GetCursor();
	if (row >= 0 && row < lyrics_grid_.GetCount()) {
		int orig_idx = atoi(lyrics_grid_.Get(row, 0).ToString()) - 1;
		if (orig_idx >= 0 && orig_idx < song_.lines.GetCount()) {
			if (PromptYesNo("Haluatko varmasti poistaa valitun rivin?")) {
				song_.lines.Remove(orig_idx);
				SaveUndoState();
				SyncGrid();
				dock_sections_.Load(song_);
				Log("Deleted lyric line.");
			}
		}
	}
}

void MainWindow::OnLyricsGridUpdateRow()
{
	int row = lyrics_grid_.GetCursor();
	if (row < 0 || row >= lyrics_grid_.GetCount()) return;
	
	int orig_idx = atoi(lyrics_grid_.Get(row, 0).ToString()) - 1;
	if (orig_idx >= 0 && orig_idx < song_.lines.GetCount()) {
		String edited_text = lyrics_grid_.Get(row, 2).ToString();
		if (song_.lines[orig_idx].text != edited_text) {
			song_.lines[orig_idx].text = edited_text;
			SaveUndoState();
			SyncGrid();
		}
	}
}

void MainWindow::LyricsGridMenu(Bar& bar)
{
	bar.Add("Add Line Above", [=] { AddLyricsLine(false); });
	bar.Add("Add Line Below", [=] { AddLyricsLine(true); });
	int row = lyrics_grid_.GetCursor();
	if (row >= 0) {
		bar.Add("Delete Line", THISBACK(DeleteSelectedLyricsLine));
	}
}

void MainWindow::AddRule()
{
	SongRule r;
	r.id = Format("M%d", rules_.rules.GetCount() + 1);
	r.category = "AI Mitigation Rule";
	r.weight = 10;
	r.text = "New songwriting rule description";
	rules_.rules.Add(r);
	
	SaveUndoState();
	SyncRulesGrid();
	
	for (int row = 0; row < rules_grid_.GetCount(); row++) {
		int orig = rules_grid_.Get(row, 4);
		if (orig == rules_.rules.GetCount() - 1) {
			rules_grid_.SetCursor(row);
			break;
		}
	}
	Log("Added new songwriting rule.");
}

void MainWindow::DeleteSelectedRule()
{
	int row = rules_grid_.GetCursor();
	if (row >= 0 && row < rules_grid_.GetCount()) {
		int orig_idx = rules_grid_.Get(row, 4);
		if (orig_idx >= 0 && orig_idx < rules_.rules.GetCount()) {
			if (PromptYesNo("Haluatko varmasti poistaa valitun säännön?")) {
				rules_.rules.Remove(orig_idx);
				SaveUndoState();
				SyncRulesGrid();
				Log("Deleted songwriting rule.");
			}
		}
	}
}

void MainWindow::OnRulesGridUpdateRow()
{
	int row = rules_grid_.GetCursor();
	if (row < 0 || row >= rules_grid_.GetCount()) return;
	
	int orig_idx = rules_grid_.Get(row, 4);
	if (orig_idx >= 0 && orig_idx < rules_.rules.GetCount()) {
		String id = rules_grid_.Get(row, 0).ToString();
		String cat = rules_grid_.Get(row, 1).ToString();
		int weight = GetValueInt(rules_grid_.Get(row, 2));
		String desc = rules_grid_.Get(row, 3).ToString();
		
		SongRule& rule = rules_.rules[orig_idx];
		if (rule.id != id || rule.category != cat || rule.weight != weight || rule.text != desc) {
			rule.id = id;
			rule.category = cat;
			rule.weight = weight;
			rule.text = desc;
			SaveUndoState();
			SyncRulesGrid();
		}
	}
}

void MainWindow::RulesGridMenu(Bar& bar)
{
	bar.Add("Add Rule", THISBACK(AddRule));
	int row = rules_grid_.GetCursor();
	if (row >= 0) {
		bar.Add("Delete Rule", THISBACK(DeleteSelectedRule));
	}
}
