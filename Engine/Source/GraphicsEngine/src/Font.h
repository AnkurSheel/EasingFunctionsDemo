//  *******************************************************************************************************************
//  Font   version:  1.0   Ankur Sheel  date: 05/16/2008
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#ifndef Font_h__
#define Font_h__

#include "Font.hxx"

namespace Base
{
	class cString;
}

namespace Graphics
{
	class ITexture;
	class cFontShader;
}

namespace Graphics
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /// Class Declaration for \c IMyFontinterface
	///
	///
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class cMyFont
		: public IMyFont
		, Base::cNonCopyable
	{
	public:
		cMyFont();

	private:
		~cMyFont();
		bool VInitialize(const Base::cString & strFontDescFilename);
		void VRender(const XMFLOAT4X4 & inMatWorld, const XMFLOAT4X4 & inMatView,
			const XMFLOAT4X4 & inMatProjection, const Base::cColor & textColor);
		stVertexData VGetCharVertexData(const int iCharAsciiValue);
		int VGetFontHeight() const;
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	  /// Parses and stores the character definitions from the font description file name
		///
	  /// @param[in] strFontDescFilename The file name of the font description file
		///
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void ParseFontDesc(const Base::cString & strFontDescFilename);
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	  /// Creates and Initializes the font shader for use
		///
		///
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool InitializeShader();
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	  /// Releases and destroys all the resources
		///
		///
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void Cleanup();

	private:
	  /// Map with key as ascii value of char. Value is the char descriptor for the character denoted by this ascii value
		typedef std::map<int, const CharDescriptor> CharDescriptorMap;

		CharDescriptorMap m_CharDescriptorMap;  /// The map contaning all the character descriptions with their ascii value as the key
		shared_ptr<cFontShader> m_pShader;  /// The shader responsible for rendering the font.
		shared_ptr<ITexture> m_pTexture;  /// The Texture of the sprite
		Base::cString m_strFontTexPath;  /// The path of the font texture file
		int m_iTextureWidth;  /// The width of the texture file
		int m_iTextureHeight;  /// The height of the texture file
		int m_iFontHeight;  /// The height of each line of text
	};
}  // namespace Graphics
#endif  // Font_h__
