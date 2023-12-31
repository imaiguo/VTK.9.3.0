// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
/**
 * @class   vtkCocoaGLView
 * @brief   Cocoa OpenGL rendering context
 *
 *
 * This class is a subclass of Cocoa's NSView; it uses Objective-C++.
 * This class overrides several NSView methods.
 * To provide the usual VTK keyboard user interface, it overrides the
 * following methods: acceptsFirstResponder, keyDown:,
 * keyUp:, and flagsChanged:
 * To provide the usual VTK mouse user interface, it overrides the
 * following methods: mouseMoved:, mouseEntered:,
 * mouseExited: scrollWheel:, mouseDown:, rightMouseDown:,
 * otherMouseDown:, mouseDragged:, rightMouseDragged:, otherMouseDragged:,
 * and updateTrackingAreas.
 * To provide file dropping support, it implements the following methods:
 * draggingEntered: and performDragOperation:.
 * To be able to render and draw onscreen, it overrides drawRect:.
 *
 * Compatibility notes:
 * - this class was previously a subclass of NSOpenGLView,
 * but starting with VTK 5.0 is now a subclass of NSView.
 * - starting with VTK 6.3 this class overrides the more modern
 * updateTrackingAreas instead of resetCursorRects.
 * - starting with VTK 8.1 this class properly supports Retina
 * displays and implements viewWillMoveToWindow: and
 * viewDidChangeBackingProperties to do so.
 * - starting with VTK 9.0 this class now also overrides initWithCoder:.
 * It also implements draggingEntered: and performDragOperation: and thus
 * declares conformance to the NSDraggingDestination protocol.
 *
 * @sa
 * vtkCocoaRenderWindow vtkCocoaRenderWindowInteractor
 */

#ifndef vtkCocoaGLView_h
#define vtkCocoaGLView_h

#import "vtkRenderingOpenGL2Module.h" // For export macro
#import <Cocoa/Cocoa.h>

// Note: This file should be includable by both pure Objective-C and Objective-C++ source files.
// To achieve this, we use the neat technique below:
#ifdef __cplusplus
// Forward declarations
VTK_ABI_NAMESPACE_BEGIN
class vtkCocoaRenderWindow;
class vtkCocoaRenderWindowInteractor;

// Type declarations
typedef vtkCocoaRenderWindow* vtkCocoaRenderWindowRef;
typedef vtkCocoaRenderWindowInteractor* vtkCocoaRenderWindowInteractorRef;
VTK_ABI_NAMESPACE_END
#else
// Type declarations
typedef void* vtkCocoaRenderWindowRef;
typedef void* vtkCocoaRenderWindowInteractorRef;
#endif

VTKRENDERINGOPENGL2_EXPORT
@interface vtkCocoaGLView : NSView<NSDraggingDestination>
{
@private
  vtkCocoaRenderWindowRef _myVTKRenderWindow;
  NSTrackingArea* _rolloverTrackingArea;
}

- (vtkCocoaRenderWindowRef)getVTKRenderWindow;
- (void)setVTKRenderWindow:(vtkCocoaRenderWindowRef)theVTKRenderWindow;

- (vtkCocoaRenderWindowInteractorRef)getInteractor;

@end

#endif /* vtkCocoaGLView_h */
// VTK-HeaderTest-Exclude: vtkCocoaGLView.h
