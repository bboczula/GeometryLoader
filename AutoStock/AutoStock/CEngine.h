#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <iostream>

class CEngine
{
private:
	Microsoft::WRL::ComPtr<ID3D11Device> pD3DDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pD3DImmediateContext;
public:
	CEngine(HWND* parentWindowHandler)
	{
		std::cout << "Entering CEngine constructor" << std::endl;
		DWORD deviceFlags = D3D11_CREATE_DEVICE_SINGLETHREADED;
#if defined(_DEBUG)
		deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_FEATURE_LEVEL featureLevels[]
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0
		};

		D3D_FEATURE_LEVEL chosenFeatureLevel;

		HRESULT result = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, deviceFlags, featureLevels,
			ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &pD3DDevice, &chosenFeatureLevel, &pD3DImmediateContext);
		if (FAILED(result))
		{
			std::cout << "ERROR: Device creation failed!" << std::endl;
		}
	}
};