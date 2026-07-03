#ifndef _VerseDesk_AppState_h_
#define _VerseDesk_AppState_h_

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct AppState {
	String active_tab_name;    // primary: stable tab label
	int    active_tab_index = 0; // fallback if label not found
	int    editor_mode      = 0; // 0 = View, 1 = Edit
	Vector<String> recent_project_files;

	void Jsonize(JsonIO& jio) {
		jio("active_tab_name",  active_tab_name)
		   ("active_tab_index", active_tab_index)
		   ("editor_mode",      editor_mode)
		   ("recent_project_files", recent_project_files);
	}
};

int CountLeadingWhitespace(const String& s);
bool ParseArrayHeaderLine(const String& line, String& prefix, String& suffix);
void NormalizeToonArrayCounts(Vector<String>& lines, int start, int base_indent);
String NormalizeToonArrayCounts(const String& text);

#endif
