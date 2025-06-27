#include "PlaylistManager.hpp"

bool PlaylistManager::addPlaylist(const std::string& name) {
    if (playlists.count(name)) return false;
    playlists[name] = Playlist();
    return true;
}

bool PlaylistManager::removePlaylist(const std::string& name) {
    return playlists.erase(name) > 0;
}

std::vector<std::string> PlaylistManager::getPlaylistNames() const {
    std::vector<std::string> names;
    for (const auto& pair : playlists) {
        names.push_back(pair.first);
    }
    return names;
}

std::unordered_map<std::string, Playlist>& PlaylistManager::getPlaylists(){
    return playlists;
}

Playlist* PlaylistManager::getPlaylist(const std::string& name) {
    auto it = playlists.find(name);
    if (it != playlists.end()) {
        return &it->second;
    }
    return nullptr;
}
