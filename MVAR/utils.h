#pragma once


class Utils{
public:
    static CString  GetFileName( CString strFilePath );
    static UINT readFile( const char* path, BYTE** rBuffer );
    static BOOL WriteFile( const char* path,  const unsigned char* buffer, int len );
};