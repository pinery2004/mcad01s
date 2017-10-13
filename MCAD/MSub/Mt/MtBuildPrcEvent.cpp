//==========================================================================================
//  K.Matsu. Copyright ( C) 2004-2005.
//
//  MODULE: MtBuildPrcEvent.cpp
//
//		キューに処理イベントを追加する		
//
//	引数	MtPrcEventQueue* pRbQue		キュー
//			{
//				MINT		iRType		リクエスト情報タイプ
//				MtPrcEvent	Rb			リクエストバッファ
//			}n
//				MINT		iEnd		(0) または (999)	終了コード
//
//	戻り値	ステイタス	0: 終了コード == 999
//						1: 終了コード == 0
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MCAD.h"

#include "MgDefine.h"
#include "MtPrcEventQueue.h"
#include "MtPrcEvent.h"
#include "Md.h"									//DNT//
#include "MCadLib.h"							//DNT//

namespace MC
{

MINT MtBuildPrcEvent( MtPrcEventQueue* pRbQue, ...) {
//
    va_list		ap;
	MINT		iRType;
	MtPrcEvent	Rb;
    MINT		what2get,rc;
	MCHAR		*fcp1;
	MREAL		*rp;
    MtBinary	*rbbinary;

	rc = 0;

	MINT isz;

//	ASSERT( iRType!=0);
//	ASSERT( iRType!=MT_RTNONE);

	va_start( ap, pRbQue);
	
    for ( iRType=va_arg(ap,int); iRType!=0; iRType=va_arg(ap,int))	{

        if ( iRType == MTRT_DXF0)
			iRType = 0;											// OK to use 0 now //

        if ( iRType == 999)
			MQUIT;
   
		memset( &Rb, 0, sizeof( Rb));

        Rb.m_restype = iRType;

        what2get = iRType;

        switch ( what2get) {
            case MTRT_MENUCOMMAND:
                fcp1 = va_arg( ap, MCHAR* );
				if ( fcp1 == NULL)
					break;
				isz = ( MINT)Mstrlen( fcp1)+1;
				isz = (( MINT)Mstrlen( fcp1)+1) * sizeof( MCHAR);
                if ( ( Rb.m_resval.rstring = ( MCHAR* )new char[( ( MINT)Mstrlen( fcp1)+1) * sizeof( MCHAR)]) == NULL)
					MQUIT;
				Mstrcpy( Rb.m_resval.rstring, fcp1);
                break;
            case MTRT_SHORT:
                Rb.m_resval.rint = va_arg( ap, short);
                break;
            case MTRT_LONG:
                Rb.m_resval.rlong = va_arg( ap, long);
                break;
            case MTRT_REAL:
                Rb.m_resval.rreal = va_arg( ap, MREAL);
                break;
            case MTRT_LBTNDWN:
                rp = va_arg( ap, MREAL *);
				Rb.SetPoint( rp);
                break;
            case MTRT_MBTNDWN:
                rp = va_arg( ap, MREAL *);
				Rb.SetPoint( rp);
                break;
            case MTRT_RBTNDWN:
                rp = va_arg( ap, MREAL *);
				Rb.SetPoint( rp);
                break;
            case MTRT_BINARY:
                rbbinary = va_arg( ap, struct MtBinary*);
                Rb.m_resval.rbinary.clen = rbbinary->clen;
                if ( (Rb.m_resval.rbinary.buf = (MCHAR* )new char[rbbinary->clen])==NULL)
					MQUIT;
                memcpy( Rb.m_resval.rbinary.buf, rbbinary->buf, rbbinary->clen);
                break;
            case MTRT_VOID:
				MQUIT;
		}

        // キューに追加
		pRbQue->Push( &Rb);
	}
	rc = 1;

exit:
    va_end( ap);
	return rc;
}

} // namespace MC
