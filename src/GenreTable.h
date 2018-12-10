#pragma once
#ifndef __ID3_GENRE_TABLE__
#define __ID3_GENRE_TABLE__

/**
*  ID3 genre ID에 따라 반환할 장르명에 대한 lookup table
*/


namespace ID3GenreTable
{
	unsigned int MAXGENRE = 191; ///<총 장르 수

	const wchar_t* genreTable[] = {
	L"Blues",
	L"Classic Rock",
	L"Country",
	L"Dance",
	L"Disco",
	L"Funk",
	L"Grunge",
	L"Hip-Hop",
	L"Jazz",
	L"Metal",
	L"New Age",
	L"Oldies",
	L"Other",
	L"Pop",
	L"R&B",
	L"Rap",
	L"Reggae",
	L"Rock",
	L"Techno",
	L"Industrial",
	L"Alternative",
	L"Ska",
	L"Death Metal",
	L"Pranks",
	L"Soundtrack",
	L"Euro-Techno",
	L"Ambient",
	L"Trip-Hop",
	L"Vocal",
	L"Jazz+Funk",
	L"Fusion",
	L"Trance",
	L"Classical",
	L"Instrumental",
	L"Acid",
	L"House",
	L"Game",
	L"Sound Clip",
	L"Gospel",
	L"Noise",
	L"Alt. Rock",
	L"Bass",
	L"Soul",
	L"Punk",
	L"Space",
	L"Meditative",
	L"Instrum. Pop",
	L"Instrum. Rock",
	L"Ethnic",
	L"Gothic",
	L"Darkwave",
	L"Techno-Indust.",
	L"Electronic",
	L"Pop-Folk",
	L"Eurodance",
	L"Dream",
	L"Southern Rock",
	L"Comedy",
	L"Cult",
	L"Gangsta",
	L"Top 40",
	L"Christian Rap",
	L"Pop/Funk",
	L"Jungle",
	L"Native American",
	L"Cabaret",
	L"New Wave",
	L"Psychadelic",
	L"Rave",
	L"Showtunes",
	L"Trailer",
	L"Lo-Fi",
	L"Tribal",
	L"Acid Punk",
	L"Acid Jazz",
	L"Polka",
	L"Retro",
	L"Musical",
	L"Rock & Roll",
	L"Hard Rock",
	L"Folk",
	L"Folk/Rock",
	L"National Folk",
	L"Swing",
	L"Fusion",
	L"Bebob",
	L"Latin",
	L"Revival",
	L"Celtic",
	L"Bluegrass",
	L"Avantgarde",
	L"Gothic Rock",
	L"Progress. Rock",
	L"Psychadel. Rock",
	L"Symphonic Rock",
	L"Slow Rock",
	L"Big Band",
	L"Chorus",
	L"Easy Listening",
	L"Acoustic",
	L"Humour",
	L"Speech",
	L"Chanson",
	L"Opera",
	L"Chamber Music",
	L"Sonata",
	L"Symphony",
	L"Booty Bass",
	L"Primus",
	L"Porn Groove",
	L"Satire",
	/*
	* winamp 1.0에서 추가된 장르
	*/
				L"Slow Jam",
				L"Club",
				L"Tango",
				L"Samba",
				L"Folklore",
				L"Ballad",
				L"Power Ballad",
				L"Rhythmic Soul",
				L"Freestyle",
				L"Duet",
				L"Punk Rock",
				L"Drum Solo",
				L"A Capella",
				L"Euro-House",
				L"Dance Hall",
				L"Goa",
				L"Drum & Bass",
				L"Club-House",
				L"Hardcore",
				L"Terror",
				L"Indie",
				L"BritPop",
				L"Negerpunk",
				L"Polsk Punk",
				L"Beat",
				L"Christian Gangsta Rap",
				L"Heavy Metal",
				L"Black Metal",
				L"Crossover",
				L"Contemporary Christian",
				L"Christian Rock",
				/* 
				* winamp 1.91에서 추가된 장르
				*/
					L"Merengue",
					L"Salsa",
					L"Thrash Metal",
					L"Anime",
					L"Jpop",
					L"Synthpop",
					/*
					* winamp 5.6에서 추가된 장르
					*/
						L"Abstract",
						L"Art Rock",
						L"Baroque",
						L"Bhangra",
						L"Big Beat",
						L"Breakbeat",
						L"Chillout",
						L"Downtempo",
						L"Dub",
						L"EBM",
						L"Eclectic",
						L"Electro",
						L"Electroclash",
						L"Emo",
						L"Experimental",
						L"Garage",
						L"Global",
						L"IDM",
						L"Illbient",
						L"Industro - Goth",
						L"Jam Band",
						L"Krautrock",
						L"Leftfield",
						L"Lounge",
						L"Math Rock",
						L"New Romantic",
						L"Nu - Breakz",
						L"Post - Punk",
						L"Post - Rock",
						L"Psytrance",
						L"Shoegaze",
						L"Space Rock",
						L"Trop Rock",
						L"World Music",
						L"Neoclassical",
						L"Audiobook",
						L"Audio theatre",
						L"Neue Deutsche Welle",
						L"Podcast",
						L"Indie - Rock",
						L"G - Funk",
						L"Dubstep",
						L"Garage Rock",
						L"Psybient"
	}; ///<장르 테이블
}
#endif