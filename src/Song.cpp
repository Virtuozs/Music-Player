#include <Song.hpp>

std::string Song::getTitle() const {
    return title;
}

std::string Song::getArtist() const {
    return artist;
}

std::string Song::getGenre() const {
    return genre;
}

int Song::getDuration() const {
    return duration;
}

void Song::setTitle(const std::string& newTitle) {
    title = newTitle;
}

void Song::setArtist(const std::string& newArtist) {
    artist = newArtist;
}

void Song::setGenre(const std::string& newGenre) {
    genre = newGenre;
}

void Song::setDuration(int newDuration) {
    duration = newDuration;
}