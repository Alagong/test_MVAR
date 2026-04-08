
// MVARDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "MVAR.h"
#include "MVARDlg.h"
#include "afxdialogex.h"

#include "FontHelper.h"
#include "utils.h"

#include <filesystem>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMVARDlg dialog


CMVARDlg::CMVARDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MVAR_DIALOG, pParent)
    , m_fontPath(_T(""))
    , m_descender(FALSE)
    , m_outFontPath(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CMVARDlg::~CMVARDlg()
{

}

void CMVARDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT1, m_fontPath);
    DDX_Radio(pDX, IDC_RADIO1, m_descender);
    DDX_Text(pDX, IDC_EDIT2, m_outFontPath);
}

BEGIN_MESSAGE_MAP(CMVARDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDOK, &CMVARDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDC_BUTTON1, &CMVARDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CMVARDlg message handlers

BOOL CMVARDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMVARDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMVARDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMVARDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


#include "MVAR_impl.c"

void construct_mvar( MVARTable& table, ET_UInt32 tag )
{
    table.major = 1;
    table.minor = 0;
    table.valueRecordSize = 8;
    table.valueRecordCount = 1;
    table.valueRecords = (MVAR_ValueRecord*) calloc (1, table.valueRecordCount * table.valueRecordSize );
    for( int i=0; i < 1; i++ )
    {
        table.valueRecords[i].valueTag = tag;
        table.valueRecords[i].deltaSetOuterIndex = 0;
        table.valueRecords[i].deltaSetInnerIndex = 0;
    }

    ItemVariationStore* store = &table.itemVariationStore;
    store->format = 1;
    // region list
    store->regionList.axisCount = 2;
    store->regionList.regionCount = 2;
    store->regionList.records = (RegionAxisCoordinates*)calloc(1, store->regionList.axisCount * store->regionList.regionCount * 6);
    store->regionList.records[1].startCoord = -1 << 14;
    store->regionList.records[1].peakCoord = -1 << 14;
    store->regionList.records[1].endCoord = 0;
    store->regionList.records[0].startCoord = -1 << 14;
    store->regionList.records[0].peakCoord = 0;
    store->regionList.records[0].endCoord = 1 << 14;
    store->regionList.records[3].startCoord = 0;
    store->regionList.records[3].peakCoord = 1 << 14;
    store->regionList.records[3].endCoord = 1 << 14;
    store->regionList.records[2].startCoord = -1 << 14;
    store->regionList.records[2].peakCoord = 0;
    store->regionList.records[2].endCoord = 1 << 14;

    store->itemVariationDataCount = 1;
    store->subtables = (ItemVariationDataSubtable*) calloc ( 1, sizeof(ItemVariationDataSubtable) );
    store->subtables[0].itemCount = 1;
    store->subtables[0].regionIndexCount = 2;
    store->subtables[0].regionIndexArray = (ET_UInt16*) calloc (1, 2 * sizeof(ET_UInt16) );
    store->subtables[0].regionIndexArray[0] = 0;
    store->subtables[0].regionIndexArray[1] = 1;
    store->subtables[0].wordDeltaCount = 2;
    store->subtables[0].deltas = (ET_UInt32*) calloc ( 1, 2 * sizeof(ET_UInt32));
    store->subtables[0].deltas[0] = -100;
    store->subtables[0].deltas[1] = -950;
}

void clean_regionList( VariationRegionList* list )
{
    if( list )
    {
        if( list->records )
            free( list->records );
    }
}

void clean_itemVariationDataSubtable( ItemVariationDataSubtable* subtable )
{
    if( subtable )
    {
        if( subtable->regionIndexArray )
            free( subtable->regionIndexArray );

        if( subtable->deltas )
            free( subtable->deltas );
    }
}

void clean_variationStore(ItemVariationStore& store)
{
    if( store.subtables )
    {
        for( int i=0; i < store.itemVariationDataCount; i++ )
            clean_itemVariationDataSubtable( &store.subtables[i] );
        free( store.subtables );
    }

    clean_regionList( &store.regionList );
}

void clean_mvar(MVARTable& mvar)
{
    if( mvar.valueRecords )
        free( mvar.valueRecords );

    clean_variationStore( mvar.itemVariationStore );
}

CString
CMVARDlg::GetFileName( CString strFilePath )
{
    CString name = _T("");
    int     len;
    int     pos;

    len = strFilePath.GetLength( );
    pos = strFilePath.ReverseFind( '.' );
    if( pos < 0 )
    {
        return strFilePath;
    }
    if( pos >= 0 )
    {
        name = strFilePath.Left( pos );
    }
    return name;
}


CString CMVARDlg::retrive_outfontPath(CString& source)
{
    CString ret = GetFileName( source );
    ret.Append(_T("_mvar.ttf"));
    return ret;
}

void CMVARDlg::OnBnClickedButton1()
{
    CFileDialog  dlg( TRUE, _T(".ttf"), NULL, OFN_HIDEREADONLY |OFN_OVERWRITEPROMPT, 
                      _T("Font Files (*.ttf)|*.ttf|All Files (*.*)|*.*||"), NULL ) ;

    if( dlg.DoModal() == IDOK )
    {
        m_fontPath = dlg.GetPathName();
        UpdateData( FALSE );
    }
}

bool CMVARDlg::make_mvar( MVARTable& mvar, INT axis_count, INT axis_index, INT& asc_min_delta, INT& asc_max_delta, BOOL& asc_mvar, INT& dsc_min_delta, INT& dsc_max_delta, BOOL& dsc_mvar )
{
    mvar.major = 1;
    mvar.minor = 0;
    mvar.valueRecordSize = 8;
    mvar.valueRecordCount = 0;
    if( asc_mvar )
        mvar.valueRecordCount ++;
    if( dsc_mvar )
        mvar.valueRecordCount ++;

    if( mvar.valueRecordCount == 0 )
        return true;

    mvar.valueRecords = (MVAR_ValueRecord*) calloc (1, mvar.valueRecordCount * mvar.valueRecordSize );
    for( int i=0; i < mvar.valueRecordCount; i++ )
    {
        if( mvar.valueRecordCount == 1 )
            mvar.valueRecords[i].valueTag = asc_mvar ? ET_MAKE_TAG('h','a','s','c') : ET_MAKE_TAG('h','d','s','c');
        else
            mvar.valueRecords[i].valueTag = i == 0 ? ET_MAKE_TAG('h','a','s','c') : ET_MAKE_TAG('h','d','s','c');

        mvar.valueRecords[i].deltaSetOuterIndex = 0;
        mvar.valueRecords[i].deltaSetInnerIndex = i;
    }

    ItemVariationStore* store = &mvar.itemVariationStore;
    store->format = 1;
    // region list
    store->regionList.axisCount = axis_count;
    store->regionList.regionCount = 2;
    store->regionList.records = (RegionAxisCoordinates*)calloc(1, store->regionList.axisCount * store->regionList.regionCount * 6);
    for( ET_UInt16 i=0; i < store->regionList.regionCount; i++ )
    {
        for( ET_UInt16 j=0; j < store->regionList.axisCount; j++ )
        {
            if( j == axis_index )
            {
                if( i == 0 )
                {
                    store->regionList.records[i * store->regionList.regionCount + j].startCoord = -1 << 14;
                    store->regionList.records[i * store->regionList.regionCount + j].peakCoord = -1 << 14;
                    store->regionList.records[i * store->regionList.regionCount + j].endCoord = 0;
                }
                else
                {
                    store->regionList.records[i * store->regionList.regionCount + j].startCoord = 0;
                    store->regionList.records[i * store->regionList.regionCount + j].peakCoord = 1 << 14;
                    store->regionList.records[i * store->regionList.regionCount + j].endCoord = 1 << 14;;
                }
            }
            else
            {
                store->regionList.records[i * store->regionList.regionCount + j].startCoord = -1 << 14;
                store->regionList.records[i * store->regionList.regionCount + j].peakCoord = 0;
                store->regionList.records[i * store->regionList.regionCount + j].endCoord = 1 << 14;
            }
        }
    }

    store->itemVariationDataCount = mvar.valueRecordCount;
    store->subtables = (ItemVariationDataSubtable*) calloc ( 1, store->itemVariationDataCount * sizeof(ItemVariationDataSubtable) );
    store->subtables[0].itemCount = 1;
    store->subtables[0].regionIndexCount = 2;
    store->subtables[0].regionIndexArray = (ET_UInt16*) calloc (1, 2 * sizeof(ET_UInt16) );
    store->subtables[0].regionIndexArray[0] = 0;
    store->subtables[0].regionIndexArray[1] = 1;
    store->subtables[0].wordDeltaCount = 2;
    store->subtables[0].deltas = (ET_UInt32*) calloc ( 1, 2 * sizeof(ET_UInt32));
    store->subtables[0].deltas[0] = mvar.valueRecordCount == 1 ? ( asc_mvar ? asc_min_delta : dsc_min_delta ) : asc_min_delta;
    store->subtables[0].deltas[1] = mvar.valueRecordCount == 1 ? ( asc_mvar ? asc_max_delta : dsc_max_delta ) : asc_max_delta;

    if(mvar.valueRecordCount > 1 )
    {
        store->subtables[1].itemCount = 1;
        store->subtables[1].regionIndexCount = 2;
        store->subtables[1].regionIndexArray = (ET_UInt16*) calloc (1, 2 * sizeof(ET_UInt16) );
        store->subtables[1].regionIndexArray[0] = 0;
        store->subtables[1].regionIndexArray[1] = 1;
        store->subtables[1].wordDeltaCount = 2;
        store->subtables[1].deltas = (ET_UInt32*) calloc ( 1, 2 * sizeof(ET_UInt32));
        store->subtables[1].deltas[0] = dsc_min_delta;
        store->subtables[1].deltas[1] = dsc_max_delta;
    }
    
    return true;
}


void CMVARDlg::OnBnClickedOk()
{
    UpdateData(TRUE);

    INT  asc_min_delta, asc_max_delta, dsc_min_delta, dsc_max_delta;
    BOOL asc_mvar, dsc_mvar;
    INT  axis_count, hght_index;

    MVARTable table;
    ET_UInt32 tag;

    // 1. load font
    FontHelper fh;
    BYTE* buffer = NULL;
    UINT     len = Utils::readFile( m_fontPath.GetBuffer(), &buffer );
    if( len == 0 || !buffer )
    {
        MessageBox(_T("∂¡»°◊÷ÃÂ ß∞‹£°"));
        goto Exit;
    }

    fh.SFNT_Wrapper_Lock( buffer, len );

    axis_count = fh.Get_HGHT_AXIS_Limit( hght_index, asc_min_delta, asc_max_delta, asc_mvar, dsc_min_delta, dsc_max_delta, dsc_mvar );
    if( hght_index < 0 ) // no hght or is not variable font
        goto Exit;

    tag = ET_MAKE_TAG('h', 'a', 's', 'c');
    if( m_descender )
        tag = ET_MAKE_TAG('h', 'd', 's', 'c');



    make_mvar( table, axis_count, hght_index, asc_min_delta, asc_max_delta, asc_mvar, dsc_min_delta, dsc_max_delta, dsc_mvar );


    //construct_mvar( table, tag );

    do{
        ET_UInt8* mvar_buffer = NULL;
        ET_UInt32 length = 0;
        BYTE* new_font = NULL;
        LONG  new_font_len = 0;

        length = Write_MVAR( &table, NULL );

        mvar_buffer = (ET_UInt8*) calloc ( 1, length + 1);
        Write_MVAR( &table, mvar_buffer );

        fh.Update_Table( ET_MAKE_TAG('M','V','A','R'), mvar_buffer, length );

        fh.Export_Font( buffer, len, &new_font, &new_font_len );

        CString out_path = retrive_outfontPath(m_fontPath);

        Utils::WriteFile( out_path.GetBuffer(), new_font, new_font_len );

        free( mvar_buffer );

    }while(0);

    clean_mvar( table );

Exit:
    fh.SFNT_Wrapper_unLock( buffer, len );
    if( buffer )
        free( buffer );
    CDialogEx::OnOK();
}


