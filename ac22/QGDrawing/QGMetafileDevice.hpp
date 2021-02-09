// *****************************************************************************
//
//                                Class MetafileDevice
//
// Module:			QGDrawing
// Namespace:		QG
// Contact person:	DGY
//
// SG compatible
//
// *****************************************************************************

#if !defined QGMETAFILEDEVICE_HPP
#define QGMETAFILEDEVICE_HPP

#pragma once

// --------------------------------- Includes ----------------------------------
#include "QGBase.hpp"
#include "QGDrawableDevice.hpp"

// ============================== class MetafileDevice ==============================

namespace QG {

class Metafile;

class QG_DLL_EXPORT MetafileDevice : public QG::DrawableDevice {

private:
	MetafileDevice (const MetafileDevice&);
	MetafileDevice& operator=(const MetafileDevice&);

public:
	MetafileDevice (QG::Metafile& metaFile, const QG::Rect& boundsRectInPixels);
	MetafileDevice (QG::Metafile& metaFile, const QG::Rect& boundsRectInPixels, double dpiValue); 	// any virtual resolution can be set as initial value

	double			GetRealResolution (void) const override;

	void			SetDeviceResolutionType (bool lowResType);
};


}	// namespace QG
// ______________________________ class MetafileDevice ______________________________

#endif	// QGMETAFILEDEVICE_HPP
