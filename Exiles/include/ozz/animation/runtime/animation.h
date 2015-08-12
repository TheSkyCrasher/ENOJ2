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

#ifndef OZZ_OZZ_ANIMATION_RUNTIME_ANIMATION_H_
#define OZZ_OZZ_ANIMATION_RUNTIME_ANIMATION_H_

#include "ozz/base/platform.h"
#include "ozz/base/io/archive_traits.h"

namespace ozz {
namespace io { class IArchive; class OArchive; }
namespace animation {

// Forward declares the AnimationBuilder, used to instantiate an Animation.
namespace offline { class AnimationBuilder; }

// Forward declaration of key frame's type.
struct TranslationKey;
struct RotationKey;
struct ScaleKey;

// Defines a runtime skeletal animation clip.
// The runtime animation data structure stores animation keyframes, for all the
// joints of a skeleton. This structure is usually filled by the
// AnimationBuilder and deserialized/loaded at runtime.
// For each transformation type (translation, rotation and scale), Animation
// structure stores a single array of keyframes that contains all the tracks
// required to animate all the joints of a skeleton, matching breadth-first
// joints order of the runtime skeleton structure. In order to optimize cache
// coherency when sampling the animation, Keyframes in this array are sorted by
// time, then by track number.
class Animation {
 public:

  // Builds a default animation.
  Animation();

  // Declares the public non-virtual destructor.
  ~Animation();

  // Gets the animation clip duration.
  float duration() const {
    return duration_;
  }

  // Gets the number of animated tracks.
  int num_tracks() const {
    return num_tracks_;
  }

  // Returns the number of SoA elements matching the number of tracks of *this
  // animation. This value is useful to allocate SoA runtime data structures.
  int num_soa_tracks() const {
    return (num_tracks_ + 3) / 4;
  }

  // Gets the buffer of translations keys.
  ozz::Range<const TranslationKey> translations() const {
    return translations_;
  }

  // Gets the buffer of rotation keys.
  ozz::Range<const RotationKey> rotations() const {
    return rotations_;
  }

  // Gets the buffer of scale keys.
  ozz::Range<const ScaleKey> scales() const {
    return scales_;
  }

  // Get the estimated animation's size in bytes.
  size_t size() const;

  // Serialization functions.
  // Should not be called directly but through io::Archive << and >> operators.
  void Save(ozz::io::OArchive& _archive) const;
  void Load(ozz::io::IArchive& _archive, uint32_t _version);

 protected:
 private:

  // Disables copy and assignation.
  Animation(Animation const&);
  void operator=(Animation const&);

  // AnimationBuilder class is allowed to instantiate an Animation.
  friend class offline::AnimationBuilder;

  // Internal destruction function.
  void Destroy();

  // Stores all translation/rotation/scale keys begin and end of buffers.
  ozz::Range<TranslationKey> translations_;
  ozz::Range<RotationKey> rotations_;
  ozz::Range<ScaleKey> scales_;

  // Duration of the animation clip.
  float duration_;

  // The number of joint tracks. Can differ from the data stored in translation/
  // rotation/scale buffers because of SoA requirements.
  int num_tracks_;
};
}  // animation

namespace io {
OZZ_IO_TYPE_VERSION(2, animation::Animation)
OZZ_IO_TYPE_TAG("ozz-animation", animation::Animation)
}  // io
}  // ozz
#endif  // OZZ_OZZ_ANIMATION_RUNTIME_ANIMATION_H_
