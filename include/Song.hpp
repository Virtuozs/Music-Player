#ifndef SONG_HPP
#define SONG_HPP

#include <string>

class Song {
    private:
        std::string title;
        std::string artist;
        std::string genre;
        int duration;
    
    public:
        Song(const std::string& title, const std::string& artist, const std::string& genre, int duration)
            : title(title), artist(artist), genre(genre), duration(duration) {}

        std::string getTitle() const;
        std::string getArtist() const;
        std::string getGenre() const;
        int getDuration() const;

        void setTitle(const std::string& newTitle);
        void setArtist(const std::string& newArtist);
        void setGenre(const std::string& newGenre);
        void setDuration(int newDuration);

        void play() const;
        void pause() const;
        void stop() const;

        void displayInfo() const;
};

#endif