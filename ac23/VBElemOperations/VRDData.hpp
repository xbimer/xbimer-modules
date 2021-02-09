// *********************************************************************************************************************
// Description:		View Related Derived Data
//
// Module:			VBElemOperations
// Namespace:		VBD
// Contact person:	VaLa, LB
//
// SG compatible
// *********************************************************************************************************************

#ifndef	VRDData_HPP
#define	VRDData_HPP

#pragma once

// --- Includes	--------------------------------------------------------------------------------------------------------

#include "GSFriend.hpp"
#include "ODBObject.hpp"
#include "ODBRefList.hpp"
#include "ODBDependencyNode.hpp"
#include "VBElemOperationsExport.hpp"

// --- Forward declarations	--------------------------------------------------------------------------------------------

struct TRANMAT;

namespace VBD {
	class VRDData;

	class ConnectionData;

	class View;
	typedef ODB::Ref<View> ViewRef;
	typedef ODB::ConstRef<View> ViewConstRef;
}
class VRDDataTest;

namespace VBElem {
	class ModelElement;
	typedef ODB::Ref<ModelElement> ModelElementRef;
	typedef ODB::ConstRef<ModelElement> ModelElementConstRef;

}

// --- Type definitions	------------------------------------------------------------------------------------------------
namespace VBD {
	typedef ODB::Ref<VBD::VRDData> VRDDataRef;
	typedef ODB::ConstRef<VBD::VRDData> VRDDataConstRef;
	typedef ODB::RefList<VBD::VRDData> VRDDataRefList;
	typedef ODB::ConstRefList<VBD::VRDData> VRDDataConstRefList;
}

// === VRDData class ===================================================================================================

namespace VBD {

class VB_ELEM_OPERATIONS_DLL_EXPORT VRDData : public ODB::Object
{
	DECLARE_CLASS_INFO
	
public:
	class VB_ELEM_OPERATIONS_DLL_EXPORT IsolatedState : public ODB::Object::IsolatedState
	{
		friend class VRDData;

	public:
		virtual ~IsolatedState ();
	};

private:
	static USize maxVRDDataSize;

public:
	static const ODB::Id		modelElementVRDDataAssocId;			// Model element (e.g. Wall) VRDData association ID
	static const ODB::Id		viewVRDDataAssocId;					// VBDView VRDData association ID

	VRDData ();
	VRDData (const GS::ClassInfo* classinfo);
	VRDData (const VRDData& source);
	~VRDData ();

private:
	virtual GSErrCode						CalculateData (VBElem::ModelElementConstRef		elem, 
														   VBD::ViewConstRef				view, 
														   const ConnectionData*			params) = 0;
protected:
	static VBD::VRDDataRef					GetNotFullyCalculatedViewRelatedDerivedData (VBElem::ModelElementConstRef elem, VBD::ViewConstRef view);
public:
	static VBD::VRDDataRef					GetViewRelatedDerivedData (VBElem::ModelElementConstRef	elem, VBD::ViewConstRef	view);
	static bool								HasViewRelatedDerivedData (VBElem::ModelElementConstRef	elem, VBD::ViewConstRef	view);

	virtual VBElem::ModelElementConstRef	GetModelElement (void) const;
	virtual VBElem::ModelElementRef			GetModelElement (void);
	static VBD::VRDDataRefList				GetVRDDataList (VBElem::ModelElementRef elem);
	static VBD::VRDDataRefList				GetVRDDataList (VBD::ViewRef view);
	static USize							GetMaxVRDDataSize (void);

	virtual void							Transform (const TRANMAT* tran);

private:
	virtual bool							IsCalculated () const = 0;

public:
	static VBD::VRDDataRef					CalculateViewRelatedDerivedData (VBElem::ModelElementConstRef	elem, 
																			 VBD::ViewConstRef				view, 
																			 const ConnectionData*			params);
protected:
	template<class T>
	static void								InvalidateByView (VBD::ViewRef view);
	static void								InvalidateByView (VBD::ViewRef view);
public:
	static void								InvalidateByElem (VBElem::ModelElementRef elem);
private:
	static void								Invalidate (VRDDataRef vrdData, VBD::ViewRef view);

public:
	static UInt32							GetVRDDataCount (VBElem::ModelElementConstRef elem);

	void									ConnectToView (VBD::ViewRef targetView);
	static void								CopyAllViewRelatedDerivedData	(VBD::ViewConstRef sourceView, VBD::ViewRef targetView);
public:
	virtual GSErrCode						StoreIsolatedState (ODB::Object::IsolatedState*			iState) const override;
	virtual GSErrCode						RestoreIsolatedState (const ODB::Object::IsolatedState*	iState) override;
	virtual GSErrCode						WriteIsolatedState (GS::OChannel&						oc) const override;
	virtual GSErrCode						ReadIsolatedState (GS::IChannel&						ic) override;
	virtual ODB::Object::IsolatedState*		CreateIsolatedState (void) const override;
	virtual USize							GetIsolatedSize (void) const override;

public:
	static VBD::VRDDataRef					GetNotFullyCalculatedViewRelatedDerivedData (GS::Friend<VRDDataTest>, VBElem::ModelElementConstRef elem, VBD::ViewConstRef view);
	bool									IsCalculated (GS::Friend<VRDDataTest>) const;
	template<class T>
	static void								InvalidateByView (GS::Friend<VRDDataTest>, VBD::ViewRef view);
	static void								InvalidateByView (GS::Friend<VRDDataTest>, VBD::ViewRef view);
};


template<class T>
void	VRDData::InvalidateByView (VBD::ViewRef view)
{
	VRDDataRefList vrdDataList = GetVRDDataList (view);
	for (VRDDataRef vrdData : vrdDataList) {
		if (ODB::IsType<T> (vrdData))
			Invalidate (vrdData, view);
	}
}		//VRDData::InvalidateByView


template<class T>
void	VRDData::InvalidateByView (GS::Friend<VRDDataTest>, VBD::ViewRef view)
{
	InvalidateByView<T> (view);
}		//VRDData::InvalidateByView


VB_ELEM_OPERATIONS_DLL_EXPORT		extern ODB::DependencyNode	vrdDataSyncronizerFromSourceViews;
}		//namespace VBD

#endif