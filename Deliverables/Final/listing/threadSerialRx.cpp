UINT MMSLSDlg::ThreadSerialRx(LPVOID param)
{
    /* Wait for 1st connection to serial port: OnConnect */
    ::WaitForSingleObject( EvSerial.m_hObject , INFINITE); 
    tstring szData;
    CDemoEzdDlg *dlg = (CDemoEzdDlg *)param;

    while(1)
    {
        ::WaitForSingleObject( dlg->m_theCommPort.GetWaitForEvent() , INFINITE);
        dlg->m_theCommPort.ReadAvailable (szData);

        if(szData == MSG_CONNECTED) /* Connected */
        {
            dlg->AppendToConsole(_T("Connected!"));
            dlg->AppendToConsole(_T("Initializing...")); /* Resetting the machine */
/* Enable Disconnect and disable connect */
            dlg->m_conn_btn.EnableWindow(false);
            dlg->m_disconn_btn.EnableWindow(true);
            dlg->m_mmsls_conn_status.SetWindowText(_T("Status: Connected"));
            EvPingMach.SetEvent(); /* Set event to stop PingMach thread */ 
        }
        if(szData == MSG_HOME) /* Home: Reset done */
        {
            dlg->AppendToConsole(_T("Done!"));
            /* Enable Calibration and disable Processing */
            dlg->EnableCalib(true);
            dlg->EnableProcessing(false);
        }
        if(szData == MSG_MANUAL_CALIB) /* Manual Calibration done: Sent by OnCalibDone */ 
        {
            dlg->AppendToConsole(_T("Calibration complete!"));
            dlg->AppendToConsole(_T("Ready!"));
            /* Disable Calibration and Enable Processing */
            dlg->EnableCalib(false);
            dlg->EnableProcessing(true);
        }
        if(szData == MSG_RUN) /* Recoating done:  */ 
            EvRecoatEnd.SetEvent(); /* Signal event to Run */
        if(szData == MSG_IDLE)
        /* Update Laser state */
            dlg->m_mmsls_proc_status.SetWindowText(_T("Status: IDLE"));
        if(szData == EMERG_BYTE) /* Emergency stop activated  */ 
            EvStop.SetEvent(); /* Signal event to Run */
    }

    return 0;
}
