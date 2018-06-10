/******************************************************************************************
*	Chili DirectX Framework Version 16.07.20											  *
*	Graphics.h																			  *
*	Copyright 2016 PlanetChili <http://www.planetchili.net>								  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#pragma once
#include <d3d11.h>
#include <wrl.h>
#include "ChiliException.h"
#include "Colors.h"

#include "Rect.h"
#include "Surface.h"
#include "Vec2.h"

class Graphics
{
public:
	class Exception : public ChiliException
	{
	public:
		Exception( HRESULT hr,const std::wstring& note,const wchar_t* file,unsigned int line );
		std::wstring GetErrorName() const;
		std::wstring GetErrorDescription() const;
		virtual std::wstring GetFullMessage() const override;
		virtual std::wstring GetExceptionType() const override;
	private:
		HRESULT hr;
	};

	class FileException : public ChiliException
	{
	public:
		FileException( const std::wstring& note, const std::exception& exception, const wchar_t* file, unsigned int line );
		std::wstring GetErrorName() const;
		std::wstring GetErrorDescription() const;
		virtual std::wstring GetFullMessage() const override;
		virtual std::wstring GetExceptionType() const override;
	private:
		const std::exception ex;
	}; 
private:
	// vertex format for the framebuffer fullscreen textured quad
	struct FSQVertex
	{
		float x,y,z;		// position
		float u,v;			// texcoords
	};
public:
	Graphics( class HWNDKey& key );
	Graphics( const Graphics& ) = delete;
	Graphics& operator=( const Graphics& ) = delete;
	void EndFrame();
	void BeginFrame();
	Color GetPixel( int x, int y ) const;
	void PutPixel( int x,int y,int r,int g,int b )
	{
		PutPixel( x,y,{ unsigned char( r ),unsigned char( g ),unsigned char( b ) } );
	}
	void PutPixel( int x,int y,Color c );
	void DrawLine( Vei2 p1, Vei2 p2, Color colour );

	template <typename E>
	void DrawBox( Vei2 topLeft, Vei2 bottomRight, Color colour, E effect )
	{
		assert( bottomRight.x >= topLeft.x );
		assert( bottomRight.y >= topLeft.y );

		int left = std::max( topLeft.x, 0 );
		int right = std::min( bottomRight.x, ScreenWidth - 1 );
		int top = std::max( topLeft.y, 0 );
		int bottom = std::min( bottomRight.y, ScreenHeight - 1 );

		for ( int j = top; j <= bottom; j++ )
		{
			for ( int i = left; i <= right; i++ )
			{
				effect( colour, i, j, *this );
				//PutPixel( i, j, colour );
			}
		}
	}

	void DrawBoxBorder( Vei2 topLeft, Vei2 bottomRight, Color colour );

	template<typename E>
	void DrawSprite( const int x, const int y, const Surface& surface, E effect )
	{
		DrawSprite( x, y, surface.GetRect(), surface, effect );
	}

	template<typename E>
	void DrawSprite( const int x, const int y, const RectI& sourceRect, const Surface& surface, E effect )
	{
		DrawSprite( x, y, sourceRect, GetScreenRect(), surface, effect );
	}

	template<typename E>
	void DrawSprite( int x, int y, RectI sourceRect, const RectI& clippingRect, const Surface& surface, E effect )
	{
		assert( sourceRect.left >= 0 );
		assert( sourceRect.right <= surface.GetWidth() );
		assert( sourceRect.top >= 0 );
		assert( sourceRect.bottom <= surface.GetHeight() );

		if ( x < clippingRect.left )
		{
			sourceRect.left += clippingRect.left - x;
			x = clippingRect.left;
		}

		if ( y < clippingRect.top )
		{
			sourceRect.top += clippingRect.top - y;
			y = clippingRect.top;
		}

		if ( x + sourceRect.GetWidth() > clippingRect.right )
		{
			sourceRect.right -= x + sourceRect.GetWidth() - clippingRect.right;
		}

		if ( y + sourceRect.GetHeight() > clippingRect.bottom )
		{
			sourceRect.bottom -= y + sourceRect.GetHeight() - clippingRect.bottom;
		}

		for ( int sx = sourceRect.left; sx < sourceRect.right; sx++ )
		{
			for ( int sy = sourceRect.top; sy < sourceRect.bottom; sy++ )
			{
				effect( surface.GetPixel( sx, sy ), x + sx - sourceRect.left, y + sy - sourceRect.top, *this );
			}
		}
	}

	~Graphics();
private:
	Microsoft::WRL::ComPtr<IDXGISwapChain>				pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11Device>				pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>			pImmediateContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		pRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				pSysBufferTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	pSysBufferTextureView;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pPixelShader;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>			pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer>				pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			pInputLayout;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>			pSamplerState;
	D3D11_MAPPED_SUBRESOURCE							mappedSysBufferTexture;
	Color*                                              pSysBuffer = nullptr;
public:
	static constexpr int ScreenWidth = 1440;
	static constexpr int ScreenHeight = 800;
	static RectI GetScreenRect();
};
