#include "MainWindow.h"

void MainWindow::ToolBarTabLyrics(Bar& bar)
{
	bar.Add("Open Project", CtrlImg::open(), THISBACK(OpenProject));
	bar.Add("Save Project", CtrlImg::save(), THISBACK(SaveProject));
	bar.Separator();
	
	bar.Add("Paste Lyrics", CtrlImg::paste(), THISBACK(PasteLyrics));
	bar.Add("Copy Lyrics to Clipboard", CtrlImg::copy(), THISBACK(CopyLyricsToClipboard));
	bar.Separator();
	
	bar.Add("Copy Task to Clipboard", CtrlImg::go_forward(), [=] { CopyTaskToClipboardWith("revise"); });
	bar.Add("Paste Result from Clipboard", CtrlImg::open(), [=] { PasteResultFromClipboardWith("revise"); });
	
	bar.Separator();
	bar.Add("Freeze Selected", CtrlImg::smallcheck(), THISBACK(FreezeSelectedLines));
	bar.Separator();
	bar.Add(rating_selector_, 130);
}

void MainWindow::ToolBarTabRules(Bar& bar)
{
	bar.Add("Open Rules JSON", CtrlImg::open(), THISBACK(OpenRules));
	bar.Add("Save Rules JSON", CtrlImg::save(), THISBACK(SaveRules));
	bar.Separator();
	
	bar.Add("Load Default Finnish", CtrlImg::go_forward(), [=] { LoadDefaultRules("fi"); });
	bar.Add("Load Default English", CtrlImg::go_forward(), [=] { LoadDefaultRules("en"); });
}

void MainWindow::ToolBarTabDebug(Bar& bar)
{
	bar.Add("Clear Log", CtrlImg::remove(), [=] { debug_tab_.Clear(); });
}
