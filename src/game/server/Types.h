#ifndef GAME_SERVER_TYPES_H
#define GAME_SERVER_TYPES_H

// Mine Types.
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
    AMMOTYPE_Force, // Done.
    AMMOTYPE_Fire, // Done.
    AMMOTYPE_Slime, // WIP: Slow motion for Target Attacker.
    AMMOTYPE_Freeze, // WIP: Freeze you Target Attacker.

    AMMOTYPE_END,

    NUM_AMMOTYPE = AMMOTYPE_END - 1,
};

// Ammo Types.
enum
{
    // Damage
    AMMOPRICE_Copper_Dmg = 2,
    
    // Force
    AMMOPRICE_Lead_Force = 0,
    
    // Explosion
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

// Attacker Types.
enum
{
    AttackerType_None,
    AttackerType_Dagger,
    AttackerType_Crawler,

    END_AttackerType,

    NUM_AttackerType = END_AttackerType - 1,
};

// Move Type.
enum
{
    MoveType_STOP,
    MoveType_Up,
    MoveType_Right,
    MoveType_Down,
    MoveType_Left,
    MoveType_Random,
    END_MoveType,

    NUM_MoveType = END_MoveType - 1,
};
#endif