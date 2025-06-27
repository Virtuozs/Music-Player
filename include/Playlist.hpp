#ifndef PLAYLIST_HPP
#define PLAYLIST_HPP

#include <Song.hpp>
class UI;

class Playlist {
    public:
        struct SongNode {
            Song song;
            SongNode* next;
            SongNode* prev;

            SongNode(const Song& song) : song(song), next(nullptr), prev(nullptr) {}
        };
        Playlist();
        ~Playlist();

        void addSong(const Song& song, UI& ui);
        void removeSong(const std::string& title);

        Song* getCurrentSong() const;
        
        void playNext();
        void playPrevious();

        // void displayPlaylist() const;
        SongNode* getHead();
        int getSize() const;
        bool isEmpty() const;

    private:
        SongNode* head;
        SongNode* tail;
        SongNode* current;
        int size;
    
};

#endif