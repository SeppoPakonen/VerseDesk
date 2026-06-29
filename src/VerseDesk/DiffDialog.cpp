#include "DiffDialog.h"

DiffDialog::DiffDialog(const SongModel& old_song, const SongModel& new_song)
	: old_song_(old_song), new_song_(new_song)
{
	Title("Compare AI Revisions - Lyrics Editor").Sizeable().Zoomable();
	SetRect(0, 0, 900, 600);

	// Grid layout
	Add(grid_diff_.HSizePos(8, 8).TopPos(8, 380));
	grid_diff_.AddColumn("Line #", 10);
	grid_diff_.AddColumn("Section", 15);
	grid_diff_.AddColumn("Old Lyrics", 30);
	grid_diff_.AddColumn("New Lyrics", 30);
	grid_diff_.AddColumn("Decision", 15);
	grid_diff_.AddColumn("Comment (AI instructions)", 30).Edit(edit_comment_);
	grid_diff_.WhenLeftDouble = THISBACK(OnGridClick);

	// General feedback edit
	Label* lbl_feedback = new Label();
	lbl_feedback->SetLabel("Feedback / Instructions for further AI revisions:");
	Add(lbl_feedback->LeftPos(8, 400).TopPos(396, 20));
	Add(edit_feedback_.HSizePos(8, 8).TopPos(416, 100));

	// Buttons
	Add(btn_accept_all_.LeftPos(8, 120).BottomPos(8, 30));
	btn_accept_all_.SetLabel("Accept All");
	btn_accept_all_.WhenAction = THISBACK(OnAcceptAll);

	Add(btn_reject_all_.LeftPos(136, 120).BottomPos(8, 30));
	btn_reject_all_.SetLabel("Reject All");
	btn_reject_all_.WhenAction = THISBACK(OnRejectAll);

	Add(btn_write_task_.RightPos(136, 220).BottomPos(8, 30));
	btn_write_task_.SetLabel("Write Task & Paste New");
	btn_write_task_.WhenAction = THISBACK(OnWriteTask);

	Add(btn_close_.RightPos(8, 120).BottomPos(8, 30));
	btn_close_.SetLabel("Close / Apply");
	btn_close_.WhenAction = THISBACK(OnClose);

	// Populate diffs
	int old_count = old_song.lines.GetCount();
	int new_count = new_song.lines.GetCount();
	int max_count = old_count > new_count ? old_count : new_count;

	for (int i = 0; i < max_count; i++) {
		String old_text = i < old_count ? old_song.lines[i].text : "";
		String new_text = i < new_count ? new_song.lines[i].text : "";
		String section = i < new_count ? new_song.lines[i].section : (i < old_count ? old_song.lines[i].section : "");

		// Only show actual changes
		if (old_text != new_text) {
			DiffRow r;
			r.index = i;
			r.section = section;
			r.old_text = old_text;
			r.new_text = new_text;
			r.accepted = true;
			r.comment = i < old_count ? old_song.lines[i].comment : "";
			diffs_.Add(r);
		}
	}

	SyncGrid();
}

void DiffDialog::SyncGrid()
{
	int sel = grid_diff_.GetCursor();
	grid_diff_.Clear();

	for (int i = 0; i < diffs_.GetCount(); i++) {
		const DiffRow& r = diffs_[i];
		
		Color decision_bg = r.accepted ? Color(220, 255, 220) : Color(255, 220, 220);
		Color decision_fg = r.accepted ? Color(0, 128, 0) : Color(128, 0, 0);
		
		AttrText decision_cell = AttrText(r.accepted ? "Accept" : "Reject")
			.NormalPaper(decision_bg).NormalInk(decision_fg)
			.Paper(r.accepted ? Color(180, 240, 180) : Color(240, 180, 180)).Ink(decision_fg);

		grid_diff_.Add(
			Format("%d", r.index + 1),
			r.section,
			r.old_text,
			r.new_text,
			decision_cell,
			r.comment
		);
	}

	if (sel >= 0 && sel < grid_diff_.GetCount())
		grid_diff_.SetCursor(sel);
}

void DiffDialog::OnGridClick()
{
	int row = grid_diff_.GetCursor();
	if (row >= 0 && row < diffs_.GetCount()) {
		// Read current comment in case inline editing was active
		diffs_[row].comment = grid_diff_.Get(row, 5).ToString();
		
		// Toggle acceptance
		diffs_[row].accepted = !diffs_[row].accepted;
		SyncGrid();
	}
}

void DiffDialog::OnAcceptAll()
{
	for (int i = 0; i < diffs_.GetCount(); i++) {
		diffs_[i].accepted = true;
	}
	SyncGrid();
}

void DiffDialog::OnRejectAll()
{
	for (int i = 0; i < diffs_.GetCount(); i++) {
		diffs_[i].accepted = false;
	}
	SyncGrid();
}

void DiffDialog::OnWriteTask()
{
	// Ensure comments and edits are written back from grid
	for (int i = 0; i < diffs_.GetCount(); i++) {
		diffs_[i].comment = grid_diff_.Get(i, 5).ToString();
	}
	task_requested = true;
	Break(IDOK);
}

void DiffDialog::OnClose()
{
	// Ensure comments and edits are written back from grid
	for (int i = 0; i < diffs_.GetCount(); i++) {
		diffs_[i].comment = grid_diff_.Get(i, 5).ToString();
	}
	Break(IDOK);
}

SongModel DiffDialog::GetResultSong()
{
	SongModel result = old_song_;
	
	// Copy AI review and metadata from new song
	result.ai_review = new_song_.ai_review;
	result.artist_style = new_song_.artist_style;
	result.artist_image = new_song_.artist_image;
	result.genre = new_song_.genre;
	result.suno_style = new_song_.suno_style;
	
	// Construct lines list based on acceptances
	for (int i = 0; i < diffs_.GetCount(); i++) {
		const DiffRow& r = diffs_[i];
		int idx = r.index;
		
		// Update comments first
		if (idx >= 0 && idx < result.lines.GetCount()) {
			result.lines[idx].comment = r.comment;
		}

		if (r.accepted) {
			// Apply new text and section attributes
			if (idx >= 0 && idx < result.lines.GetCount()) {
				result.lines[idx].text = r.new_text;
				if (idx < new_song_.lines.GetCount()) {
					result.lines[idx].section = new_song_.lines[idx].section;
					result.lines[idx].attributes = new_song_.lines[idx].attributes;
				}
			} else if (idx >= result.lines.GetCount() && idx < new_song_.lines.GetCount()) {
				// Added line
				result.lines.Add(new_song_.lines[idx]);
			}
		}
	}
	
	return result;
}
