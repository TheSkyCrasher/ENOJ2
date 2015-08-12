//============================================================================//
//                                                                            //
// ozz-animation, 3d skeletal animation libraries and tools.                  //
// https://code.google.com/p/ozz-animation/                                   //
//                                                                            //
//----------------------------------------------------------------------------//
//                                                                            //
// Copyright (c) 2012-2015 Guillaume Blanc                                    //
//                                                                            //
// This software is provided 'as-is', without any express or implied          //
// warranty. In no event will the authors be held liable for any damages      //
// arising from the use of this software.                                     //
//                                                                            //
// Permission is granted to anyone to use this software for any purpose,      //
// including commercial applications, and to alter it and redistribute it     //
// freely, subject to the following restrictions:                             //
//                                                                            //
// 1. The origin of this software must not be misrepresented; you must not    //
// claim that you wrote the original software. If you use this software       //
// in a product, an acknowledgment in the product documentation would be      //
// appreciated but is not required.                                           //
//                                                                            //
// 2. Altered source versions must be plainly marked as such, and must not be //
// misrepresented as being the original software.                             //
//                                                                            //
// 3. This notice may not be removed or altered from any source               //
// distribution.                                                              //
//                                                                            //
//============================================================================//

#ifndef OZZ_OZZ_BASE_MATHS_SOA_TRANSFORM_H_
#define OZZ_OZZ_BASE_MATHS_SOA_TRANSFORM_H_

#include "ozz/base/platform.h"
#include "ozz/base/maths/soa_float.h"
#include "ozz/base/maths/soa_quaternion.h"

namespace ozz {
namespace math {

// Stores an affine transformation with separate translation, rotation and scale
// attributes.
struct SoaTransform {
  SoaFloat3 translation;
  SoaQuaternion rotation;
  SoaFloat3 scale;

  static OZZ_INLINE SoaTransform identity() {
    const SoaTransform ret = {SoaFloat3::zero(), SoaQuaternion::identity(), SoaFloat3::one()};
    return ret;
  }
};
}  // math
}  // ozz
#endif  // OZZ_OZZ_BASE_MATHS_SOA_TRANSFORM_H_
