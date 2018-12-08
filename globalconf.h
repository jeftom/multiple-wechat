#ifndef GLOBALCONF_H
#define GLOBALCONF_H
class QWindow;
QList<HWND>      g_listWindowHandle;
QList<QWindow*>  g_listWindow;
bool             g_threadLogin  = true;
bool             g_threadLogout = true;
QMap<QWindow*, HWND> g_mapWin2Hand;
QMap<HWND, QWindow*> g_mapHand2Win;
#endif // GLOBALCONF_H
