#ifndef _LyricsEditor_DiffDialog_h_
#define _LyricsEditor_DiffDialog_h_

#include <CtrlLib/CtrlLib.h>
#include "SongModel.h"

using namespace Upp;

class DiffDialog : public TopWindow {
public:
	typedef DiffDialog CLASSNAME;
	DiffDialog(const SongModel& old_song, const SongModel& new_song);

	struct DiffRow : Moveable<DiffRow> {
		int    index;
		String section;
		String old_text;
		String new_text;
		bool   accepted = true;
		String comment;
	};

	Vector<DiffRow> diffs_;
	SongModel       old_song_;
	SongModel       new_song_;
	bool            task_requested = false;

	SongModel GetResultSong();
	String GetFeedbackText() { return (~edit_feedback_).ToString(); }

private:
	ArrayCtrl  grid_diff_;
	DocEdit    edit_feedback_;
	Button     btn_accept_all_;
	Button     btn_reject_all_;
	Button     btn_write_task_;
	Button     btn_close_;
	
	EditString edit_comment_; // Inline grid editor for comments

	void SyncGrid();
	void OnAcceptAll();
	void OnRejectAll();
	void OnWriteTask();
	void OnGridClick();
	void OnClose();
};

#endif
