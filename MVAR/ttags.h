#pragma once

typedef UINT  FT_Tag;

#ifndef FT_MAKE_TAG
#define FT_MAKE_TAG( _x1, _x2, _x3, _x4 ) \
          (FT_Tag)                        \
          ( ( (UINT)_x1 << 24 ) |     \
            ( (UINT)_x2 << 16 ) |     \
            ( (UINT)_x3 <<  8 ) |     \
              (UINT)_x4         )
#endif


static UINT ttag_avar = FT_MAKE_TAG( 'a', 'v', 'a', 'r' );
static UINT tag_base  = FT_MAKE_TAG( 'B', 'A', 'S', 'E' );
static UINT tag_bdat  = FT_MAKE_TAG( 'b', 'd', 'a', 't' );
static UINT tag_bdf   = FT_MAKE_TAG( 'B', 'D', 'F', ' ' );
static UINT tag_bhed  = FT_MAKE_TAG( 'b', 'h', 'e', 'd' );
static UINT tag_bloc  = FT_MAKE_TAG( 'b', 'l', 'o', 'c' );
static UINT tag_bsln  = FT_MAKE_TAG( 'b', 's', 'l', 'n' );
static UINT tag_cbdT  = FT_MAKE_TAG( 'C', 'B', 'D', 'T' );
static UINT tag_cblc  = FT_MAKE_TAG( 'C', 'B', 'L', 'C' );
static UINT tag_cff   = FT_MAKE_TAG( 'C', 'F', 'F', ' ' );
static UINT tag_cff2  = FT_MAKE_TAG( 'C', 'F', 'F', '2' );
static UINT tag_cid   = FT_MAKE_TAG( 'C', 'I', 'D', ' ' );
static UINT tag_cmap  = FT_MAKE_TAG( 'c', 'm', 'a', 'p' );
static UINT tag_colR  = FT_MAKE_TAG( 'C', 'O', 'L', 'R' );
static UINT tag_cpal  = FT_MAKE_TAG( 'C', 'P', 'A', 'L' );
static UINT tag_cvar  = FT_MAKE_TAG( 'c', 'v', 'a', 'r' );
static UINT tag_cvt   = FT_MAKE_TAG( 'c', 'v', 't', ' ' );
static UINT tag_dsig  = FT_MAKE_TAG( 'D', 'S', 'I', 'G' );
static UINT tag_ebdt  = FT_MAKE_TAG( 'E', 'B', 'D', 'T' );
static UINT tag_eblc  = FT_MAKE_TAG( 'E', 'B', 'L', 'C' );
static UINT tag_ebsc  = FT_MAKE_TAG( 'E', 'B', 'S', 'C' );
static UINT tag_feat  = FT_MAKE_TAG( 'f', 'e', 'a', 't' );
static UINT tag_fond  = FT_MAKE_TAG( 'F', 'O', 'N', 'D' );
static UINT tag_FPGM  = FT_MAKE_TAG( 'f', 'p', 'g', 'm' );
static UINT tag_fvar  = FT_MAKE_TAG( 'f', 'v', 'a', 'r' );
static UINT tag_gasp  = FT_MAKE_TAG( 'g', 'a', 's', 'p' );
static UINT tag_gdef  = FT_MAKE_TAG( 'G', 'D', 'E', 'F' );
static UINT tag_glyf  = FT_MAKE_TAG( 'g', 'l', 'y', 'f' );
static UINT tag_gpos  = FT_MAKE_TAG( 'G', 'P', 'O', 'S' );
static UINT tag_gsub  = FT_MAKE_TAG( 'G', 'S', 'U', 'B' );
static UINT tag_gvar  = FT_MAKE_TAG( 'g', 'v', 'a', 'r' );
static UINT tag_hvar  = FT_MAKE_TAG( 'H', 'V', 'A', 'R' );
static UINT tag_hdmx  = FT_MAKE_TAG( 'h', 'd', 'm', 'x' );
static UINT tag_head  = FT_MAKE_TAG( 'h', 'e', 'a', 'd' );
static UINT tag_hhea  = FT_MAKE_TAG( 'h', 'h', 'e', 'a' );
static UINT tag_hmtx  = FT_MAKE_TAG( 'h', 'm', 't', 'x' );
static UINT tag_jstf  = FT_MAKE_TAG( 'J', 'S', 'T', 'F' );
static UINT tag_just  = FT_MAKE_TAG( 'j', 'u', 's', 't' );
static UINT tag_kern  = FT_MAKE_TAG( 'k', 'e', 'r', 'n' );
static UINT tag_lcar  = FT_MAKE_TAG( 'l', 'c', 'a', 'r' );
static UINT tag_loca  = FT_MAKE_TAG( 'l', 'o', 'c', 'a' );
static UINT tag_ltsh  = FT_MAKE_TAG( 'L', 'T', 'S', 'H' );
static UINT tag_lwfn  = FT_MAKE_TAG( 'L', 'W', 'F', 'N' );
static UINT tag_math  = FT_MAKE_TAG( 'M', 'A', 'T', 'H' );
static UINT tag_maxp  = FT_MAKE_TAG( 'm', 'a', 'x', 'p' );
static UINT tag_meta  = FT_MAKE_TAG( 'M', 'E', 'T', 'A' );
static UINT tag_mmfx  = FT_MAKE_TAG( 'M', 'M', 'F', 'X' );
static UINT tag_mmsd  = FT_MAKE_TAG( 'M', 'M', 'S', 'D' );
static UINT tag_mort  = FT_MAKE_TAG( 'm', 'o', 'r', 't' );
static UINT tag_morx  = FT_MAKE_TAG( 'm', 'o', 'r', 'x' );
static UINT tag_mvar  = FT_MAKE_TAG( 'M', 'V', 'A', 'R' );
static UINT tag_name  = FT_MAKE_TAG( 'n', 'a', 'm', 'e' );
static UINT tag_opbd  = FT_MAKE_TAG( 'o', 'p', 'b', 'd' );
static UINT tag_os2   = FT_MAKE_TAG( 'O', 'S', '/', '2' );
static UINT tag_otto  = FT_MAKE_TAG( 'O', 'T', 'T', 'O' );
static UINT tag_pclt  = FT_MAKE_TAG( 'P', 'C', 'L', 'T' );
static UINT tag_post  = FT_MAKE_TAG( 'p', 'o', 's', 't' );
static UINT tag_prep  = FT_MAKE_TAG( 'p', 'r', 'e', 'p' );
static UINT tag_prop  = FT_MAKE_TAG( 'p', 'r', 'o', 'p' );
static UINT tag_sbix  = FT_MAKE_TAG( 's', 'b', 'i', 'x' );
static UINT tag_sfnt  = FT_MAKE_TAG( 's', 'f', 'n', 't' );
static UINT tag_sing  = FT_MAKE_TAG( 'S', 'I', 'N', 'G' );
static UINT tag_trak  = FT_MAKE_TAG( 't', 'r', 'a', 'k' );
static UINT tag_true  = FT_MAKE_TAG( 't', 'r', 'u', 'e' );
static UINT tag_ttc   = FT_MAKE_TAG( 't', 't', 'c', ' ' );
static UINT tag_ttcf  = FT_MAKE_TAG( 't', 't', 'c', 'f' );
static UINT tag_vdmx  = FT_MAKE_TAG( 'V', 'D', 'M', 'X' );
static UINT tag_vhea  = FT_MAKE_TAG( 'v', 'h', 'e', 'a' );
static UINT tag_vmtx  = FT_MAKE_TAG( 'v', 'm', 't', 'x' );
static UINT tag_vvar  = FT_MAKE_TAG( 'V', 'V', 'A', 'R' );
static UINT tag_woff  = FT_MAKE_TAG( 'w', 'O', 'F', 'F' );
static UINT tag_wof2  = FT_MAKE_TAG( 'w', 'O', 'F', '2' );


static UINT tag_ftfh = FT_MAKE_TAG('F', 'T', 'F', 'H');
static UINT tag_ftfg = FT_MAKE_TAG('F', 'T', 'F', 'G');
static UINT tag_cglf = FT_MAKE_TAG( 'c', 'g', 'l', 'f' );
static UINT tag_brsh = FT_MAKE_TAG( 'b', 'r', 's', 'h' );
static UINT tag_assy = FT_MAKE_TAG( 'a', 's', 's', 'y' );
static UINT tag_csty = FT_MAKE_TAG( 'c', 's', 't', 'y' );
static UINT tag_vgrp = FT_MAKE_TAG( 'v', 'g', 'r', 'p' );
static UINT tag_vsty = FT_MAKE_TAG( 'v', 's', 't', 'y' );
static UINT tag_smap = FT_MAKE_TAG( 's', 'm', 'a', 'p' );
static UINT tag_scen = FT_MAKE_TAG( 's', 'c', 'e', 'n' );
static UINT tag_emap = FT_MAKE_TAG( 'e', 'm', 'a', 'p' );
static UINT tag_escn = FT_MAKE_TAG( 'e', 's', 'c', 'n' );
static UINT tag_eimg = FT_MAKE_TAG( 'e', 'i', 'm', 'g' );
static UINT tag_emot = FT_MAKE_TAG( 'e', 'm', 'o', 't' );
static UINT tag_ganm = FT_MAKE_TAG( 'g', 'a', 'n', 'm' );
static UINT tag_dmap = FT_MAKE_TAG( 'd', 'm', 'a', 'p' );
static UINT tag_dtpl = FT_MAKE_TAG( 'd', 't', 'p', 'l' );

