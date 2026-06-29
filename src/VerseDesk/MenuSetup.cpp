#include "MainWindow.h"

void MainWindow::MainMenu(Bar& bar)
{
	bar.Add("File",    THISBACK(MenuFile));
	bar.Add("Edit",    THISBACK(MenuEdit));
	bar.Add("Task",    THISBACK(MenuTask));
	bar.Add("View",    THISBACK(MenuView));
	bar.Add("Windows", THISBACK(MenuWindows));
	bar.Add("Help",    THISBACK(MenuHelp));
}

void MainWindow::MenuTask(Bar& bar)
{
	bar.Sub("Revise Lyrics", [=](Bar& sub) {
		sub.Add("Copy Task to Clipboard", [=] { CopyTaskToClipboardWith("revise"); });
		sub.Add("Paste Result from Clipboard", [=] { PasteResultFromClipboardWith("revise"); });
	});
	
	bar.Sub("Initialize Colors & Ratings", [=](Bar& sub) {
		sub.Add("Copy Task to Clipboard", [=] { CopyTaskToClipboardWith("init"); });
		sub.Add("Paste Result from Clipboard", [=] { PasteResultFromClipboardWith("init"); });
	});
	
	bar.Sub("Evaluate Rules", [=](Bar& sub) {
		sub.Add("Copy Task to Clipboard", [=] { CopyTaskToClipboardWith("eval"); });
		sub.Add("Paste Result from Clipboard", [=] { PasteResultFromClipboardWith("eval"); });
	});

	bar.Sub("Total Conversion", [=](Bar& sub) {
		sub.Add("Copy Task to Clipboard", THISBACK(CopyTotalConversionTask));
		sub.Add("Paste Result from Clipboard", THISBACK(PasteTotalConversionResult));
	});
}


void MainWindow::MenuFile(Bar& bar)
{
	bar.Add("New Project", [=] {
		if (song_.lines.GetCount() > 0 && !PromptYesNo("Haluatko aloittaa uuden projektin ja tyhjentää nykyiset tiedot?"))
			return;
		song_ = SongModel();
		song_.title = "Nimetön biisi";
		current_project_path_ = "";
		modified_ = false;
		dock_metadata_.Load(song_);
		SyncGrid();
		dock_ai_review_.Load(song_.ai_review, rules_);
		Log("Created new empty project");
	}).Key(K_CTRL_N);

	bar.Add("Open Project…", THISBACK(OpenProject)).Key(K_CTRL_O);
	bar.Sub("Recent Files", THISBACK(MenuRecentProjects));
	bar.Add("Save Project", THISBACK(SaveProject)).Key(K_CTRL_S);
	bar.Add("Save Project As…", THISBACK(SaveProjectAs));
	bar.Separator();
	
	bar.Add("Open Rules…", THISBACK(OpenRules));
	bar.Add("Save Rules", THISBACK(SaveRules));
	bar.Add("Save Rules As…", THISBACK(SaveRulesAs));
	bar.Separator();

	bar.Add("Exit", [=] { Break(); }).Key(K_ALT_F4);
}

void MainWindow::MenuRecentProjects(Bar& bar)
{
	if (recent_project_files_.IsEmpty()) {
		bar.Add("(None)", [] {});
		return;
	}

	for (int i = 0; i < recent_project_files_.GetCount(); i++) {
		String path = recent_project_files_[i];
		String label = Format("&%d %s", i + 1, GetFileTitle(path));
		bar.Add(label, [=] { OpenProjectFile(path); });
	}
}

void MainWindow::MenuEdit(Bar& bar)
{
	bar.Add("Freeze/Unfreeze selected lines", THISBACK(FreezeSelectedLines)).Key(K_F2);
	bar.Separator();

	bar.Add("Paste Lyrics", THISBACK(PasteLyrics)).Key(K_CTRL | K_SHIFT | K_L);
	bar.Add("Copy Lyrics to Clipboard", THISBACK(CopyLyricsToClipboard)).Key(K_CTRL | K_SHIFT | K_K);
	bar.Separator();

	bar.Add("Copy Task to Clipboard", THISBACK(CopyTaskToClipboard)).Key(K_CTRL | K_SHIFT | K_C);
	bar.Add("Paste Result from Clipboard", THISBACK(PasteResultFromClipboard)).Key(K_CTRL | K_SHIFT | K_V);
}

void MainWindow::MenuView(Bar& bar)
{
	bar.Add("Reset to Default Dock Layout", [=] { OnResetDockLayout(); });
	bar.Separator();
	bar.Add("Manage Layouts…", [=] { DockManager(); });
	bar.Add("Save Layout As…", [=] { OnSaveLayoutAs(); });
	bar.Separator();
	bar.Add("Show Metadata Dock", [=] { SetDockVisible(dock_metadata_, true); });
	bar.Add("Show Line Evaluator Dock", [=] { SetDockVisible(dock_line_eval_, true); });
	bar.Add("Show AI Review Dock", [=] { SetDockVisible(dock_ai_review_, true); });
}

void MainWindow::MenuWindows(Bar& bar)
{
	bar.Sub("Dock Windows", [=](Bar& sub) { DockWindowMenu(sub); });
}

void MainWindow::MenuHelp(Bar& bar)
{
	bar.Add("About", [=] {
		PromptOK("LyricsEditor v1.0\n\n"
		         "Custom U++ GUI tool for collaborative songwriting with AI agents.\n"
		         "Supports structured rules evaluation, frozen lines preservation, and undo-history traces.");
	});
}
