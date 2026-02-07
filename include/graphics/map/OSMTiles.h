#pragma once

#include "graphics/map/GeoPoint.h"
#include "graphics/map/MapTileSettings.h"
#include <cstdio>
#include <cstring>
#include <functional>
#include <stdint.h>

#ifndef IMG_PATH_LEN
#define IMG_PATH_LEN 64
#endif

/**
 * Generic OSM raster tile loader (singleton class).
 * Of course can also load other raster styles than just osm, lol.
 */
template <class IMG> class OSMTiles
{
  public:
    struct Tile : public GeoPoint {
        Tile(float lat, float lon, uint8_t zoom) : GeoPoint(lat, lon, zoom) { filename[0] = '\0'; }
        Tile(uint32_t xTile, uint32_t yTile, uint8_t zoom) : GeoPoint(xTile, yTile, zoom) { filename[0] = '\0'; }
        char filename[IMG_PATH_LEN];
    };

    // create instance of this class and provide cb function for loading images
    static OSMTiles *create(std::function<bool(const char *, IMG *)> cb);

    // filename caching for GeoPoint tile with zoom level fallback
    bool load(OSMTiles::Tile &tile, IMG *img)
    {
        if (!tile.filename[0]) {
            std::snprintf(tile.filename, IMG_PATH_LEN, "%s/%s%d/%d/%d.%s", MapTileSettings::getPrefix(),
                          MapTileSettings::getTileStyle(), tile.zoomLevel, tile.xTile, tile.yTile,
                          MapTileSettings::getTileFormat());
        }
        
        // Try to load the requested tile at the current zoom level
        if (loadcb(tile.filename, img)) {
            return true;
        }
        
        // If tile not found, try fallback to lower zoom levels
        // Each zoom level down means tile coordinates are divided by 2
        for (uint8_t fallbackZoom = tile.zoomLevel - 1; fallbackZoom > 0 && fallbackZoom < tile.zoomLevel; fallbackZoom--) {
            // Calculate tile coordinates at lower zoom level
            uint32_t scaledX = tile.xTile >> (tile.zoomLevel - fallbackZoom);
            uint32_t scaledY = tile.yTile >> (tile.zoomLevel - fallbackZoom);
            
            char fallbackName[IMG_PATH_LEN];
            std::snprintf(fallbackName, IMG_PATH_LEN, "%s/%s%d/%d/%d.%s", MapTileSettings::getPrefix(),
                          MapTileSettings::getTileStyle(), fallbackZoom, scaledX, scaledY,
                          MapTileSettings::getTileFormat());
            
            if (loadcb(fallbackName, img)) {
                // Successfully loaded a lower zoom level tile
                return true;
            }
        }
        
        return false;
    }

  protected:
    bool load(const GeoPoint &tile, IMG *img)
    {
        char name[IMG_PATH_LEN];
        std::snprintf(name, IMG_PATH_LEN, "%s/%s%d/%d/%d.%s", MapTileSettings::getPrefix(), MapTileSettings::getTileStyle(),
                      tile.zoomLevel, tile.xTile, tile.yTile, MapTileSettings::getTileFormat());
        return loadcb(name, img);
    }

  private:
    OSMTiles() = default;
    static std::function<bool(const char *, IMG *)> loadcb;
};

template <class IMG> OSMTiles<IMG> *OSMTiles<IMG>::create(std::function<bool(const char *, IMG *)> cb)
{
    loadcb = cb;
    return new OSMTiles;
}

template <class IMG> std::function<bool(const char *, IMG *)> OSMTiles<IMG>::loadcb;
