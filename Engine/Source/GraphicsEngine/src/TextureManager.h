//  *******************************************************************************************************************
//  TextureManager   version:  1.0   Ankur Sheel  date: 2012/10/28
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#ifndef TEXTUREMANAGER_H__
#define TEXTUREMANAGER_H__

#include "TextureManager.hxx"

namespace Graphics
{
	class ITexture;
}

namespace Graphics
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Class Declaration for \c ITextureManager interface
	///
	///
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class cTextureManager
		: public ITextureManager
	{
		/// Map of textures. The key is the texture path. Value is a shared_ptr to the actual texture */
		typedef std::map<UINT64, shared_ptr <ITexture> > TextureMap;

	private:
		cTextureManager();
		~cTextureManager();
		shared_ptr<ITexture> VGetTexture(const Base::cString & strTexturePath);
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// Checks if the texture has already been loaded. Returns a pointer to the texture if it is found. NULL otherwise.
		///
		/// @param[in] ulTextureHash The hash of the name of the texture filename
		/// @return Pointer to the texture if it is found in the texture list. NULL otherwise
		///
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		shared_ptr<ITexture> Find(const UINT64 ulTextureHash);

	private:
		TextureMap m_pTextures;  /// map of the textures that have already been loaded */
		static ITextureManager * s_pTexturemanager;  /// static object of this class */

	private:
		friend ITextureManager * ITextureManager::GetInstance();
		friend void ITextureManager::Destroy();
	};
}  // namespace Graphics
#endif  // TEXTUREMANAGER_H__
