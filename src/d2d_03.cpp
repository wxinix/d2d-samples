#include <d2d1.h>
#include <d2d1helper.h>
#include <windows.h>
#pragma comment(lib, "d2d1")

ID2D1Factory *g_pD2DFactory = nullptr;
ID2D1HwndRenderTarget *g_pRT = nullptr;

#define HIBA_00 TEXT("Error:Program initialisation process.")
HINSTANCE g_hInstance;
int g_nCmdShow;
char g_szClassName[] = "WindowsApp";
HWND g_hwnd;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void D2D_drawing(ID2D1HwndRenderTarget *pRT);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int nCmdShow)
{
  static TCHAR szAppName[] = TEXT("StdWinClassName");
  HWND hwnd;
  MSG msg;
  WNDCLASS wc;
  g_nCmdShow = nCmdShow;
  g_hInstance = hInstance;

  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = WndProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = hInstance;
  wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
  wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
  wc.lpszMenuName = nullptr;
  wc.lpszClassName = TEXT("WIN0");

  if (!RegisterClass(&wc)) {
    MessageBox(nullptr, HIBA_00, TEXT("Program Start"), MB_ICONERROR);
    return 0;
  }

  g_hwnd = CreateWindow(TEXT("WIN0"),
                        TEXT("g_hwnd"),
                        (WS_OVERLAPPED | WS_SYSMENU | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX),
                        50,
                        50,
                        800,
                        600,
                        nullptr,
                        nullptr,
                        hInstance,
                        nullptr);

  ShowWindow(g_hwnd, g_nCmdShow);
  UpdateWindow(g_hwnd);

  while (GetMessage(&msg, nullptr, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  HDC hdc;
  PAINTSTRUCT ps;

  switch (message) {

    case WM_CREATE:
      D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &g_pD2DFactory);
      g_pD2DFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
                                            D2D1::HwndRenderTargetProperties(hwnd, D2D1::SizeU(800, 600)),
                                            &g_pRT);
      return 0;

    case WM_PAINT:
      hdc = BeginPaint(hwnd, &ps);
      EndPaint(hwnd, &ps);
      D2D_drawing(g_pRT);
      return 0;

    case WM_CLOSE:
      g_pRT->Release();
      g_pD2DFactory->Release();
      DestroyWindow(hwnd);
      return 0;

    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;

    default:
      return DefWindowProc(hwnd, message, wParam, lParam);
  }
}

void D2D_drawing(ID2D1HwndRenderTarget *pRT)
{
  pRT->BeginDraw();
  pRT->Clear(D2D1::ColorF(D2D1::ColorF::White));

  ID2D1SolidColorBrush *br;
  pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black, 1.0f), &br);

  pRT->DrawLine(D2D1::Point2F(10.0f, 50.0f),
                D2D1::Point2F(250.0f, 500.0f),
                br,
                1.0f);

  pRT->EndDraw();
}
