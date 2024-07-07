#include "Application.hpp"

#include <iostream>

#include "MatrixCreator.hpp"

Application::Application(HINSTANCE hInstance, int nCmdShow) :
    // Initialize Window
    _window(Setup::SetupWindow(hInstance, nCmdShow)),

    // Initialize D3D11
    _controller(Setup::SetupController(this->_window)),
    _windowRTV(Setup::SetupDepthStencilRTV(this->_controller)),
    _scene(Setup::SetupScene(this->_controller)),

    // Initialize Shaders
    _vertexShaderPtr(std::unique_ptr<VertexShader>
    			 (dynamic_cast<VertexShader*>
    			 (Setup::SetupShader(this->_controller, ShaderType::VERTEX_SHADER, L"Src/Shaders/VertexShader.hlsl")))),

    //_hullShader();
    //_domainShader(),
    //_geometryShader(),
    _pixelShaderPtr(std::unique_ptr<PixelShader>
    			(dynamic_cast<PixelShader*>
    			(Setup::SetupShader(this->_controller, ShaderType::PIXEL_SHADER, L"Src/Shaders/PixelShader.hlsl")))),

    //_computeShader()

    // Initialize InputLayout and Sampler
    _inputLayout(Setup::SetupInputLayout(this->_controller, *this->_vertexShaderPtr)),
    _sampler(Setup::SetupSampler(this->_controller)),

	// Initalize ConstantBuffers
	_worldMatrixConstantBuffer(Setup::CreateWorldMatrixConstantBuffer(this->_controller)),
	_pixelShaderConstantBuffer(Setup::CreatePixelShaderConstantBuffer(this->_controller, this->_scene))
{
    // Window
    assert(&this->_window != nullptr);

    // Controller
    assert(this->_controller.GetContext() != nullptr);
    assert(this->_controller.GetDevice() != nullptr);
    assert(this->_controller.GetSwapChain() != nullptr);

    D3D11_VIEWPORT vp = this->_controller.GetViewPort();
    assert(&vp != nullptr);

    // RTV
    assert(this->_windowRTV.GetDSV() != nullptr);
    assert(this->_windowRTV.GetRTV() != nullptr);

    //Scene
    /*
     * TODO: Implement asserts for scene, mesh, submesh and camera
    */

	// Vertex
	assert(sizeof(Vertex) % 16 == 0);
    
    // Shaders
	assert(this->_vertexShaderPtr != nullptr);
	assert(this->_pixelShaderPtr != nullptr);
    assert(this->_vertexShaderPtr->GetShaderBlob() != nullptr);
    assert(this->_pixelShaderPtr->GetShaderBlob() != nullptr);

    // Input layout + Sampler
    assert(this->_inputLayout.GetInputLayout() != nullptr);
    assert(this->_sampler.GetSamplerState() != nullptr);

	// Constant Buffers
	assert(this->_worldMatrixConstantBuffer.GetBuffer() != nullptr);
	assert(this->_pixelShaderConstantBuffer.GetBuffer() != nullptr);
}

int Application::Run()
{
    Setup(); 
    Render();

    return 0;
}

void Application::Setup()
{
	ID3D11Buffer* worldMatrixBuffer = this->_worldMatrixConstantBuffer.GetBuffer();
	ID3D11Buffer* viewProjectionBuffer = this->_scene.GetCurrentCamera().GetConstantBuffer();
	
	this->_controller.GetContext()->VSSetConstantBuffers(0, 1, &worldMatrixBuffer);
	this->_controller.GetContext()->VSSetConstantBuffers(1, 1, &viewProjectionBuffer);

	ID3D11Buffer* pixelShaderConstBuffer = this->_pixelShaderConstantBuffer.GetBuffer();
	this->_controller.GetContext()->PSSetConstantBuffers(0, 1, &pixelShaderConstBuffer);
}

void Application::Render()
{
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
		
		this->_renderer.Render(this->_controller, this->_windowRTV, *this->_vertexShaderPtr, *this->_pixelShaderPtr, this->_inputLayout, this->_scene, this->_sampler);
		this->_controller.GetSwapChain()->Present(0, 0);
		
		this->_scene.GetCurrentCamera().UpdateInternalConstantBuffer(this->_controller.GetContext());
			
		DX::XMFLOAT4X4 worldMatrixFloat4x4 = matrixCreator.CreateWorldXMFLOAT4X4(currentAngle);
		this->_worldMatrixConstantBuffer.UpdateBuffer(this->_controller.GetContext(), &worldMatrixFloat4x4, sizeof(worldMatrixFloat4x4));

		this->_clock.End();
		float deltaTime = this->_clock.GetDeltaTime();
 
		this->_input.ReadInput(this->_scene.GetCurrentCamera(), deltaTime);
		currentAngle -= rotationalSpeed * deltaTime;
	}
}



