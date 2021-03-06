#import "UIImage+MGLAdditions.h"

#include <mbgl/util/image+MGLAdditions.hpp>

@implementation UIImage (MGLAdditions)

- (nullable instancetype)initWithMGLStyleImage:(const mbgl::style::Image *)styleImage
{
    CGImageRef image = CGImageFromMGLPremultipliedImage(styleImage->getImage().clone());
    if (!image) {
        return nil;
    }

    if (self = [self initWithCGImage:image scale:styleImage->getPixelRatio() orientation:UIImageOrientationUp])
    {
        if (styleImage->isSdf())
        {
            self = [self imageWithRenderingMode:UIImageRenderingModeAlwaysTemplate];
        }
    }
    CGImageRelease(image);
    return self;
}

- (std::unique_ptr<mbgl::style::Image>)mgl_styleImage {
    BOOL isTemplate = self.renderingMode == UIImageRenderingModeAlwaysTemplate;
    return std::make_unique<mbgl::style::Image>(MGLPremultipliedImageFromCGImage(self.CGImage),
                                                float(self.scale), isTemplate);
}

@end
