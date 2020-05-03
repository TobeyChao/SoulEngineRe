#include "TGATextureLoader.h"
#include <fstream>
#include <sstream>
namespace Soul
{
	struct TargaHeader
	{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};

	bool LoadTarga(const wchar_t * filename, unsigned char** targaData, int& height, int& width)
	{
		int error, bpp, imageSize, index, i, j, k;
		FILE* filePtr;
		unsigned int count;
		TargaHeader targaFileHeader;
		unsigned char* targaImage;


		// Open the targa file for reading in binary.
		error = _wfopen_s(&filePtr, filename, L"rb");

		if (error != 0)
		{
			return false;
		}

		// Read in the file header.
		count = (unsigned int)fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);
		if (count != 1)
		{
			return false;
		}

		// Get the important information from the header.
		height = (int)targaFileHeader.height;
		width = (int)targaFileHeader.width;
		bpp = (int)targaFileHeader.bpp;

		// Check that it is 32 bit and not 24 bit.
		if (bpp != 32)
		{
			return false;
		}

		// Calculate the size of the 32 bit image data.
		imageSize = width * height * 4;

		// Allocate memory for the targa image data.
		targaImage = new unsigned char[imageSize];
		if (!targaImage)
		{
			return false;
		}

		// Read in the targa image data.
		count = (unsigned int)fread(targaImage, 1, imageSize, filePtr);
		if (count != imageSize)
		{
			return false;
		}

		// Close the file.
		error = fclose(filePtr);
		if (error != 0)
		{
			return false;
		}

		// Allocate memory for the targa destination data.
		*targaData = new unsigned char[imageSize];
		if (!*targaData)
		{
			return false;
		}

		// Initialize the index into the targa destination data array.
		index = 0;

		// Initialize the index into the targa image data.
		k = (width * height * 4) - (width * 4);

		// Now copy the targa image data into the targa destination array in the correct order since the targa format is stored upside down.
		std::ofstream out(L"image.txt");
		std::ostringstream buffer;
		for (j = 0; j < height; j++)
		{
			for (i = 0; i < width; i++)
			{
				(*targaData)[index + 0] = targaImage[k + 2];  // Red.
				(*targaData)[index + 1] = targaImage[k + 1];  // Green.
				(*targaData)[index + 2] = targaImage[k + 0];  // Blue
				(*targaData)[index + 3] = targaImage[k + 3];  // Alpha
				
				buffer << "Pixel" << targaImage[k + 2] - '0' << " " << targaImage[k + 1] - '0'
					<< " " << targaImage[k + 0] - '0' << " " << targaImage[k + 3] - '0' << std::endl;
															 // Increment the indexes into the targa data.
			
				k += 4;
				index += 4;
			}
			// Set the targa image data index back to the preceding row at the beginning of the column since its reading it in upside down.
			k -= (width * 8);
		}
		out << buffer.str();
		// Release the targa image data now that it was copied into the destination array.
		delete[] targaImage;
		targaImage = 0;

		return true;
	}

	HRESULT Soul::CreateTGATextureFromFileEX(ID3D11Device * d3dDevice, ID3D11DeviceContext * deviceContext,
		const wchar_t * szFileName, ID3D11Resource** tex, ID3D11ShaderResourceView ** textureView, size_t maxsize)
	{
		unsigned char* targaData;
		ID3D11Texture2D* texture;

		int height, width;
		D3D11_TEXTURE2D_DESC textureDesc;
		HRESULT hResult;
		unsigned int rowPitch;

		// Load the targa image data into memory.
		hResult = LoadTarga(szFileName, &targaData, height, width);
		if (FAILED(hResult))
		{
			return S_FALSE;
		}

		// Setup the description of the texture.
		textureDesc.Height = height;
		textureDesc.Width = width;
		textureDesc.MipLevels = 0;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		// Create the empty texture.
		hResult = d3dDevice->CreateTexture2D(&textureDesc, NULL, &texture);
		if (FAILED(hResult))
		{
			return S_FALSE;
		}

		// Set the row pitch of the targa image data.
		rowPitch = (width * 4) * sizeof(unsigned char);

		// Copy the targa image data into the texture.
		deviceContext->UpdateSubresource(texture, 0, NULL, targaData, rowPitch, 0);


		// Setup the shader resource view description.
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = -1;

		// Create the shader resource view for the texture.
		hResult = d3dDevice->CreateShaderResourceView(texture, &srvDesc, textureView);
		if (FAILED(hResult))
		{
			return S_FALSE;
		}

		// Generate mipmaps for this texture.
		deviceContext->GenerateMips(*textureView);

		// Release the targa image data now that the image data has been loaded into the texture.
		delete[] targaData;
		targaData = 0;
		if (tex)
		{
			*tex = texture;
		}
		else
		{
			texture->Release();
		}

		return S_OK;
	}
}