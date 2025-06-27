#include <RecentlyPlayed.hpp>
#include <iostream>

RecentlyPlayed::RecentlyPlayed(size_t maxSize) : maxSize(maxSize) {}

void RecentlyPlayed::addSong(const Song& song) {
    if (recentlyPlayedSongs.size() >= maxSize) {
        recentlyPlayedSongs.pop_front();
    }
    recentlyPlayedSongs.push_back(song);
}

void RecentlyPlayed::clear() {
    recentlyPlayedSongs.clear();
}

Song RecentlyPlayed::getCurrentSong() const {
    if (!recentlyPlayedSongs.empty()) {
        return recentlyPlayedSongs.back();
    }
    throw std::runtime_error("No recently played song.");
}

const std::deque<Song>& RecentlyPlayed::getAllRecentlyPlayed() const {
    return recentlyPlayedSongs;
}
