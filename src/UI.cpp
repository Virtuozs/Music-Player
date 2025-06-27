#include <UI.hpp>

UI::UI() : isInitialized(false) {}

UI::~UI() {
    if (isInitialized) {
        finalize();
    }
}

void UI::initialize() {
    initscr();
    curs_set(0);
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    start_color();
    isInitialized = true;
}

void UI::finalize() {
    if (isInitialized) {
        endwin();
        isInitialized = false;
    }
}


int UI::showMenu(const std::string& title, const std::vector<std::string>& options) {
    if (!isInitialized) {
        throw std::runtime_error("UI not initialized");
    }

    size_t selected = 0;
    while (true)
    {
        print(title, 0, 0, true);
        for (size_t i = 0; i < options.size(); ++i) {
            print((i == selected ? "> " : " ") + options[i], i + 1, 0, i == selected);
        }

        refreshScreen();

        int key = getKeyPress();
        switch (key)
        {
            case KEY_UP: selected = (selected - 1 + options.size()) % options.size(); break;
            case KEY_DOWN: selected = (selected + 1) % options.size(); break;
            case 10: return selected;
            case 'q': return -1;
        }
    }
}

int UI::showMenuGrouped(const std::string& title, const std::vector<std::string>& options) {
    if (!isInitialized) {
        throw std::runtime_error("UI not initialized");
    }

    int selected = 0;

    auto isSelectable = [](const std::string& item) {
        return item.rfind("  - ", 0) == 0;
    };

    while (selected < (int)options.size() && !isSelectable(options[selected])) {
        ++selected;
    }

    while (true) {
        print(title, 0, 0, true);

        for (size_t i = 0; i < options.size(); ++i) {
            bool isHighlight = (i == selected);
            bool isSong = isSelectable(options[i]);

            if (!isSong && isHighlight) {
                print(options[i], i + 1, 0, false); 
            } else {
                print((isHighlight ? "> " : "  ") + options[i], i + 1, 0, isHighlight);
            }
        }

        refreshScreen();
        int key = getKeyPress();

        if (key == KEY_UP || key == KEY_DOWN) {
            int next = selected;
            do {
                if (key == KEY_UP)
                    next = (next - 1 + options.size()) % options.size();
                else if (key == KEY_DOWN)
                    next = (next + 1) % options.size();
            } while (!isSelectable(options[next]) && next != selected);

            selected = next;
        } else if (key == 10) {
            if (isSelectable(options[selected])) {
                return selected;
            }
        } else if (key == 'q') {
            return -1;
        }
    }
}

int UI::showMenuWithPlaylist(const std::string& title,
                             const std::vector<std::string>& options,
                             const std::string& playlistName,
                             Playlist& playlist) {
    if (!isInitialized) {
        throw std::runtime_error("UI not initialized");
    }

    size_t selected = 0;
    while (true) {
        clearScreen();

        print("Managing Playlist: " + playlistName, 0, 0, true);

        Playlist::SongNode* node = playlist.getHead();
        Song* current = playlist.getCurrentSong();

        int row = 1;
        while (node) {
            bool isCurrent = (&node->song == current);
            std::string line = (isCurrent ? ">> " : "   ") + node->song.getTitle() + (isCurrent ? "  [Now Playing]" : "");
            if (isCurrent) {
                attron(A_BOLD);
                mvprintw(row++, 0, "%s", line.c_str());
                attroff(A_BOLD);
            } else {
                mvprintw(row++, 0, "%s", line.c_str());
            }
            node = node->next;
        }

        row++;

        print(title, row++, 0, true);
        for (size_t i = 0; i < options.size(); ++i) {
            std::string prefix = (i == selected ? "> " : "  ");
            print(prefix + options[i], row + i, 0, i == selected);
        }

        refreshScreen();

        int key = getKeyPress();
        switch (key) {
            case KEY_UP:
                selected = (selected - 1 + options.size()) % options.size();
                break;
            case KEY_DOWN:
                selected = (selected + 1) % options.size();
                break;
            case 10: // Enter
                return selected;
            case 'q':
                return -1;
        }
    }
}



void UI::showMessage(const std::string& message) {
    if (!isInitialized) {
        throw std::runtime_error("UI not initialized");
    }

    clearScreen();
    print(message, 0, 0);
    print("Press any key to continue...", 1, 0);
    refreshScreen();
    getKeyPress();
}

bool UI::confirmAction(const std::string& message) {
    clearScreen();
    print(message, 0, 0);
    print("Are you sure? (y/n)", 1, 0);
    refreshScreen();

    while (true) {
        int ch = getKeyPress();
        if (ch == 'y' || ch == 'Y') return true;
        if (ch == 'n' || ch == 'N') return false;
    }
}

std::string UI::getStringInput(const std::string& prompt, int y, int x) {
    if (!isInitialized) {
        throw std::runtime_error("UI not initialized");
    }

    echo();
    char stringBuffer[256];
    print(prompt, y, x);
    getnstr(stringBuffer, sizeof(stringBuffer) - 1);
    noecho();
    return std::string(stringBuffer);
}

int UI::getIntInput(const std::string& prompt, int y, int x) {
    if (!isInitialized) {
        throw std::runtime_error("UI not initialized");
    }

    echo();
    char inputBuffer[64];
    int result = 0;
    bool validInput = false;

    while (!validInput) {
        move(y, x);
        clrtoeol();
        print(prompt, y, x);
        getnstr(inputBuffer, sizeof(inputBuffer) - 1);

        try {
            result = std::stoi(inputBuffer);
            validInput = true;
        } catch (const std::invalid_argument&) {
            print("Invalid input. Please enter a number.", y + 1, x);
            refreshScreen();
        } catch (const std::out_of_range&) {
            print("Input out of range. Please enter a smaller number.", y + 1, x);
            refreshScreen();
        }
    }

    noecho();
    return result;
}

void UI::print(const std::string& text, int y, int x, bool highlight) {
    if (!isInitialized) {
        throw std::runtime_error("UI not initialized");
    }

    move(y, x);
    clrtoeol();

    if (highlight) {
        attron(A_BOLD); 
        mvprintw(y, x, "%s", text.c_str());
        attroff(A_BOLD);
    } else {
        mvprintw(y, x, "%s", text.c_str());
    }
}

int UI::getMaxRows() const {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_row;
}

void UI::clearScreen() { ::clear(); }

void UI::refreshScreen() { ::refresh(); }

int UI::getKeyPress() { return getch(); }
