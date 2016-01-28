//  *******************************************************************************************************************
//  Texture   version:  1.0   Ankur Sheel  date:  2011/07/21
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#ifndef Texture_h__
#define Texture_h__

#include "Texture.hxx"

namespace Graphics
{
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /// Class Declaration for \c ITextureinterface
  ///
  ///
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  class cTexture : public ITexture, public Base::cNonCopyable
  {
  public:
    cTexture();
    ~cTexture();
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Loads and creates the texture from the resource cache
    ///
    /// @param[in] texturePath he path of the texture file
    /// @return True if the texture is created. False otherwise
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool Initialize(const Base::cString& texturePath);

  private:
    ID3D11ShaderResourceView* VGetTexture() const OVERRIDE { return m_pTexture; }
    const Base::cString& VGetTexturePath() const OVERRIDE { return m_TexturePath; }
    void Cleanup();

  private:
    ID3D11ShaderResourceView* m_pTexture;  /// The texture resource */
    Base::cString m_TexturePath;
  };
}  // namespace Graphics
#endif  // Texture_h__
