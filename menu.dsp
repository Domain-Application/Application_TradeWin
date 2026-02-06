# Microsoft Developer Studio Project File - Name="menu" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=menu - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "menu.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "menu.mak" CFG="menu - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "menu - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "menu - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "menu - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W2 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 wlib32.lib wininet.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libc.lib"

!ELSEIF  "$(CFG)" == "menu - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 dlib32.lib /nologo /stack:0x100000,0x100000 /subsystem:windows /debug /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"mfc42.lib" /nodefaultlib:"mfcs42.lib" /nodefaultlib:"msvcrt.lib" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "menu - Win32 Release"
# Name "menu - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\A_STATE.CPP
# End Source File
# Begin Source File

SOURCE=.\ADJ.CPP
# End Source File
# Begin Source File

SOURCE=.\ADJITEM.CPP
# End Source File
# Begin Source File

SOURCE=.\AdjOffset.cpp
# End Source File
# Begin Source File

SOURCE=.\ARINV.CPP
# End Source File
# Begin Source File

SOURCE=.\ARINV_UNLOCK.cpp
# End Source File
# Begin Source File

SOURCE=.\ARINV_WB.CPP
# End Source File
# Begin Source File

SOURCE=.\AutoInvoice.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoOffsetInvoice.cpp
# End Source File
# Begin Source File

SOURCE=.\B_state.cpp
# End Source File
# Begin Source File

SOURCE=.\BANK.CPP
# End Source File
# Begin Source File

SOURCE=.\BANKP.CPP
# End Source File
# Begin Source File

SOURCE=.\BANKR.CPP
# End Source File
# Begin Source File

SOURCE=.\Blist.cpp
# End Source File
# Begin Source File

SOURCE=.\Category.cpp
# End Source File
# Begin Source File

SOURCE=.\ChequeNo.cpp
# End Source File
# Begin Source File

SOURCE=.\ChequeReturnRegister.cpp
# End Source File
# Begin Source File

SOURCE=.\Chkvar.cpp
# End Source File
# Begin Source File

SOURCE=.\CI.CPP
# End Source File
# Begin Source File

SOURCE=.\CIQ.CPP
# End Source File
# Begin Source File

SOURCE=.\Country.cpp
# End Source File
# Begin Source File

SOURCE=.\CR.CPP
# End Source File
# Begin Source File

SOURCE=.\cust.cpp
# End Source File
# Begin Source File

SOURCE=.\Daily.cpp
# End Source File
# Begin Source File

SOURCE=.\DEBTOR.CPP
# End Source File
# Begin Source File

SOURCE=.\detail.cpp
# End Source File
# Begin Source File

SOURCE=.\DiscAmt.cpp
# End Source File
# Begin Source File

SOURCE=.\Dp.cpp
# End Source File
# Begin Source File

SOURCE=.\DpDo.cpp
# End Source File
# Begin Source File

SOURCE=.\Dpp.cpp
# End Source File
# Begin Source File

SOURCE=.\driver.cpp
# End Source File
# Begin Source File

SOURCE=.\Ds.cpp
# End Source File
# Begin Source File

SOURCE=.\DT.CPP
# End Source File
# Begin Source File

SOURCE=.\DuplicateCR.cpp
# End Source File
# Begin Source File

SOURCE=.\Emode.cpp
# End Source File
# Begin Source File

SOURCE=.\EmodeAmt.cpp
# End Source File
# Begin Source File

SOURCE=.\ENQBANK.CPP
# End Source File
# Begin Source File

SOURCE=.\Exp_ap.cpp
# End Source File
# Begin Source File

SOURCE=.\Exp_inv.cpp
# End Source File
# Begin Source File

SOURCE=.\fc.cpp
# End Source File
# Begin Source File

SOURCE=.\FChange.cpp
# End Source File
# Begin Source File

SOURCE=.\GetWt.cpp
# End Source File
# Begin Source File

SOURCE=.\GI.CPP
# End Source File
# Begin Source File

SOURCE=.\GLCODE.CPP
# End Source File
# Begin Source File

SOURCE=.\GLDAT.CPP
# End Source File
# Begin Source File

SOURCE=.\Grade.cpp
# End Source File
# Begin Source File

SOURCE=.\gst.cpp
# End Source File
# Begin Source File

SOURCE=.\gst_type.cpp
# End Source File
# Begin Source File

SOURCE=.\GwEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\Incentive.cpp
# End Source File
# Begin Source File

SOURCE=.\INIT_CR.CPP
# End Source File
# Begin Source File

SOURCE=.\INIT_INV.CPP
# End Source File
# Begin Source File

SOURCE=.\INIT_PAY.CPP
# End Source File
# Begin Source File

SOURCE=.\INIT_WAC.CPP
# End Source File
# Begin Source File

SOURCE=.\INV_MSG.CPP
# End Source File
# Begin Source File

SOURCE=.\IPAY.CPP
# End Source File
# Begin Source File

SOURCE=.\Logerr.cpp
# End Source File
# Begin Source File

SOURCE=.\lorry.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\menu.cpp
# End Source File
# Begin Source File

SOURCE=.\menu.rc
# End Source File
# Begin Source File

SOURCE=.\MICODE.CPP
# End Source File
# Begin Source File

SOURCE=.\MIFARE.CPP
# End Source File
# Begin Source File

SOURCE=.\mnws.cpp
# End Source File
# Begin Source File

SOURCE=.\Month.cpp
# End Source File
# Begin Source File

SOURCE=.\OffsetAmt.cpp
# End Source File
# Begin Source File

SOURCE=.\OINV.CPP
# End Source File
# Begin Source File

SOURCE=.\One.cpp
# End Source File
# Begin Source File

SOURCE=.\PartyGrading.cpp
# End Source File
# Begin Source File

SOURCE=.\PASS.CPP
# End Source File
# Begin Source File

SOURCE=.\PayAllocate.cpp
# End Source File
# Begin Source File

SOURCE=.\PBANKR.CPP
# End Source File
# Begin Source File

SOURCE=.\PCR.CPP
# End Source File
# Begin Source File

SOURCE=.\PDR.CPP
# End Source File
# Begin Source File

SOURCE=.\PO.CPP
# End Source File
# Begin Source File

SOURCE=.\PPAY.CPP
# End Source File
# Begin Source File

SOURCE=.\ppublic.cpp
# End Source File
# Begin Source File

SOURCE=.\Pr.cpp
# End Source File
# Begin Source File

SOURCE=.\PREFIX.CPP
# End Source File
# Begin Source File

SOURCE=.\Profile.cpp
# End Source File
# Begin Source File

SOURCE=.\pt.cpp
# End Source File
# Begin Source File

SOURCE=.\PtEntry.cpp
# End Source File
# Begin Source File

SOURCE=.\Ptype.cpp
# End Source File
# Begin Source File

SOURCE=.\PUBLIC.CPP
# End Source File
# Begin Source File

SOURCE=.\PWAC.CPP
# End Source File
# Begin Source File

SOURCE=.\Receipt.cpp
# End Source File
# Begin Source File

SOURCE=.\Reminder.cpp
# End Source File
# Begin Source File

SOURCE=.\ResetPassword.cpp
# End Source File
# Begin Source File

SOURCE=.\S_STATE.CPP
# End Source File
# Begin Source File

SOURCE=.\SCALE.CPP
# End Source File
# Begin Source File

SOURCE=.\Scan.cpp
# End Source File
# Begin Source File

SOURCE=.\Sdp.cpp
# End Source File
# Begin Source File

SOURCE=.\SETPAYDATE.cpp
# End Source File
# Begin Source File

SOURCE=.\SETPAYDATEU.cpp
# End Source File
# Begin Source File

SOURCE=.\SetPrice.cpp
# End Source File
# Begin Source File

SOURCE=.\Slip1.cpp
# End Source File
# Begin Source File

SOURCE=.\Slip2.cpp
# End Source File
# Begin Source File

SOURCE=.\SPITEM.CPP
# End Source File
# Begin Source File

SOURCE=.\STATE.CPP
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\STKFLOW.CPP
# End Source File
# Begin Source File

SOURCE=.\sv.cpp
# End Source File
# Begin Source File

SOURCE=.\TAP.CPP
# End Source File
# Begin Source File

SOURCE=.\TAXCODE.CPP
# End Source File
# Begin Source File

SOURCE=.\TIMEFRM.CPP
# End Source File
# Begin Source File

SOURCE=.\TransferDirectTicketToMainSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\transp.cpp
# End Source File
# Begin Source File

SOURCE=.\TSO.CPP
# End Source File
# Begin Source File

SOURCE=.\Two.cpp
# End Source File
# Begin Source File

SOURCE=.\unit.cpp
# End Source File
# Begin Source File

SOURCE=.\VIEWCIQ.CPP
# End Source File
# Begin Source File

SOURCE=.\VIEWINV.CPP
# End Source File
# Begin Source File

SOURCE=.\WAC.CPP
# End Source File
# Begin Source File

SOURCE=.\WACITEM.CPP
# End Source File
# Begin Source File

SOURCE=.\Wdp.cpp
# End Source File
# Begin Source File

SOURCE=.\Wds.cpp
# End Source File
# Begin Source File

SOURCE=.\Weighing.cpp
# End Source File
# Begin Source File

SOURCE=.\Wpr.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ARINV_UNLOCK.h
# End Source File
# Begin Source File

SOURCE=.\ARINV_WB.H
# End Source File
# Begin Source File

SOURCE=.\DpDo.h
# End Source File
# Begin Source File

SOURCE=.\DT.H
# End Source File
# Begin Source File

SOURCE=.\FormatSmartCard.h
# End Source File
# Begin Source File

SOURCE=.\Incentive.h
# End Source File
# Begin Source File

SOURCE=.\Logerr.h
# End Source File
# Begin Source File

SOURCE=.\MENU.H
# End Source File
# Begin Source File

SOURCE=.\PartyGrading.h
# End Source File
# Begin Source File

SOURCE=.\Pr.h
# End Source File
# Begin Source File

SOURCE=.\PtEntry.h
# End Source File
# Begin Source File

SOURCE=.\PTYPE.H
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\Scan.h
# End Source File
# Begin Source File

SOURCE=.\SETPAYDATE.h
# End Source File
# Begin Source File

SOURCE=.\SETPAYDATEU.h
# End Source File
# Begin Source File

SOURCE=.\SetPrice.h
# End Source File
# Begin Source File

SOURCE=.\Stdafx.h
# End Source File
# Begin Source File

SOURCE=.\TransferDirectTicketToMainSystem.h
# End Source File
# Begin Source File

SOURCE=.\Weighing.h
# End Source File
# Begin Source File

SOURCE=.\Wpr.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\bitmap10.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bitmap3.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bitmap4.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bitmap5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bmp1d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Canceld.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Cancelf.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Cancelu.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\default1.bin
# End Source File
# Begin Source File

SOURCE=.\res\doc91.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Forest.bmp
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon3.ico
# End Source File
# Begin Source File

SOURCE=.\res\Icon_000.ico
# End Source File
# Begin Source File

SOURCE=.\res\Icon_062.ico
# End Source File
# Begin Source File

SOURCE=.\res\Icon_160.ico
# End Source File
# Begin Source File

SOURCE=.\res\Icon_190.ico
# End Source File
# Begin Source File

SOURCE=.\res\Icon_194.ico
# End Source File
# Begin Source File

SOURCE=.\Res\Image1d.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Image1f.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Image1u.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Image2f.bmp
# End Source File
# Begin Source File

SOURCE=.\res\lcon60.ico
# End Source File
# Begin Source File

SOURCE=.\res\menu.ico
# End Source File
# Begin Source File

SOURCE=.\res\menuDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Okd.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Okf.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Oku.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar3.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\truck.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Truck02b.ico
# End Source File
# Begin Source File

SOURCE=.\res\Truck02e.ico
# End Source File
# Begin Source File

SOURCE=.\res\Truck02k.ico
# End Source File
# Begin Source File

SOURCE=.\res\~app07k.ico
# End Source File
# Begin Source File

SOURCE=.\res\~app20aa.ico
# End Source File
# Begin Source File

SOURCE=.\res\~app22cb.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section menu : {C932BA85-4374-101B-A56C-00AA003668DC}
# 	2:21:DefaultSinkHeaderFile:msmask1.h
# 	2:16:DefaultSinkClass:CMSMask
# End Section
# Section menu : {E6E17E90-DF38-11CF-8E74-00A0C90F26F8}
# 	2:5:Class:CMSComm
# 	2:10:HeaderFile:mscomm.h
# 	2:8:ImplFile:mscomm.cpp
# End Section
# Section menu : {648A5600-2C6E-101B-82B6-000000000014}
# 	2:21:DefaultSinkHeaderFile:mscomm.h
# 	2:16:DefaultSinkClass:CMSComm
# End Section
# Section menu : {9F6AA700-D188-11CD-AD48-00AA003C9CB6}
# 	2:5:Class:CRowCursor
# 	2:10:HeaderFile:rowcursor.h
# 	2:8:ImplFile:rowcursor.cpp
# End Section
# Section menu : {04598FC4-866C-11CF-AB7C-00AA00C08FCF}
# 	2:5:Class:CCommandButton
# 	2:10:HeaderFile:commandbutton.h
# 	2:8:ImplFile:commandbutton.cpp
# End Section
# Section menu : {8BD21D13-EC42-11CE-9E0D-00AA006002F3}
# 	2:5:Class:CMdcText
# 	2:10:HeaderFile:mdctext.h
# 	2:8:ImplFile:mdctext.cpp
# End Section
# Section menu : {978C9E23-D4B0-11CE-BF2D-00AA003F40D0}
# 	2:21:DefaultSinkHeaderFile:labelcontrol1.h
# 	2:16:DefaultSinkClass:CLabelControl
# End Section
# Section menu : {6262D3A0-531B-11CF-91F6-C2863C385E30}
# 	2:21:DefaultSinkHeaderFile:msflexgrid.h
# 	2:16:DefaultSinkClass:CMSFlexGrid
# End Section
# Section menu : {5F4DF280-531B-11CF-91F6-C2863C385E30}
# 	2:5:Class:CMSFlexGrid
# 	2:10:HeaderFile:msflexgrid.h
# 	2:8:ImplFile:msflexgrid.cpp
# End Section
# Section menu : {4D6CC9A0-DF77-11CF-8E74-00A0C90F26F8}
# 	2:5:Class:CMSMask
# 	2:10:HeaderFile:msmask1.h
# 	2:8:ImplFile:msmask1.cpp
# End Section
# Section menu : {BEF6E003-A874-101A-8BBA-00AA00300CAB}
# 	2:5:Class:COleFont
# 	2:10:HeaderFile:font1.h
# 	2:8:ImplFile:font1.cpp
# End Section
# Section menu : {7BF80981-BF32-101A-8BBB-00AA00300CAB}
# 	2:5:Class:CPicture
# 	2:10:HeaderFile:picture1.h
# 	2:8:ImplFile:picture1.cpp
# End Section
# Section menu : {D7053240-CE69-11CD-A777-00DD01143C57}
# 	2:21:DefaultSinkHeaderFile:commandbutton.h
# 	2:16:DefaultSinkClass:CCommandButton
# End Section
# Section menu : {8BD21D10-EC42-11CE-9E0D-00AA006002F3}
# 	2:21:DefaultSinkHeaderFile:mdctext.h
# 	2:16:DefaultSinkClass:CMdcText
# End Section
# Section menu : {04598FC1-866C-11CF-AB7C-00AA00C08FCF}
# 	2:5:Class:CLabelControl
# 	2:10:HeaderFile:labelcontrol1.h
# 	2:8:ImplFile:labelcontrol1.cpp
# End Section
