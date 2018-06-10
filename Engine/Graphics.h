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

	template <typename E>
	void DrawLine( Vei2 p1, Vei2 p2, Color colour, E effect )
	{
		if ( p1.x > p2.x )
		{
			std::swap( p1, p2 );
		}

		float m = std::numeric_limits<float>::max();
		if ( p1.x != p2.x )
		{
			m = ((float)(p2.y - p1.y)) / (float)(p2.x - p1.x);
		}

		if ( abs( m ) > 1.0f )
		{
			if ( p1.y > p2.y )
			{
				std::swap( p1, p2 );
			}

			m = 0.0f;
			if ( p2.y != p1.y )
			{
				m = ((float)(p2.x - p1.x)) / (float)(p2.y - p1.y);
			}

			float c = p1.x - m * p1.y;

			int startY = std::max( p1.y, 0 );
			int endY = std::min( p2.y, Graphics::ScreenHeight - 1 );

			int y = startY;
			float x = m * startY + c;
			while ( (int)x >= 0 && (int)x < Graphics::ScreenWidth && y <= endY )
			{
				effect( colour, (int)x, y, *this );
				x += m;
				y++;
			}
		}
		else
		{
			float c = p1.y - m * p1.x;

			int startX = std::max( p1.x, 0 );
			int endX = std::min( p2.x, Graphics::ScreenWidth - 1 );

			int x = startX;
			float y = m * startX + c;
			while ( (int)y >= 0 && (int)y < Graphics::ScreenHeight && x <= endX )
			{
				effect( colour, x, (int)y, *this );
				y += m;
				x++;
			}
		}
	}

	template <typename E>
	void DrawBox( Vei2 topLeft, Vei2 bottomRight, Color colour, E effect )
	{
		DrawBox( RectI( topLeft, bottomRight ), colour, effect );
	}

	template <typename E>
	void DrawBox( const RectI& rect, Color colour, E effect )
	{
		assert( rect.right >= rect.left );
		assert( rect.bottom >= rect.top );

		int left = std::max( rect.left, 0 );
		int right = std::min( rect.right, ScreenWidth - 1 );
		int top = std::max( rect.top, 0 );
		int bottom = std::min( rect.bottom, ScreenHeight - 1 );

		for ( int j = top; j <= bottom; j++ )
		{
			for ( int i = left; i <= right; i++ )
			{
				effect( colour, i, j, *this );
			}
		}
	}

	template <typename E>
	void DrawBoxBorder( Vei2 topLeft, Vei2 bottomRight, Color colour, E effect, const int thickness = 1 )
	{
		DrawBoxBorder( RectI( topLeft, bottomRight ), colour, effect, thickness );
	}

	template <typename E>
	void DrawBoxBorder( const RectI& rect, Color colour, E effect, const int thickness = 1 )
	{
		assert( rect.right >= rect.left );
		assert( rect.bottom >= rect.top );

		for ( int i = 0; i < thickness; i++ )
		{
			Vei2 topLeft( rect.left + i, rect.top + i );
			Vei2 topRight( rect.right - i, rect.top + i );
			Vei2 bottomLeft( rect.left + i, rect.bottom - i );
			Vei2 bottomRight( rect.right - i, rect.bottom - i );

			if ( topLeft.x > bottomRight.x )
			{
				break;
			}

			DrawLine( topLeft, topRight, colour, effect );

			if ( topLeft.y > bottomRight.y )
			{
				break;
			}

			DrawLine( bottomLeft, bottomRight, colour, effect );

			if ( topLeft.y + 1 > bottomRight.y - 1 )
			{
				break;
			}

			DrawLine( topLeft + Vei2( 0, 1 ), bottomLeft + Vei2( 0, -1 ), colour, effect );
			DrawLine( topRight + Vei2( 0, 1 ), bottomRight + Vei2( 0, -1 ), colour, effect );
		}
	}

	template<typename E>
	void DrawSprite( const int x, const int y, const Surface& surface, E effect )
	{
		DrawSprite( x, y, surface.GetRect(), surface, effect );
	}

	template<typename E>
	void DrawSprite( RectI destinationRect, const Surface& surface, E effect )
	{
		DrawSprite( destinationRect, surface.GetRect(), surface, effect );
	}

	template<typename E>
	void DrawSprite( const int x, const int y, const RectI& sourceRect, const Surface& surface, E effect )
	{
		DrawSprite( x, y, sourceRect, GetScreenRect(), surface, effect );
	}

	template<typename E>
	void DrawSprite( RectI destinationRect, const RectI& sourceRect, const Surface& surface, E effect )
	{
		DrawSprite( destinationRect, sourceRect, GetScreenRect(), surface, effect );
	}

	template<typename E>
	void DrawSprite( int x, int y, RectI sourceRect, const RectI& clippingRect, const Surface& surface, E effect )
	{
		DrawSprite( RectI( x, x + sourceRect.GetWidth(), y, y + sourceRect.GetHeight() ), sourceRect, clippingRect, surface, effect );
	}

	template<typename E>
	void DrawSprite( RectI destinationRect, RectI sourceRect, const RectI& clippingRect, const Surface& surface, E effect )
	{
		assert( sourceRect.left >= 0 );
		assert( sourceRect.right <= surface.GetWidth() );
		assert( sourceRect.top >= 0 );
		assert( sourceRect.bottom <= surface.GetHeight() );

		const RectI originalDestination = destinationRect;
		const float scaleX = (float)destinationRect.GetWidth() / (float)surface.GetWidth();
		const float scaleInvX = 1.0f / scaleX;
		const float scaleY = (float)destinationRect.GetHeight() / (float)surface.GetHeight();
		const float scaleInvY = 1.0f / scaleY;

		if ( destinationRect.left < clippingRect.left )
		{
			destinationRect.left = clippingRect.left;
		}

		if ( destinationRect.right > clippingRect.right )
		{
			destinationRect.right = clippingRect.right;
		}

		if ( destinationRect.top < clippingRect.top )
		{
			destinationRect.top = clippingRect.top;
		}

		if ( destinationRect.bottom > clippingRect.bottom )
		{
			destinationRect.bottom = clippingRect.bottom;
		}

		for ( int dx = destinationRect.left; dx < destinationRect.right; dx++ )
		{
			int sx = (int)std::floor( scaleInvX * (dx - originalDestination.left) );
			for ( int dy = destinationRect.top; dy < destinationRect.bottom; dy++ )
			{
				int sy = (int)std::floor( scaleInvY * (dy - originalDestination.top) );

				effect( surface.GetPixel( sx, sy ), destinationRect.left + dx - destinationRect.left, destinationRect.top + dy - destinationRect.top, *this );
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
