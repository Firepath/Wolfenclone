#pragma once

class Surface;

class MapFixture
{
public:
	~MapFixture() = default;

	const Surface* GetTexture() const;
	void SetTexture( const Surface* const texture );

private:
	const Surface* Texture = nullptr;
};