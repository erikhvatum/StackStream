# StackStream
A small collection of Qt and QML widgets for viewing images and image stack collections of any size + a QML scene graph plugin providing 32 bit per channel floating point framebuffer and layer support for optimal compositing, manipulation, and display of HDR and high-depth images, including in 30-bit mode (10 bit per channel).

At this time, StackStream includes:

* The first open source QML scene graph plugin of any kind.  Implemented in C++.
* The first open source 30-bit display support for QML and QtQuick2.  Implemented in C++.
* The first open source float32-component framebuffer and texture map support for QML and QtQuick2.  Implemented in C++.
* The SSImage class, supporting higher bitdepths than QImage and floating point components (which QImage does not support at all).  Implemented in C++, exposed to QML.
* The SSLayer class, a QQuickItem for displaying SSImage objects with gamma transformation.  Implemented in C++, exposed to QML.
* An SSLayer viewer with a GUI interface for gamma transform parameters and associated classes and objects.  Implemented QML.

StackStream will soon include:

* Support for blending any arbitrary number SSLayers with any combination of the blend modes specified by SVG.
* An extensive Python API exposed via SIP.
* An SSImage and SSLayer stack flipbook presented as a table, with a complete Python API and intuitive multi-image drag and drop support.
* A QAbstractModel-derived model exposed to Python as a simple Python list in addition to its full PyQt API, eliminating the need to represent anything as a QQmlList ever again.
