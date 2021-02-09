// *****************************************************************************
//
//				Dimension Standards classes
//
// Module:			AC
// Namespace:		-
// Contact person:	JG, NYD
//
// SG compatible
//
// *****************************************************************************

#ifndef DIMESION_STANDARDS_HPP
#define DIMESION_STANDARDS_HPP

// from VBD
#include "VBDocumentExport.hpp"
#include "DimensionStandardsTypes.hpp"

// from TW
#include "TWObject.hpp"
#include "IClientCreatableObject.hpp"
#include "ITWSpecificDataContainer.hpp"
#include "LockableObjectSet.hpp"

// from VBElements
#include "VBElements/NumFormat.hpp"

// from ObjectHierarchyUtils
#include "IdSpaceContentEnumerator.hpp"

// from ObjectDatabase
#include "MVCCommand.hpp"

#include "AutoPtr.hpp"


namespace VBD
{

//========================== Class DimensionStandards  =============================

class VBDOCUMENT_DLL_EXPORT DimensionStandards :	public TW::Object,
							public GS::EventSource,
							public TW::ITWSpecificDataContainer,
							public TW::IClientCreatableObject
{
public:
	using LengthNumType			= VBElem::LenNumFormat;
	using AngleNumType			= VBElem::AngNumFormat;


DECLARE_DYNAMIC_CLASS_INFO (DimensionStandards)

public:
	enum Fields {
		FieldError			= 0,
		First				= 1,
		Name				= 1,
		Index				= 2,
		LengthDimensions	= 6,
		AngleDimension		= 7,
	};

	enum LengthType
	{
		FirstLengthType = 0,
		Linear = 0,
		Angular = 1,
		RadialLength = 2,
		Level = 3,
		Elevation = 4,
		DoorSizes = 5,
		ParapetHeight = 6,
		AreaDimensions = 7,
		NumberOfLengthTypes = 8 //End marker
	};



#define _DIMENSIONSTANDARDS_DATA_MEMBERS_																											\
private:																																			\
		/* data members */																															\
																																					\
	GS::UniString				mName;								/* name of dimension standard */												\
	short						mIndex;								/* index of dimension standard */												\
	LengthNumType				mLenNums[NumberOfLengthTypes];		/* for the linear, angular, radial-length, level, elevation, door sizes, */		\
																	/* parapet height and area dimensions */										\
	AngleNumType				mAngNum;							/* for the angle dimension */													\
	bool						mIsClientCreated;					/* client created flag*/														\

	_DIMENSIONSTANDARDS_DATA_MEMBERS_


static const GS::Array<LengthType> mLengthTypes;

// Transaction management class

private:
	class IsolatedState : public TW::Object::IsolatedState {
		private:
			friend class DimensionStandards;

			_DIMENSIONSTANDARDS_DATA_MEMBERS_

		public:
			virtual ~IsolatedState ();
	};

public:
		// constructors

	DimensionStandards ();
	DimensionStandards (const DimensionStandards& source);
	virtual ~DimensionStandards ();

		// operator overloading

	DimensionStandards&					operator= (const DimensionStandards& source);
	bool								operator== (const DimensionStandards& rhs);

	DimensionStandards*					Clone (void) const override;

		// get methods

	inline	const GS::UniString&		GetName (void) const { return mName; };
	inline	const short					GetIndex (void) const { return mIndex; };
	inline	const LengthNumType&		GetLengthDimension (const LengthType& type) const { return mLenNums [static_cast<UIndex> (type)]; }
	inline	const AngleNumType&			GetAngleDimension (void) const { return mAngNum; }

	ODB::Ref<DimensionStandardsSet>			GetOwnerSet ();
	ODB::ConstRef<DimensionStandardsSet>	GetOwnerSetConst () const;

		// set methods

	GSErrCode							SetName (const GS::UniString& newName, const bool generateUniqueIfNeeded = false);
	GSErrCode							SetIndex (const short newIndex, const bool setFirstAvaliableIfTaken = false);
	void								SetLengthDimension (const LengthType& type, const LengthNumType& newLengthDimension);
	void								SetAngleDimension (const AngleNumType& newAngleDimension);

		// auxiliary functions

	void								Clear ();
	bool								EqualsCoreData (const DimensionStandards& rhs) const;
	static bool							EqualsCoreData (const DimensionStandards& lhs, const DimensionStandards& rhs);

		// ITWSpecificDataContainer method

	virtual void						ClearTWSpecificData (void) override;

		// TW::IClientCreatableObject methods

	virtual bool						IsClientCreated () const override;
	virtual void						SetClientCreated (bool newClientCreated) override;

		// observer methods

	void								Attach (DimensionStandardsObserver& observer);
	void								Detach (DimensionStandardsObserver& observer);

	virtual void						NotifyNameChanged ();
	virtual void						NotifyIndexChanged ();
	virtual void						NotifyLengthDimensionsChanged (const LengthType& type);
	virtual void						NotifyAngleDimensionChanged ();

		// read / write methods

	GSErrCode							Read (GS::IChannel& ic) override;
	GSErrCode							Write (GS::OChannel& oc) const override;

private:
	GSErrCode							ReadVersion1 (GS::IChannel& ic, const GS::InputFrame& frame);
	GSErrCode							WriteVersion1 (GS::OChannel& oc, const GS::OutputFrame& frame) const;

public:
	static const GS::Array<LengthType>&	GetLengthTypes ();

public:
		// Transaction management methods

	virtual	GSErrCode					StoreIsolatedState   			(ODB::Object::IsolatedState* isolatedState) const override;
	virtual	GSErrCode					RestoreIsolatedState 			(const ODB::Object::IsolatedState* isolatedState) override;
	virtual	ODB::Object::IsolatedState*	CreateIsolatedState				(void) const override;
	virtual USize						GetIsolatedSize					(void) const override;
	virtual	GSErrCode					WriteIsolatedState   			(GS::OChannel& oc) const override;
	virtual	GSErrCode					ReadIsolatedState 				(GS::IChannel& ic) override;

		// TWObject methods

	GSErrCode							ReadTWIsolatedState  (GS::IChannel& ic) override;
	GSErrCode							WriteTWIsolatedState (GS::OChannel& oc) const override;

};


class VBDOCUMENT_DLL_EXPORT DimensionStandardsSet :	public TW::LockableObjectSet,
													public ODB::MD5DifferenceGenerator::ObjectSet
{
	DECLARE_DYNAMIC_CLASS_INFO (DimensionStandardsSet)


#define _DIMENSIONSTANDARDSSET_DATA_MEMBERS_																										\
private:																																			\
		/* data members */																															\
																																					\

	_DIMENSIONSTANDARDSSET_DATA_MEMBERS_

	GS::AutoPtr<ODB::MD5DifferenceGenerator>	mMD5DifferenceGenerator;

public:
	// Constructors

	DimensionStandardsSet ();
	DimensionStandardsSet (const DimensionStandardsSet& source);
	virtual	~DimensionStandardsSet ();

private:
	class IsolatedState : public TW::LockableObjectSet::IsolatedState
	{
	private:
		friend class DimensionStandardsSet;

		_DIMENSIONSTANDARDSSET_DATA_MEMBERS_

	public:
		virtual ~IsolatedState ();
	};

public:
	class VBDOCUMENT_DLL_EXPORT Sorter
	{
	public:
		static bool ByName (const DimensionStandardsConstRef& lhs, const DimensionStandardsConstRef& rhs);
		static bool ByIndex (const DimensionStandardsConstRef& lhs, const DimensionStandardsConstRef& rhs);
	};

public:
	bool									AddDimensionStandard (DimensionStandardsRef& newItemRef);
	bool									ModifyDimensionStandard (const DimensionStandards& newItemVlaue);
	bool									ModifyDimensionStandard (const DimensionStandards& newItemValue, const short onIndex);
	bool									RemoveDimensionStandard (const DimensionStandardsRef& itemToRemove);

	void									Clear ();
	void									CopyFromDimensionStandardsSet (const DimensionStandardsSetConstRef& rhs, const bool keepOriginals = false);

	DimensionStandardsRef					GetDimensionStandard (const short index);
	DimensionStandardsRef					GetDimensionStandard (const GS::UniString& name);
	DimensionStandardsConstRef				GetDimensionStandardConst (const short index) const;
	DimensionStandardsConstRef				GetDimensionStandardConst (const GS::UniString& name) const;

	DimensionStandardsRefList				GetDimensionStandardList (const bool includeCustom = false);
	DimensionStandardsConstRefList			GetDimensionStandardConstList (const bool includeCustom = false) const;
	GS::Array<DimensionStandardsRef>		GetSortedDimensionStandardList (const std::function<bool (const DimensionStandardsConstRef&, const DimensionStandardsConstRef&)> sorterFunction = Sorter::ByName, const bool includeCustom = false);
	GS::Array<DimensionStandardsConstRef>	GetSortedDimensionStandardConstList (const std::function<bool (const DimensionStandardsConstRef&, const DimensionStandardsConstRef&)> sorterFunction = Sorter::ByName, const bool includeCustom = false) const;

	USize									GetDimensionStandardCount (const bool includeCustom = false) const;
	short									GetDimensionStandardMaximumIndex () const;

	DimensionStandardsRef					GetCustomDimensionStandard ();
	DimensionStandardsConstRef				GetCustomDimensionStandardConst () const;
	DimensionStandardsRef					GetOrCreateCustomDimensionStandard ();
	bool									SetCustomDimensionStandard (DimensionStandardsRef& newCustomItemRef);
	bool									HasCustomDimensionStandard () const;
	bool									CheckIsCustomDimensionStandard (const DimensionStandardsConstRef& standardRef) const;
	bool									ClearCustomDimensionStandard ();

	bool									CheckIndexIsFree (const short index) const;
	short									GetFreeIndex () const;
	bool									CheckNameIsUnique (const GS::UniString& name) const;
	GS::UniString							GenerateUniqueName (const GS::UniString& name) const;

public:
		// TW::LockableObjectSet methods

	virtual const GS::UniString			GetObjectName (void) const override;

	virtual bool						HasClientCreatedObjects (void) const override;
	virtual TWCB::PermissionID			GetCreatePermissionID () const override;
	virtual TWCB::PermissionID			GetDeleteModifyPermissionID () const override;

	virtual GSErrCode					GenerateAndStoreClientChanges (void) override;
	virtual GSErrCode					StartDifferenceTracing (void) override;
	virtual GSErrCode					GenerateDifference (ODB::Delta* clientChanges, TW::LockableObjectSetChecker* lockableObjectSetChecker = nullptr) const override;
	virtual GSErrCode					StopDifferenceTracing (bool revertChanges = false) override;
	virtual bool						IsDifferenceTracingStarted (void) const override;

	virtual GSErrCode					OnSendChangesDone (void) override;
	virtual GSErrCode					OnSendChangesFailed (void) override;
	virtual GSErrCode					MergePreProcess (ODB::Delta& serverChanges) override;
	virtual GSErrCode					SynchronizeAfterIdSpaceMerge (const ODB::Delta& serverChanges, TWCB::ReadProjectConnectionBaseInterface* projectConnection = nullptr) override;

		// ODB::MD5DifferenceGenerator::ObjectSet

	virtual void						EnumerateObjects (ConstObjectEnumerator* enumerator) const override;

private:
	void								FilterChanges (ODB::Delta& changes) const;

public:
		// Transaction management methods

	virtual	GSErrCode					StoreIsolatedState (ODB::Object::IsolatedState* isolatedState) const override;
	virtual	GSErrCode					RestoreIsolatedState (const ODB::Object::IsolatedState* isolatedState) override;
	virtual	ODB::Object::IsolatedState*	CreateIsolatedState (void) const override;
	virtual USize						GetIsolatedSize (void) const override;
	virtual	GSErrCode					WriteIsolatedState (GS::OChannel& oc) const override;
	virtual	GSErrCode					ReadIsolatedState (GS::IChannel& ic) override;

private:
	GSErrCode							WriteIsolatedStateVersion1 (GS::OChannel& oc) const;
	GSErrCode							ReadIsolatedStateVersion1 (GS::IChannel& ic);
	GSErrCode							WriteIsolatedStateVersion2 (GS::OChannel& oc, const GS::OutputFrame& frame) const;
	GSErrCode							ReadIsolatedStateVersion2 (GS::IChannel& ic, const GS::InputFrame& frame);

public:
		// TWObject methods

	GSErrCode							ReadTWIsolatedState (GS::IChannel& ic) override;
	GSErrCode							WriteTWIsolatedState (GS::OChannel& oc) const override;

public:
	static DimensionStandardsSetRef		MergeToIdSpace (const DimensionStandardsSetConstRef& originalSet, ODB::IdSpace& idSpace);
};


class VBDOCUMENT_DLL_EXPORT ApplyDimensionStandardsChangesCommand : public MVC::CommandImp
{
	DECLARE_DYNAMIC_CLASS_INFO (ApplyDimensionStandardsChangesCommand)

private:
	GSErrCode										status;

	DimensionStandardsSetRef						originalDimensionStandardsSetRef;
	GS::AutoPtr<ODB::IdSpace>						editableIdSpace;
	DimensionStandardsSetRef						editableDimensionStandardsSetRef;
	GS::AutoPtr<ODB::MD5DifferenceGenerator>		differenceGenerator;
	GS::AutoPtr<OHUtils::IdSpaceContentEnumerator>	idSpaceContentEnumerator;

private:
	ApplyDimensionStandardsChangesCommand ();															//disabled
	ApplyDimensionStandardsChangesCommand (const ApplyDimensionStandardsChangesCommand&);				//disabled
	ApplyDimensionStandardsChangesCommand& operator= (const ApplyDimensionStandardsChangesCommand&);	//disabled

public:
	explicit ApplyDimensionStandardsChangesCommand (DimensionStandardsSetRef dimensionStandardsRef);
	virtual ~ApplyDimensionStandardsChangesCommand ();

	virtual GS::ErrorStatus			Execute (void) override;

	DimensionStandardsSetRef		GetEditableDimensionStandardsSet (void) const;
	GSErrCode						GetChanges (ODB::Delta* changes) const;

private:
	GSErrCode						FillAndTraceEditableIdSpace ();

	virtual GSErrCode				Read (GS::IChannel& ic) override;
	virtual GSErrCode				Write (GS::OChannel& oc) const override;

	GSErrCode						ReadVersion1 (GS::IChannel& ic);
	GSErrCode						WriteVersion1 (GS::OChannel& oc) const;
};


class VBDOCUMENT_DLL_EXPORT DimensionStandardsObserver : public GS::EventObserver {
protected:
		// destructor

	virtual ~DimensionStandardsObserver ();

public:
		// changed methods
	virtual void						NameChanged (const DimensionStandardsConstRef&)															= 0;
	virtual void						IndexChanged (const DimensionStandardsConstRef&)														= 0;
	virtual void						LengthDimensionsChanged (const DimensionStandards::LengthType& type, const DimensionStandardsConstRef&)	= 0;
	virtual void						AngleDimensionChanged (const DimensionStandardsConstRef&)												= 0;
};


}


#endif
