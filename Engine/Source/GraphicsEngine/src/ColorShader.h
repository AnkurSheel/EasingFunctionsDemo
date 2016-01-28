//  *******************************************************************************************************************
//  ColorShader   version:  1.0   Ankur Sheel  date: 2012/09/15
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#ifndef COLORSHADER_H__
#define COLORSHADER_H__

#include "BaseShader.h"

namespace Utilities
{
  class IResHandle;
}

namespace Graphics
{
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /// Class use to invoke HLSL shaders for drawing 3D models with position and color vertex data
  ///
  ///
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  class cColorShader : public cBaseShader
  {
  public:
    cColorShader();
    ~cColorShader();

  private:
    bool VCreateLayout(shared_ptr<Utilities::IResHandle const> shaderHandle);
  };
}  // namespace Graphics
#endif  // COLORSHADER_H__
