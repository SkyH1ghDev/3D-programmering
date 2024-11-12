#include "Application.hpp"

#include "DSGeometryData.hpp"
#include "HSGeometryData.hpp"
#include "MatrixCreator.hpp"
#include "RenderConfig.hpp"
#include "CSLightData.hpp"
#include "CSParticleData.hpp"
#include "CubeMapCollection.hpp"
#include "FileReader.hpp"
#include "ShadowMapCollection.hpp"
#include "GSParticleData.hpp"
#include "LightConfig.hpp"
#include "Particle.hpp"
#include "PSGeometryData.hpp"
#include "PSReflectionData.hpp"
#include "SpotLight.hpp"
#include "VSGeometryData.hpp"


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
		(Setup::SetupShader(this->_controller, ShaderType::VERTEX_SHADER, L"Forward/vsForward.hlsl")))),
		
	_psForward(std::unique_ptr<PixelShader>
		(dynamic_cast<PixelShader*>
		(Setup::SetupShader(this->_controller, ShaderType::PIXEL_SHADER, L"Forward/psForward.hlsl")))),

	/*
	 * Deferred Rendering
	 */

	/*
	 * Geometry Pass
	 */

    _vsDeferredGeometry(std::unique_ptr<VertexShader>
		(static_cast<VertexShader*>
		(Setup::SetupShader(this->_controller, ShaderType::VERTEX_SHADER, L"Deferred/GeometryPass/vsDeferredGeometry.hlsl")))),

    _hsDeferredGeometry(std::unique_ptr<HullShader>
		(static_cast<HullShader*>
		(Setup::SetupShader(this->_controller, ShaderType::HULL_SHADER, L"Deferred/GeometryPass/hsDeferredGeometry.hlsl")))),

    _dsDeferredGeometry(std::unique_ptr<DomainShader>
		(static_cast<DomainShader*>
		(Setup::SetupShader(this->_controller, ShaderType::DOMAIN_SHADER, L"Deferred/GeometryPass/dsDeferredGeometry.hlsl")))),

    _psDeferredReflection(std::unique_ptr<PixelShader>
    	(static_cast<PixelShader*>
    	(Setup::SetupShader(this->_controller, ShaderType::PIXEL_SHADER, L"Deferred/GeometryPass/psDeferredReflection.hlsl")))),

    _psDeferredGeometry(std::unique_ptr<PixelShader>
		(static_cast<PixelShader*>
		(Setup::SetupShader(this->_controller, ShaderType::PIXEL_SHADER, L"Deferred/GeometryPass/psDeferredGeometry.hlsl")))),

	/*
	 * Light Pass
	 */

    _csDeferredLight(std::unique_ptr<ComputeShader>
    	(static_cast<ComputeShader*>
    	(Setup::SetupShader(this->_controller, ShaderType::COMPUTE_SHADER, L"Deferred/LightPass/csDeferredLight.hlsl")))),

	_csDeferredReflection
		(std::unique_ptr<ComputeShader>
		(static_cast<ComputeShader*>
		(Setup::SetupShader(this->_controller, ShaderType::COMPUTE_SHADER, L"Deferred/LightPass/csDeferredReflection.hlsl")))),

	/*
	 * Particle Pass
	 */

	_csDeferredParticle(std::unique_ptr<ComputeShader>
		(static_cast<ComputeShader*>
		(Setup::SetupShader(this->_controller, ShaderType::COMPUTE_SHADER, L"Deferred/ParticlePass/csDeferredParticle.hlsl")))),

	_vsDeferredParticle(std::unique_ptr<VertexShader>
		(static_cast<VertexShader*>
		(Setup::SetupShader(this->_controller, ShaderType::VERTEX_SHADER, L"Deferred/ParticlePass/vsDeferredParticle.hlsl")))),

	_gsDeferredParticle(std::unique_ptr<GeometryShader>
		(static_cast<GeometryShader*>
		(Setup::SetupShader(this->_controller, ShaderType::GEOMETRY_SHADER, L"Deferred/ParticlePass/gsDeferredParticle.hlsl")))),

	_psDeferredParticle(std::unique_ptr<PixelShader>
		(static_cast<PixelShader*>
		(Setup::SetupShader(this->_controller, ShaderType::PIXEL_SHADER, L"Deferred/ParticlePass/psDeferredParticle.hlsl")))),

    // Initialize Rasterizer, InputLayout and Sampler
	_rasterizer(Setup::SetupRasterizer(this->_controller)),
    _inputLayout(Setup::SetupInputLayout(this->_controller, *this->_vsDeferredGeometry)),
    _sampler(Setup::SetupSampler(this->_controller))
{

	WindowConfig windowConfig;
	
	const size_t numGBuffers = 6;
	for (size_t i = 0; i < numGBuffers; ++i )
	{
		this->_gBuffers.push_back(Setup::SetupGBuffer(this->_controller, windowConfig.GetWidth(), windowConfig.GetHeight()));
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

	CSLightData csLightData;
    	
	csLightData.LightColour = {1.0f, 1.0f, 1.0f, 1.0f};
	csLightData.LightPosition = {0.0f, -7.5f, -10.0f, 1.0f};
	csLightData.CamPosition = this->_scene.GetCurrentCamera().GetPosition();
	csLightData.AmbientLightIntensity = 0.1f;
	csLightData.GeneralLightIntensity = 1.2f;
	
	this->_psForward->AddConstantBuffer(Setup::CreateConstantBuffer<CSLightData>(this->_controller, lightingBufferDescData, &csLightData));
}

void Application::SetupDeferredBuffers()
{
	// Geometry Pass Shader Constant Buffers
	
	// Vertex Shader
	// World Matrix Buffer

	BufferDescData vsBufferDescData;
	vsBufferDescData.Usage = D3D11_USAGE_DYNAMIC;
	vsBufferDescData.CpuAccess = D3D11_CPU_ACCESS_WRITE;

	MatrixCreator matrixCreator;
	
	const DX::XMMATRIX worldMatrix = matrixCreator.CreateWorldMatrix({0.0f, 0.0f, 0.0f, 1.0f}); 
	DX::XMFLOAT4X4 worldMatrix4x4;
	DX::XMStoreFloat4x4(&worldMatrix4x4, worldMatrix);

    const DX::XMMATRIX viewProjectionMatrix = matrixCreator.CreateViewProjectionMatrix(this->_scene.GetCurrentCamera());
	DX::XMFLOAT4X4 viewProjMatrix4x4;
	XMStoreFloat4x4(&viewProjMatrix4x4, viewProjectionMatrix);

	VSGeometryData vsGeometryData;
	vsGeometryData.WorldMatrix = worldMatrix4x4;
	vsGeometryData.ViewProjectionMatrix = viewProjMatrix4x4;
	
	this->_vsDeferredGeometry->AddConstantBuffer(Setup::CreateConstantBuffer<VSGeometryData>(this->_controller, vsBufferDescData, &vsGeometryData));

	// Hull Shader

	BufferDescData hullShaderBufferDesc;
	hullShaderBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	hullShaderBufferDesc.CpuAccess = D3D11_CPU_ACCESS_WRITE;

	HSGeometryData hullShaderData;
	hullShaderData.CameraPosition = this->_scene.GetMainCamera().GetPosition();
	hullShaderData.MeshPosition = this->_scene.GetMeshAt(0).GetCurrentPosition();

	this->_hsDeferredGeometry->AddConstantBuffer(Setup::CreateConstantBuffer<HSGeometryData>(this->_controller, hullShaderBufferDesc, &hullShaderData));

	// Domain Shader

	BufferDescData domainShaderBufferDesc;
	domainShaderBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	domainShaderBufferDesc.CpuAccess = D3D11_CPU_ACCESS_WRITE;

	DSGeometryData domainShaderData;
	domainShaderData.ViewProjectionMatrix = viewProjMatrix4x4;

	this->_dsDeferredGeometry->AddConstantBuffer(Setup::CreateConstantBuffer<DSGeometryData>(this->_controller, domainShaderBufferDesc, &domainShaderData));

	// Geometry Shader

	BufferDescData geometryShaderBufferDesc;
	geometryShaderBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	geometryShaderBufferDesc.CpuAccess = D3D11_CPU_ACCESS_WRITE;

	GSParticleData gsParticleData;
	gsParticleData.CameraPosition = this->_scene.GetCurrentCamera().GetPosition();
	gsParticleData.ViewProjectionMatrix = viewProjMatrix4x4;

	this->_gsDeferredParticle->AddConstantBuffer(Setup::CreateConstantBuffer<GSParticleData>(this->_controller, geometryShaderBufferDesc, &gsParticleData));
	
	// Pixel Shader

	BufferDescData pixelShaderBufferDesc;
	pixelShaderBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	pixelShaderBufferDesc.CpuAccess = D3D11_CPU_ACCESS_WRITE;

	PSGeometryData specularExpData;
	specularExpData.SpecularExponent = 10000.0f;

	this->_psDeferredGeometry->AddConstantBuffer(Setup::CreateConstantBuffer<PSGeometryData>(this->_controller, pixelShaderBufferDesc, &specularExpData));

	BufferDescData reflectionPixelShaderBufferDesc;
	reflectionPixelShaderBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	reflectionPixelShaderBufferDesc.CpuAccess = D3D11_CPU_ACCESS_WRITE;

	PSReflectionData psReflectionData;
	psReflectionData.SpecularExponent = 10000.0f;
	psReflectionData.CameraPosition = this->_scene.GetCurrentCamera().GetPosition();

	this->_psDeferredReflection->AddConstantBuffer(Setup::CreateConstantBuffer<PSReflectionData>(this->_controller, reflectionPixelShaderBufferDesc, &psReflectionData));
	
	// Compute Shader
	
	BufferDescData computeShaderBufferDesc;
	computeShaderBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	computeShaderBufferDesc.CpuAccess = D3D11_CPU_ACCESS_WRITE;

	CSLightData csLightData;
	
	csLightData.LightColour = {1.0f, 1.0f, 1.0f, 1.0f};
	csLightData.LightPosition = {0.0f, -7.5f, -10.0f, 1.0f};
	csLightData.CamPosition = this->_scene.GetCurrentCamera().GetPosition();
	csLightData.AmbientLightIntensity = 0.1f;
	csLightData.GeneralLightIntensity = 1.2f;
	csLightData.OutputMode = _outputMode;
	
	this->_csDeferredLight->AddConstantBuffer(Setup::CreateConstantBuffer<CSLightData>(this->_controller, computeShaderBufferDesc, &csLightData));

	BufferDescData reflectionComputeShaderDesc;
	reflectionComputeShaderDesc.Usage = D3D11_USAGE_DYNAMIC;
	reflectionComputeShaderDesc.CpuAccess = D3D11_CPU_ACCESS_WRITE;

	CSLightData csReflectionData;
	csReflectionData.LightColour = {1.0f, 1.0f, 1.0f, 1.0f};
	csReflectionData.LightPosition = {0.0f, -7.5f, -10.0f, 1.0f};
	csReflectionData.CamPosition = this->_scene.GetCurrentCamera().GetPosition();
	csReflectionData.AmbientLightIntensity = 0.1f;
	csReflectionData.GeneralLightIntensity = 1.2f;
	csReflectionData.OutputMode = _outputMode;

	this->_csDeferredReflection->AddConstantBuffer(Setup::CreateConstantBuffer<CSLightData>(this->_controller, reflectionComputeShaderDesc, &csReflectionData));

	BufferDescData particleComputeShaderDesc;
	particleComputeShaderDesc.Usage = D3D11_USAGE_DYNAMIC;
	particleComputeShaderDesc.CpuAccess = D3D11_CPU_ACCESS_WRITE;

	std::srand(std::time(nullptr));
	
	CSParticleData csParticleData;
	csParticleData.randomizedVector =
		{
		(-0.1f) + static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX / (0.1f - (-0.1f))),
		(-0.1f) + static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX / (0.1f - (-0.1f))),
		(-0.1f) + static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX / (0.1f - (-0.1f))),
		0.0f
		};
	
	this->_csDeferredParticle->AddConstantBuffer(Setup::CreateConstantBuffer<CSParticleData>(this->_controller, particleComputeShaderDesc, &csParticleData));
}

void Application::Render()
{
	RenderConfig renderConfig;
	RenderMode renderMode = renderConfig.GetRenderMode();
	
	float deltaTime = 0.0f;
	bool running = true;

	MatrixCreator matrixCreator;
	LightConfig lightConfig;
	

	HRESULT hr;

	// SHADOW MAP STUFFS

	ProjectionInfo spotLightProjectionInfo;
	
	SpotLight spotLight1(hr, this->_controller.GetDevice(), spotLightProjectionInfo, {-4.0f, -5.0f, -7.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, 20.0f);
	SpotLight spotLight2(hr, this->_controller.GetDevice(), spotLightProjectionInfo, {5.0f, -7.0f, -5.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}, 20.0f);
	SpotLight spotLight3(hr, this->_controller.GetDevice(), spotLightProjectionInfo, {-5.0f, -7.0f, -5.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}, 30.0f);
	SpotLight spotLight4(hr, this->_controller.GetDevice(), spotLightProjectionInfo, {-5.0f, 50.0f, 5.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 180.0f);

	spotLight1.GetCamera().ApplyRotation(0, DX::XMConvertToRadians(90));
	spotLight2.GetCamera().ApplyRotation(DX::XMConvertToRadians(-45), 0);
	spotLight3.GetCamera().ApplyRotation(DX::XMConvertToRadians(-30), DX::XMConvertToRadians(240));
	spotLight4.GetCamera().ApplyRotation(DX::XMConvertToRadians(-90), 0);
	
	std::vector<SpotLight> spotLights = { spotLight1, spotLight2, spotLight3, spotLight4 };

	ShadowMapCollection shadowMaps(hr, this->_controller.GetDevice(), spotLights.size());

	std::vector<CSLightData2> spotlightData =
	{
		spotLight1.GetSpotlightData(),
		spotLight2.GetSpotlightData(),
		spotLight3.GetSpotlightData(),
		spotLight4.GetSpotlightData()
	};
	
	StructuredBuffer spotLightStructuredBuffer(hr, this->_controller.GetDevice(), sizeof(CSLightData2), spotlightData.size(), spotlightData.data(), 0, 0);

	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.BorderColor[0] = samplerDesc.BorderColor[1] = samplerDesc.BorderColor[2] = samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	ID3D11SamplerState* samplerState;
	hr = this->_controller.GetDevice()->CreateSamplerState(&samplerDesc, &samplerState);

	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to create sampler state");
	}

	Sampler depthSampler(samplerState);
	
	// PARTICLE STUFFS
	
	Particle particle;
		particle.Colour = {0.0f, 1.0f, 0.0f, 0.0f};
		particle.Position = lightConfig.GetLightPosition();

	Particle spotLight1Particle;
		spotLight1Particle.Colour = {1.0f, 1.0f, 1.0f, 1.0f};
		spotLight1Particle.Position = spotLight1.GetCamera().GetPosition();

	Particle spotLight2Particle;
    	spotLight2Particle.Colour = {1.0f, 1.0f, 1.0f, 1.0f};
    	spotLight2Particle.Position = spotLight2.GetCamera().GetPosition();
	
	Particle spotLight3Particle;
    	spotLight3Particle.Colour = {1.0f, 1.0f, 1.0f, 1.0f};
    	spotLight3Particle.Position = spotLight3.GetCamera().GetPosition();
	
	Particle spotLight4Particle;
		spotLight4Particle.Colour = {1.0f, 1.0f, 1.0f, 1.0f};
		spotLight4Particle.Position = spotLight4.GetCamera().GetPosition();
	
	std::vector<Particle> particles = { particle, spotLight1Particle, spotLight2Particle, spotLight3Particle, spotLight4Particle };

	StructuredBuffer particleStructuredBuffer = StructuredBuffer(hr, this->_controller.GetDevice(), sizeof(particle), particles.size(), particles.data(), 0, 0);

	// DYNAMIC CUBE MAPPING STUFFS

	CubeMapCollection dynamicCubeMap(hr, this->_controller.GetDevice());

	ProjectionInfo cubeMapProjectionInfo;
		cubeMapProjectionInfo.FovAngleY = 90.0f;
		cubeMapProjectionInfo.AspectRatio = 1.0f;
		cubeMapProjectionInfo.NearZ = 0.1f;
		cubeMapProjectionInfo.FarZ = 100.0f;

	float yawRotations[6] = { DX::XM_PIDIV2, -DX::XM_PIDIV2, 0.0f, 0.0f, 0.0f, DX::XM_PI };
	float pitchRotations[6] = { 0.0f, 0.0f, DX::XM_PIDIV2, -DX::XM_PIDIV2, 0.0f, 0.0f };
	DX::XMFLOAT4 meshPosition = {0.0f, -5.0f, -15.0f, 1.0f};

	D3D11_VIEWPORT cubeMapViewport;
		cubeMapViewport.TopLeftX = 0;
		cubeMapViewport.TopLeftY = 0;
		cubeMapViewport.Width = 2048;
		cubeMapViewport.Height = 2048;
		cubeMapViewport.MinDepth = 0;
		cubeMapViewport.MaxDepth = 1;
	
	std::vector<Camera> cubeMapCameras;
	for (int i = 0; i < 6; ++i)
	{
		cubeMapCameras.push_back(Camera(hr, this->_controller.GetDevice(), cubeMapProjectionInfo, meshPosition, 2048, 2048));
		cubeMapCameras.at(i).ApplyRotation(pitchRotations[i], yawRotations[i]);
	}

	enum TEXTURE_CUBE_FACE_INDEX
	{
		POSITIVE_X = 0,
		NEGATIVE_X = 1,
		POSITIVE_Y = 2,
		NEGATIVE_Y = 3,
		POSITIVE_Z = 4,
		NEGATIVE_Z = 5
	};

	std::vector<RenderBuffer> cubeMapGBuffers;

	for (int i = 0; i < 6; ++i)
	{
		cubeMapGBuffers.push_back(Setup::SetupGBuffer(this->_controller, 2048, 2048));
	}
	
	while (running)
	{
		this->_clock.Start();

		// Input
        this->_input.ReadInput(this->_scene, this->_window, running, this->_msg, this->_outputMode, deltaTime);
		
		DX::BoundingFrustum boundingFrustum = DX::BoundingFrustum(matrixCreator.CreateProjectionMatrix(this->_scene.GetCurrentCamera().GetProjectionInfo()));
		
		this->_scene.GetCurrentCamera().SetBoundingFrustum(boundingFrustum);

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
			this->_renderer.RenderDeferred(this->_controller, this->_swapChain,this->_windowRTV, this->_gBuffers, *this->_vsDeferredGeometry,
			                               *this->_hsDeferredGeometry, *this->_dsDeferredGeometry, this->_rasterizer, *this->_psDeferredGeometry,
			                               *this->_csDeferredLight, this->_inputLayout, this->_scene, this->_sampler, this->_outputMode, *this->_csDeferredParticle,
			                               *this->_vsDeferredParticle, *this->_gsDeferredParticle, *this->_psDeferredParticle, particleStructuredBuffer,
			                               spotLights, shadowMaps, spotLightStructuredBuffer, depthSampler, cubeMapViewport, cubeMapCameras, dynamicCubeMap, cubeMapGBuffers,
			                               *this->_psDeferredReflection, *this->_csDeferredReflection);
		}
		
		this->_swapChain.GetSwapChain()->Present(0, 0);
		
		this->_clock.End();
		deltaTime = this->_clock.GetDeltaTime();

		
	}
}



