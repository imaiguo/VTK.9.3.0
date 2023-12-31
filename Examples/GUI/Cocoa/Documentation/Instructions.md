# SimpleCocoaVTK 4.1
# 2022-11-03
# by: Sean McBride and Mike Jackson

## Pre-Requisites:

1. you need Mac OS X 10.11.5 or later
2. you need Xcode 8.0 or later.
3. you need to know the basics of Xcode, CMake, VTK, and Cocoa already.
4. you have to build VTK separately.  The Xcode project expects VTK to be built with certain options, see below.

## Purpose of Example:

This sample project shows how to use VTK in a Mac-only Xcode-based Cocoa application.

This example is an NSDocument-based application with a single NSWindowController. The window has 3 independent VTK views in the XIB. They each illustrate a few different things:

- left view: a vtkCocoaGLView showing a cone. Uses setWantsBestResolutionOpenGLSurface:YES (high resolution on Retina).
- middle view: a vtkCocoaGLView showing a cylinder. Uses setWantsBestResolutionOpenGLSurface:NO (low resolution on Retina).
- right view: a custom NSView subclass that uses a custom CAOpenGLLayer subclass to draw. It also has code showing how to deal with the window moving from a Retina to a non-Retina display. Note however that this NSView subclass has no code to handle mouse or keyboard events.

See the screenshot (in the same folder as this file) to see what the app looks like.

## Instructions:

First, you need to build VTK.  SimpleCocoaVTK.xcodeproj requires that you build VTK with the following non-default options:

- `BUILD_SHARED_LIBS = OFF`
- `CMAKE_BUILD_TYPE = Debug`
- `VTK_VERSIONED_INSTALL = OFF`

It is recommended to also use these options, and you should synchronize your choices with the SimpleCocoaVTK.xcconfig file:

- `CMAKE_OSX_ARCHITECTURES`	(as desired)
- `CMAKE_OSX_DEPLOYMENT_TARGET` (as desired)

The other settings are not so relevant or their defaults are fine.  If you've never built VTK before, please see Ryan Glover's excellent (though dated now) document [here](http://www.vtk.org/Wiki/Cocoa_VTK).  Make sure you both build and install VTK.

Next, you have to create two "Custom Paths" in Xcode, named `vtk-debug-include` and `vtk-debug-lib`.  Do this from Preferences > Locations > Custom Paths (or Preferences > Locations > Source Trees in older versions of Xcode).  Both should point to subfolders of where your installed VTK is.  If you install into the default `/usr/local/`, set them like so:

- `vtk-debug-include` to `/usr/local/include/vtk`
- `vtk-debug-lib` to `/usr/local/lib`

Note that the path changes depending on your version of VTK.  Basically, you want the source trees to match your `CMAKE_INSTALL_PREFIX` setting.

All the VTK files in the Xcode project are referenced relative to the 'custom paths'.  This allows you to have your VTK files anywhere without changing the Xcode project itself. If files appear in red in Xcode, it's probably because your 'custom paths' are missing or wrong.

Now you should be able to build the project!

The code has many comments, and hopefully they will answer any questions you may have.

If you have any problems, please post to the [VTK Discourse forum](
https://discourse.vtk.org).

## Version History:

Changes in 4.2 (since 4.1)

- Converted this document from rtf to markdown.
- Fixed `CMakeLists.txt` to link to the `QuartzCore` framework.

Changes in 4.1 (since 4.0)

- Build with `VTK_VERSIONED_INSTALL=OFF` to keep version numbers out of paths.
- Updated for VTK 9.0, notably added `libvtkRenderingUI.a` to the Xcode project.
- Use Cocoa Auto Layout so window resizes nicely.

Changes in 4.0 (since 3.2)

- Added a `CAOpenGLLayer` based example.
- Show how to handle the window moving from a Retina to non-Retina display.
- Use more modern Objective-C (`@property`, `nullable`).
- Upgraded xib files to Xcode 8 format.

Changes in 3.2 (since 3.1)

- Updated for VTK 9.

Changes in 3.1 (since 3.0)

- Now demonstrate using `setWantsBestResolutionOpenGLSurface:YES/NO` on Retina displays.
- Now demonstrate getting the macOS system font to render non-ASCII text with.

Changes in 3.0 (since 2.0)

- Now require newer Xcode and more modern Objective-C dialect.
- Now require building VTK with `VTK_CUSTOM_LIBRARY_SUFFIX=<empty string>`, which is best practice anyway if using Xcode, so that the CMake-built VTK libraries don't constantly change filenames (by having the version number embedded).
- Added a window controller class, which is more idiomatic.
- The example now uses ARC (automatic reference counting) memory management by default. (This doesn't mean VTK needs to compiled as ARC, indeed, it cannot yet be.)

Changes in 2.0 (since 1.9)

- updated for VTK 7.1.

Changes in 1.9 (since 1.8)

- updated for VTK 6.3.

Changes in 1.8 (since 1.7)

- updated for VTK 6 and 10.7+ / Xcode 4+.

Changes in 1.7 (since 1.6)

- this example can now be built by CMake.

Changes in 1.6 (since 1.5)

- converted all nibs to xibs.
- the Xcode project had a reference to a path on my disk, removed.

Changes in 1.5 (since 1.4)

- now use two source trees instead of one.
- updated example to better reflect current versions of VTK, Mac OS, and Xcode.

Changes in 1.4 (since 1.3)

- removed prefix file
- renamed main.m to main.mm
- converted tabs to spaces

Changes in 1.3 (since 1.2)

- switched to using just one Source Tree
- tested in Mac OS X 10.5 and Xcode 3
- improved these instructions

Changes in 1.2 (since 1.1)

- using smart pointers

Changes in 1.1 (since 1.0)

- fixed some memory leaks
- minor code cleanup
- added call to print out any leaking vtk objects
- linked to Ryan Glover's great instructions on how to build vtk
