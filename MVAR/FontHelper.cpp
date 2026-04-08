#include "pch.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/ftmm.h>
#include <freetype/ftoutln.h>
#include <freetype/ftbbox.h>
#include "FontHelper.h"
#include <vector>

#include "ReplaceImpl.cpp"


#define FLOOR( x )  ( ( x ) & -64 )
#define CEIL(  x )  ( ( ( x ) + 63 ) & -64 )
#define TRUNC( x )  ( ( x ) >> 6 )

typedef struct FH_HandleRec_
{
    ET_SfntWrapper*    wrapper;
    FT_Library         ft_lib;
    FT_Face            ft_face;
}FH_HandleRec, *FH_Handle;


FontHelper::FontHelper()
{
    m_handle = (FH_Handle) calloc (1, sizeof(FH_HandleRec));
}

FontHelper::~FontHelper()
{
    if( m_handle )
        free(m_handle);
}

bool FontHelper::SFNT_Wrapper_Lock( BYTE*         font_buffer,
                                    UINT          buffer_len )
{
    if( FT_Init_FreeType( &m_handle->ft_lib ) ||
        FT_New_Memory_Face( m_handle->ft_lib, font_buffer, buffer_len, 0, &m_handle->ft_face ) ||
        sfnt_wrapper_lock( font_buffer, buffer_len, &m_handle->wrapper) )
        return false;

    return true;
}

void  FontHelper::SFNT_Wrapper_unLock( BYTE*         font_buffer,
                                       UINT          buffer_len )
{
    if( m_handle->ft_face )
        FT_Done_Face( m_handle->ft_face );
    if( m_handle->ft_lib )
        FT_Done_FreeType( m_handle->ft_lib );

    sfnt_wrapper_unlock( m_handle->wrapper );
    m_handle->wrapper = NULL;
    clean_vector( to_insert );
    clean_vector( to_replace);
    clean_vector( to_delete );
}

void FontHelper::clean_vector( std::vector< FontTableRec >& vec )
{
    for( std::vector< FontTableRec >::iterator iter = vec.begin(); iter != vec.end(); iter++ )
    {
        if( iter->data )
            free( iter->data );
    }
    vec.clear();
}

bool FontHelper::Update_Table( UINT32           ttag,
                               BYTE*            table,
                               LONG             len )
{
    return updateTable( m_handle->wrapper, ttag, table, len, to_replace, to_insert );
}

bool FontHelper::Export_Font( BYTE*         font_buffer,
                              UINT          buffer_len,
                              BYTE** rBuffer, LONG* rLen )
{
    return exportFont( font_buffer, m_handle->wrapper, rBuffer, rLen, to_replace, to_insert, to_delete );
}


bool FontHelper::GetTable( BYTE* font_buffer, UINT32 ttag,
                           BYTE**  rTable,  LONG*  rLen )
{

    ET_TableRecord* rec = get_table_record( m_handle->wrapper, ttag );
    if( !rec )
        return false;

    *rTable = font_buffer + rec->offset;
    *rLen   = rec->length;

    return true;
}

bool FontHelper::RemoveTable( UINT32 ttag )
{
    FontTableRec rec = { ttag, NULL, 0 };
    to_delete.push_back( rec );
    return true;
}

int FontHelper::GetnumberOfGlyphs( BYTE* font_buffer,
                                   UINT  buffer_len )
{
    INT numGlyphs = 0;
    ET_TableRecord* rec = get_table_record( m_handle->wrapper, tag_maxp );
    if( !rec )
        return 0;

    BYTE* table = font_buffer + rec->offset;
    BYTE* p = table + 4;
    TTF_Get_UShort(p, numGlyphs);
    return numGlyphs;
}

int FontHelper::GetnumberOfHMetrics( BYTE* font_buffer,
                                     UINT  buffer_len )
{
    INT numberOfHMetrics = 0;
    ET_TableRecord* rec = get_table_record( m_handle->wrapper, tag_hhea );
    if( !rec )
        return false;

    BYTE* table = font_buffer + rec->offset;
    if( table + rec->length > font_buffer + buffer_len ) // invalid data
        return -1;

    BYTE* p = table + 34;
    TTF_Get_UShort(p, numberOfHMetrics);
    return numberOfHMetrics;
}

INT FontHelper::Get_HGHT_AXIS_Limit( INT& hght_index, INT& asc_min_delta, INT& asc_max_delta, BOOL& asc_mvar, INT& dsc_min_delta, INT& dsc_max_delta, BOOL& dsc_mvar )
{
    if( !FT_HAS_MULTIPLE_MASTERS(m_handle->ft_face) )
        return -1;

    UINT hght_axis_tag = FT_MAKE_TAG('H','G','H','T');
    FT_MM_Var* masters = NULL;
    FT_Fixed coords[16];
    int i;
    FT_Long threshold = m_handle->ft_face->units_per_EM / 100;

    FT_Get_MM_Var( m_handle->ft_face, &masters );

    for( i=0; i < masters->num_axis; i++ )
    {
        coords[i] = masters->axis[i].def;
        if( masters->axis[i].tag == hght_axis_tag )
            break;
    }

    hght_index = i;

    FT_BBox box;
    FT_Set_Pixel_Sizes( m_handle->ft_face, m_handle->ft_face->units_per_EM, m_handle->ft_face->units_per_EM );

    FT_Set_Var_Design_Coordinates( m_handle->ft_face, 0, NULL );
    FT_Load_Char(m_handle->ft_face, 0x30, FT_LOAD_DEFAULT);
    FT_Outline_Get_BBox( &m_handle->ft_face->glyph->outline, &box );
    INT default_asc_delta = m_handle->ft_face->ascender - TRUNC(box.yMax);
    INT default_dsc_delta = m_handle->ft_face->descender - TRUNC(box.yMin);


    INT minimum_yMax, minimum_yMin;
    coords[i] = masters->axis[i].minimum;
    FT_Set_Var_Design_Coordinates( m_handle->ft_face, i + 1, coords );
    FT_Load_Char(m_handle->ft_face, 0x30, FT_LOAD_DEFAULT);
    FT_Outline_Get_BBox( &m_handle->ft_face->glyph->outline, &box );
    minimum_yMax = TRUNC(box.yMax);
    minimum_yMin = TRUNC(box.yMin);
    asc_min_delta = m_handle->ft_face->ascender - TRUNC(box.yMax);
    dsc_min_delta = m_handle->ft_face->descender - TRUNC(box.yMin);


    INT maximum_yMax, maximum_yMin;
    coords[i] = masters->axis[i].maximum;
    FT_Set_Var_Design_Coordinates( m_handle->ft_face, i + 1, coords );
    FT_Load_Char(m_handle->ft_face, 0x30, FT_LOAD_DEFAULT);
    FT_Outline_Get_BBox( &m_handle->ft_face->glyph->outline, &box );
    maximum_yMax = TRUNC(box.yMax);
    maximum_yMin = TRUNC(box.yMin);
    asc_max_delta = m_handle->ft_face->ascender - TRUNC(box.yMax);
    dsc_max_delta = m_handle->ft_face->descender - TRUNC(box.yMin);


    asc_mvar = abs(asc_min_delta - default_asc_delta) < threshold ? ( abs(asc_max_delta - default_asc_delta) < 2 ? false : true ) : true;
    dsc_mvar = abs(dsc_min_delta - default_dsc_delta) < threshold ? ( abs(dsc_max_delta - default_dsc_delta) < 2 ? false : true ) : true;

    if( asc_mvar )
    {
        asc_min_delta = minimum_yMax + default_asc_delta - m_handle->ft_face->ascender;
        asc_max_delta = maximum_yMax + default_asc_delta - m_handle->ft_face->ascender;
    }

    if( dsc_mvar )
    {
        dsc_min_delta = minimum_yMin + default_dsc_delta - m_handle->ft_face->descender;
        dsc_max_delta = maximum_yMin + default_dsc_delta - m_handle->ft_face->descender;
    }

    FT_Done_MM_Var( m_handle->ft_lib, masters );

    return masters->num_axis;
}