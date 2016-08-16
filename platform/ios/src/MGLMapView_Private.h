#import <Mapbox/Mapbox.h>

#include <mbgl/mbgl.hpp>

/// Minimum size of an annotation’s accessibility element.
extern const CGSize MGLAnnotationAccessibilityElementMinimumSize;

@interface MGLMapView (Internal)

/// Currently shown popover representing the selected annotation.
@property (nonatomic) UIView<MGLCalloutView> *calloutViewForSelectedAnnotation;

/** Triggers another render pass even when it is not necessary. */
- (void)setNeedsGLDisplay;

- (mbgl::Map *)mbglMap;

/** Converts NSTimeInterval to mbgl::Duration */
mbgl::Duration MGLDurationInSeconds(NSTimeInterval duration);

/** Returns whether the map view is currently loading or processing any assets required to render the map */
- (BOOL)isFullyLoaded;

/** Empties the in-memory tile cache. */
- (void)didReceiveMemoryWarning;

@end
