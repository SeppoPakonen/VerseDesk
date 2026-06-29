#ifndef _LyricsEditor_DockViews_h_
#define _LyricsEditor_DockViews_h_

#include <CtrlLib/CtrlLib.h>
#include <Docking/Docking.h>

using namespace Upp;

#include "SongModel.h"
#include "RuleModel.h"

class MetadataDock : public DockableCtrl {
public:
	typedef MetadataDock CLASSNAME;
	MetadataDock();

	void Load(const SongModel& song);
	void Save(SongModel& song);

	Event<> WhenChanged;

private:
	ArrayCtrl   grid;
	EditString  edit_title;
	EditString  edit_artist;
	EditString  edit_author;
	EditInt     edit_comp_yr;
	EditInt     edit_composition_yr;
	EditInt     edit_lyrics_yr;
	EditString  edit_genre;
	EditString  edit_suno_style;
	DropList    edit_gender;
	EditInt     edit_singer_count;
	DropList    edit_lang;
	EditString  edit_dialect;
	EditString  edit_artist_style;
	EditString  edit_artist_image;

	void OnCtrlChanged() { WhenChanged(); }
};

class LineEvalDock : public DockableCtrl {
public:
	typedef LineEvalDock CLASSNAME;
	LineEvalDock();

	void Load(const LyricsLine& line);
	void Save(LyricsLine& line);
	void Clear();

	Event<> WhenChanged;

private:
	ArrayCtrl   grid;
	DropList    edit_magic;
	DropList    edit_singability;
	DropList    edit_punchiness;
	DropList    edit_satisfaction;
	DropList    edit_rhyme;
	DropList    edit_low_friction;
	DropList    edit_corr;
	DropList    edit_corr_allit;
	DropList    edit_corr_internal;
	DropList    edit_corr_end;

	EditString  edit_grp_allit;
	EditString  edit_grp_internal;
	EditString  edit_grp_end;
	EditString  edit_color;
	EditString  edit_comment;

	bool        loading_ = false;

	void OnCtrlChanged() { if(!loading_) WhenChanged(); }
};

class AiReviewDock : public DockableCtrl {
public:
	typedef AiReviewDock CLASSNAME;
	AiReviewDock();

	void Load(const AiReview& review, const RuleSet& rules);

private:
	Label     lbl_magic_;
	DocEdit   edit_feedback_;
	ArrayCtrl grid_rules_;
};

class SectionsDock : public DockableCtrl {
public:
	typedef SectionsDock CLASSNAME;
	SectionsDock();

	void Load(SongModel& song);

	Event<> WhenChanged;

private:
	ArrayCtrl  grid;
	EditString edit_attr1;
	EditString edit_attr2;
	SongModel* song_ptr = nullptr;

	void OnUpdateRow();
};

#endif
