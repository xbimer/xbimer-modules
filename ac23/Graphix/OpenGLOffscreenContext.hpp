// *****************************************************************************
// File:			OpenGLOffscreenContext.hpp
//
// Description:
//
// Project:
//
// Contact person:	SZI, DGY
//
// SG compatible
// *****************************************************************************

#ifndef _OPENGLOFFSCREENCONTEXT_HPP_
#define _OPENGLOFFSCREENCONTEXT_HPP_

#include "DrawingBridge.hpp"
#include "IRenderingCore.hpp"
#include "IResourceFactory.hpp"
#include "IRenderingContext.hpp"

namespace NewDisplay {

class NativeImageImpl;
class NativeContextImpl;

}

namespace Graphix {

struct DataForExternalDraw;

// --- NativeImageCreatorTask definition ---------------------------------------

class GRAPHIX_DLL_EXPORT NativeImageCreatorTask : public IRenderingTask
{
private:
	NewDisplay::NativeImage	nativeImage;
	ITexture*				colorSource;
	ITexture*				resolveTarget;	// if source is multisampled
	IRenderPass*			renderPass;		// if source is multisampled
	double					scaleFactor;
	bool					targetIsPerPixelAlpha;

public:
	NativeImageCreatorTask (int universeID, ITexture* texture, double resolutionfactor, bool perpixelalpha);

	void Execute (IRenderingContext* context, IResourceFactory* factory) override;
	void Dispose () override;

	NewDisplay::NativeImage GetNativeImage ()	{ return nativeImage; }
};

DataForExternalDraw* BeginOpenGL (NewDisplay::NativeImageImpl* forImageImpl, bool multisampling);
void EndOpenGLAndDraw (DataForExternalDraw* context, NewDisplay::NativeContextImpl& contextToDrawTo);

}

#endif
