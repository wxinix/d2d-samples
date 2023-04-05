#include <d2d1.h>
#include <d2d1helper.h>
#include <windows.h>

#pragma comment(lib, "d2d1")

ID2D1Factory *g_pD2DFactory = nullptr;
ID2D1HwndRenderTarget *g_pRT = nullptr;

#define HIBA_00 TEXT("Error:Program initialisation process.")

HINSTANCE g_hInstance{nullptr};
int g_nCmdShow{0};
char g_szClassName[] = "WindowsApp";
HWND g_hwnd{nullptr};

LRESULT CALLBACK MyWndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int nCmdShow)
{
  g_nCmdShow = nCmdShow;
  g_hInstance = hInstance;

  // Preparing the window instance
  WNDCLASS wc;
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = MyWndProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = hInstance;
  wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
  wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
  wc.lpszMenuName = nullptr;
  wc.lpszClassName = g_szClassName;

  // Registering the application window
  if (!RegisterClass(&wc)) {
    MessageBox(nullptr, HIBA_00, TEXT("Program Start"), MB_ICONERROR);
    return 0;
  }

  // Creating the window
  g_hwnd = CreateWindow(g_szClassName,
                        TEXT("d2d_01"),
                        (WS_OVERLAPPED | WS_SYSMENU | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX),
                        50,
                        50,
                        800,
                        600,
                        nullptr,
                        nullptr,
                        hInstance,
                        nullptr);

  // Display the window
  ShowWindow(g_hwnd, g_nCmdShow);
  UpdateWindow(g_hwnd);

  // Message handling loop
  MSG msg;
  while (GetMessage(&msg, nullptr, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK MyWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  PAINTSTRUCT ps;
  HDC hdc;

  switch (message) {

    case WM_CREATE:
      D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &g_pD2DFactory);
      g_pD2DFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
                                            D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(800, 600)),
                                            &g_pRT);
      return 0;

    case WM_PAINT:
      g_pRT->BeginDraw();
      g_pRT->Clear(D2D1::ColorF(D2D1::ColorF::Gray));
      g_pRT->EndDraw();
      hdc = BeginPaint(hWnd, &ps);
      TextOut(hdc, 0, 0, "Hello, Windows!", 15);
      EndPaint(hWnd, &ps);
      return 0;

    case WM_CLOSE:
      g_pRT->Release();
      g_pD2DFactory->Release();
      DestroyWindow(hWnd);
      return 0;

    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;

    default:
      return DefWindowProc(hWnd, message, wParam, lParam);
  }
}