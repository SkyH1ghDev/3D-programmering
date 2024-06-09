#include "Mesh.hpp"

#include <iostream>

Mesh::Mesh(HRESULT &hr, ID3D11Device *device, const MeshData &meshData)
{
    VertexInfo vertexInfo = meshData.VertexInfo;
    
    this->_vertexBuffer = VertexBuffer(hr, device, vertexInfo.SizeOfVertex, vertexInfo.NrOfVerticesInBuffer, vertexInfo.VertexVector); // Might work, unsure

	if (FAILED(hr))
	{
		std::cerr << "Creating Mesh VertexBuffer failed \n";
	}

    std::vector<SubMeshInfo> subMeshInfoVector = meshData.SubMeshInfoList;

    for (SubMeshInfo& subMeshInfo : subMeshInfoVector)
    {
    	// AMBIENT
		ImageData ambientData = subMeshInfo.AmbientTextureData;
    	
		D3D11_TEXTURE2D_DESC ambientTexture2DDesc;
		ambientTexture2DDesc.Width = static_cast<UINT>(ambientData.Width);
		ambientTexture2DDesc.Height = static_cast<UINT>(ambientData.Height);
		ambientTexture2DDesc.MipLevels = 1;
		ambientTexture2DDesc.ArraySize = 1;
		ambientTexture2DDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		ambientTexture2DDesc.SampleDesc = DXGI_SAMPLE_DESC{1, 0};
		ambientTexture2DDesc.Usage = D3D11_USAGE_IMMUTABLE;
		ambientTexture2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		ambientTexture2DDesc.CPUAccessFlags = 0;
		ambientTexture2DDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA ambientTexture2DData;
		ambientTexture2DData.pSysMem = ambientData.TextureData.data(); 
		ambientTexture2DData.SysMemPitch = static_cast<UINT>(ambientData.Width) * ambientData.RGBAChannels;
		ambientTexture2DData.SysMemSlicePitch = 0;

    	ID3D11Texture2D* ambientTexture;
    	hr = device->CreateTexture2D(&ambientTexture2DDesc, &ambientTexture2DData, &ambientTexture);

		if (FAILED(hr))
		{
			std::cerr << "Failed To Create Ambient Texture2D \n";
		}
    	
    	ID3D11ShaderResourceView* ambientSRV;
    	device->CreateShaderResourceView(ambientTexture, nullptr, &ambientSRV);

    	// DIFFUSE
		ImageData diffuseData = subMeshInfo.DiffuseTextureData;
            	
		D3D11_TEXTURE2D_DESC diffuseTexture2DDesc;
		diffuseTexture2DDesc.Width = static_cast<UINT>(diffuseData.Width);
		diffuseTexture2DDesc.Height = static_cast<UINT>(diffuseData.Height);
		diffuseTexture2DDesc.MipLevels = 1;
		diffuseTexture2DDesc.ArraySize = 1;
		diffuseTexture2DDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		diffuseTexture2DDesc.SampleDesc = DXGI_SAMPLE_DESC{1, 0};
		diffuseTexture2DDesc.Usage = D3D11_USAGE_IMMUTABLE;
		diffuseTexture2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		diffuseTexture2DDesc.CPUAccessFlags = 0;
		diffuseTexture2DDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA diffuseTexture2DData;
		diffuseTexture2DData.pSysMem = diffuseData.TextureData.data(); 
		diffuseTexture2DData.SysMemPitch = static_cast<UINT>(diffuseData.Width) * diffuseData.RGBAChannels;
		diffuseTexture2DData.SysMemSlicePitch = 0;

    	ID3D11Texture2D* diffuseTexture;
    	hr = device->CreateTexture2D(&diffuseTexture2DDesc, &diffuseTexture2DData, &diffuseTexture);

		if (FAILED(hr))
		{
			std::cerr << "Failed To Create Diffuse Texture2D \n";
		}
    	
    	ID3D11ShaderResourceView* diffuseSRV;
    	device->CreateShaderResourceView(diffuseTexture, nullptr, &diffuseSRV);
    	
		// SPECULAR 
		ImageData specularData = subMeshInfo.SpecularTextureData;
				
		D3D11_TEXTURE2D_DESC specularTexture2DDesc;
		specularTexture2DDesc.Width = static_cast<UINT>(specularData.Width);
		specularTexture2DDesc.Height = static_cast<UINT>(specularData.Height);
		specularTexture2DDesc.MipLevels = 1;
		specularTexture2DDesc.ArraySize = 1;
		specularTexture2DDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		specularTexture2DDesc.SampleDesc = DXGI_SAMPLE_DESC{1, 0};
		specularTexture2DDesc.Usage = D3D11_USAGE_IMMUTABLE;
		specularTexture2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		specularTexture2DDesc.CPUAccessFlags = 0;
		specularTexture2DDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA specularTexture2DData;
		specularTexture2DData.pSysMem = specularData.TextureData.data(); 
		specularTexture2DData.SysMemPitch = static_cast<UINT>(specularData.Width) * specularData.RGBAChannels;
		specularTexture2DData.SysMemSlicePitch = 0;

		ID3D11Texture2D* specularTexture;
		hr = device->CreateTexture2D(&specularTexture2DDesc, &specularTexture2DData, &specularTexture);

		if (FAILED(hr))
		{
			std::cerr << "Failed To Create Diffuse Texture2D \n";
		}
            	
		ID3D11ShaderResourceView* specularSRV;
		device->CreateShaderResourceView(specularTexture, nullptr, &specularSRV);

    	this->_subMeshes.push_back(SubMesh(subMeshInfo.startIndexValue, subMeshInfo.nrOfIndicesInSubMesh, subMeshInfo.specularExponent,
    										   ambientSRV, ambientTexture, diffuseSRV,
    										   diffuseTexture, specularSRV, specularTexture));
	}
}
