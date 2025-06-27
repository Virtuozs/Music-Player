#include <MusicLibrary.hpp>
#include <iostream>
#include <algorithm>

MusicLibrary::MusicLibrary() : root(nullptr) {}

MusicLibrary::~MusicLibrary() {
    deleteTree(root);
}

void MusicLibrary::deleteTree(GenreNode* node) {
    if (!node) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

GenreNode* MusicLibrary::insertGenreHelper(GenreNode* node, const std::string& genreName) {
    if (!node) return new GenreNode(genreName);

    if (genreName < node->name)
        node->left = insertGenreHelper(node->left, genreName);
    else if (genreName > node->name)
        node->right = insertGenreHelper(node->right, genreName);
    return node;
}

GenreNode* MusicLibrary::searchGenreHelper(GenreNode* node, const std::string& genreName) const {
    if (!node || node->name == genreName)
        return node;
    if (genreName < node->name)
        return searchGenreHelper(node->left, genreName);
    else
        return searchGenreHelper(node->right, genreName);
}

void MusicLibrary::addSong(const Song& song) {
    std::string genreName = song.getGenre();
    GenreNode* genreNode = searchGenreHelper(root, genreName);
    if (!genreNode) {
        root = insertGenreHelper(root, genreName);
        genreNode = searchGenreHelper(root, genreName);
    }
    genreNode->songs.push_back(song);
}

void MusicLibrary::inOrderTraversal(GenreNode* node, std::function<void(GenreNode*)> visit) const {
    if (!node) return;
    inOrderTraversal(node->left, visit);
    visit(node);
    inOrderTraversal(node->right, visit);
}

std::vector<std::string> MusicLibrary::getSongMenuOptions() const {
    std::vector<std::string> options;
    std::function<void(GenreNode*)> visit = [&options](GenreNode* genre) {
        if (!genre->songs.empty()) {
            options.push_back("[" + genre->name + "]");
            for (const Song& song : genre->songs) {
                options.push_back("  - " + song.getTitle());
            }
        }
    };
    inOrderTraversal(root, visit);
    return options;
}

bool MusicLibrary::removeSongHelper(GenreNode* node, const std::string& title) {
    if (!node) return false;
    for (auto it = node->songs.begin(); it != node->songs.end(); ++it) {
        if (it->getTitle() == title) {
            node->songs.erase(it);
            return true;
        }
    }
    return removeSongHelper(node->left, title) || removeSongHelper(node->right, title);
}

bool MusicLibrary::removeSongByTitle(const std::string& title) {
    return removeSongHelper(root, title);
}

Song* MusicLibrary::getSongByTitle(const std::string& title) const {
    Song* result = nullptr;
    std::function<void(GenreNode*)> search = [&](GenreNode* genre) {
        for (const Song& song : genre->songs) {
            if (song.getTitle() == title) {
                result = new Song(song);
                return;
            }
        }
    };
    inOrderTraversal(root, search);
    return result;
}


