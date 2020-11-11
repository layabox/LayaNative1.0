
#include "layaPaint.h"

namespace MonkVG {
    layaPaint::layaPaint() {
        _gradientImage = VG_INVALID_HANDLE;
    }

    layaPaint::~layaPaint() {
        if (_gradientImage != VG_INVALID_HANDLE) {
            //delete (MonkVG::IImage*)_gradientImage; TODO 
        }
    }

    void layaPaint::setGLState() {

    }

    void layaPaint::buildLinearGradientImage(VGfloat pathWidth, VGfloat pathHeight) {

    }

    void layaPaint::buildRadialGradientImage(VGfloat pathWidth, VGfloat pathHeight) {

    }
    
    void layaPaint::buildLinear2x3GradientImage(VGfloat pathWidth, VGfloat pathHeight) {

    }

    void layaPaint::buildRadial2x3GradientImage(VGfloat pathWidth, VGfloat pathHeight) {

    }

    void layaPaint::buildGradientImage(VGfloat pathWidth, VGfloat pathHeight) {

    }

}