// *****************************************************************************
//
//	              Class	IElemAttributeModifier
//
// Module:			AC
// Namespace:		Phasing
// Contact person:	BB
//
// SG compatible
//
// *****************************************************************************


#if !defined ATTRIBUTEMODIFIER_HPP
#define ATTRIBUTEMODIFIER_HPP

#include "PhasingExport.hpp"
#include "GSUtilsDefs.h"

// from VBAttributes
#include "ADBAttributeIndex.hpp"

struct GS_RGBColor;

namespace Phasing {

// ======================= Class IElemAttributeModifier ===========================

class PHASING_DLL_EXPORT IElemAttributeModifier {

public:
	// line
	virtual ADB::AttributeIndex	GetLineType	(void) const = 0;
	virtual short				GetLinePen	(void) const = 0;

	// fill
	virtual ADB::AttributeIndex	GetFillType			(void) const = 0;
	virtual short				GetForegroundPen	(void) const = 0;
	virtual short				GetBackgroundPen	(void) const = 0;
	virtual GS_RGBColor			GetBackgroundColor	(void) const = 0;
	virtual GS_RGBColor			GetForegroundColor	(void) const = 0;

	virtual ~IElemAttributeModifier ();
};

} // namespace Phasing

#endif // ATTRIBUTEMODIFIER_HPP
