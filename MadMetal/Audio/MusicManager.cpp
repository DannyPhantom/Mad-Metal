#include "MusicManager.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

MusicManager::MusicManager(Audio &audio) : m_audio(audio)
{
	addSong(AllGoodThingsIAmTheEnemySong(), GameFactory::Objects::OBJECT_UI_ALL_GOOD_THINGS_SONG_I_AM_THE_ENEMY);
	addSong(DisturbedVengefulOneSong(), GameFactory::Objects::OBJECT_UI_DISTURBED_SONG_TEXTURE_THE_VENGEFUL_ONE);
	addSong(SickPuppiedYoureGoingDownSong(), GameFactory::Objects::OBJECT_UI_SICK_PUPPIES_SONG_TEXTURE_YOURE_GOING_DOWN);
	addSong(MetalMusicDarknessFallsSong(), GameFactory::Objects::OBJECT_UI_METAL_MUSIC_SONG_DARKNESS_FALLS);
}


MusicManager::~MusicManager()
{
}


void MusicManager::addSong(Sound song, GameFactory::Objects songTexture) {
	m_songs.push_back(song);	
	m_songTextures.push_back(songTexture);
}

void MusicManager::playSong(int index) {
	m_audio.playMusic(m_songs.at(index), 1);
	GameFactory::instance()->makeObject(m_songTextures.at(index), NULL, NULL, NULL);
}

void MusicManager::changeSong() {
	srand(time(NULL));
	int songIndex = 0;
	do {
		songIndex = rand() % m_songs.size();
	} while (songIndex == m_currentSong.getLibraryIndex());
	m_currentSong = m_songs.at(songIndex);
	playSong(songIndex);
}

void MusicManager::update() {
	if (Audio::getMusicFinished()) {
		changeSong();
	}
}