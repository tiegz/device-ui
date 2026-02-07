# Map Tile Zoom Level Fallback and Distance Scale Features

## Overview

This implementation adds two new features to the map display:

1. **Automatic Zoom Level Fallback**: When a tile at the requested zoom level is not available, the system automatically tries lower zoom levels and stretches the tile to provide visual reference.

2. **Distance Scale Display**: A dynamic distance scale overlay that automatically adjusts based on the current zoom level and latitude.

## Feature 1: Zoom Level Fallback

### How It Works

When the map tries to load a tile at zoom level N but the tile is not found:
1. The system tries zoom level N-1
2. If not found, tries N-2, and so on, down to zoom level 1
3. Tile coordinates are automatically scaled: `scaledX = xTile >> (zoomLevel - fallbackZoom)`
4. The first available tile found is loaded and stretched by LVGL to fill the space

### Example

```
Requested: zoom 15, tile (17437, 11371)
Not found at zoom 15, trying fallback...
Trying zoom 14, tile (8718, 5685)  [coordinates divided by 2]
Trying zoom 13, tile (4359, 2842)  [coordinates divided by 4]
Found tile at zoom 13! Loading and stretching to cover zoom 15 area.
```

### Benefits

- No more blank areas on the map when high-resolution tiles are missing
- Progressive degradation provides context even at lower detail
- Automatic and transparent to the user
- Extends all the way to the highest zoom level for continuity

### Code Location

`include/graphics/map/OSMTiles.h`, lines 44-60

## Feature 2: Distance Scale

### How It Works

The distance scale is calculated using the Mercator projection formula:

```cpp
metersPerPixel = (EARTH_CIRCUMFERENCE * cos(latitude_rad)) / (tileSize * 2^zoom)
```

The scale automatically selects nice round numbers from:
- 20m, 50m, 100m, 200m, 500m (for close zooms)
- 1km, 2km, 5km, 10km, 20km (for far zooms)

### Usage

```cpp
// Enable the distance scale
mapPanel.setDistanceScaleVisible(true);

// The scale updates automatically when:
// - Zoom level changes (setZoom)
// - Map position changes (center, scroll, moveHome, moveCurrent)
// - Panel dimensions change (updateDimensions)

// Disable the distance scale
mapPanel.setDistanceScaleVisible(false);
```

### Display Format

- Meters for distances under 1km: "20 m", "500 m"
- Kilometers for larger distances: "2 km", "5 km", "10 km"
- Automatically omits decimals for whole numbers

### Code Location

- Declaration: `include/graphics/map/MapPanel.h`, lines 59-60, 101
- Implementation: `source/graphics/map/MapPanel.cpp`, lines 501-574

## Testing

Unit tests are provided in `tests/test_TileFallback.cpp`:

1. **Tile Coordinate Scaling Tests**: Verify that tile coordinates are correctly scaled when falling back through zoom levels
2. **Distance Scale Calculation Tests**: Verify the Mercator projection math at various zoom levels and latitudes

## Technical Details

### Tile Coordinate Scaling Formula

At zoom level `z`, there are `2^z` tiles in each direction.
When falling back from zoom `N` to zoom `M`:
```
scaledX = xTile >> (N - M)  // Bit shift right = divide by 2^(N-M)
scaledY = yTile >> (N - M)
```

### Distance Per Pixel Formula

```
metersPerPixel = (40075017 * cos(lat_rad)) / (256 * 2^zoom)
```

Where:
- 40075017 = Earth's circumference in meters at the equator
- cos(lat_rad) = Mercator projection correction for latitude
- 256 = tile size in pixels
- 2^zoom = number of tiles per dimension

## Backwards Compatibility

Both features are fully backward compatible:

1. **Zoom Fallback**: Automatically enabled. If all fallback levels fail, the system falls back to the existing behavior (showing noTileImage placeholder).

2. **Distance Scale**: Disabled by default. Call `setDistanceScaleVisible(true)` to enable.

## Performance Considerations

- **Fallback**: Tries each zoom level sequentially until a tile is found. In the worst case (no tiles available), it tries all zoom levels from N-1 to 1.
- **Distance Scale**: Calculations are lightweight (one cosine, one division, one loop through 10 options). Updates only occur on zoom/position changes, not every frame.
