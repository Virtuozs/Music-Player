#ifndef PLAYLISTMANAGER_HPP
#define PLAYLISTMANAGER_HPP

#include "Playlist.hpp"
#include <unordered_map>
#include <string>
#include <vector>

class PlaylistManager {
    private:
        std::unordered_map<std::string, Playlist> playlists;
    
    public:
        bool addPlaylist(const std::string& name);
        bool removePlaylist(const std::string& name);
        std::vector<std::string> getPlaylistNames() const;
        std::unordered_map<std::string, Playlist>& getPlaylists();
        Playlist* getPlaylist(const std::string& name);
    };

#endif