#ifndef _PROCS_H
#define _PROCS_H

BOOL CALLBACK SplashWinProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK MainProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK TextWinProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ConfigWinProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK PreviewWinProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AboutProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK SaveOptProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);

#endif