#pragma once

namespace BlockHeightType
{
    enum
    {
        HT_PLATE,
        HT_U_45,
        HT_D_45,
        HT_FULL_BlOCK,
        HT_UD_225,
        HT_UU_225,
        HT_DU_225,
        HT_DD_225,
        HT_UD_675,
        HT_UU_675,
        HT_DU_675,
        HT_DD_675,
        HT_VERTICAL_PILLAR,
        HT_HORIZONTAL_PILLAR,
        HT_CONNECTABLE_PILLAR,
        HT_STRUCTURE_BLOCK,
        HT_UNDEFINED
    };
}

namespace BlockMetadata
{
    enum Normal
    {
        META_ZERO,
        META_1,
        META_2,
        META_3,
        META_4,
        META_5,
        META_6,
        META_7,
        META_8,
        META_9,
        META_A,
        META_B,
        META_C,
        META_D,
        META_E,
        META_DEFAULT,
        META_UNDEFINED
    };
    enum Grass
    {
        GRASSMETA_UL45,
        GRASSMETA_UR45,
        GRASSMETA_UL225,
        GRASSMETA_UR225,
        GRASSMETA_UL675,
        GRASSMETA_UR675,
        GRASSMETA_LEFT,
        GRASSMETA_RIGHT,
        GRASSMETA_DL45,
        GRASSMETA_DR45,
        GRASSMETA_DL225,
        GRASSMETA_DR225,
        GRASSMETA_DL675,
        GRASSMETA_DR675,
        GRASSMETA_DOWN,
        GRASSMETA_UP,
        GRASSMETA_UNDEFINED
    };
}

namespace BlockType
{
    enum
    {
        TYPE_AIR,
        TYPE_STONE,
        TYPE_DIRT,
        TYPE_GRASS,
        TYPE_BRICK,
        TYPE_STONEBRICK,
        TYPE_GLASS,
        TYPE_PLANKS,
        TYPE_WOOD,
        TYPE_LEAVES,
        TYPE_WINDOWS,
        TYPE_DOORS,
        TYPE_RESERVED_C,
        TYPE_RESERVED_D,
        TYPE_RESERVED_E,
        TYPE_RESERVED_F,
    };
}
