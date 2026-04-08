#include "types.h"

ET_UInt32 calc_space_need( ItemVariationStore* table )
{
    ET_UInt32 need = 8; // format + variationRegionListOffset + itemVariationDataCount;
    ET_UInt32 i;

    if( !table )
        return 0;

    need += 4 * table->itemVariationDataCount;  // + ItemVariationDataOffsets array offset

    for( i = 0; i < table->itemVariationDataCount; i++ )
    {
        ET_Bool             bLong = ET_False;
        ET_UInt32 longdeltaCount = 0;
        need += 6;   // itemCount + wordDeltaCount + regionIndexCount
        need += 2 * table->subtables[i].regionIndexCount;  // + regionIndexes array
        if( table->subtables[i].wordDeltaCount & 0x8000 )
            bLong = ET_True;

        longdeltaCount = table->subtables[i].wordDeltaCount & 0x7FFF;

        if( bLong )
            need += 2 * (table->subtables[i].regionIndexCount + longdeltaCount);
        else
            need += (table->subtables[i].regionIndexCount + longdeltaCount);

    }


    // VariationregionList
    need += 4; // axisCount + regionCount
    need += table->regionList.axisCount * table->regionList.regionCount * 3 * 2;

    return need;
}

ET_UInt32  Write_MVAR_VStore( ItemVariationStore* table, ET_UInt8* p )
{
    ET_UInt32    need = calc_space_need( table );    
    ET_UInt32  length = 0;
    ET_UInt8* variationRegionListOffset = NULL;
    ET_UInt8* itemVariationDataOffsets = NULL;
    ET_UInt32 i, j, k, z;

    if( !need )
        return ET_False;


    if( p )    ET_WRITE_16( p, table->format )    length += 2;

    if( p )    variationRegionListOffset = p;
    if( p )    p += 4;   length += 4; // variationRegionListOffset, to be written

    if( p )    ET_WRITE_16( p, table->itemVariationDataCount );   length += 2;

    if( p )    itemVariationDataOffsets = p;
    if( p )    p += 4 * table->itemVariationDataCount;    length += 4 * table->itemVariationDataCount; // itemVariationDataOffsets, to be written

    // first write Region List, offset is p - buffer;
    {
        RegionAxisCoordinates* regionAxis = table->regionList.records;
        ET_UInt32 offset = length;
        if( variationRegionListOffset ) ET_WRITE_32( variationRegionListOffset, offset )

        if( p )  ET_WRITE_16( p, table->regionList.axisCount )    length += 2;
        if( p )  ET_WRITE_16( p, table->regionList.regionCount )  length += 2;
        for( i=0; i < table->regionList.regionCount; i++ )
        {
            for( j=0; j < table->regionList.axisCount; j++ )
            {
                if( p )  ET_WRITE_16( p, regionAxis[i * table->regionList.axisCount + j].startCoord )  length += 2;
                if( p )  ET_WRITE_16( p, regionAxis[i * table->regionList.axisCount + j].peakCoord )   length += 2;
                if( p )  ET_WRITE_16( p, regionAxis[i * table->regionList.axisCount + j].endCoord )    length += 2;
            }
        }
    }

    // then write ItemVariationData subtable
    {
        for( i=0; i < table->itemVariationDataCount; i++ )
        {
            ET_UInt32 offset = length;
            if( itemVariationDataOffsets ) ET_WRITE_32( itemVariationDataOffsets, offset )

            ItemVariationDataSubtable* sub = &table->subtables[i];
            ET_Bool bLong = sub->wordDeltaCount & 0x8000;
            ET_UInt32 longCount = sub->wordDeltaCount & 0x7FFF;

            if( longCount > sub->regionIndexCount )
                longCount = sub->regionIndexCount;

            if( p )  ET_WRITE_16( p, sub->itemCount )         length += 2;
            if( p )  ET_WRITE_16( p, sub->wordDeltaCount )    length += 2;
            if( p )  ET_WRITE_16( p, sub->regionIndexCount )  length += 2;
            for( j=0; j < sub->regionIndexCount; j++ )
            {
                if( p )   ET_WRITE_16( p, sub->regionIndexArray[j] )  length += 2;
            }

            for( j=0; j < sub->itemCount; j++ )
            {
                for( k=0; k < sub->regionIndexCount; k++ )
                {
                    if( bLong )
                    {
                        if( k < longCount )
                        {
                            if( p )  ET_WRITE_32( p, sub->deltas[j * sub->regionIndexCount + k] )  length += 4;
                        }
                        else
                        {
                            if( p )  ET_WRITE_16( p, sub->deltas[j * sub->regionIndexCount + k] )  length += 2;
                        }
                    }
                    else
                    {
                        if( k < longCount )
                        {
                            if( p )  ET_WRITE_16( p, sub->deltas[j * sub->regionIndexCount + k] )  length += 2;
                        }
                        else
                        {
                            if( p )  ET_WRITE_8( p, sub->deltas[j * sub->regionIndexCount + k] )  length += 1;
                        }
                    }
                }
            }
        }
    }


    return length;
}


ET_UInt32  Write_MVAR( MVARTable* mvar, ET_UInt8* p )
{
    ET_UInt32 offset = 12 + mvar->valueRecordCount * mvar->valueRecordSize;
    ET_UInt32 length = 0;
    if( p )    ET_WRITE_16( p, mvar->major )    length += 2;
    if( p )    ET_WRITE_16( p, mvar->minor )    length += 2;

    if( p )    p += 2;                          length += 2; // skip reserved
    if( p )    ET_WRITE_16( p, mvar->valueRecordSize )  length += 2;
    if( p )    ET_WRITE_16( p, mvar->valueRecordCount ) length += 2;
    if( p )    ET_WRITE_16( p, offset )                 length += 2;

    for( int i=0; i < mvar->valueRecordCount; i++ )
    {
        if( p )  ET_WRITE_32( p, mvar->valueRecords[i].valueTag )            length += 4;
        if( p )  ET_WRITE_16( p, mvar->valueRecords[i].deltaSetOuterIndex )  length += 2;
        if( p )  ET_WRITE_16( p, mvar->valueRecords[i].deltaSetInnerIndex )  length += 2;
    }

    length += Write_MVAR_VStore( &mvar->itemVariationStore, p );

    return length;
}
