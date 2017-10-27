#pragma once

#include <vector>
#include <gdiplus.h>
#pragma comment(lib,"gdiplus.lib")
using namespace Gdiplus;


struct SNxSong
{
	CString strSong;
	uint64_t ui64TrimIn;
	uint64_t ui64TrimOut;
	uint64_t ui64SequenceIn;
	uint64_t ui64SequenceOut;
}; 
struct SNxFont
{
	SNxFont(CString strFaceName,CString strDisplayName)
	{
		this->strFaceName = strFaceName;
		this->strDisplayName = strDisplayName;
	}
	CString strFaceName;
	CString strDisplayName;
};
struct SNxFrameBuffer
{
	SNxFrameBuffer()
	{
		pBufferData           = NULL;
		iBufferDataWidth      = 0;
		iBufferDataHeight     = 0;
		iBufferDataUsedWidth  = 0;
		iBufferDataUsedHeight = 0;

	}
	BYTE *pBufferData;
	int iBufferDataWidth;
	int iBufferDataHeight;
	int iBufferDataUsedWidth;
	int iBufferDataUsedHeight;
};

struct SNxGlobalData
{
	SNxFrameBuffer sBackgroundFrameBuffer;
	SNxFrameBuffer sTemplateCGFrameBuffer;
	SNxFrameBuffer sTemplateComposerFrameBuffer;
	SNxFrameBuffer sTemplateIconBuffer;
	CString strCurrentSongContent;
};

struct SNxSongProperty
{
	SNxSongProperty()
	{
		vFonts.clear();
		Init();
	}

	~SNxSongProperty()
	{
		Flush();

		int nCount= vFonts.size();

		for(int i = 0; i< nCount; i++)
		{
			SNxFont* pFont = vFonts[i];
			if(pFont != NULL)
			{
				delete pFont;
				pFont = NULL;
			}
		}
		vFonts.clear();
		nCount= vSongs.size();

		for(int i = 0; i< nCount; i++)
		{
			SNxSong* pSong = vSongs[i];
			if(pSong != NULL)
			{
				delete pSong;
				pSong = NULL;
			}
		}
		vSongs.clear();
	}

	bool DrawCG(int iWidth,int iHeight,CString strCGContent,void **in_buff,unsigned char *io_pCGBuffer,bool bIcon = false)
	{
		Log();

		CWnd *pDeskTop = CWnd::GetDesktopWindow();

		CDC *pDC = pDeskTop->GetDC();
		CDC memDC;

		memDC.CreateCompatibleDC(pDC);

		DWORD dwLastError = GetLastError();
		if (dwLastError) {
			CString temp;
			temp.Format("%s%d", "CreateCompatibleDC  ", dwLastError);
			AfxMessageBox(temp);
		}

		BITMAPINFO bitinfo;
		bitinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bitinfo.bmiHeader.biWidth = iWidth;
		bitinfo.bmiHeader.biHeight = -iHeight;
		bitinfo.bmiHeader.biBitCount = 24;
		bitinfo.bmiHeader.biPlanes = 1;
		bitinfo.bmiHeader.biClrUsed = 0;
		bitinfo.bmiHeader.biCompression = BI_RGB;

		CBitmap *m_pOldBmp = NULL;
		CBitmap *m_pMemBmp = new CBitmap();       //根据图片的大小创建一个兼容位图
		m_pMemBmp->CreateCompatibleBitmap(pDC, iWidth, iHeight);
		dwLastError = GetLastError();
		if (dwLastError) {
			CString temp;
			temp.Format("%s%d","CreateCompatibleBitmap  ", dwLastError);
			AfxMessageBox(temp);
		}

		m_pOldBmp = memDC.SelectObject(m_pMemBmp);

		SetDIBits(memDC.GetSafeHdc(), (HBITMAP)m_pMemBmp->m_hObject,
			0, iHeight, (LPVOID)in_buff, (BITMAPINFO*)&bitinfo, DIB_RGB_COLORS);
		dwLastError = GetLastError();
		if (dwLastError) {
			CString temp;
			temp.Format("%s%d", "SetDIBits  ", dwLastError);
			AfxMessageBox(temp);
		}
		memDC.SetBkMode(TRANSPARENT);

		//Color
		COLORREF color = RGB(Red, Green, Blue);
		memDC.SetTextColor(color);

		CFont font;
		font.CreateFont(800, 0, 0, 0, 400, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_SWISS, "微软雅黑");
		SelectObject(memDC.GetSafeHdc(), font);

	//	memDC.TextOut(iWidth/2, iHeight/2, strCGContent);
		CRect rectDraw;
		rectDraw.top = 0;
		rectDraw.left = 0;
		rectDraw.right = 3840;
		rectDraw.bottom = 2160;
		memDC.DrawText(strCGContent, rectDraw, DT_CENTER| DT_VCENTER| DT_SINGLELINE);
		dwLastError = GetLastError();
		if (dwLastError) {
			CString temp;
			temp.Format("%s%d", "TextOut  ", dwLastError);
			AfxMessageBox(temp);
		}

		int er = GetDIBits(memDC.GetSafeHdc(), (HBITMAP)m_pMemBmp->m_hObject, 0, iHeight,
			(LPVOID)io_pCGBuffer, (BITMAPINFO*)&bitinfo, DIB_RGB_COLORS);
		dwLastError = GetLastError();
		if (dwLastError|| er == 0) {
			CString temp;
			temp.Format("%s%d", "GetDIBits  ", dwLastError);
			AfxMessageBox(temp);
		}

		memDC.DeleteDC();

		m_pMemBmp->DeleteObject();
		delete m_pMemBmp;
		
		font.DeleteObject();

		pDeskTop->ReleaseDC(pDC);

		return true;
	};
	
	/*
	bool DrawCG(int iWidth, int iHeight, CString strCGContent, void **in_buff,unsigned char *io_pCGBuffer, bool bIcon = false)
	{
		Log();

		CWnd *pDeskTop = CWnd::GetDesktopWindow();

		CDC *pDC = pDeskTop->GetDC();
		CDC memDC;
		memDC.CreateCompatibleDC(pDC);

		DWORD dwLastError = GetLastError();

		HDC hDC = memDC.GetSafeHdc();



		dwLastError = GetLastError();

		CBitmap oBitmap;
		CRect rcTextRect = CRect(0, 0, iWidth, iHeight);

		oBitmap.CreateBitmap(rcTextRect.Width(), rcTextRect.Height(), 1, 32, NULL);
		DWORD dset = oBitmap.SetBitmapBits(iWidth*iHeight*3, in_buff);
		memDC.SelectObject(&oBitmap);
		dwLastError = GetLastError();

		LOGFONT logfont = { 0 };

		logfont.lfWidth = dFontSize * 100;
		logfont.lfHeight = dFontSize * 100;


		CString 	strFace = strFontFace;
		_tcscpy_s(logfont.lfFaceName, strFace.GetBuffer());



		HFONT hFont = ::CreateFontIndirect(&logfont);

		HFONT hOldFont = (HFONT)::SelectObject(hDC, hFont);
		int oldMode = 0;

		//oldMode=::SetBkMode(hDC,TRANSPARENT);

		//Color
		COLORREF color = RGB(Red, Green, Blue);
		::SetTextColor(hDC, color);

		dwLastError = GetLastError();

		//CharacterExtra
		CString strContect = strCGContent;




		Graphics graphics(hDC);
		graphics.SetSmoothingMode(SmoothingModeDefault);
		graphics.SetInterpolationMode(InterpolationModeHighQuality);
		FontFamily fontFamily(strFace.AllocSysString());
		GraphicsPath path;


		graphics.SetSmoothingMode(SmoothingModeHighQuality);
		graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);


		StringFormat strFormat;

		//strFormat.SetLineAlignment( StringAlignmentCenter ); //StringAlignmentNear StringAlignmentCenter
		strFormat.SetAlignment(StringAlignmentCenter);



		//wchar_t wszDbgString[128];
		//wsprintf(wszDbgString,_T("Font w = %d,h = %d"),rectString.Width,rectString.Height);
		//OutputDebugString(wszDbgString);

		int FontSize = dFontSize * 100 * 2;
		int nTextX = rcTextRect.Width() / 2;

		int nTextY = rcTextRect.Height()*dPositionY / 100;




		if (!bIcon)
		{//如果不是获得模板窗口的icon
			if (rcTextRect.Width() == 3840)
			{

				path.AddString(strContect.AllocSysString(), strContect.GetLength(), &fontFamily, FontStyleRegular, FontSize, Gdiplus::Point(nTextX / 2, nTextY), &strFormat);

				Matrix matrix(1, 0, 0, 1, 1, 1);
				matrix.Scale(2, 1);
				path.Transform(&matrix);

			}
			else if (rcTextRect.Width() == 960)
			{
				path.AddString(strContect.AllocSysString(), strContect.GetLength(), &fontFamily, FontStyleRegular, FontSize, Gdiplus::Point(nTextX, nTextY), &strFormat);
			}
			else if (rcTextRect.Width() == 480)
			{
				path.AddString(strContect.AllocSysString(), strContect.GetLength(), &fontFamily, FontStyleRegular, FontSize, Gdiplus::Point(nTextX * 2, nTextY * 2), &strFormat);

				Matrix matrix(1, 0, 0, 1, 1, 1);
				matrix.Scale(0.5, 0.5);
				path.Transform(&matrix);

			}
		}
		else
		{//如果是获得模板窗口的ICON
			path.AddString(strContect.AllocSysString(), strContect.GetLength(), &fontFamily, FontStyleRegular, FontSize, Gdiplus::Point(nTextX, nTextY), &strFormat);
		}


		//gdi+ Draw Outline
		if (nOutlineWeight > 0)
		{
			if (rcTextRect.Width() == 480)
			{
				//避免低质量字体放大。
				Pen pen(Color(OutlineRed, OutlineGreen, OutlineBlue), nOutlineWeight / 2);

				graphics.DrawPath(&pen, &path);
			}
			else
			{
				Pen pen(Color(OutlineRed, OutlineGreen, OutlineBlue), nOutlineWeight);

				graphics.DrawPath(&pen, &path);

			}
		}

		SolidBrush brush(Color(Red, Green, Blue));

		//Gdiplus::Image image(_T("C:\\AutoStorm\\NaxData\\Texture\\bumpy003.tga"));
		//TextureBrush textureBrush(&image);
		//graphics.FillPath(&textureBrush,&path);
		//gdi+ Draw Text
		graphics.FillPath(&brush, &path);




		//CDC Draw Text
		//::TextOut(hDC,nTextX ,nTextY,strContect,strContect.GetLength());

		BITMAP *phBitmap = new BITMAP;
		oBitmap.GetBitmap(phBitmap);

		//if(m_puszCGFrameBuffer == NULL)
		//{
		//	m_puszCGFrameBuffer = new unsigned char [(phBitmap->bmWidth)*(phBitmap->bmHeight)*(phBitmap->bmBitsPixel/8)];
		//}

		unsigned char *pCG = io_pCGBuffer;

		oBitmap.GetBitmapBits((phBitmap->bmWidth)*(phBitmap->bmHeight) * 32 / 8, pCG);

		if (phBitmap != NULL)
		{
			delete phBitmap;
		}



		::SelectObject(hDC, hOldFont);

		DeleteObject(hFont);


		::SetBkMode(hDC, oldMode);


		memDC.DeleteDC();

		pDeskTop->ReleaseDC(pDC);

		return true;
	};*/
	bool SaveTemplate()
	{
		

	/*	oFile.Close();*/
		
		return true;
	}

	void Log()
	{
	
	}

	int nVersion;
	bool bBypass;

	CString strFontFace;
	int nFontFaceIndex;
	BYTE Red; 
	BYTE Green;
	BYTE Blue;
	BYTE Opacity;
	double dFontSize;

	BYTE OutlineRed; 
	BYTE OutlineGreen;
	BYTE OutlineBlue;
	BYTE nOutlineWeight;

	double dPositionX;
	double dPositionY;
	std::vector<SNxFont*> vFonts;
	std::vector<SNxSong*> vSongs;
	SNxGlobalData sGlobalData;

	CString strTemplateName;
	
	void Serialize(CArchive& ar)
	{
		if(ar.IsStoring())
		{
			ar << nVersion;
			ar << bBypass;
			ar << nFontFaceIndex;
			ar << Red;
			ar << Green;
			ar << Blue;
			ar << Opacity;
			ar << dFontSize;
			ar << OutlineRed;
			ar << OutlineGreen;
			ar << OutlineBlue;

			ar << nOutlineWeight;
			
			ar << dPositionX;
			ar << dPositionY;
		}
		else
		{
			ar>>nVersion;
			ar >> bBypass;
			ar >> nFontFaceIndex;
			ar >> Red;
			ar >> Green;
			ar >> Blue;
			ar >> Opacity;
			ar >> dFontSize;
			ar >> OutlineRed;
			ar >> OutlineGreen;
			ar >> OutlineBlue;

			ar >> nOutlineWeight;
			
			ar >> dPositionX;
			ar >> dPositionY;
		}
	};
	void Init()
	{
		nVersion = 1;

		SNxFont* pFont = new SNxFont(_T("微软雅黑"),_T("Microsoft Yahei"));
		vFonts.push_back(pFont);
		
		pFont = new SNxFont(_T("仿宋"),_T("Fang Song"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("楷体"),_T("Kai Ti"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("宋体"),_T("Song Ti"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("新宋体"),_T("New Song Ti"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("方正舒体"),_T("Fangzheng Shu Ti"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("方正姚体"),_T("Fangzheng Yao Ti"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("隶书"),_T("Li Shu"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("幼圆"),_T("You Yuan"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("华文彩云"),_T("HuaWen Cai Yun"));
		vFonts.push_back(pFont);


		pFont = new SNxFont(_T("华文仿宋"),_T("HuaWen Fang Song"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("华文琥珀"),_T("HuaWen Hu Po"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("华文楷体"),_T("HuaWen Kai Ti"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("华文隶书"),_T("HuaWen Li Shu"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("华文宋体"),_T("HuaWen Song Ti"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("华文细黑"),_T("HuaWen Xi Hei"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("华文行楷"),_T("HuaWen Xing Kai"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("华文新魏"),_T("HuaWen Xin Wei"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("华文中宋"),_T("HuaWen Zhong Song"));
		vFonts.push_back(pFont);


		pFont = new SNxFont(_T("黑体"),_T("Hei Ti"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Arial"),_T("Arial"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Tahoma"),_T("Tahoma"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Times New Roman"),_T("Times New Roman"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Fixedsys"),_T("Fixedsys"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Terminal"),_T("Terminal"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Microsoft Uighur"),_T("Microsoft Uighur"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Microsoft Yi Baiti"),_T("Microsoft Yi Baiti"));
		vFonts.push_back(pFont);


		pFont = new SNxFont(_T("Microsoft Sans Serif"),_T("Microsoft Sans Serif"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Microsoft Tai Le"),_T("Microsoft Tai Le"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Microsoft New Tai Lue"),_T("Microsoft New Tai Lue"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Microsoft PhagsPa"),_T("Microsoft PhagsPa"));
		vFonts.push_back(pFont);



		pFont = new SNxFont(_T("Modern"),_T("Modern"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Roman"),_T("Roman"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Script"),_T("Script"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Courier"),_T("Courier"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Courier New"),_T("Courier New"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Courier New Baltic"),_T("Courier New Baltic"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Courier New CE"),_T("Courier New CE"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Courier New CYR"),_T("Courier New CYR"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Courier New Greek"),_T("Courier New Greek"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Courier New TUR"),_T("Courier New TUR"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("MS Serif"),_T("MS Serif"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("MS Sans Serif"),_T("MS Sans Serif"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("MS Gothic"),_T("MS Gothic"));
		vFonts.push_back(pFont);


		pFont = new SNxFont(_T("MS PGothic"),_T("MS PGothic"));
		vFonts.push_back(pFont);


		pFont = new SNxFont(_T("MS UI Gothic"),_T("MS UI Gothic"));
		vFonts.push_back(pFont);



		pFont = new SNxFont(_T("MS Mincho"),_T("MS Mincho"));
		vFonts.push_back(pFont);


		pFont = new SNxFont(_T("MS PMincho"),_T("MS PMincho"));
		vFonts.push_back(pFont);


		pFont = new SNxFont(_T("Small Fonts"),_T("Small Fonts"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("ASI_System"),_T("ASI_System"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Marlett"),_T("Marlett"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Arabic Transparent"),_T("Arabic Transparent"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Arial Baltic"),_T("Arial Baltic"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Arial CE"),_T("Arial CE"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Arial CYR"),_T("Arial CYR"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Arial Greek"),_T("Arial Greek"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Arial TUR"),_T("Arial TUR"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Batang"),_T("Batang"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("BatangChe"),_T("BatangChe"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Gungsuh"),_T("Gungsuh"));
		vFonts.push_back(pFont);



		pFont = new SNxFont(_T("DaunPenh"),_T("DaunPenh"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("DokChampa"),_T("DokChampa"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Estrangelo Edessa"),_T("Estrangelo Edessa"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Euphemia"),_T("Euphemia"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Gautami"),_T("Gautami"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Vani"),_T("Vani"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Gulim"),_T("Gulim"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("GulimChe"),_T("GulimChe"));
		vFonts.push_back(pFont);
		
		pFont = new SNxFont(_T("Dotum"),_T("Dotum"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("DotumChe"),_T("DotumChe"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Impact"),_T("Impact"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Iskoola Pota"),_T("Iskoola Pota"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Kalinga"),_T("Kalinga"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Kartika"),_T("Kartika"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Khmer UI"),_T("Khmer UI"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Lao UI"),_T("Lao UI"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Latha"),_T("Latha"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Lucida Console"),_T("Lucida Console"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Malgun Gothic"),_T("Malgun Gothic"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Mangal"),_T("Mangal"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Meiryo"),_T("Meiryo"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Meiryo UI"),_T("Meiryo UI"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Microsoft JhengHei"),_T("Microsoft JhengHei"));
		vFonts.push_back(pFont);
		
		pFont = new SNxFont(_T("MingLiU"),_T("MingLiU"));
		vFonts.push_back(pFont);
		

		pFont = new SNxFont(_T("PMingLiU"),_T("PMingLiU"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("MingLiU_HKSCS"),_T("MingLiU_HKSCS"));
		vFonts.push_back(pFont);
		
		pFont = new SNxFont(_T("MingLiU-ExtB"),_T("MingLiU-ExtB"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("PMingLiU-ExtB"),_T("PMingLiU-ExtB"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("MingLiU_HKSCS-ExtB"),_T("MingLiU_HKSCS-ExtB"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Mongolian Baiti"),_T("Mongolian Baiti"));
		vFonts.push_back(pFont);




		pFont = new SNxFont(_T("MV Boli"),_T("MV Boli"));
		vFonts.push_back(pFont);

		

		pFont = new SNxFont(_T("Nyala"),_T("Nyala"));
		vFonts.push_back(pFont);



		pFont = new SNxFont(_T("Plantagenet Cherokee"),_T("Plantagenet Cherokee"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Raavi"),_T("Raavi"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Segoe Script"),_T("Segoe Script"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Segoe UI"),_T("Segoe UI"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Segoe UI Semibold"),_T("Segoe UI Semibold"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Segoe UI Light"),_T("Segoe UI Light"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Segoe UI Symbol"),_T("Segoe UI Symbol"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Shruti"),_T("Shruti"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("SimSun-ExtB"),_T("SimSun-ExtB"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Sylfaen"),_T("Sylfaen"));
		vFonts.push_back(pFont);

		

		pFont = new SNxFont(_T("Times New Roman Baltic"),_T("Times New Roman Baltic"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Times New Roman CE"),_T("Times New Roman CE"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Times New Roman CYR"),_T("Times New Roman CYR"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Times New Roman Greek"),_T("Times New Roman Greek"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Times New Roman TUR"),_T("Times New Roman TUR"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Tunga"),_T("Tunga"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Vrinda"),_T("Vrinda"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Shonar Bangla"),_T("Shonar Bangla"));
		vFonts.push_back(pFont);

		

		pFont = new SNxFont(_T("Angsana New"),_T("Angsana New"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Aparajita"),_T("Aparajita"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Cordia New"),_T("Cordia New"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Ebrima"),_T("Ebrima"));
		vFonts.push_back(pFont);

		pFont = new SNxFont(_T("Gisha"),_T("Gisha"));
		vFonts.push_back(pFont);


		pFont = new SNxFont(_T("Kokila"),_T("Kokila"));
		vFonts.push_back(pFont);


		pFont = new SNxFont(_T("Leelawadee"),_T("Leelawadee"));
		vFonts.push_back(pFont);


		pFont = new SNxFont(_T("MoolBoran"),_T("MoolBoran"));
		vFonts.push_back(pFont);


		pFont = new SNxFont(_T("Symbol"),_T("Symbol"));
		vFonts.push_back(pFont);


		pFont = new SNxFont(_T("Utsaah"),_T("Utsaah"));
		vFonts.push_back(pFont);


		pFont = new SNxFont(_T("Vijaya"),_T("Vijaya"));
		vFonts.push_back(pFont);


		pFont = new SNxFont(_T("Wingdings"),_T("Wingdings"));
		vFonts.push_back(pFont);


		pFont = new SNxFont(_T("Andalus"),_T("Andalus"));
		vFonts.push_back(pFont);


		pFont = new SNxFont(_T("Arabic Typesetting"),_T("Arabic Typesetting"));
		vFonts.push_back(pFont);


		pFont = new SNxFont(_T("Simplified Arabic"),_T("Simplified Arabic"));
		vFonts.push_back(pFont);
	}

	void Flush()
	{
		//if(sGlobalData.sTemplateIconBuffer.pBufferData!= NULL)
		//{
		//	GlobalFree(sGlobalData.sTemplateIconBuffer.pBufferData);
		//	//sGlobalData.sTemplateIconBuffer.pBufferData = NULL;
		//}

		//if(sGlobalData.sTemplateComposerFrameBuffer.pBufferData!= NULL)
		//{
		//	GlobalFree(sGlobalData.sTemplateComposerFrameBuffer.pBufferData);
		//	//sGlobalData.sTemplateComposerFrameBuffer.pBufferData = NULL;
		//}
		//if(sGlobalData.sTemplateCGFrameBuffer.pBufferData!= NULL)
		//{
		//	GlobalFree(sGlobalData.sTemplateCGFrameBuffer.pBufferData);
		//	//sGlobalData.sTemplateCGFrameBuffer.pBufferData = NULL;
		//}
		//if(sGlobalData.sBackgroundFrameBuffer.pBufferData!= NULL)
		//{
		//	GlobalFree(sGlobalData.sBackgroundFrameBuffer.pBufferData);
		//	//sGlobalData.sBackgroundFrameBuffer.pBufferData = NULL;
		//} 
	}
};




