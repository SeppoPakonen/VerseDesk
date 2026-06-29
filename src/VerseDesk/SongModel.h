#ifndef _LyricsEditor_SongModel_h_
#define _LyricsEditor_SongModel_h_

#include <Core/Core.h>

using namespace Upp;

inline int GetValueInt(const Value& v)
{
	if (IsNull(v)) return 0;
	if (v.Is<double>()) {
		double d = v;
		if (IsNull(d)) return 0;
		return (int)d;
	}
	if (v.Is<int>()) return (int)v;
	return atoi(v.ToString());
}

struct LyricsLine : Moveable<LyricsLine> {
	String text;
	String section;
	String attributes;
	bool   frozen = false;
	int    magic_score = 0;
	int    singability = 0;
	int    punchiness = 0;
	int    satisfaction = 0;
	int    rhyme_quality = 0;
	int    low_friction = 0;
	int    correction_need = 0;
	int    correction_need_alliteration = 0;
	int    correction_need_internal = 0;
	int    correction_need_end = 0;
	String rhyme_group_alliteration;
	String rhyme_group_internal;
	String rhyme_group_end;
	String color;
	String comment;

	void Jsonize(JsonIO& json) {
		json
			("text", text)
			("section", section)
			("attributes", attributes)
			("frozen", frozen)
			("magic_score", magic_score)
			("singability", singability)
			("punchiness", punchiness)
			("satisfaction", satisfaction)
			("rhyme_quality", rhyme_quality)
			("low_friction", low_friction)
			("correction_need", correction_need)
			("correction_need_alliteration", correction_need_alliteration)
			("correction_need_internal", correction_need_internal)
			("correction_need_end", correction_need_end)
			("rhyme_group_alliteration", rhyme_group_alliteration)
			("rhyme_group_internal", rhyme_group_internal)
			("rhyme_group_end", rhyme_group_end)
			("color", color)
			("comment", comment)
		;
	}
};

struct AiReview {
	String feedback;
	double overall_magic = 0.0;
	VectorMap<String, int> rule_scores;

	AiReview() {}
	
	// Copy constructor
	AiReview(const AiReview& src) {
		*this = src;
	}

	void Jsonize(JsonIO& json) {
		json
			("feedback", feedback)
			("overall_magic", overall_magic)
		;
		if (json.IsLoading()) {
			rule_scores.Clear();
			Value val = json.Get("rule_scores");
			if (IsValueArray(val)) {
				ValueArray arr(val);
				for (int i = 0; i < arr.GetCount(); i++) {
					Value m = arr[i];
					if (IsValueMap(m)) {
						ValueMap vm(m);
						String k;
						int v = 0;
						
						int idx_key = vm.Find("key");
						int idx_id = vm.Find("id");
						if (idx_key >= 0) k = vm.GetValue(idx_key).ToString();
						else if (idx_id >= 0) k = vm.GetValue(idx_id).ToString();
						
						int idx_val = vm.Find("value");
						int idx_score = vm.Find("score");
						if (idx_val >= 0) v = vm.GetValue(idx_val);
						else if (idx_score >= 0) v = vm.GetValue(idx_score);
						
						if (!k.IsEmpty()) {
							rule_scores.Add(k, v);
						}
					}
				}
			}
		} else {
			json("rule_scores", rule_scores);
		}
	}

	// Copy assignment operator
	void operator=(const AiReview& src) {
		feedback = src.feedback;
		overall_magic = src.overall_magic;
		rule_scores = clone(src.rule_scores);
	}
};

struct AltLine : Moveable<AltLine> {
	String text;

	void Jsonize(JsonIO& json) {
		json("text", text);
	}
};

struct ConversionAlternative : Moveable<ConversionAlternative> {
	String title;
	String name;
	Vector<AltLine> lines;

	void Jsonize(JsonIO& json) {
		json
			("title", title)
			("name", name)
			("lines", lines)
		;
	}
};

struct TotalConversionResponse {
	Vector<ConversionAlternative> alternatives;

	void Jsonize(JsonIO& json) {
		json("alternatives", alternatives);
	}
};

struct SongModel {
	String title;
	String artist;
	String author;
	int    completion_year = 0;
	int    composition_year = 0;
	int    lyrics_year = 0;
	String genre;
	String suno_style;
	String gender; // male, female, both
	int    singer_count = 1;
	String language; // e.g. "fi", "en"
	String dialect;
	String artist_style;
	String artist_image;
	Vector<LyricsLine> lines;
	AiReview ai_review;

	SongModel() {}

	// Copy constructor
	SongModel(const SongModel& src) {
		*this = src;
	}

	void Jsonize(JsonIO& json) {
		json
			("title", title)
			("artist", artist)
			("author", author)
			("completion_year", completion_year)
			("composition_year", composition_year)
			("lyrics_year", lyrics_year)
			("genre", genre)
			("suno_style", suno_style)
			("gender", gender)
			("singer_count", singer_count)
			("language", language)
			("dialect", dialect)
			("artist_style", artist_style)
			("artist_image", artist_image)
			("lines", lines)
			("ai_review", ai_review)
		;
	}

	// Copy assignment operator
	void operator=(const SongModel& src) {
		title = src.title;
		artist = src.artist;
		author = src.author;
		completion_year = src.completion_year;
		composition_year = src.composition_year;
		lyrics_year = src.lyrics_year;
		genre = src.genre;
		suno_style = src.suno_style;
		gender = src.gender;
		singer_count = src.singer_count;
		language = src.language;
		dialect = src.dialect;
		artist_style = src.artist_style;
		artist_image = src.artist_image;
		lines = clone(src.lines);
		ai_review = src.ai_review;
	}
};

#endif
