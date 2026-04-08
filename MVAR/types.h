#ifndef __MVAR_TYPES_H__
#define __MVAR_TYPES_H__

#ifdef __cplusplus
extern "C"{
#endif
    
typedef unsigned char  ET_UInt8;
typedef signed char    ET_Int8;
typedef unsigned short ET_UInt16;
typedef signed short   ET_Int16;
typedef unsigned int   ET_UInt32;
typedef signed int     ET_Int32;
typedef signed short   F2DOT14;
typedef unsigned char  ET_Bool;

#define ET_True  1
#define ET_False 0

#ifndef NULL
#define NULL 0
#endif

#define ET_MAKE_TAG( a, b, c, d )   ((((ET_UInt32)(a)) << 24) | \
                                     (((ET_UInt32)(b)) << 16) | \
                                     (((ET_UInt32)(c)) <<  8) | \
                                     (ET_UInt32)(d) )

#define ET_CONVERSE16( V )    ((((ET_UInt16)(V) & 0xFF00) >> 8) | (((ET_UInt16)(V) & 0xFF) << 8))
#define ET_CONVERSE32( V )    ((((ET_UInt132)(V) & 0xFF000000) >> 24) | (((ET_UInt32)(V) & 0xFF0000) >> 8) | (((ET_UInt32)(V) & 0xFF00) << 8) | (((ET_UInt32)(V) & 0xFF) << 24))

#define ET_WRITE_8(P, V)      {(*(P)) = (V); (P)++;}
#define ET_WRITE_16(P, V)     {((ET_UInt8*)(P))[0] = ((V) & 0xFF00) >> 8; ((ET_UInt8*)(P))[1] = ((V) & 0xFF); ((ET_UInt8*)(P)) += 2;}
#define ET_WRITE_32(P, V)     {((ET_UInt8*)(P))[0] = ((V) & 0xFF000000) >> 24; ((ET_UInt8*)(P))[1] = ((V) & 0xFF0000) >> 16; ((ET_UInt8*)(P))[2] = ((V) & 0xFF00) >> 8; ((ET_UInt8*)(P))[3] = ((V) & 0xFF); ((ET_UInt8*)(P)) += 4;}

typedef struct RegionAxisCoordinates_
{
    F2DOT14    startCoord;
    F2DOT14    peakCoord;
    F2DOT14    endCoord;
}RegionAxisCoordinates;


typedef struct VariationRegionList_
{
    ET_UInt16    axisCount;
    ET_UInt16    regionCount;
    RegionAxisCoordinates*  records; // axisCount * regionCount        
}VariationRegionList;

typedef struct ItemVariationDataSubtable_
{
    ET_UInt16    itemCount;
    ET_UInt16    wordDeltaCount;
    ET_UInt16    regionIndexCount;
    ET_UInt16*   regionIndexArray;
    ET_UInt32*   deltas; //itemCount * regionIndexCount;
}ItemVariationDataSubtable;

typedef struct ItemVariationStore_
{
    ET_UInt16             format;
    VariationRegionList   regionList;    
    ET_UInt16             itemVariationDataCount;
    ItemVariationDataSubtable* subtables; // itemVariationDataCount
}ItemVariationStore;


typedef struct MVAR_ValueRecord_
{
    ET_UInt32    valueTag;
    ET_UInt16    deltaSetOuterIndex;
    ET_UInt16    deltaSetInnerIndex;
}MVAR_ValueRecord;

typedef struct MVARTable_
{
    ET_UInt16    major;
    ET_UInt16    minor;
    ET_UInt16    valueRecordSize;
    ET_UInt16    valueRecordCount;
    MVAR_ValueRecord*  valueRecords;
    ItemVariationStore    itemVariationStore;
}MVARTable;

#ifdef __cplusplus
}
#endif

#endif
