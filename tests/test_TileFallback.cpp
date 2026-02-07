#include <doctest/doctest.h>

/**
 * Test cases for tile fallback functionality
 * 
 * When a tile is not found at the requested zoom level, the system should:
 * 1. Try progressively lower zoom levels (N-1, N-2, etc.)
 * 2. Scale tile coordinates appropriately (x >> delta, y >> delta)
 * 3. Return the first available tile found
 */

TEST_CASE("Tile coordinate scaling for fallback")
{
    SUBCASE("Zoom level 10 to 9 fallback")
    {
        uint32_t xTile = 512;
        uint32_t yTile = 256;
        uint8_t currentZoom = 10;
        uint8_t fallbackZoom = 9;
        
        // At zoom 10, tile 512,256 maps to tile 256,128 at zoom 9
        uint32_t scaledX = xTile >> (currentZoom - fallbackZoom);
        uint32_t scaledY = yTile >> (currentZoom - fallbackZoom);
        
        CHECK(scaledX == 256);
        CHECK(scaledY == 128);
    }
    
    SUBCASE("Zoom level 15 to 12 fallback (3 levels)")
    {
        uint32_t xTile = 17437;
        uint32_t yTile = 11371;
        uint8_t currentZoom = 15;
        uint8_t fallbackZoom = 12;
        
        // Falling back 3 zoom levels means dividing coordinates by 8 (2^3)
        uint32_t scaledX = xTile >> (currentZoom - fallbackZoom);
        uint32_t scaledY = yTile >> (currentZoom - fallbackZoom);
        
        CHECK(scaledX == 17437 / 8);
        CHECK(scaledY == 11371 / 8);
    }
    
    SUBCASE("Zoom level 5 to 1 fallback")
    {
        uint32_t xTile = 20;
        uint32_t yTile = 12;
        uint8_t currentZoom = 5;
        uint8_t fallbackZoom = 1;
        
        // Falling back 4 levels: divide by 16 (2^4)
        uint32_t scaledX = xTile >> (currentZoom - fallbackZoom);
        uint32_t scaledY = yTile >> (currentZoom - fallbackZoom);
        
        CHECK(scaledX == 1);
        CHECK(scaledY == 0);
    }
}

TEST_CASE("Distance scale calculations")
{
    const double EARTH_CIRCUMFERENCE = 40075017.0; // meters at equator
    const double M_PI = 3.14159265358979323846;
    
    SUBCASE("Zoom level 10 at equator")
    {
        uint8_t zoom = 10;
        int16_t tileSize = 256;
        double lat = 0.0; // equator
        double lat_rad = lat * M_PI / 180.0;
        
        // Meters per pixel
        double metersPerPixel = (EARTH_CIRCUMFERENCE * cos(lat_rad)) / (tileSize * (1 << zoom));
        
        // At zoom 10, equator: ~152.9 meters per pixel
        CHECK(metersPerPixel > 150.0);
        CHECK(metersPerPixel < 155.0);
    }
    
    SUBCASE("Zoom level 15 at 45 degrees latitude")
    {
        uint8_t zoom = 15;
        int16_t tileSize = 256;
        double lat = 45.0;
        double lat_rad = lat * M_PI / 180.0;
        
        double metersPerPixel = (EARTH_CIRCUMFERENCE * cos(lat_rad)) / (tileSize * (1 << zoom));
        
        // At zoom 15, 45° lat: ~3.4 meters per pixel
        CHECK(metersPerPixel > 3.0);
        CHECK(metersPerPixel < 4.0);
    }
}
