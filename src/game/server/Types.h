#ifndef GAME_SERVER_TYPES_H
#define GAME_SERVER_TYPES_H

enum
{
    MINETYPE_NONE = 0,
    MINETYPE_COPPER,
    MINETYPE_LEAD,
    MINETYPE_COAL,
};

enum
{
    AMMOTYPE_NONE = 0, // Done. (just nothing XD)
    AMMOTYPE_Dmg, // Done.
    AMMOTYPE_BreakDefense, // Done.
    AMMOTYPE_Fire, // Done.
    AMMOTYPE_Slime, // WIP: Slow motion for Target Tee.
    AMMOTYPE_Freeze, // WIP: Freeze you Target Tee.

    AMMOTYPE_END,

    NUM_AMMOTYPE = AMMOTYPE_END - 1,
};

enum
{
    // Damage
    AMMOPRICE_Copper_Dmg = 2,
    
    // Break Defense
    AMMOPRICE_Lead_BreakDefense = 5,
    
    // Fire
    AMMOPRICE_Lead_Fire = 5,
    AMMOPRICE_Coal_Fire = 3,
    
    // Slime
    AMMOPRICE_Copper_Slime = 5,
    AMMOPRICE_Lead_Slime = 5,
    AMMOPRICE_Coal_Slime = 2,
    
    // Freeze!
    AMMOPRICE_Copper_Freeze = 20,
    AMMOPRICE_Lead_Freeze = 20,
    AMMOPRICE_Coal_Freeze = 20,
};

#endif