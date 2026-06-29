#include "DockViews.h"

// Helper to populate rating DropLists
static void PopulateRatings(DropList& dl, int min_val, int max_val)
{
	dl.Clear();
	for (int i = min_val; i <= max_val; i++) {
		String lbl = Format("%d", i);
		if (i == 0) lbl += " (Neutral)";
		else if (i > 0) lbl = "+" + lbl;
		dl.Add(i, lbl);
	}
}

static void PopulateCorrectionNeed(DropList& dl)
{
	dl.Clear();
	dl.Add(0, "0 (None)");
	dl.Add(1, "1 (Low)");
	dl.Add(2, "2 (Medium)");
	dl.Add(3, "3 (High)");
	dl.Add(4, "4 (Critical)");
}

// ---------------------------------------------------------------------------
// MetadataDock

MetadataDock::MetadataDock()
{
	Title("Song Metadata");

	Add(grid.SizePos());
	grid.AddColumn("Key", 30);
	grid.AddColumn("Value", 70);

	static const struct {
		const char* name;
		Ctrl* ctrl;
	} fields[] = {
		{ "Title", &edit_title },
		{ "Artist", &edit_artist },
		{ "Author", &edit_author },
		{ "Completion Year", &edit_comp_yr },
		{ "Composition Year", &edit_composition_yr },
		{ "Lyrics Year", &edit_lyrics_yr },
		{ "Genre", &edit_genre },
		{ "Suno Style", &edit_suno_style },
		{ "Gender", &edit_gender },
		{ "Singer Count", &edit_singer_count },
		{ "Language", &edit_lang },
		{ "Dialect", &edit_dialect },
		{ "Artist Style", &edit_artist_style },
		{ "Artist Image", &edit_artist_image }
	};

	edit_gender.Add("male", "Male");
	edit_gender.Add("female", "Female");
	edit_gender.Add("both", "Both");

	edit_lang.Add("fi", "Finnish (FI)");
	edit_lang.Add("en", "English (EN)");

	for (int i = 0; i < 14; i++) {
		grid.Add(fields[i].name);
		grid.SetCtrl(i, 1, *fields[i].ctrl);
		
		if (dynamic_cast<EditField*>(fields[i].ctrl)) {
			dynamic_cast<EditField*>(fields[i].ctrl)->WhenAction = THISBACK(OnCtrlChanged);
		} else if (dynamic_cast<DropList*>(fields[i].ctrl)) {
			dynamic_cast<DropList*>(fields[i].ctrl)->WhenAction = THISBACK(OnCtrlChanged);
		}
	}

	grid.SetLineCy(EditField::GetStdHeight() + DPI(4));
}

void MetadataDock::Load(const SongModel& song)
{
	edit_title.SetData(song.title);
	edit_artist.SetData(song.artist);
	edit_author.SetData(song.author);
	edit_comp_yr.SetData(song.completion_year);
	edit_composition_yr.SetData(song.composition_year);
	edit_lyrics_yr.SetData(song.lyrics_year);
	edit_genre.SetData(song.genre);
	edit_suno_style.SetData(song.suno_style);
	edit_gender.SetData(song.gender);
	edit_singer_count.SetData(song.singer_count);
	edit_lang.SetData(song.language);
	edit_dialect.SetData(song.dialect);
	edit_artist_style.SetData(song.artist_style);
	edit_artist_image.SetData(song.artist_image);
}

void MetadataDock::Save(SongModel& song)
{
	song.title = (~edit_title).ToString();
	song.artist = (~edit_artist).ToString();
	song.author = (~edit_author).ToString();
	song.completion_year = ~edit_comp_yr;
	song.composition_year = ~edit_composition_yr;
	song.lyrics_year = ~edit_lyrics_yr;
	song.genre = (~edit_genre).ToString();
	song.suno_style = (~edit_suno_style).ToString();
	song.gender = (~edit_gender).ToString();
	song.singer_count = ~edit_singer_count;
	song.language = (~edit_lang).ToString();
	song.dialect = (~edit_dialect).ToString();
	song.artist_style = (~edit_artist_style).ToString();
	song.artist_image = (~edit_artist_image).ToString();
}


LineEvalDock::LineEvalDock()
{
	Title("Line Evaluator");

	Add(grid.SizePos());
	grid.AddColumn("Key", 30);
	grid.AddColumn("Value", 70);

	PopulateRatings(edit_magic, 0, 10);
	PopulateRatings(edit_singability, -5, 10);
	PopulateRatings(edit_punchiness, -5, 10);
	PopulateRatings(edit_satisfaction, -5, 10);
	PopulateRatings(edit_rhyme, -5, 10);
	PopulateRatings(edit_low_friction, 0, 10);

	PopulateCorrectionNeed(edit_corr);
	PopulateCorrectionNeed(edit_corr_allit);
	PopulateCorrectionNeed(edit_corr_internal);
	PopulateCorrectionNeed(edit_corr_end);

	static const struct {
		const char* name;
		Ctrl* ctrl;
	} fields[] = {
		{ "Magic Rating", &edit_magic },
		{ "Singability", &edit_singability },
		{ "Punchiness", &edit_punchiness },
		{ "Satisfaction", &edit_satisfaction },
		{ "Rhyme Quality", &edit_rhyme },
		{ "Low Friction", &edit_low_friction },
		{ "Corr Need (Overall)", &edit_corr },
		{ "Corr Need (Allit)", &edit_corr_allit },
		{ "Corr Need (Internal)", &edit_corr_internal },
		{ "Corr Need (End)", &edit_corr_end },
		{ "Alliteration Grp", &edit_grp_allit },
		{ "Internal Rhyme Grp", &edit_grp_internal },
		{ "End Rhyme Grp", &edit_grp_end },
		{ "Metaphorical Color", &edit_color },
		{ "Line Comments", &edit_comment }
	};

	for (int i = 0; i < 15; i++) {
		grid.Add(fields[i].name);
		grid.SetCtrl(i, 1, *fields[i].ctrl);

		if (dynamic_cast<DropList*>(fields[i].ctrl)) {
			dynamic_cast<DropList*>(fields[i].ctrl)->WhenAction = THISBACK(OnCtrlChanged);
		} else if (dynamic_cast<EditField*>(fields[i].ctrl)) {
			dynamic_cast<EditField*>(fields[i].ctrl)->WhenAction = THISBACK(OnCtrlChanged);
		}
	}

	grid.SetLineCy(EditField::GetStdHeight() + DPI(4));
}

void LineEvalDock::Load(const LyricsLine& line)
{
	loading_ = true;
	edit_magic.SetData(line.magic_score);
	edit_singability.SetData(line.singability);
	edit_punchiness.SetData(line.punchiness);
	edit_satisfaction.SetData(line.satisfaction);
	edit_rhyme.SetData(line.rhyme_quality);
	edit_low_friction.SetData(line.low_friction);
	edit_corr.SetData(line.correction_need);
	edit_corr_allit.SetData(line.correction_need_alliteration);
	edit_corr_internal.SetData(line.correction_need_internal);
	edit_corr_end.SetData(line.correction_need_end);
	edit_grp_allit.SetData(line.rhyme_group_alliteration);
	edit_grp_internal.SetData(line.rhyme_group_internal);
	edit_grp_end.SetData(line.rhyme_group_end);
	edit_color.SetData(line.color);
	edit_comment.SetData(line.comment);
	loading_ = false;
}

void LineEvalDock::Save(LyricsLine& line)
{
	line.magic_score = GetValueInt(~edit_magic);
	line.singability = GetValueInt(~edit_singability);
	line.punchiness = GetValueInt(~edit_punchiness);
	line.satisfaction = GetValueInt(~edit_satisfaction);
	line.rhyme_quality = GetValueInt(~edit_rhyme);
	line.low_friction = GetValueInt(~edit_low_friction);
	line.correction_need = GetValueInt(~edit_corr);
	line.correction_need_alliteration = GetValueInt(~edit_corr_allit);
	line.correction_need_internal = GetValueInt(~edit_corr_internal);
	line.correction_need_end = GetValueInt(~edit_corr_end);
	line.rhyme_group_alliteration = (~edit_grp_allit).ToString();
	line.rhyme_group_internal = (~edit_grp_internal).ToString();
	line.rhyme_group_end = (~edit_grp_end).ToString();
	line.color = (~edit_color).ToString();
	line.comment = (~edit_comment).ToString();
}

void LineEvalDock::Clear()
{
	loading_ = true;
	edit_magic.SetData(Null);
	edit_singability.SetData(Null);
	edit_punchiness.SetData(Null);
	edit_satisfaction.SetData(Null);
	edit_rhyme.SetData(Null);
	edit_low_friction.SetData(Null);
	edit_corr.SetData(Null);
	edit_corr_allit.SetData(Null);
	edit_corr_internal.SetData(Null);
	edit_corr_end.SetData(Null);
	edit_grp_allit.SetData(Null);
	edit_grp_internal.SetData(Null);
	edit_grp_end.SetData(Null);
	edit_color.SetData(Null);
	edit_comment.SetData(Null);
	loading_ = false;
}


// ---------------------------------------------------------------------------
// AiReviewDock

AiReviewDock::AiReviewDock()
{
	Title("AI Review & Analysis");

	Add(lbl_magic_.LeftPos(8, 200).TopPos(8, 20));
	lbl_magic_.SetLabel("Overall Magic Score: —");

	Add(edit_feedback_.HSizePos(8, 8).TopPos(36, 120));
	edit_feedback_.SetReadOnly();

	Add(grid_rules_.HSizePos(8, 8).VSizePos(164, 8));
	grid_rules_.ColumnSortThreeState();
	grid_rules_.AddColumn("Rule ID", 15).Sorting();
	grid_rules_.AddColumn("Category", 20).Sorting();
	grid_rules_.AddColumn("Rule Description", 50).Sorting();
	grid_rules_.AddColumn("Score (0-10)", 15).SortingBy([](const Value& a, const Value& b) {
		String sa = a.ToString();
		String sb = b.ToString();
		int val_a = sa == "N/A" ? -1 : atoi(~sa);
		int val_b = sb == "N/A" ? -1 : atoi(~sb);
		return val_a < val_b ? -1 : (val_a > val_b ? 1 : 0);
	});
}

void AiReviewDock::Load(const AiReview& review, const RuleSet& rules)
{
	lbl_magic_.SetLabel(Format("Overall Magic Score: %.1f / 10", review.overall_magic));
	edit_feedback_.SetData(review.feedback);

	grid_rules_.Clear();
	for (const SongRule& rule : rules.rules) {
		int score = review.rule_scores.Get(rule.id, -1);
		String score_str = score >= 0 ? Format("%d / 10", score) : "N/A";
		grid_rules_.Add(rule.id, rule.category, rule.text, score_str);
	}
}

// ---------------------------------------------------------------------------
// SectionsDock

SectionsDock::SectionsDock()
{
	Title("Part Attributes");

	Add(grid.SizePos());
	grid.ColumnSortThreeState();
	grid.AddColumn("Part", 40).Sorting();
	grid.AddColumn("Attr 1", 30).Edit(edit_attr1).Sorting();
	grid.AddColumn("Attr 2", 30).Edit(edit_attr2).Sorting();

	grid.WhenUpdateRow = THISBACK(OnUpdateRow);
	grid.SetLineCy(EditField::GetStdHeight() + DPI(4));
}

void SectionsDock::Load(SongModel& song)
{
	song_ptr = &song;

	int cursor = grid.GetCursor();
	String saved_sec = cursor >= 0 ? grid.Get(cursor, 0).ToString() : "";

	grid.Clear();

	Vector<String> unique_sections;
	Index<String> seen;

	for (const LyricsLine& line : song.lines) {
		if (!line.section.IsEmpty() && seen.Find(line.section) < 0) {
			seen.Add(line.section);
			unique_sections.Add(line.section);
		}
	}

	for (const String& sec : unique_sections) {
		String attrs;
		for (const LyricsLine& line : song.lines) {
			if (line.section == sec) {
				attrs = line.attributes;
				break;
			}
		}

		String a1, a2;
		Vector<String> parts = Split(attrs, ',');
		if (parts.GetCount() > 0) a1 = TrimBoth(parts[0]);
		if (parts.GetCount() > 1) a2 = TrimBoth(parts[1]);

		grid.Add(sec, a1, a2);
	}

	if (!saved_sec.IsEmpty()) {
		for (int i = 0; i < grid.GetCount(); i++) {
			if (grid.Get(i, 0).ToString() == saved_sec) {
				grid.SetCursor(i);
				break;
			}
		}
	}
}

void SectionsDock::OnUpdateRow()
{
	int row = grid.GetCursor();
	if (row < 0 || !song_ptr) return;

	String section_name = grid.Get(row, 0).ToString();
	String attr1 = grid.Get(row, 1).ToString();
	String attr2 = grid.Get(row, 2).ToString();

	String combined = attr1;
	if (!attr2.IsEmpty()) {
		if (!combined.IsEmpty()) combined << ", ";
		combined << attr2;
	}

	for (LyricsLine& line : song_ptr->lines) {
		if (line.section == section_name) {
			line.attributes = combined;
		}
	}

	WhenChanged();
}
