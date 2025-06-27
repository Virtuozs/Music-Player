#include "Playlist.hpp"
#include <UI.hpp>
#include <iostream>

Playlist::Playlist() : head(nullptr), tail(nullptr), current(nullptr), size(0) {}

Playlist::~Playlist() {
    SongNode* currentNode = head;
    while (currentNode != nullptr) {
        SongNode* nextNode = currentNode->next;
        delete currentNode;
        currentNode = nextNode;
    }
}

void Playlist::addSong(const Song& song, UI& ui) {
    SongNode* temp = head;
    while (temp != nullptr) {
        if (temp->song.getTitle() == song.getTitle()) {
            ui.showMessage("Song \"" + song.getTitle() + "\" is already in the playlist.");
            return;
        }
        temp = temp->next;
    }

    SongNode* newNode = new SongNode(song);
    
    if (isEmpty()) {
        head = tail = current = newNode;
    } else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
    size++;
    ui.showMessage("Song \"" + song.getTitle() + "\" added to the playlist.");    
}

void Playlist::removeSong(const std::string& title) {
    SongNode* currentNode = head;
    while (currentNode != nullptr) {
        if (currentNode->song.getTitle() == title) {
            if (current == currentNode) {
                if (currentNode->next) {
                    current = currentNode->next;
                } else if (currentNode->prev) {
                    current = currentNode->prev;
                } else {
                    current = nullptr;
                }
            }

            if (currentNode->prev) {
                currentNode->prev->next = currentNode->next;
            } else {
                head = currentNode->next;
            }

            if (currentNode->next) {
                currentNode->next->prev = currentNode->prev;
            } else {
                tail = currentNode->prev;
            }

            delete currentNode;
            size--;
            return;
        }
        currentNode = currentNode->next;
    }
}


Song* Playlist::getCurrentSong() const {
    return current ? &current->song : nullptr;
}

void Playlist::playNext() {
    if (current && current->next) {
        current = current->next;
    } else {
        current = head;  
    }
}

void Playlist::playPrevious() {
    if (current && current->prev) {
        current = current->prev;
    } else {
        current = tail; 
    }
}

int Playlist::getSize() const {
    return size;
}

bool Playlist::isEmpty() const {
    return size == 0;
}

Playlist::SongNode* Playlist::getHead() {
    return head;
}
