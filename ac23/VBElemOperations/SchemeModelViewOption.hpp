// *********************************************************************************************************************
// Description:		SchemeModelViewOptions
//
// Module:			VBElemOperations
// Namespace:		VBElemOperations
// Contact person:	NYD, JG, CSP, KG, NAD
//
// SG compatible
// *********************************************************************************************************************
#if !defined (_SchemeModelViewOption_HPP_)
#define _SchemeModelViewOption_HPP_

#include "VBElemOperationsExport.hpp"

// from GSRoot
#include "Object.hpp"

#include "ADBAttributeIndex.hpp"
#include "ADBGeneralDefinitions.hpp"


class VB_ELEM_OPERATIONS_DLL_EXPORT SchemeModelViewOption : public GS::Object {
	DECLARE_CLASS_INFO
protected:
	ADB::AttributeIndex	surfaceIndex;
	ADB::AttributeIndex	cutLineIndex;
	short	cutLinePen;
	ADB::AttributeIndex	uncutLineIndex;
	short	uncutLinePen;

public:
	bool operator== (const SchemeModelViewOption& rhs) const;
	bool operator!= (const SchemeModelViewOption& rhs) const;

protected:
	SchemeModelViewOption ();
	SchemeModelViewOption (const ADB::AttributeIndex& surfaceIndex, const ADB::AttributeIndex& cutLineIndex, short cutLinePen, const ADB::AttributeIndex& uncutLineIndex, short uncutLinePen);

public:
	inline ADB::AttributeIndex	GetSurfaceAttributeIndex (void) const { return surfaceIndex; }
	inline ADB::AttributeIndex	GetCutLineAttributeIndex (void) const { return cutLineIndex; }
	inline short				GetCutLinePenIndex (void) const { return cutLinePen; }
	inline ADB::AttributeIndex	GetUncutLineAttributeIndex (void) const { return uncutLineIndex; }
	inline short				GetUncutLinePenIndex (void) const { return uncutLinePen; }

	void						SetSurfaceAttributeIndex (const ADB::AttributeIndex& index);
	void						SetCutLineAttributeIndex (const ADB::AttributeIndex& index);
	void						SetCutLinePenIndex (short index);
	void						SetUncutLineAttributeIndex (const ADB::AttributeIndex& index);
	void						SetUncutLinePenIndex (short index);

	// ----- Attribute reindexing ----------------------------------------------------

 	virtual void		CollectUsedAttrs (ADB::IUsedAttrCollection& usedAttrCollection) const;
	virtual void		CollectUsedPens (ADB::IUsedPenCollection& usedPenCollection) const;
	virtual void		ReindAttr (ADB::IAttributeNewIndexTable* newIndexTable);

	// ----- GS::Object --------------------------------------------------------------

public:
	virtual GSErrCode	Read (GS::IChannel& ic) override;
	virtual GSErrCode	Write (GS::OChannel& oc) const override;

	// ----- Other Serialization -----------------------------------------------------

	virtual GSErrCode			ReadXML (GS::XMLIChannel& ic) = 0;
	virtual GSErrCode			WriteXML (GS::XMLOChannel& oc) const = 0;

	// ----- Auxilary functions ------------------------------------------------------

private:
	GSErrCode ReadVersion10 (GS::IChannel& ic);
	GSErrCode ReadVersion20 (GS::IChannel& ic);
	GSErrCode WriteVersion10 (GS::OChannel& oc) const;
	GSErrCode WriteVersion20 (GS::OChannel& oc) const;
};

#endif // _STAIRSCHEMEMODELVIEWOPTION_HPP_
