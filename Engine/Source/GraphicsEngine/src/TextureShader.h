//  *******************************************************************************************************************
//  TextureShader   version:  1.0   Ankur Sheel  date: 2012/09/24
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#ifndef TextureShader_h__
#define TextureShader_h__

#include "BaseShader.h"

namespace Utilities
{
  class IResHandle;
}

namespace Graphics
{
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /// Class use to invoke HLSL shaders for drawing 3D models with position and texture vertex data
  ///
  ///
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  class cTextureShader : public cBaseShader
  {
  public:
    struct stPixelBufferData
    {
      XMFLOAT4 pixelColor;  /// The color of the pixel that will be used to draw the font text. */
      int hasTexture;  /// True if rendering a texture. This should be int because HLSL packs things into 4 bytes. */
    };

    cTextureShader();
    ~cTextureShader();
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Sets the diffuse color
    ///
    /// @param[in] colorDiffuse The diffuse color for the shader
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void SetDiffuseColor(const Base::cColor& colorDiffuse);

  protected:
    virtual bool VInitialize(const Base::cString& strShaderName);
    bool VCreateLayout(shared_ptr<Utilities::IResHandle const> shaderHandle);
    virtual bool VSetShaderParameters(const XMFLOAT4X4& inMatWorld, const XMFLOAT4X4& inMatView,
                                      const XMFLOAT4X4& inMatProjection);
    virtual void VRenderShader();
    virtual void VCleanup();
    bool CreatePixelBuffer();
    bool CreateSampleState();

  protected:
    ID3D11Buffer* m_pPixelBuffer;  /// The shaders pixel constant buffer */
    XMFLOAT4 m_DiffuseColor;       /// The diffuse color */

  private:
    ID3D11SamplerState* m_pSampleState;  /// The sampler state pointer is used to interface with the texture shader.
                                         /// Itallows us to modify how the pixels are written to the polygon face when
                                         /// shaded. */
  };
}  // namespace Graphics
#endif  // TextureShader_h__
