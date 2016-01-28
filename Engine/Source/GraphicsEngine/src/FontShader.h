//  *******************************************************************************************************************
//  FontShader   version:  1.0   Ankur Sheel  date: 2012/10/03
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#ifndef FONTSHADER_H__
#define FONTSHADER_H__

#include "TextureShader.h"

namespace Graphics
{
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /// Class use to invoke HLSL shaders for displaying text on screen
  ///
  ///
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  class cFontShader : public cTextureShader
  {
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constant buffer to be passed to the shader.
    ///
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    struct PixelBufferType
    {
      XMFLOAT4 pixelColor;  /// The color of the pixel that will be used to draw the font text. */
    };

  public:
    cFontShader();
    ~cFontShader();

  private:
    bool VSetShaderParameters(const XMFLOAT4X4& inMatWorld, const XMFLOAT4X4& inMatView,
                              const XMFLOAT4X4& inMatProjection);
  };
}  // namespace Graphics
#endif  // FONTSHADER_H__
