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

#define NOMINMAX

#include <algorithm>
#include <assert.h>
#include <d3d11.h>
#include <wrl.h>
#include "ChiliException.h"
#include "Colors.h"

#include "Rect.h"
#include "Surface.h"
#include "Vec2.h"

class Graphics;

namespace PixelEffect
{
	class Effect
	{
	public:
		virtual void Process( Color sourceColour, int destX, int destY, Graphics& gfx ) const = 0;
	};
}

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

	void DrawLine( Vei2 p1, Vei2 p2, Color colour, std::unique_ptr<PixelEffect::Effect>& effect )
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
				effect->Process( colour, (int)x, y, *this );
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
				effect->Process( colour, x, (int)y, *this );
				y += m;
				x++;
			}
		}
	}

	void DrawBox( Vei2 topLeft, Vei2 bottomRight, Color colour, std::unique_ptr<PixelEffect::Effect>& effect )
	{
		DrawBox( RectI( topLeft, bottomRight ), colour, effect );
	}

	void DrawBox( const RectI& rect, Color colour, std::unique_ptr<PixelEffect::Effect>& effect )
	{
		DrawBox( rect, Graphics::GetScreenClippingRect(), colour, effect );
	}


	void DrawBox( const RectI& rect, const RectI& clippingRect, Color colour, std::unique_ptr<PixelEffect::Effect>& effect )
	{
		assert( rect.right >= rect.left );
		assert( rect.bottom >= rect.top );

		assert( clippingRect.left >= 0 );
		assert( clippingRect.right < Graphics::ScreenWidth );
		assert( clippingRect.top >= 0 );
		assert( clippingRect.bottom < Graphics::ScreenHeight );

		int left = std::max( rect.left, clippingRect.left );
		int right = std::min( rect.right, clippingRect.right );
		int top = std::max( rect.top, clippingRect.top );
		int bottom = std::min( rect.bottom, clippingRect.bottom );

		for ( int j = top; j <= bottom; j++ )
		{
			for ( int i = left; i <= right; i++ )
			{
				effect->Process( colour, i, j, *this );
			}
		}
	}

	void DrawBorderedBox( Vei2 topLeft, Vei2 bottomRight, Color boxColour, Color borderColour, std::unique_ptr<PixelEffect::Effect>& effect, const int thickness = 1 )
	{
		DrawBorderedBox( RectI( topLeft, bottomRight ), boxColour, borderColour, effect, thickness );
	}

	void DrawBorderedBox( const RectI& rect, Color boxColour, Color borderColour, std::unique_ptr<PixelEffect::Effect>& effect, const int thickness = 1 )
	{
		const RectI boxRect = rect.GetExpanded( -thickness );
		DrawBox( boxRect, boxColour, effect );
		DrawBoxBorder( rect, borderColour, effect, thickness );
	}

	void DrawBorderedBox( const RectI& boxRect, const RectI& borderRect, Color boxColour, Color borderColour, std::unique_ptr<PixelEffect::Effect>& effect )
	{
		DrawBox( boxRect, boxColour, effect );
		DrawBoxBorder( borderRect, boxRect, borderColour, effect );
	}

	void DrawBoxBorder( Vei2 topLeft, Vei2 bottomRight, Color colour, std::unique_ptr<PixelEffect::Effect>& effect, const int thickness = 1 )
	{
		const RectI borderRect( topLeft, bottomRight );
		DrawBoxBorder( borderRect, colour, effect );
	}

	void DrawBoxBorder( const RectI& borderRect, Color colour, std::unique_ptr<PixelEffect::Effect>& effect, const int thickness = 1 )
	{
		const RectI boxRect = borderRect.GetExpanded( -thickness );
		DrawBoxBorder( borderRect, boxRect, colour, effect );
	}

	void DrawBoxBorder( const RectI& borderRect, const RectI& boxRect, Color colour, std::unique_ptr<PixelEffect::Effect>& effect )
	{
		DrawBoxBorder( borderRect, boxRect, Graphics::GetScreenClippingRect(), colour, effect );
	}

	void DrawBoxBorder( const RectI& borderRect, const RectI& boxRect, const RectI& clippingRect, Color colour, std::unique_ptr<PixelEffect::Effect>& effect )
	{
		assert( borderRect.right >= borderRect.left );
		assert( borderRect.bottom >= borderRect.top );

		assert( boxRect.right >= boxRect.left );
		assert( boxRect.bottom >= boxRect.top );

		assert( borderRect.left <= boxRect.left );
		assert( borderRect.right >= boxRect.right );
		assert( borderRect.top <= boxRect.top );
		assert( borderRect.bottom >= boxRect.bottom );

		assert( clippingRect.left >= 0 );
		assert( clippingRect.right < Graphics::ScreenWidth );
		assert( clippingRect.top >= 0 );
		assert( clippingRect.bottom < Graphics::ScreenHeight );

		int left = std::max( borderRect.left, clippingRect.left );
		int right = std::min( borderRect.right, clippingRect.right );
		int top = std::max( borderRect.top, clippingRect.top );
		int bottom = std::min( borderRect.bottom, clippingRect.bottom );

		for ( int x = left; x <= right; x++ )
		{
			for ( int y = top; y <= bottom; y++ )
			{
				if ( y >= boxRect.top && y <= boxRect.bottom &&
					x >= boxRect.left && x <= boxRect.right )
				{
					y = boxRect.bottom;
					continue;
				}

				effect->Process( colour, x, y, *this );
			}
		}
	}

	void DrawSprite( const int x, const int y, const Surface& surface, std::unique_ptr<PixelEffect::Effect>& effect )
	{
		DrawSprite( x, y, surface.GetRect(), surface, effect );
	}

	void DrawSprite( RectI destinationRect, const Surface& surface, std::unique_ptr<PixelEffect::Effect>& effect )
	{
		DrawSprite( destinationRect, surface.GetRect(), surface, effect );
	}

	void DrawSprite( const int x, const int y, const RectI& sourceRect, const Surface& surface, std::unique_ptr<PixelEffect::Effect>& effect )
	{
		DrawSprite( x, y, sourceRect, GetScreenClippingRect(), surface, effect );
	}

	void DrawSprite( RectI destinationRect, const RectI& sourceRect, const Surface& surface, std::unique_ptr<PixelEffect::Effect>& effect )
	{
		DrawSprite( destinationRect, sourceRect, GetScreenClippingRect(), surface, effect );
	}

	void DrawSprite( int x, int y, RectI sourceRect, const RectI& clippingRect, const Surface& surface, std::unique_ptr<PixelEffect::Effect>& effect )
	{
		DrawSprite( RectI( x, x + sourceRect.GetWidth(), y, y + sourceRect.GetHeight() ), sourceRect, clippingRect, surface, effect );
	}

	void DrawSprite( RectI destinationRect, RectI sourceRect, const RectI& clippingRect, const Surface& surface, std::unique_ptr<PixelEffect::Effect>& effect )
	{
		assert( sourceRect.left >= 0 );
		assert( sourceRect.right >= sourceRect.left );
		assert( sourceRect.right <= surface.GetWidth() );
		assert( sourceRect.top >= 0 );
		assert( sourceRect.bottom >= sourceRect.top );
		assert( sourceRect.bottom <= surface.GetHeight() );

		assert( clippingRect.left >= 0 );
		assert( clippingRect.right < Graphics::ScreenWidth );
		assert( clippingRect.top >= 0 );
		assert( clippingRect.bottom < Graphics::ScreenHeight );

		if ( destinationRect.GetWidth() <= 0 ||
			destinationRect.GetHeight() <= 0 )
		{
			return;
		}

		const RectI originalDestination = destinationRect;
		const float scaleX = (float)destinationRect.GetWidth() / (float)sourceRect.GetWidth();
		const float scaleInvX = 1.0f / scaleX;
		const float scaleY = (float)destinationRect.GetHeight() / (float)sourceRect.GetHeight();
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

		for ( int dx = destinationRect.left; dx <= destinationRect.right; dx++ )
		{
			int sx = std::min( sourceRect.GetWidth() - 1, (int)std::floor( scaleInvX * (dx - originalDestination.left) ) ) + sourceRect.left;
			for ( int dy = destinationRect.top; dy <= destinationRect.bottom; dy++ )
			{
				int sy = std::min( sourceRect.GetHeight() - 1, (int)std::floor( scaleInvY * (dy - originalDestination.top)) ) + sourceRect.top;

				effect->Process( surface.GetPixel( sx, sy ), dx, dy, *this );
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
	static RectI GetScreenClippingRect();
};
