﻿#pragma once

#include <windows.h>
#include <xstring>
#include <iostream>

#include "Controller.hpp"
#include "InputLayout.hpp"
#include "RenderTargetView.hpp"
#include "Sampler.hpp"
#include "Scene.hpp"
#include "Shader.hpp"
#include "SwapChain.hpp"

class Setup
{
public:
    
    static HWND SetupWindow(HINSTANCE hInstance, int nCmdShow);
    static Controller SetupController();
    static SwapChain SetupSwapChain(Controller& controller, HWND window);
    static RenderTargetView SetupRenderTargetView(Controller& controller, SwapChain& swapChain);
    static RenderTargetView SetupGBuffer(Controller& controller);
    static Scene SetupScene(Controller& controller);
    static Shader* SetupShader(Controller& controller, ShaderType shaderType, std::wstring hlslFilename);
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
