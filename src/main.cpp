#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>

#include "MusicLibrary.hpp"
#include "PlaylistManager.hpp"
#include "UI.hpp"
#include "Song.hpp"
#include "RecentlyPlayed.hpp"

std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    size_t last = str.find_last_not_of(" \t\n\r");
    return (first == std::string::npos || last == std::string::npos)
        ? ""
        : str.substr(first, last - first + 1);
}

void displayPlaylist(UI& ui, Playlist& playlist) {
    Playlist::SongNode* node = playlist.getHead();
    Song* currentSong = playlist.getCurrentSong();
    
    if (!node) {
        ui.print("No songs in the playlist.", 0, 0);
        return;
    }

    int yOffset = 2;
    while (node) {
        std::string songTitle = "  - " + node->song.getTitle();
        if (currentSong && node->song.getTitle() == currentSong->getTitle()) {
            songTitle += "  [Now Playing]";
        }

        ui.print(songTitle, yOffset, 0, (currentSong && node->song.getTitle() == currentSong->getTitle()));
        node = node->next;
        ++yOffset;
    }
}

void displaySongLibraryMenu(MusicLibrary& library, UI& ui) {
    ui.clearScreen();
    ui.print("Song Library", 0, 0, true);

    std::vector<std::string> libraryMenuOptions = {
        "1. Add Song",
        "2. View All Songs by Genre",
        "3. Remove Song",
    };

    while (true)
    {
        ui.clearScreen();
        int choice = ui.showMenu("Song Library", libraryMenuOptions);

        switch (choice) {
            case 0: {
                ui.clearScreen();
                ui.print("Add a new song to the library", 0, 0);
                std::string genre = ui.getStringInput("Enter Genre: ", 1, 0);
                std::string title = ui.getStringInput("Enter Song Title: ", 2, 0);
                std::string artist = ui.getStringInput("Enter Artist: ", 3, 0);
                int duration = ui.getIntInput("Enter Duration (in seconds): ", 4, 0);

                Song newSong(title, artist, genre, duration);
                library.addSong(newSong);
                ui.clearScreen();
                ui.showMessage("Song added successfully!");
                break;
            }
            case 1: {
                ui.clearScreen();
                std::vector<std::string> songMenu = library.getSongMenuOptions();
                std::vector<std::string> selectableSongs;

                for (const auto& entry : songMenu) {
                    if (entry.rfind("  - ", 0) == 0) {
                        selectableSongs.push_back(entry.substr(4));
                    }
                }

                if (selectableSongs.empty()) {

                    ui.showMessage("No songs available in the library.");
                } else {
                    ui.clearScreen();
                    int selected = ui.showMenuGrouped("Song list", songMenu);

                    if (selected >= 0 && selected < (int)songMenu.size()) {
                        const std::string& entry = songMenu[selected];

                        if (entry.rfind("  - ", 0) == 0) {
                            std::string selectedTitle = entry.substr(4);
                            ui.showMessage("You selected: " + selectedTitle);

                        } else {
                            ui.showMessage("Please select a song, not a genre.");
                        }
                    }
                }
                break;
            }
            case 2: {
                ui.clearScreen();
                std::vector<std::string> songOptions = library.getSongMenuOptions();

                if (songOptions.empty()) {
                    ui.showMessage("No songs available in the library.");
                    break;
                }
                
                ui.clearScreen();
                int selected = ui.showMenuGrouped("Select a song to delete", songOptions);

                if (selected >= 0 && selected < (int)songOptions.size()) {
                    std::string line = songOptions[selected];

                    std::string title = line.substr(4);

                    if (ui.confirmAction("Are you sure you want to delete \"" + title + "\"?")) {
                        bool removed = library.removeSongByTitle(title);
                        if (removed)
                            ui.showMessage("Song removed successfully.");
                        else
                            ui.showMessage("Song not found.");
                    }
                }
                break;
            }
            default:
                return;
        }
    }
    
}

void manageSinglePlaylist(UI& ui, const std::string& playlistName, Playlist& playlist, const MusicLibrary& library, RecentlyPlayed& recentlyPlayed) {
    while (true) {
        ui.clearScreen();
        std::vector<std::string> options = {
            "Add Song to Playlist",
            "Remove Song from Playlist",
            "Play Next Song",
            "Play Previous Song",
        };

        int choice = ui.showMenuWithPlaylist("Playlist Options", options, playlistName, playlist);

        switch (choice) {
            case 0: {
                ui.clearScreen();
                std::vector<std::string> songMenu = library.getSongMenuOptions();

                if (songMenu.empty()) {
                    ui.showMessage("No songs available in the library.");
                    break;
                }
                ui.clearScreen();
                int selected = ui.showMenuGrouped("Select a song to add", songMenu);
                if (selected >= 0 && selected < (int)songMenu.size()) {
                    std::string line = songMenu[selected];
                    
                    if (line.rfind("  - ", 0) == 0) {
                        std::string rawTitle = trim(line.substr(4));
                        Song* selectedSong = library.getSongByTitle(rawTitle);
                        if (selectedSong) {
                            playlist.addSong(*selectedSong, ui);
                        } else {
                            ui.showMessage("Error: Could not find song in library.");
                        }
                    }
                }
                break;
            }
            case 1: {
                std::vector<std::string> songOptions;
                std::vector<std::string> flatTitles;
            
                Song* temp = playlist.getCurrentSong();
                if (!temp) {
                    ui.showMessage("No songs in the playlist.");
                    break;
                }
            
                std::vector<std::string> playlistSongs;
                Playlist::SongNode* node = playlist.getHead();
                while (node) {
                    flatTitles.push_back(node->song.getTitle());
                    playlistSongs.push_back("  - " + node->song.getTitle());
                    node = node->next;
                }
            
                if (playlistSongs.empty()) {
                    ui.showMessage("No songs in the playlist.");
                    break;
                }
            
                songOptions.push_back("[Playlist-"+playlistName+"]");
                songOptions.insert(songOptions.end(), playlistSongs.begin(), playlistSongs.end());
                
                ui.clearScreen();
                int selected = ui.showMenuGrouped("Select a song to remove", songOptions);
                if (selected >= 0 && selected < (int)songOptions.size()) {
                    std::string line = songOptions[selected];
                    if (line.rfind("  - ", 0) == 0) {
                        std::string rawTitle = trim(line.substr(4));
                        playlist.removeSong(rawTitle);
                        ui.showMessage("Song \"" + rawTitle + "\" removed from playlist.");
                    }
                }
                break;
            }
            case 2:
                if (playlist.getHead() == nullptr) {
                    ui.showMessage("No songs in the playlist to play next.");
                    break;
                }
                playlist.playNext();
                recentlyPlayed.addSong(*playlist.getCurrentSong());
                break;
            case 3:
                if (playlist.getHead() == nullptr) {
                    ui.showMessage("No songs in the playlist to play previous.");
                    break;
                }
                playlist.playPrevious();
                recentlyPlayed.addSong(*playlist.getCurrentSong());
                break;
            default:
                return;
        }
    }
}

void displayManagePlaylistMenu(UI& ui,PlaylistManager& playlistManager, const MusicLibrary& library, RecentlyPlayed& recentlyPlayed) {
    ui.clearScreen();
    while (true) {
        std::vector<std::string> options = {
            "Add New Playlist",
            "Remove Existing Playlist",
            "Manage Existing Playlist",
        };
        auto& playlists = playlistManager.getPlaylists();
        ui.clearScreen();
        int choice = ui.showMenu("Manage Playlists", options);

        switch (choice) {
            case 0: { 
                ui.clearScreen();
                ui.print("Add a new playlist", 0, 0);
                std::string name = ui.getStringInput("Enter new playlist name: ", 1, 0);
                if (playlists.find(name) != playlists.end()) {
                    ui.showMessage("Playlist \"" + name + "\" already exists.");
                } else {
                    playlists[name] = Playlist();
                    ui.showMessage("Playlist \"" + name + "\" added.");
                }
                break;
            }
            case 1: {
                ui.clearScreen();
                ui.print("Remove an existing playlist", 0, 0);
                if (playlists.empty()) {
                    ui.showMessage("No playlists available to remove.");
                    break;
                }

                std::vector<std::string> playlistNames;
                for (const auto& pair : playlists) {
                    playlistNames.push_back(pair.first);
                }
                ui.clearScreen();
                int selected = ui.showMenu("Select Playlist to Remove", playlistNames);
                if (selected >= 0 && selected < (int)playlistNames.size()) {
                    std::string toRemove = playlistNames[selected];
                    playlists.erase(toRemove);
                    ui.showMessage("Playlist \"" + toRemove + "\" removed.");
                }
                break;
            }
            case 2: {
                if (playlists.empty()) {
                    ui.showMessage("No playlists available to manage.");
                    break;
                }

                std::vector<std::string> playlistNames;
                for (const auto& pair : playlists) {
                    playlistNames.push_back(pair.first);
                }
                ui.clearScreen();
                int selected = ui.showMenu("Select Playlist to Manage", playlistNames);
                if (selected >= 0 && selected < (int)playlistNames.size()) {
                    std::string selectedName = playlistNames[selected];
                    manageSinglePlaylist(ui, selectedName, playlists[selectedName], library, recentlyPlayed);
                }
                break;
            }
            case 3:
            case -1: 
                return;
        }
    }
}

void displayRecentlyPlayedMenu(UI& ui, RecentlyPlayed& recentlyPlayed) {
    while (true)
    {
        ui.clearScreen();
    
        const auto& songs = recentlyPlayed.getAllRecentlyPlayed();
        if (songs.empty()) {
            ui.showMessage("No recently played songs.");
            return;
        }
    
        std::vector<std::string> songTitles;
        for (size_t i = 0; i < songs.size(); ++i) {
            songTitles.push_back(std::to_string(i + 1) + ". " + songs[i].getTitle());
        }
    
        int choice = ui.showMenu("Recently Played Songs", songTitles);
        switch (choice)
        {
        default:
            return;
        }
    }
}


int main() {
    MusicLibrary library;
    PlaylistManager playlistManager;
    UI ui;
    RecentlyPlayed recentlyPlayed;

    ui.initialize();

    std::vector<std::string> mainMenuOptions = {
        "1. Song Library",
        "2. Manage Playlist",
        "3. Recently Played",
    };

    while (true) {
        ui.clearScreen();
        int choice = ui.showMenu("Main Menu", mainMenuOptions);

        switch (choice) {
            case 0:
                displaySongLibraryMenu(library, ui);
                break;
            case 1:{
                std::unordered_map<std::string, Playlist> playlists = playlistManager.getPlaylists();
                displayManagePlaylistMenu(ui, playlistManager, library, recentlyPlayed);
                break;
            }
            case 2:
                displayRecentlyPlayedMenu(ui, recentlyPlayed);
                break;
            case -1:
                return 0;
            default:
                ui.showMessage("Invalid choice, please try again.");
        }
        ui.refreshScreen();
    }

    ui.finalize();

    return 0;
}
