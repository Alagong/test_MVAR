#include <vector>
#include "ttags.h"

typedef signed int FT_Error;

#define FT_Err_Too_Many_Tables  0x1110
#define ET_Err_Ok  0
#define ET_Err_Invalid_Argument 0x1000
#define ET_Err_Out_Of_Memory    0x1001
#define ET_Err_Invalid_Table    0x1002




typedef struct ET_OffsetTable_Rec
{
    INT       sfntVersion;
    USHORT    numTables;
    USHORT    searchRange;
    USHORT    entrySelector;
    USHORT    rangeShift;
} ET_OffsetTable;

typedef struct ET_TableRecord_Rec
{
    UINT    tag;
    UINT    checkSum;
    UINT    offset;
    UINT    length;
} ET_TableRecord;

typedef struct ET_SfntWrapper_Rec
{
    ET_OffsetTable  offsetTable;
    ET_TableRecord* tableRecord;    /* the count is offsetTable.numTables */
} ET_SfntWrapper;

#define ET_CONVERSE_16( val )       ( (((USHORT)(val) & 0xFF00) >> 8) | (((USHORT)(val) & 0x00FF) << 8) )

#define ET_CONVERSE_32( val )       ( (((UINT)(val) & 0xFF000000) >> 24) | \
                                      (((UINT)(val) & 0x00FF0000) >> 8)  | \
                                      (((UINT)(val) & 0x0000FF00) << 8)  | \
                                      (((UINT)(val) & 0x000000FF) << 24) )

/* FT_Stream operation */
#define ET_SAFE_STREAM_SEEK( stream_, offset_ )         \
        {                                                       \
            err_no = FT_Stream_Seek( stream_, offset_ );        \
            if( err_no ) goto exit;                             \
        }

#define ET_SAFE_STREAM_READ( stream_, dst_, size_ )             \
        {                                                       \
            err_no = FT_Stream_Read( stream_, dst_, size_ );    \
            if( err_no ) goto exit;                             \
        }


#define  TTF_Save_16Bit( BUF, VAL )            \
{                                              \
    ( *BUF++ ) = ( BYTE )( VAL >> 8 );     \
    ( *BUF++ ) = ( BYTE )( VAL & 0xFFL );  \
}

#define  TTF_Save_32Bit( BUF, VAL )            \
{                                              \
	( *BUF++ ) = ( BYTE )( VAL >> 24 );     \
	( *BUF++ ) = ( BYTE )( VAL >> 16 );     \
	( *BUF++ ) = ( BYTE )( VAL >> 8 );      \
	( *BUF++ ) = ( BYTE )( VAL & 0xFFL );   \
}

#define TTF_Get_Char( data, x ) x = (FT_Char)(*data++);
#define TTF_Get_Byte( data, x ) x = (BYTE)(*data++);
#define TTF_Get_Short( data, x ) { x = (FT_Short)( ( data[0] << 8 ) | data[1] ); data += 2; }
#define TTF_Get_UShort( data, x ) { x = (USHORT)( ( data[0] << 8 ) | data[1] ); data += 2; }

#define TTF_Get_Long( data, x )  { x = (INT)( ( data[0] << 24 ) | ( data[1] << 16 ) | ( data[2] << 8 ) | data[3] ); data += 4; }
#define TTF_Get_ULong( data, x )  { x = (UINT)( ( data[0] << 24 ) | ( data[1] << 16 ) | ( data[2] << 8 ) | data[3] ); data += 4; }


static unsigned int calc_table_checksum( unsigned int * table, unsigned int length )
{
    /*
    unsigned int sum = 0L;
    unsigned int * end = table + ((length + 3) & ~3) / sizeof(unsigned int);

    while( table < end )
        sum += *table++;
    return sum;
    */
    return 0;
}

static void*  ET_malloc( INT size )
{
    if( size > 0 )
        return malloc( size );
    return NULL;
}

static void  ET_free( void* memory )
{
    free( memory );
}


static void
sfnt_init_wrapper_offsetTable( INT num_tables, ET_OffsetTable* table )
{
    USHORT power = 1, exponent = 0;

    while( power * 2 <= num_tables )
    {
        power *= 2;
        exponent ++;
    }

    table->numTables     = num_tables;
    table->searchRange   = power * 16;
    table->entrySelector = exponent;
    table->rangeShift    = table->numTables * 16 - table->searchRange;
}

static FT_Error
sfnt_wrapper_lock( BYTE*         font_buffer,
                   UINT          buffer_len,
                   ET_SfntWrapper** r_wrapper )
{
    INT err_no = ET_Err_Ok;
    BYTE*    p = NULL;
    ET_SfntWrapper*  wrapper = NULL;
    ET_OffsetTable*  table   = NULL;
    USHORT   num_tables   = 0;
    ET_TableRecord rec, *cur, *pre;
    INT i, j;

    if( !font_buffer || buffer_len == 0 )
    {
        err_no = ET_Err_Invalid_Argument;
        goto exit;
    }

    p = font_buffer;

    wrapper = (ET_SfntWrapper*)malloc( sizeof(ET_SfntWrapper) );
    if( !wrapper )
    {
        err_no = ET_Err_Out_Of_Memory;
        goto exit;
    }
    memset( wrapper, 0, sizeof(ET_SfntWrapper) );

    /* offset table */
    table = &wrapper->offsetTable;

    TTF_Get_Long(   p, table->sfntVersion );
    TTF_Get_UShort( p, table->numTables );
    TTF_Get_UShort( p, table->searchRange );
    TTF_Get_UShort( p, table->entrySelector );
    TTF_Get_UShort( p, table->rangeShift );

    /* table records */
    wrapper->tableRecord = (ET_TableRecord*)malloc( sizeof(ET_TableRecord) * table->numTables );
    if( !wrapper->tableRecord )
    {
        err_no = ET_Err_Out_Of_Memory;
        goto exit;
    }
    memset( wrapper->tableRecord, 0, sizeof(ET_TableRecord) * table->numTables );

    for( i = 0; i < table->numTables; i++ )
    {
        TTF_Get_ULong( p, rec.tag );
        TTF_Get_ULong( p, rec.checkSum );
        TTF_Get_ULong( p, rec.offset );
        TTF_Get_ULong( p, rec.length );

        cur = &wrapper->tableRecord[num_tables];
        *cur = rec;
        num_tables ++;
    }

    bool need_sort = true;
    if( need_sort )
    for( i = 0; i < num_tables; i++ )
    {
        pre = &wrapper->tableRecord[i];
        for( j = i + 1; j < num_tables; j++ )
        {
            cur = &wrapper->tableRecord[j];
            if( cur->tag < pre->tag )
            {
                rec  = *cur;
                *cur = *pre;
                *pre = rec;
            }
        }
    }

    if( num_tables > 0xFFF )
    {
        err_no = ET_Err_Invalid_Table;
        goto exit;
    }

     sfnt_init_wrapper_offsetTable( num_tables, table );

exit:
    if( err_no && wrapper )
    {
        if( wrapper->tableRecord )
            free( wrapper->tableRecord );
        free( wrapper );
        wrapper = NULL;
    }
    if( r_wrapper ) *r_wrapper = wrapper;
    return err_no;
}

static BOOL
sfnt_is_igore( std::vector<UINT>* ignores,
               UINT            tag )
{
    if( !ignores )
        return 0;

    for( UINT i : *ignores )
    {
        if( i == tag )
            return 1;
    }

    return 0;
}


static void
sfnt_wrapper_unlock( ET_SfntWrapper * wrapper )
{
    if( wrapper )
    {
        if( wrapper->tableRecord )
            ET_free( wrapper->tableRecord );
        ET_free( wrapper );
    }
}

static ET_TableRecord*  get_table_record( ET_SfntWrapper * wrapper, INT tag )
{
    if( wrapper && tag )
    {
        USHORT i, num_tables = wrapper->offsetTable.numTables;
        for( i = 0; i < num_tables; i++ )
        {
            ET_TableRecord* rec = &wrapper->tableRecord[i];
            if( rec->tag == tag ) return rec;
        }
    }

    return NULL;
}


static bool updateTable( ET_SfntWrapper * wrapper, 
                         UINT32           ttag,
                         BYTE*            table,
                         LONG             len,
                         std::vector< FontTableRec >&  to_replace,
                         std::vector< FontTableRec >&  to_insert)
{
    if( !wrapper || !table )
        return false;

    BYTE* copy = (BYTE*)malloc( len );
    memcpy( copy, table, len );

    FontTableRec rec = { ttag, copy, len };
    if( get_table_record( wrapper, ttag ) )
    {
        // Replace table
        to_replace.push_back( rec );
    }
    else
    {
        // Insert table
        to_insert.push_back( rec );
    }

    return true;
}

static FontTableRec*
find_in_array( UINT32 tag, std::vector< FontTableRec >& vec )
{
    int size = vec.size();
    int i    = 0;

    for( i=0; i < size; i++ )
    {
        if( vec[i].ttag == tag )
            return &vec[i];
    }

    return NULL;
}

bool exportFont( BYTE* fontBuffer,
                 ET_SfntWrapper * wrapper,
                 BYTE** rBuffer, LONG* rLen,
                 std::vector< FontTableRec >&  to_replace,
                 std::vector< FontTableRec >&  to_insert,
                 std::vector< FontTableRec >&  to_delete)
{
    LONG   dstLen = 1024 * 1024;
    BYTE*  dst = NULL;

    if( !wrapper )
        return false;

    dst = (BYTE*) calloc (1, dstLen );
    if( !dst )
        return FALSE;

    // calculate num_tables;
    LONG  old_num_tables = wrapper->offsetTable.numTables;
    LONG  new_num_tables = old_num_tables + to_insert.size();
    ET_TableRecord*  newRecord = (ET_TableRecord*) calloc (1, sizeof(ET_TableRecord) * new_num_tables );  // 申请多点空间，此时没计算需要删除的表
    new_num_tables = 0;
    for( LONG i=0; i < old_num_tables + to_insert.size(); i++ )
    {
        if( i < old_num_tables )
        {
            if( find_in_array(wrapper->tableRecord[i].tag, to_delete) != NULL )
                continue;

            newRecord[new_num_tables++] = wrapper->tableRecord[i];
        }
        else
        {
            FontTableRec&  table  = to_insert[ i - old_num_tables ];
            newRecord[new_num_tables].tag      = table.ttag;
            newRecord[new_num_tables].length   = (table.len + 3) & (~3);
            newRecord[new_num_tables].offset   = 0;
            newRecord[new_num_tables].checkSum = 0;
            new_num_tables ++;
        }
    }

    bool need_sort = true;
    if( need_sort )
    do{
        ET_TableRecord rec, *pre, *cur;

        for( LONG i = 0; i < new_num_tables; i++ )
        {
            pre = &wrapper->tableRecord[i];
            for( LONG j = i + 1; j < new_num_tables; j++ )
            {
                cur = &wrapper->tableRecord[i];
                if( cur->tag < pre->tag )
                {
                    rec  = *cur;
                    *cur = *pre;
                    *pre = rec;
                }
            }
        }
    }while(0);

    // new offsetTable
    ET_OffsetTable  new_offsetTable = {0};
    new_offsetTable = wrapper->offsetTable;
    sfnt_init_wrapper_offsetTable( new_num_tables, &new_offsetTable );

    // export font
    BYTE* p   = dst + 12 + new_num_tables * 16;
    BYTE* limit = dst + dstLen;
    LONG  offset = 0;

    for( LONG i=0; i < new_num_tables; i++ )
    {
        ET_TableRecord* rec = &newRecord[i];
        FontTableRec* entry = NULL;
        BYTE*  table = NULL;
        LONG   len   = 0;

        offset = p - dst;

        if( (entry = find_in_array(rec->tag, to_replace)) != NULL )
        {
            table = entry->data;
            len   = entry->len;
        }
        else if( (entry = find_in_array(rec->tag, to_insert)) != NULL )
        {
            table = entry->data;
            len   = entry->len;
        }
        else
        {
            table = fontBuffer + rec->offset;
            len   = rec->length;
        }

        rec->length   = len;
        rec->checkSum = calc_table_checksum( (unsigned int*)table, len );
        rec->offset   = offset;

        if( p + len >= limit )
        {
            LONG new_cnt = dstLen + len + 1024 * 1024;
            new_cnt = (new_cnt + 3) & (~3);

            BYTE* tmp = (BYTE*) realloc( dst, new_cnt );
            ASSERT( tmp );

            dst    = tmp;
            dstLen = new_cnt;
            limit  = dst + dstLen;
            p      = dst + offset;
        }

        memcpy( p, table, len );
        p += len;

        // 4Byte alignment
        p += (4 - (len % 4)) % 4;
    }

    offset = p - dst;

    // write offsetTable & recordTable
    p = dst;

    TTF_Save_32Bit( p, new_offsetTable.sfntVersion )
    TTF_Save_16Bit( p, new_offsetTable.numTables )
    TTF_Save_16Bit( p, new_offsetTable.searchRange )
    TTF_Save_16Bit( p, new_offsetTable.entrySelector )
    TTF_Save_16Bit( p, new_offsetTable.rangeShift )

    for( LONG i=0; i < new_num_tables; i++ )
    {
        ET_TableRecord& rec = newRecord[i];
        TTF_Save_32Bit( p, rec.tag )
        TTF_Save_32Bit( p, rec.checkSum )
        TTF_Save_32Bit( p, rec.offset )
        TTF_Save_32Bit( p, rec.length )
    }

    *rBuffer = dst;
    *rLen    = offset;

    if( newRecord )
        free( newRecord );

    return TRUE;
}
