#ifndef RECENTLYPLAYER_HPP
#define RECENTLYPLAYER_HPP

#include <deque>
#include <string>
#include <Song.hpp>

class RecentlyPlayed {
    private:
        std::deque<Song> recentlyPlayedSongs;
        size_t maxSize;

    public:
        RecentlyPlayed(size_t maxSize = 15);

        void addSong(const Song& song);
        void clear();
        Song getCurrentSong() const;
        const std::deque<Song>& getAllRecentlyPlayed() const;
};

#endif