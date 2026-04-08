#include "pch.h"
#include "utils.h"

CString
Utils::GetFileName( CString strFilePath )
{
    CString name = _T("");
    int     len;
    int     pos;

    len = strFilePath.GetLength( );
    pos = strFilePath.ReverseFind( '\\' );
    if( pos < 0 )
    {
        pos = strFilePath.ReverseFind( '/' );
    }
    if( pos >= 0 )
    {
        name = strFilePath.Right( len - pos - 1 );
    }
    return name;
}

UINT
Utils::readFile( const char* path, BYTE** rBuffer )
{
    UINT len = 0;
    UINT haveRead = 0;
    BYTE* buffer = NULL;
    FILE *f = fopen( path, "rb");
    if( !f )
        return 0;

    fseek( f, 0, SEEK_END );
    len = ftell(f);
    fseek( f, 0, SEEK_SET );

    buffer = (BYTE*) calloc ( 1, len + 1);
    if( !buffer ) 
    {
        len = 0;
        goto Fail;
    }

    while (haveRead < len)
    {
        int unit = 2048;
        if( haveRead + unit > len )
            unit = len - haveRead;

        haveRead += fread( buffer + haveRead, 1, unit, f );
    }

    if( rBuffer )
    {
        *rBuffer = buffer;
        buffer = NULL;
    }

Fail:
    if( f )
        fclose( f );

    if( buffer )
        free(buffer);

    return len;
}

BOOL  Utils::WriteFile( const char* path,  const unsigned char* buffer, int len )
{
    if( !path || !buffer || !len )
        return FALSE;

    FILE* f = fopen( path, "wb" );
    if( !f )
        return FALSE;

    int haveWrite = 0;

    while( haveWrite < len )
    {
        int unit = 2048;

        if( unit + haveWrite > len )
            unit = len - haveWrite;

        haveWrite += fwrite( buffer + haveWrite, 1, unit, f );
    }
    

    fclose( f );

    return TRUE;
}
