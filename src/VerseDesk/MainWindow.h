#ifndef _VerseDesk_MainWindow_h_
#define _VerseDesk_MainWindow_h_

#include <CtrlLib/CtrlLib.h>
#include <Docking/Docking.h>


#define IMAGECLASS VerseDeskImg
#define IMAGEFILE <VerseDesk/VerseDesk.iml>
#include <Draw/iml_header.h>

using namespace Upp;

#include "AppState.h"
#include "DebugTab.h"
#include "DockViews.h"
#include "SongModel.h"
#include "RuleModel.h"

class MainWindow : public DockWindow {
public:
	typedef MainWindow CLASSNAME;

	MainWindow();
	void DockInit() override;
	void Close() override;

	void Log(const String& msg) { LogInfo(log_, "App", msg); }

	// Project loading/saving
	void OpenProject();
	void OpenProjectFile(const String& path);
	void SaveProject();
	void SaveProjectAs();
	void SaveProjectFile(const String& path);
	
	// Rules loading/saving
	void OpenRules();
	void OpenRulesFile(const String& path);
	void SaveRules();
	void SaveRulesAs();
	void SaveRulesFile(const String& path);
	void LoadDefaultRules(const String& lang);

	// Clipboard actions
	void CopyTaskToClipboard();
	void CopyTaskToClipboardWith(String task_name);
	void PasteResultFromClipboard();
	void PasteResultFromClipboardWith(String task_name);
	void CopyTotalConversionTask();
	void PasteTotalConversionResult();
	void PasteLyrics();
	void CopyLyricsToClipboard();
	
	// Actions
	void FreezeSelectedLines();
	void UpdateSelectedLineEval();
	void OnGridCursor();

	void AddLyricsLine(bool below);
	void DeleteSelectedLyricsLine();
	void OnLyricsGridUpdateRow();
	void LyricsGridMenu(Bar& bar);

	void AddRule();
	void DeleteSelectedRule();
	void OnRulesGridUpdateRow();
	void RulesGridMenu(Bar& bar);
	
	void SaveUndoState();
	void SyncGrid();
	void SyncRulesGrid();
	void SyncHistoryGrid();
	void RestoreHistoryVersion();
	void SyncUI();
	void SyncTitle();

	void SetInitialProject(const String& path) { current_project_path_ = path; }

private:
	// ---- Log
	AppLog log_;

	// ---- Frames
	MenuBar menu_;
	ToolBar toolbar_;
	StatusBar statusbar_;
	
	void ClearStatusBar() { statusbar_.Set(""); }

	// ---- Main area
	TabCtrl    main_tabs_;
	ParentCtrl tab_lyrics_;
	ParentCtrl tab_rules_;
	ParentCtrl tab_history_;
	ArrayCtrl  lyrics_grid_;
	ArrayCtrl  rules_grid_;
	ArrayCtrl  history_grid_;

	EditString edit_lyrics_text;
	EditString edit_rule_id;
	EditString edit_rule_cat;
	EditInt    edit_rule_weight;
	EditString edit_rule_desc;
	
	DebugLog   debug_tab_;

	// ---- Dock panels
	MetadataDock dock_metadata_;
	LineEvalDock dock_line_eval_;
	AiReviewDock dock_ai_review_;
	SectionsDock dock_sections_;

	// ---- State
	SongModel song_;
	RuleSet   rules_;
	String    current_project_path_;
	String    current_rules_path_;
	Vector<String> recent_project_files_;
	
	bool        loaded_       = false;
	bool        modified_     = false;
	int         current_tab_  = 0;
	String      default_layout_data_;
	AppRegistry registry_;

	// ---- Dock lifecycle
	void InitDockers();
	void OnResetDockLayout();
	void CacheDefaultLayout();
	void SetDefaultLayout();
	void SetDockVisible(DockableCtrl& dc, bool visible);
	void OnSaveLayoutAs();
	void OnLoadLayout();

	// ---- Persistence
	bool LoadUserLayout();
	void SaveUserLayout();
	void LoadAppState();
	void SaveAppState();
	void InitRegistry();
	void AddRecentProjectFile(const String& path);
	void MenuRecentProjects(Bar& bar);
	String GetProjectDialogDir() const;
	String GetRulesDialogDir() const;

	// ---- Tab/toolbar coordination
	void UpdateToolBar(Bar& bar);
	void OnMainTabChanged();

	// ---- Toolbars
	void ToolBarTabLyrics(Bar& bar);
	void ToolBarTabRules(Bar& bar);
	void ToolBarTabDebug(Bar& bar);

	// ---- Menus
	void MainMenu(Bar& bar);
	void MenuFile(Bar& bar);
	void MenuEdit(Bar& bar);
	void MenuTask(Bar& bar);
	void MenuView(Bar& bar);
	void MenuWindows(Bar& bar);
	void MenuHelp(Bar& bar);

	// ---- Dynamic Rating Column Selection
	DropList rating_selector_;
	void OnRatingSelectorChanged();
};

#endif
