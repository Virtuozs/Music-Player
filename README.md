# Music Player Application

A terminal-based music player with playlist management, organized music library, and recently played tracking.

## Features

- 🎵 **Music Library Management**
  - Organize songs by genre (automatically sorted in a binary tree)
  - Add/remove songs
  - View all songs grouped by genre

- 🎧 **Playlist System**
  - Create multiple playlists
  - Add/remove songs from playlists
  - Navigate through playlist (next/previous song)
  - View current playing song

- ⏮ **Recently Played Tracking**
  - Maintains history of last 15 played songs
  - View recently played items

- 🖥 **NCurses-based UI**
  - Interactive menus and navigation
  - Clean terminal interface
  - Keyboard-controlled

## Installation

### Prerequisites
- C++17 compiler
- CMake (version 3.10 or higher)
- NCurses library

### Build Instructions

```bash
# Clone the repository
git clone https://github.com/yourusername/music-player.git
cd music-player

# Create build directory
mkdir build && cd build

# Build the project
cmake ..
make

# Run the application
./music_player
```

## Usage

### Main Menu Navigation
- Use Up/Down arrow keys to navigate
- Enter to select
- q to go back/exit


