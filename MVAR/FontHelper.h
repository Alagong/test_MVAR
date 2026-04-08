#pragma once
#include <vector>


typedef struct ET_SfntWrapper_Rec  ET_SfntWrapper;
typedef struct FH_HandleRec_*      FH_Handle;

typedef struct FontTableRec_
{
    UINT32  ttag;
    BYTE*   data;
    LONG    len;
}FontTableRec;

class FontHelper
{
public:
    FontHelper();
    ~FontHelper();

public:
    bool SFNT_Wrapper_Lock( BYTE*         font_buffer,
                            UINT          buffer_len );

    void SFNT_Wrapper_unLock( BYTE*         font_buffer,
                              UINT          buffer_len );

    bool Update_Table( UINT32           ttag,
                       BYTE*            table,
                       LONG             len );

    bool Export_Font( BYTE*         font_buffer,
                      UINT          buffer_len,
                      BYTE** rBuffer, LONG* rLen );

    bool GetTable( BYTE* font_buffer, UINT32 ttag,
                   BYTE**  rTable,  LONG*  rLen );

    bool RemoveTable( UINT32 ttag );

    
    int GetnumberOfGlyphs( BYTE* font_buffer,
                           UINT  buffer_len );

    int GetnumberOfHMetrics( BYTE* font_buffer,
                             UINT  buffer_len );

    INT Get_HGHT_AXIS_Limit( INT& hght_index, INT& asc_min_delta, INT& asc_max_delta, BOOL& asc_mvar, INT& dsc_min_delta, INT& dsc_max_delta, BOOL& dsc_mvar );

private:
    void   clean_vector( std::vector< FontTableRec >& vec );
    std::vector< FontTableRec >  to_replace;
    std::vector< FontTableRec >  to_insert;
    std::vector< FontTableRec >  to_delete;
    FH_Handle                    m_handle;
};

