#ifndef MUSICLIBRARY_HPP
#define MUSICLIBRARY_HPP

#include <string>
#include <vector>
#include <functional>
#include "Song.hpp"

class GenreNode {
    public:
        std::string name;
        GenreNode* left;
        GenreNode* right;
        std::vector<Song> songs;

        GenreNode(const std::string& name) : name(name), left(nullptr), right(nullptr) {}
};

class MusicLibrary {
    private:
        GenreNode* root;

        GenreNode* insertGenreHelper(GenreNode* node, const std::string& genre);
        GenreNode* searchGenreHelper(GenreNode* node, const std::string& genre) const;
        void inOrderTraversal(GenreNode* node, std::function<void(GenreNode*)> visit) const;
        bool removeSongHelper(GenreNode* node, const std::string& title);
        void deleteTree(GenreNode* node);

    public:
        MusicLibrary();
        ~MusicLibrary();

        void addSong(const Song& song);
        bool removeSongByTitle(const std::string& title);
        Song* getSongByTitle(const std::string& title) const;
        std::vector<std::string> getSongMenuOptions() const;
};

#endif