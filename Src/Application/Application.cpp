#include "Application.hpp"

#include <iostream>

#include "MatrixCreator.hpp"

Application::Application(HINSTANCE hInstance, int nCmdShow) :

    // Initialize Window
    _window(Setup::SetupWindow(hInstance, nCmdShow)),

    // Initialize D3D11
    _controller(Setup::SetupController(this->_window)),
	_swapChain(Setup::SetupSwapChain(this->_controller, this->_window)),
    _windowRTV(Setup::SetupRenderTargetView(this->_controller, this->_swapChain)),
    _scene(Setup::SetupScene(this->_controller)),
	
    // Initialize Shaders

	/*
	 * Forward Rendering 
	 */

	_forwardVertexShader(std::unique_ptr<VertexShader>
		(dynamic_cast<VertexShader*>
		(Setup::SetupShader(this->_controller, ShaderType::VERTEX_SHADER, L"vsForward.hlsl")))),
		
	_forwardPixelShader(std::unique_ptr<PixelShader>
		(dynamic_cast<PixelShader*>
		(Setup::SetupShader(this->_controller, ShaderType::PIXEL_SHADER, L"psForward.hlsl")))),

	/*
	 * Deferred Rendering
	 */

	/*
	 * Geometry Pass
	 */

    _deferredVertexShaderGeometry(std::unique_ptr<VertexShader>
		(dynamic_cast<VertexShader*>
		(Setup::SetupShader(this->_controller, ShaderType::VERTEX_SHADER, L"vsDeferredGeometry.hlsl")))),

    //_hullShader();
    //_domainShader(),
    //_geometryShader(),
    _deferredPixelShaderGeometry(std::unique_ptr<PixelShader>
		(dynamic_cast<PixelShader*>
		(Setup::SetupShader(this->_controller, ShaderType::PIXEL_SHADER, L"psDeferredGeometry.hlsl")))),

	/*
	 * Light Pass
	 */

    _computeShader(std::unique_ptr<ComputeShader>
    	(dynamic_cast<ComputeShader*>
    	(Setup::SetupShader(this->_controller, ShaderType::COMPUTE_SHADER, L"csDeferredLight.hlsl")))),

    // Initialize InputLayout and Sampler
    _inputLayout(Setup::SetupInputLayout(this->_controller, *this->_deferredVertexShaderGeometry)),
    _sampler(Setup::SetupSampler(this->_controller)),

	// Initalize ConstantBuffers
	_worldMatrixConstantBuffer(Setup::CreateWorldMatrixConstantBuffer(this->_controller)),
	_lightingConstants(Setup::CreatePixelShaderConstantBuffer(this->_controller, this->_scene))
{
	const size_t numGBuffers = 3;
	for (size_t i = 0; i < numGBuffers; ++i )
	{
		this->_gBuffers.emplace_back(Setup::SetupGBuffer(this->_controller));
	}

	
}

int Application::Run()
{
	RunAsserts();
    Setup(); 
    Render();

    return 0;
}

void Application::RunAsserts()
{
    // Window
    assert(&this->_window != nullptr);

    // Controller
    assert(this->_controller.GetContext() != nullptr);
    assert(this->_controller.GetDevice() != nullptr);
    assert(this->_swapChain.GetSwapChain() != nullptr);

    D3D11_VIEWPORT vp = this->_controller.GetViewPort();
    assert(&vp != nullptr);

    // RTV
    assert(this->_windowRTV.GetRTV() != nullptr);
	
    //Scene
    /*
     * TODO: Implement asserts for scene, mesh, submesh and camera
    */

    // Shaders
	assert(this->_deferredVertexShaderGeometry != nullptr);
	assert(this->_deferredPixelShaderGeometry != nullptr);
    assert(this->_deferredVertexShaderGeometry->GetShaderBlob() != nullptr);
    assert(this->_deferredPixelShaderGeometry->GetShaderBlob() != nullptr);

    // Input layout + Sampler
    assert(this->_inputLayout.GetInputLayout() != nullptr);
    assert(this->_sampler.GetSamplerState() != nullptr);

	// Constant Buffers
	assert(this->_worldMatrixConstantBuffer.GetBuffer() != nullptr);
	assert(this->_lightingConstants.GetBuffer() != nullptr);
}


void Application::Setup()
{
	/*
	 * Forward Rendering
	 */

	// Vertex Shader
	this->_forwardVertexShader->AddConstantBuffer(this->_worldMatrixConstantBuffer.GetBuffer());
	this->_forwardVertexShader->AddConstantBuffer(this->_scene.GetCurrentCamera().GetConstantBuffer());

	// Pixel Shader
	this->_forwardPixelShader->AddConstantBuffer(this->_lightingConstants.GetBuffer());
	
	/*
	 * Deferred Rendering
	 */
	
	// Geometry Pass Shader Constant Buffers
	
	// Vertex Shader
	this->_deferredVertexShaderGeometry->AddConstantBuffer(this->_worldMatrixConstantBuffer.GetBuffer());
	this->_deferredVertexShaderGeometry->AddConstantBuffer(this->_scene.GetCurrentCamera().GetConstantBuffer());

	// Pixel Shader
	this->_computeShader->AddConstantBuffer(this->_lightingConstants.GetBuffer());
}

void Application::Render()
{
	RenderConfig renderConfig;
	RenderMode renderMode = renderConfig.GetRenderMode();
	
    float rotationalSpeed = 0.0f;
	WorldMatrixConfig worldMatrixConfig;
	float currentAngle = worldMatrixConfig.GetInitialAngle();
	
	MatrixCreator matrixCreator;
	
	while (this->_input.Exit(this->_msg))
	{
		this->_clock.Start();
     		
		if (PeekMessage(&this->_msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&this->_msg);
			DispatchMessage(&this->_msg);
		}

		if (renderMode == Forward)
		{
			this->_renderer.RenderForward(this->_controller, this->_windowRTV, *this->_forwardVertexShader, *this->_forwardPixelShader, this->_inputLayout, this->_scene, this->_sampler);
		}
		if (renderMode == Deferred)
		{
			this->_renderer.RenderDeferred(this->_controller, this->_swapChain,this->_windowRTV, this->_gBuffers, *this->_deferredVertexShaderGeometry, *this->_deferredPixelShaderGeometry, *this->_computeShader, this->_inputLayout, this->_scene, this->_sampler);
		}
		
		this->_swapChain.GetSwapChain()->Present(0, 0);
		
		this->_scene.GetCurrentCamera().UpdateInternalConstantBuffer(this->_controller.GetContext());
			
		DX::XMFLOAT4X4 worldMatrixFloat4x4 = matrixCreator.CreateWorldXMFLOAT4X4(currentAngle);
		this->_worldMatrixConstantBuffer.UpdateBuffer(this->_controller.GetContext(), &worldMatrixFloat4x4, sizeof(worldMatrixFloat4x4));

		this->_clock.End();
		float deltaTime = this->_clock.GetDeltaTime();
 
		this->_input.ReadInput(this->_scene.GetCurrentCamera(), deltaTime);
		currentAngle -= rotationalSpeed * deltaTime;
	}
}



