#include "Application.hpp"

#include "DomainShaderData.hpp"
#include "HullShaderData.hpp"
#include "MatrixCreator.hpp"
#include "RenderConfig.hpp"
#include "LightData.hpp"
#include "OutputModeData.hpp"
#include "SpecularExpData.hpp"


Application::Application(HINSTANCE hInstance, int nCmdShow) :

    // Initialize Window
    _window(Setup::SetupWindow(hInstance, nCmdShow)),

    // Initialize D3D11
    _controller(Setup::SetupController()),
	_swapChain(Setup::SetupSwapChain(this->_controller, this->_window)),
    _windowRTV(Setup::SetupRenderTargetView(this->_controller, this->_swapChain)),
    _scene(Setup::SetupScene(this->_controller)),
	
    // Initialize Shaders

	/*
	 * Forward Rendering 
	 */

	_vsForward(std::unique_ptr<VertexShader>
		(dynamic_cast<VertexShader*>
		(Setup::SetupShader(this->_controller, ShaderType::VERTEX_SHADER, L"vsForward.hlsl")))),
		
	_psForward(std::unique_ptr<PixelShader>
		(dynamic_cast<PixelShader*>
		(Setup::SetupShader(this->_controller, ShaderType::PIXEL_SHADER, L"psForward.hlsl")))),

	/*
	 * Deferred Rendering
	 */

	/*
	 * Geometry Pass
	 */

    _vsDeferredGeometry(std::unique_ptr<VertexShader>
		(static_cast<VertexShader*>
		(Setup::SetupShader(this->_controller, ShaderType::VERTEX_SHADER, L"vsDeferredGeometry.hlsl")))),

    _hsDeferredGeometry(std::unique_ptr<HullShader>
		(static_cast<HullShader*>
		(Setup::SetupShader(this->_controller, ShaderType::HULL_SHADER, L"hsDeferredGeometry.hlsl")))),

    _dsDeferredGeometry(std::unique_ptr<DomainShader>
		(static_cast<DomainShader*>
		(Setup::SetupShader(this->_controller, ShaderType::DOMAIN_SHADER, L"dsDeferredGeometry.hlsl")))),

    //_gsDeferredGeometry(std::unique_ptr<GeometryShader>
	//	(static_cast<GeometryShader*>
	//	(Setup::SetupShader(this->_controller, ShaderType::GEOMETRY_SHADER, L"gsDeferredGeometry.hlsl")))),

    _psDeferredGeometry(std::unique_ptr<PixelShader>
		(static_cast<PixelShader*>
		(Setup::SetupShader(this->_controller, ShaderType::PIXEL_SHADER, L"psDeferredGeometry.hlsl")))),

	/*
	 * Light Pass
	 */

    _csDeferredLight(std::unique_ptr<ComputeShader>
    	(static_cast<ComputeShader*>
    	(Setup::SetupShader(this->_controller, ShaderType::COMPUTE_SHADER, L"csDeferredLight.hlsl")))),

    // Initialize Rasterizer, InputLayout and Sampler
	_rasterizer(Setup::SetupRasterizer(this->_controller)),
    _inputLayout(Setup::SetupInputLayout(this->_controller, *this->_vsDeferredGeometry)),
    _sampler(Setup::SetupSampler(this->_controller))
{
	const size_t numGBuffers = 6;
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
	assert(this->_vsDeferredGeometry != nullptr);
	assert(this->_psDeferredGeometry != nullptr);
    assert(this->_vsDeferredGeometry->GetShaderBlob() != nullptr);
    assert(this->_psDeferredGeometry->GetShaderBlob() != nullptr);

    // Input layout + Sampler
	assert(this->_rasterizer.GetRasterizerState() != nullptr);
    assert(this->_inputLayout.GetInputLayout() != nullptr);
    assert(this->_sampler.GetSamplerState() != nullptr);
}


void Application::Setup()
{
	RenderConfig renderConfig;
	RenderMode renderMode = renderConfig.GetRenderMode();
	
	/*
	 * Forward Rendering
	 */

	if (renderMode == Forward)
	{
		SetupForwardBuffers();
	}

	/*
	 * Deferred Rendering
	 */
	
	if (renderMode == Deferred)
	{
		SetupDeferredBuffers();
	}
}

void Application::SetupForwardBuffers()
{
	// Vertex Shader

	// World Matrix Buffer
	
	BufferDescData worldMatrixBufferDescData;
	worldMatrixBufferDescData.Usage = D3D11_USAGE_DYNAMIC;
	worldMatrixBufferDescData.CpuAccess = D3D11_CPU_ACCESS_WRITE;

	MatrixCreator matrixCreator;
	DX::XMMATRIX worldMatrix = matrixCreator.CreateWorldMatrix({0.0f, 0.0f, 0.0f, 1.0f}); 

	DX::XMFLOAT4X4 worldMatrix4x4;
	DX::XMStoreFloat4x4(&worldMatrix4x4, worldMatrix);
	
	this->_vsForward->AddConstantBuffer(Setup::CreateConstantBuffer<DX::XMFLOAT4X4>(this->_controller, worldMatrixBufferDescData, &worldMatrix4x4));

	BufferDescData viewProjectionMatrixBufferDescData;
	viewProjectionMatrixBufferDescData.Usage = D3D11_USAGE_DYNAMIC;
	viewProjectionMatrixBufferDescData.CpuAccess = D3D11_CPU_ACCESS_WRITE;
    	
	const DX::XMMATRIX viewProjectionMatrix = matrixCreator.CreateViewProjectionMatrix(this->_scene.GetCurrentCamera());
	DX::XMFLOAT4X4 viewProjMatrix4x4;
	XMStoreFloat4x4(&viewProjMatrix4x4, viewProjectionMatrix);
	
	this->_vsForward->AddConstantBuffer(Setup::CreateConstantBuffer<DX::XMFLOAT4X4>(this->_controller, viewProjectionMatrixBufferDescData, &viewProjMatrix4x4));

	// Pixel Shader
	
	BufferDescData lightingBufferDescData;
	lightingBufferDescData.Usage = D3D11_USAGE_DYNAMIC;
	lightingBufferDescData.CpuAccess = D3D11_CPU_ACCESS_WRITE;

	LightData lightData;
    	
	lightData.LightColour = {1.0f, 1.0f, 1.0f, 1.0f};
	lightData.LightPosition = {0.0f, -7.5f, -10.0f, 1.0f};
	lightData.CamPosition = this->_scene.GetCurrentCamera().GetPosition();
	lightData.AmbientLightIntensity = 0.1f;
	lightData.GeneralLightIntensity = 1.2f;
	
	this->_psForward->AddConstantBuffer(Setup::CreateConstantBuffer<LightData>(this->_controller, lightingBufferDescData, &lightData));
}

void Application::SetupDeferredBuffers()
{
	// Geometry Pass Shader Constant Buffers
	
	// Vertex Shader
	// World Matrix Buffer

	BufferDescData vertexShaderBufferDesc;
	vertexShaderBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexShaderBufferDesc.CpuAccess = D3D11_CPU_ACCESS_WRITE;

	MatrixCreator matrixCreator;
	const DX::XMMATRIX worldMatrix = matrixCreator.CreateWorldMatrix({0.0f, 0.0f, 0.0f, 1.0f}); 

	DX::XMFLOAT4X4 worldMatrix4x4;
	DX::XMStoreFloat4x4(&worldMatrix4x4, worldMatrix);
	
	this->_vsDeferredGeometry->AddConstantBuffer(Setup::CreateConstantBuffer<DX::XMFLOAT4X4>(this->_controller, vertexShaderBufferDesc, &worldMatrix4x4));

	BufferDescData vertexShaderBufferDesc2;
	vertexShaderBufferDesc2.Usage = D3D11_USAGE_DYNAMIC;
	vertexShaderBufferDesc2.CpuAccess = D3D11_CPU_ACCESS_WRITE;
	
    const DX::XMMATRIX viewProjectionMatrix = matrixCreator.CreateViewProjectionMatrix(this->_scene.GetCurrentCamera());
	DX::XMFLOAT4X4 viewProjMatrix4x4;
	XMStoreFloat4x4(&viewProjMatrix4x4, viewProjectionMatrix);
	
	this->_vsDeferredGeometry->AddConstantBuffer(Setup::CreateConstantBuffer<DX::XMFLOAT4X4>(this->_controller, vertexShaderBufferDesc2, &viewProjMatrix4x4));

	// Hull Shader

	BufferDescData hullShaderBufferDesc;
	hullShaderBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	hullShaderBufferDesc.CpuAccess = D3D11_CPU_ACCESS_WRITE;

	HullShaderData hullShaderData;
	hullShaderData.CameraPosition = this->_scene.GetMainCamera().GetPosition();
	hullShaderData.MeshPosition = this->_scene.GetMeshAt(0).GetCurrentPosition();

	this->_hsDeferredGeometry->AddConstantBuffer(Setup::CreateConstantBuffer<HullShaderData>(this->_controller, hullShaderBufferDesc, &hullShaderData));

	// Domain Shader

	BufferDescData domainShaderBufferDesc;
	domainShaderBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	domainShaderBufferDesc.CpuAccess = D3D11_CPU_ACCESS_WRITE;

	DomainShaderData domainShaderData;
	domainShaderData.ViewProjectionMatrix = viewProjMatrix4x4;

	this->_dsDeferredGeometry->AddConstantBuffer(Setup::CreateConstantBuffer<DomainShaderData>(this->_controller, domainShaderBufferDesc, &domainShaderData));
	
	// Pixel Shader

	BufferDescData pixelShaderBufferDesc;
	pixelShaderBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	pixelShaderBufferDesc.CpuAccess = D3D11_CPU_ACCESS_WRITE;

	SpecularExpData specularExpData;
	specularExpData.SpecularExponent = 10000.0f;

	this->_psDeferredGeometry->AddConstantBuffer(Setup::CreateConstantBuffer<SpecularExpData>(this->_controller, pixelShaderBufferDesc, &specularExpData));

	// Compute Shader
	
	BufferDescData computeShaderBufferDesc;
	computeShaderBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	computeShaderBufferDesc.CpuAccess = D3D11_CPU_ACCESS_WRITE;

	LightData lightData;
	
	lightData.LightColour = {1.0f, 1.0f, 1.0f, 1.0f};
	lightData.LightPosition = {0.0f, -7.5f, -10.0f, 1.0f};
	lightData.CamPosition = this->_scene.GetCurrentCamera().GetPosition();
	lightData.AmbientLightIntensity = 0.1f;
	lightData.GeneralLightIntensity = 1.2f;
	
	this->_csDeferredLight->AddConstantBuffer(Setup::CreateConstantBuffer<LightData>(this->_controller, computeShaderBufferDesc, &lightData));

	BufferDescData computeBufferDesc2;
	computeBufferDesc2.Usage = D3D11_USAGE_DYNAMIC;
	computeBufferDesc2.CpuAccess = D3D11_CPU_ACCESS_WRITE;

	OutputModeData outputMode;
	outputMode.OutputMode = this->_outputMode;
	
	this->_csDeferredLight->AddConstantBuffer(Setup::CreateConstantBuffer<OutputModeData>(this->_controller, computeBufferDesc2, &outputMode));
}

void Application::Render()
{
	RenderConfig renderConfig;
	RenderMode renderMode = renderConfig.GetRenderMode();
	
	float deltaTime = 0.0f;
	bool running = true;

	while (running)
	{
		this->_clock.Start();

		// Input
        this->_input.ReadInput(this->_scene, this->_window, running, this->_msg, this->_outputMode, deltaTime);

		// Oscillate first mesh
		for (size_t i = 0; i < this->_scene.GetNumOscillatingMeshes(); ++i)
		{
			this->_scene.GetOscillatingMeshAt(i).PerformOscillation(deltaTime);
		}
		
		if (PeekMessage(&this->_msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&this->_msg);
			DispatchMessage(&this->_msg);
		}

		// Render
		if (renderMode == Forward)
		{
			this->_renderer.RenderForward(this->_controller, this->_windowRTV, *this->_vsForward, this->_rasterizer, *this->_psForward, this->_inputLayout, this->_scene, this->_sampler);
		}
		if (renderMode == Deferred)
		{
			this->_renderer.RenderDeferred(this->_controller, this->_swapChain,this->_windowRTV, this->_gBuffers, *this->_vsDeferredGeometry, *this->_hsDeferredGeometry, *this->_dsDeferredGeometry, this->_rasterizer, *this->_psDeferredGeometry, *this->_csDeferredLight, this->_inputLayout, this->_scene, this->_sampler, this->_outputMode);
		}
		
		this->_swapChain.GetSwapChain()->Present(0, 0);
		
		this->_clock.End();
		deltaTime = this->_clock.GetDeltaTime();

		
	}
}



