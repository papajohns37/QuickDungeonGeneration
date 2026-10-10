#include "WallThicknessEnforcer.h"

void WallThicknessEnforcer::enforceWallThickness(TileMap& map, const RandomWalkRoomGeneratorConfig& config){
    //Enforce wall width
    for (int y = config.topPadding; y < config.mapHeight - config.bottomPadding; y++){
        int count = 0;
        for (int x = config.leftPadding; x < config.mapWidth - config.rightPadding; x++){
            if (map(x, y) != config.floorValue){
                count++;
            } else {
                if (count < config.wallWidthMinimum){
                    for (int xd = x - count; xd < x; xd++){
                        map(xd, y) = config.floorValue;
                    }
                }
                count = 0;
            }
        }
        const int end = config.mapWidth - config.rightPadding;
        if (count > 0 && count < config.wallWidthMinimum) {
            for (int xd = end - count; xd < end; xd++)
                map(xd, y) = config.floorValue;
        }
    }

    //Enforce wall height
    for (int x = config.leftPadding; x < config.mapWidth - config.rightPadding; x++){
        int count = 0;
        for (int y = config.topPadding; y < config.mapHeight - config.bottomPadding; y++){
            if (map(x, y) != config.floorValue){
                count++;
            } else {
                if (count < config.wallHeightMinimum){
                    for (int yd = y - count; yd < y; yd++){
                        map(x, yd) = config.floorValue;
                    }
                }
                count = 0;
            }
        }
        const int end = config.mapHeight - config.bottomPadding;
        if (count > 0 && count < config.wallHeightMinimum) {
            for (int yd = end - count; yd < end; yd++)
                map(x, yd) = config.floorValue;
        }
    }
}