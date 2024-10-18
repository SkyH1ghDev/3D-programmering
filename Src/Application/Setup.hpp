#pragma once

#include <windows.h>
#include <xstring>
#include <iostream>

#include "Controller.hpp"
#include "InputLayout.hpp"
#include "RenderBuffer.hpp"
#include "Sampler.hpp"
#include "Scene.hpp"
#include "Shader.hpp"
#include "SwapChain.hpp"
#include "Rasterizer/Rasterizer.hpp"

class Setup
{
public:
    
    static HWND SetupWindow(HINSTANCE hInstance, int nCmdShow);
    static Controller SetupController();
    static SwapChain SetupSwapChain(Controller& controller, HWND window);
    static RenderBuffer SetupRenderTargetView(Controller& controller, SwapChain& swapChain);
    static RenderBuffer SetupGBuffer(Controller& controller, const UINT& width, const UINT& height);
    static Scene SetupScene(Controller& controller);
    static Shader* SetupShader(Controller& controller, ShaderType shaderType, std::wstring hlslFilename);
	static Rasterizer SetupRasterizer(Controller& controller);
    static InputLayout SetupInputLayout(Controller& controller, const Shader& vertexShader);
    static Sampler SetupSampler(Controller& controller);
	
	template <typename T>
	static ConstantBuffer CreateConstantBuffer(Controller &controller, BufferDescData& bufferFlags, T* data)
	{
		HRESULT hr;
		
		ConstantBuffer constantBuffer = ConstantBuffer(hr, controller.GetDevice(), sizeof(*data),
														data, 0, 0, 0,
														bufferFlags);

		if (FAILED(hr))
		{
			throw std::runtime_error("Failed to create Constant Buffer");
		}

		return constantBuffer;
	}
};
