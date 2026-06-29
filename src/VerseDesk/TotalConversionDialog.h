#ifndef _LyricsEditor_TotalConversionDialog_h_
#define _LyricsEditor_TotalConversionDialog_h_

#include <CtrlLib/CtrlLib.h>
#include "SongModel.h"
#include "RuleModel.h"

using namespace Upp;

class TotalConversionDialog : public TopWindow {
public:
	typedef TotalConversionDialog CLASSNAME;
	TotalConversionDialog();

	DocEdit    edit_instructions_;
	Button     btn_copy_task_;
	Button     btn_cancel_;

	void OnCopyTask();
};

class TotalConversionCompareDialog : public TopWindow {
public:
	typedef TotalConversionCompareDialog CLASSNAME;
	TotalConversionCompareDialog(const SongModel& old_song, const RuleSet& rules);

	SongModel GetResultSong() const;
	bool LoadAlternatives(const String& toon_text);

private:
	ArrayCtrl               grid_compare_;
	DocEdit                 edit_iteration_;
	Button                  btn_copy_feedback_;
	Button                  btn_paste_new_;
	Button                  btn_apply_;
	Button                  btn_accept_all_;
	Button                  btn_reject_all_;
	Button                  btn_cancel_;

	SongModel               old_song_;
	const RuleSet&          rules_;
	TotalConversionResponse response_;
	Vector<int>             selected_alt_idx_;

	void SyncGrid();
	void OnGridClick();
	void OnGridDoubleLeft();
	void OnGridMenu(Bar& bar);
	void OnCopyFeedback();
	void OnPasteNew();
	void OnApply();
	void OnAcceptAll();
	void OnRejectAll();
	void OnCancel();
};

bool LoadTotalConversionResponse(TotalConversionResponse& response, const String& toon_text);

#endif
