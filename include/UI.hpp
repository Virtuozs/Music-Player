#ifndef UI_HPP
#define UI_HPP

#include <Playlist.hpp>
#include <string>
#include <vector>
#include <ncurses.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdexcept>


class UI {
    private:
        bool isInitialized;
    public:
        UI();
        ~UI();

        void initialize();
        void finalize();

        int showMenu(const std::string& title, const std::vector<std::string>& options);
        int showMenuGrouped(const std::string& title, const std::vector<std::string>& options);
        int showMenuWithPlaylist(const std::string& title, const std::vector<std::string>& options, const std::string& playlistName, Playlist& playlist);
        void showMessage(const std::string& message);
        bool confirmAction(const std::string& message);

        std::string getStringInput(const std::string& prompt, int y = 0, int x = 0);
        int getIntInput(const std::string& prompt, int y = 0, int x = 0);

        void print(const std::string& text, int y = 0, int x = 0, bool highlight = false);
        int getMaxRows() const;
        void clearScreen();
        void refreshScreen();
        int getKeyPress();
};

#endif