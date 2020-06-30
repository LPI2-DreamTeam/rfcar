void MMSLSDlg::OnBtnGetFile() 
{
    CFileDialog dlg(TRUE,_T(""),NULL,
                    OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
                    _T("Ezcad file(*.ezd)|*.ezd|"));
    if(dlg.DoModal()==IDOK)
    {
        CString fName = dlg.GetPathName();
        int nErr, layCnt, nCaps;
        double xmin, ymin, xmax, ymax, z;
        double &dMinx=xmin, &dMiny=ymin, &dMaxx=xmax, &dMaxy=ymax, &dz=z;
        TCHAR aux[256];
        CRect rect;
        CDC* pDC;
        Color black(0,0,0);
        
        // Loading file
        _tcscpy(aux,fName);
        nErr = lmc1_LoadEzdFile(aux);
        if(nErr!=LMC1_ERR_SUCCESS)
        {
            AfxMessageBox(_T("Read ezdfile failed!"));
            return;
        }
        layCnt = lmc1_GetEntityCount();
        if( !layCnt ) return;
        
        m_strFileName = fName; // setting name inside fname box

        // filling in all layers
        Layer::clear();
        this->layers.clear();
        for(unsigned int i = 0; i < layCnt; i++)
        {
            nErr = lmc1_GetEntityName(i, aux);
            if( !nErr ) // success
            {
                lmc1_GetEntSize(aux, dMinx, dMiny, dMaxx, dMaxy, dz);
                fillLayer(aux, dMinx, dMiny, dMaxx, dMaxy);
            }
        }

        // Painting Canvas
        if(m_pPrevBmp!=NULL)
        {
                delete m_pPrevBmp;
                m_pPrevBmp = NULL;
        }
        if(m_canvas==NULL) return;    	
        m_canvas->GetClientRect(rect);
        CClientDC previewDC(m_canvas);
        nCaps = previewDC.GetDeviceCaps(BITSPIXEL);
        pDC = &previewDC;

        // Get all objects in database and display them
        m_pPrevBmp = lmc1_GetPrevBitmap(m_canvas->GetSafeHwnd(),
                                        rect.Width(), rect.Height() );
        OnDrawPreview(black);

        // Update UI elems
        fillTable();
        fillCntBar();
        updatePosBar(-1);

        createPens(); // create pens
}
