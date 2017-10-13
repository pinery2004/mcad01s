////////////////////////////////////////////////////////////////////////////
//    コマンド処理用バックグラウンドのタスクの起動
////////////////////////////////////////////////////////////////////////////
#pragma once

namespace MC
{

void	MtCmdOpen();
void	MtCmdClose();

UINT	MtThreadProcedure( LPVOID);
bool	MtLaunch( void);
void	MtCloseThread( void);
MINT	MtStopThread( void);
bool	MtChkStopThread( void);
	
} // namespace MC