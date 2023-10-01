#pragma once

namespace box
{
	class IAsset
	{
	public:
		virtual ~IAsset() = default;
	};

	class IAssetProvider
	{
	public:
		virtual ~IAssetProvider() = default;
	};
}