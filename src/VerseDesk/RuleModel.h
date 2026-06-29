#ifndef _LyricsEditor_RuleModel_h_
#define _LyricsEditor_RuleModel_h_

#include <Core/Core.h>

using namespace Upp;

struct SongRule : Moveable<SongRule> {
	String id;
	String text;
	int    weight = 10;
	String category; // "Lyrics Rule" or "AI Mitigation Rule"

	void Jsonize(JsonIO& json) {
		json
			("id", id)
			("text", text)
			("weight", weight)
			("category", category)
		;
	}
};

struct RuleSet {
	Vector<SongRule> rules;

	void Jsonize(JsonIO& json) {
		json("rules", rules);
	}
};

struct SongModel;
String BuildTaskPrompt(const SongModel& song, const RuleSet& rules, const String& task_name);

#endif
