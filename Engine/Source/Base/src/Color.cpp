//  *******************************************************************************************************************
//  Color   version:  1.0   Ankur Sheel  date: 2012/09/03
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#include "stdafx.h"
#include "Color.h"

using namespace Base;

const cColor cColor::BLACK = cColor(0, 0, 0, 255);
const cColor cColor::WHITE = cColor(255, 255, 255, 255);
const cColor cColor::RED = cColor(255, 0, 0, 255);
const cColor cColor::BLUE = cColor(0, 0, 255, 255);
const cColor cColor::GREEN = cColor(0, 255, 0, 255);
const cColor cColor::GRAY = cColor(190, 190, 190, 255);
const cColor cColor::TURQUOISE = cColor(64, 224, 208, 255);
const cColor cColor::YELLOW = cColor(255, 255, 0, 255);
const cColor cColor::TAN = cColor(210, 180, 140, 255);
const cColor cColor::ORANGE = cColor(255, 165, 0, 255);
const cColor cColor::VIOLET = cColor(238, 130, 238, 255);

//  *******************************************************************************************************************
cColor::cColor()
  : m_Red(0)
  , m_Green(0)
  , m_Blue(0)
  , m_Alpha(0)
{
}

//  *******************************************************************************************************************
cColor::cColor(int red, int green, int blue, int alpha)
{
  m_Red = Clamp<int>(red, 0, 255);
  m_Green = Clamp<int>(green, 0, 255);
  m_Blue = Clamp<int>(blue, 0, 255);
  m_Alpha = Clamp<int>(alpha, 0, 255);
}

//  *******************************************************************************************************************
cColor::cColor(float red, float green, float blue, float alpha)
  : m_Red(static_cast<uint8>(RoundReal32ToInt32(red * 255)))
  , m_Green(static_cast<uint8>(RoundReal32ToInt32(green * 255)))
  , m_Blue(static_cast<uint8>(RoundReal32ToInt32(blue * 255)))
  , m_Alpha(static_cast<uint8>(RoundReal32ToInt32(alpha * 255)))
{
  Clamp<uint8>(m_Red, 0, 255);
  Clamp<uint8>(m_Blue, 0, 255);
  Clamp<uint8>(m_Green, 0, 255);
  Clamp<uint8>(m_Alpha, 0, 255);
}

//  *******************************************************************************************************************
cColor::~cColor()
{
}

//  *******************************************************************************************************************
void cColor::GetColorComponentsInFloat(float& red, float& blue, float& green, float& alpha) const
{
  red = static_cast<float>(m_Red) / 255.0f;
  blue = static_cast<float>(m_Green) / 255.0f;
  green = static_cast<float>(m_Blue) / 255.0f;
  alpha = static_cast<float>(m_Alpha) / 255.0f;
}

//  *******************************************************************************************************************
uint32 Base::cColor::GetAsARGB() const
{
  return (m_Alpha << 24 | m_Red << 16 | m_Green << 8 | m_Blue << 0);
}
