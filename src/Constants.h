#pragma once

namespace Constants {
    // Map dimensions
    constexpr int MIN_MAP_WIDTH = 1;
    constexpr int MIN_MAP_HEIGHT = 1;
    constexpr int MAX_MAP_WIDTH = 1024;
    constexpr int MAX_MAP_HEIGHT = 1024;
    constexpr int DEFAULT_NEW_MAP_WIDTH = 32;
    constexpr int DEFAULT_NEW_MAP_HEIGHT = 32;

    // Tile settings
    constexpr int DEFAULT_TILE_SIZE = 32;
    constexpr int PALETTE_TILE_COUNT = 12;

    // Window settings
    constexpr int DEFAULT_WINDOW_WIDTH = 800;
    constexpr int DEFAULT_WINDOW_HEIGHT = 600;

    // View settings
    constexpr int GRID_STEP = 20;
    constexpr int SCENE_WIDTH = 4000;
    constexpr int SCENE_HEIGHT = 3000;
    constexpr double ZOOM_STEP = 1.25;
    constexpr double MIN_ZOOM = 0.25;
    constexpr double MAX_ZOOM = 4.0;
    
    // Tools
    constexpr int TOOL_PAINT = 0;
    constexpr int TOOL_FILL = 1;
}
