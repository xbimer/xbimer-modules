// *********************************************************************************************************************
// Description:		Floor Plan Theoretical Cutting Plane Definition Class As Not Event Source
//
// Module:			VBD
// Namespace:		VBD
// Contact person:	ZM
//
// SG compatible
// *********************************************************************************************************************

#if !defined (FLOOR_PLAN_CUT_DATA_NOEVENT_HPP)
#define FLOOR_PLAN_CUT_DATA_NOEVENT_HPP

#pragma once


// from GSRoot
//#include	"Definitions.hpp"
#include	"Md5.hpp"

// from ElementManager
#include	"GeneralElem.hpp"
#include	"FloorStructure.hpp"
#include	"VBElementsBase/ModelElement.hpp"

#include	"VBDocumentExport.hpp"
#include	"VBDViewSetting.hpp"

#include	"Real.hpp"

// --- Predeclarations	------------------------------------------------------------------------------------------------

namespace VBD {
	class CurrentStoryCutLevels;

	class ViewSetting;
	typedef ODB::Ref<ViewSetting>				ViewSettingRef;
	typedef ODB::ConstRef<ViewSetting>			ViewSettingConstRef;

}


namespace VBD {
// --- Class declaration -----------------------------------------------------------------------------------------------

// ****************************************************************************************
//									FloorPlanCutLevels
// ****************************************************************************************

// ... Declaration ......................................................................................................
	class VBDOCUMENT_DLL_EXPORT FloorPlanCutLevels : public TW::Object,
													 public ViewSettingBase	{
		DECLARE_DYNAMIC_CLASS_INFO (FloorPlanCutLevels)
	protected:
#define _FLOORPLANCUTLEVELS_DATA_MEMBERS_						\
		GSTime				lastAttributeTime;					\
		double				currCutLevel;						\
		double				topCutLevel;						\
		double				bottomCutLevel;						\
		short				topCutBaseStoryRelativeIndex;		\
		short				bottomCutBaseStoryRelativeIndex;	\
		double				fixLevel2Absolute0;
		_FLOORPLANCUTLEVELS_DATA_MEMBERS_

		MD5::FingerPrint	cutDataCheckSum;
		MD5::FingerPrint	CalcCheckSum (void) const;
	private:
		friend class CurrentStoryCutLevels;
		// ... Transaction management class ....................................................................................
	protected:
		class IsolatedState : public ODB::Object::IsolatedState
		{
		private:
			friend class FloorPlanCutLevels;

			_FLOORPLANCUTLEVELS_DATA_MEMBERS_

		public:
			virtual ~IsolatedState ();

		};
// ... Constructors ....................................................................................................
	public:
		FloorPlanCutLevels ();
		FloorPlanCutLevels (const VBD::FloorPlanCutLevels& source);
		virtual		~FloorPlanCutLevels ();
	
// ... operator= .......................................................................................................
		FloorPlanCutLevels&	operator= (ODB::ConstRef<FloorPlanCutLevels> source);

		bool		Set (double			cutLevelCurrent,
			double		cutLevelTop,
			double		cutLevelBottom,
			short		topStoryRelInd,
			short		bottomStoryRelIndex,
			double		levelFix2Absolute0);

		void		Get (double*		cutLevelCurrent,
			double*		cutLevelTop,
			double*		cutLevelBottom,
			short*		topStoryRelInd,
			short*		bottomStoryRelIndex,
			double*		levelFix2Absolute0) const;


		inline  double				GetBelowFixAbsoluteLevel (void) const;
		inline	MD5::FingerPrint	GetCutDataCheckSum (void) const;

		bool	operator == (const FloorPlanCutLevels& rhs) const;
		bool	operator != (const FloorPlanCutLevels& rhs) const;
	
// ... Derived data methods ............................................................................................
	private:
		ODB::Ref<CurrentStoryCutLevels>			GetDerivedData ();
		ODB::ConstRef<CurrentStoryCutLevels>	GetDerivedData () const;
	public: 
		bool						HasDerivedData () const;
		static void					CopyDerivedData (ODB::ConstRef<FloorPlanCutLevels> from, ODB::ConstRef<FloorPlanCutLevels> to);
		double						GetCutPlaneAbsoluteLevel (void) const;
		double						GetAboveCutPlaneAbsoluteLevel (void) const;
		double						GetBelowPlaneAbsoluteLevel (void) const;
		short						GetCurrentFloor () const;

		void	CalculateCutAltitudes (VBElem::ElemViewDepthLimitations	viewDepth,
			double*								aboveAltitude,
			double*								cutAltitude,
			double*								belowAltitude) const;

		void	CalculateCutLevels (VBElem::ElemViewDepthLimitations	viewDepth,
			double*								aboveLevel,
			double*								cutLevel,
			double*								belowLevel) const;

		EDB::StoryWithRange			GetCurrStoryWithRange () const;

		double						GetCutPlaneLevel (void) const;
		double						GetAboveCutPlaneLevel (void) const;
		double						GetBelowPlaneLevel (void) const;

		double						GetBelowFixLevel (void) const;
		bool						IsRelativeCalculated (void) const;

		GSErrCode					WriteContentForCheckSum (GS::OChannel& oc) const;

// ... Overridden virtual methods (GS::Object) .........................................................................
	public:
		virtual FloorPlanCutLevels*					Clone (void) const override;

// ... Transaction management methods ..................................................................................
	public:
		virtual GSErrCode						StoreIsolatedState (ODB::Object::IsolatedState* iState) const override;
		virtual GSErrCode						RestoreIsolatedState (const ODB::Object::IsolatedState* iState) override;
		virtual void							Isolate (void) override;
		virtual ODB::Object::IsolatedState*		CreateIsolatedState (void) const override;
		virtual USize							GetIsolatedSize (void) const override;
		virtual GSErrCode						ReadIsolatedState (GS::IChannel& ic) override;
		virtual GSErrCode						WriteIsolatedState (GS::OChannel& oc) const override;
	private:
		GSErrCode								ReadIsolatedStateVersion1 (GS::IChannel& ic, const GS::InputFrame& iFrame);
		GSErrCode								WriteIsolatedStateVersion1 (GS::OChannel& oc, const GS::OutputFrame& oFrame) const;
		GSErrCode								ReadCoreIsolatedStateVersion1 (GS::IChannel& ic, const GS::InputFrame& /*iFrame*/);
		GSErrCode								WriteCoreIsolatedStateVersion1 (GS::OChannel& oc, const GS::OutputFrame& /*oFrame*/) const;
	public:
		virtual GSErrCode						ReadTWIsolatedState (GS::IChannel& ic) override;
		virtual GSErrCode						WriteTWIsolatedState (GS::OChannel& oc) const override;
	private:
		GSErrCode								ReadTWIsolatedStateVersionLast (GS::IChannel& ic, const GS::InputFrame& iFrame);
		GSErrCode								WriteTWIsolatedStateVersionLast (GS::OChannel& oc, const GS::OutputFrame& oFrame) const;

// ... XML serialization ...............................................................................................
	public:
		virtual GSErrCode						ReadIsolatedStateXML (GS::XMLIChannel& ix) override;
		virtual GSErrCode						WriteIsolatedStateXML (GS::XMLOChannel& ox) const override;
// ... Accessors .......................................................................................................

// ... Link management .................................................................................................

	public:
		static ODB::Association11 <VBD::ViewSetting, VBD::FloorPlanCutLevels>&		GetViewSettingAssociationID (void);

		static ODB::ConstRef<FloorPlanCutLevels>	GetAttribute (ODB::ConstRef<FloorPlanCutLevels> floorPlanCutLevels);
		static ODB::Ref<FloorPlanCutLevels>			GetAttribute (ODB::Ref<FloorPlanCutLevels> floorPlanCutLevels);

		GS::UniString						GetStoredName (void) const;
		void								SetLastConnectionTime (const GSTime& value);
		const GSTime&						GetLastConnectionTime (void) const { return lastAttributeTime; }
		virtual bool						HasAttribute (void) const override;
		void								SetAttribute (ODB::Ref<FloorPlanCutLevels> dummy);
		//Also needs a constructor with Attribute class as the only argument

		bool								IsDerivedDataEqual (const ODB::ConstRef<CurrentStoryCutLevels> currentStoryCutLevels) const;

	};



	// --- Initializations -------------------------------------------------------------------------------------------------

	inline  FloorPlanCutLevels::FloorPlanCutLevels () : TW::Object (&classInfo),
		lastAttributeTime (0),
		currCutLevel (1.1),
		topCutLevel (0.0),
		topCutBaseStoryRelativeIndex (1),	//BL2007.01.31.	#55819
		bottomCutLevel (0.0),
		bottomCutBaseStoryRelativeIndex (0),
		fixLevel2Absolute0 (0)
	{
		cutDataCheckSum = CalcCheckSum ();
	}


	// --- Get Information from the Current Floor Plan Cut -----------------------------------------------------------------

	inline  double  FloorPlanCutLevels::GetBelowFixAbsoluteLevel (void) const
	{
		return (fixLevel2Absolute0);
	}

	inline  MD5::FingerPrint	FloorPlanCutLevels::GetCutDataCheckSum (void) const
	{
		return cutDataCheckSum;
	}

	inline bool	FloorPlanCutLevels::operator == (const FloorPlanCutLevels& rhs) const
	{
		return
			Geometry::IsNear<double> (currCutLevel, rhs.currCutLevel)			&&
			Geometry::IsNear<double> (topCutLevel, rhs.topCutLevel)				&&
			Geometry::IsNear<double> (bottomCutLevel, rhs.bottomCutLevel)		&&
			topCutBaseStoryRelativeIndex == rhs.topCutBaseStoryRelativeIndex	&&
			bottomCutBaseStoryRelativeIndex == rhs.bottomCutBaseStoryRelativeIndex;
	}

	inline bool	FloorPlanCutLevels::operator != (const FloorPlanCutLevels& rhs) const
	{
		return !(*this == rhs);
	}


	typedef ODB::Ref<FloorPlanCutLevels> FloorPlanCutLevelsRef;
	typedef ODB::ConstRef<FloorPlanCutLevels> FloorPlanCutLevelsConstRef;

	// ****************************************************************************************
	//									CurrentStoryCutLevels
	// ****************************************************************************************

	class VBDOCUMENT_DLL_EXPORT CurrentStoryCutLevels : public TW::Object {

		DECLARE_DYNAMIC_CLASS_INFO (CurrentStoryCutLevels)
	private:
		// temporary data dependent from the current story
#define _CURRENTSTORYCUTLEVELS_DATA_MEMBERS_			\
		bool				relativeCalculated;			\
		double				currCutLevelCalculated;		\
		double				topCutLevelCalculated;		\
		double				currentStoryLevel;			\
		double				topBase2Current;			\
		double				bottomBase2Current;			\
		short				currentFloorIndex;
		_CURRENTSTORYCUTLEVELS_DATA_MEMBERS_

			// ... Transaction management class ....................................................................................
	protected:
		class IsolatedState : public ODB::Object::IsolatedState
		{
		private:
			friend class CurrentStoryCutLevels;

			_CURRENTSTORYCUTLEVELS_DATA_MEMBERS_

		public:
			virtual ~IsolatedState ();

		};
	private:
		friend class VBD::FloorPlanCutLevels;
		// ... Constructors ....................................................................................................
		CurrentStoryCutLevels ();
		CurrentStoryCutLevels (const FloorPlanCutLevelsRef floorPlanCutDataCore);
	public:

		~CurrentStoryCutLevels ();

		bool Set (double		cutLevelCurrent,
			double		cutLevelTop,
			double		cutLevelBottom,
			short		topStoryRelInd,
			short		bottomStoryRelIndex,
			double		levelFix2Absolute0);

		void	Get (double*		cutLevelCurrent,
			double*		cutLevelTop,
			double*		cutLevelBottom,
			short*		topStoryRelInd,
			short*		bottomStoryRelIndex,
			double*		levelFix2Absolute0) const;
	
		CurrentStoryCutLevels (const CurrentStoryCutLevels& source);
		CurrentStoryCutLevels&	operator= (const CurrentStoryCutLevels& source);

		bool	operator == (const CurrentStoryCutLevels& rhs) const;
		bool	operator != (const CurrentStoryCutLevels& rhs) const;

		bool	SetCurrentFloor (EDB::FloorStructureConstRef floorStructure, short floorIndex);
		short	GetCurrentFloor () const;

		static bool		SetCurrentFloor (ODB::Ref<FloorPlanCutLevels> floorPlanCutLevelsRef, EDB::FloorStructureConstRef floorStructure, short floorIndex);
		static short	GetCurrentFloor (ODB::ConstRef<FloorPlanCutLevels> floorPlanCutLevelsRef);

		inline  double  GetCutPlaneAbsoluteLevel (void) const;
		inline  double  GetAboveCutPlaneAbsoluteLevel (void) const;
		inline  double  GetBelowPlaneAbsoluteLevel (void) const;

		void	CalculateCutAltitudes	(VBElem::ElemViewDepthLimitations	viewDepth,
			double*								aboveAltitude,
			double*								cutAltitude,
			double*								belowAltitude) const;

		void	CalculateCutLevels		(VBElem::ElemViewDepthLimitations	viewDepth,
			double*								aboveLevel,
			double*								cutLevel,
			double*								belowLevel) const;

		EDB::StoryWithRange			GetCurrStoryWithRange () const;

		inline  double				GetCutPlaneLevel (void) const;
		inline  double				GetAboveCutPlaneLevel (void) const;
		inline  double				GetBelowPlaneLevel (void) const;

		inline  double				GetBelowFixLevel (void) const;
		inline  bool				IsRelativeCalculated (void) const;

		// ... Core Data Setters getters .......................................................................................
	private:
		void							SetFloorPlanCutLevels (FloorPlanCutLevelsRef floorPlanCutLevels);
		FloorPlanCutLevelsConstRef		GetFloorPlanCutLevels (void) const;
		FloorPlanCutLevelsRef			GetFloorPlanCutLevels (void);
	
		// ... Overridden virtual methods (GS::Object) .........................................................................
	public:
		virtual CurrentStoryCutLevels*					Clone (void) const override;

		// ... Transaction management methods ..................................................................................
	public:
		virtual GSErrCode						StoreIsolatedState (ODB::Object::IsolatedState* iState) const override;
		virtual GSErrCode						RestoreIsolatedState (const ODB::Object::IsolatedState* iState) override;
		virtual void							Isolate (void) override;
		virtual ODB::Object::IsolatedState*		CreateIsolatedState (void) const override;
		virtual USize							GetIsolatedSize (void) const override;
		virtual GSErrCode						ReadIsolatedState (GS::IChannel& ic) override;
		virtual GSErrCode						WriteIsolatedState (GS::OChannel& oc) const override;
	private:
		GSErrCode								ReadIsolatedStateVersion1 (GS::IChannel& ic, const GS::InputFrame& iFrame);
		GSErrCode								WriteIsolatedStateVersion1 (GS::OChannel& oc, const GS::OutputFrame& oFrame) const;
		GSErrCode								ReadCoreIsolatedStateVersion1 (GS::IChannel& ic, const GS::InputFrame& /*iFrame*/);
		GSErrCode								WriteCoreIsolatedStateVersion1 (GS::OChannel& oc, const GS::OutputFrame& /*oFrame*/) const;
	public:
		virtual GSErrCode						ReadTWIsolatedState (GS::IChannel& ic) override;
		virtual GSErrCode						WriteTWIsolatedState (GS::OChannel& oc) const override;
	private:
		GSErrCode								ReadTWIsolatedStateVersionLast (GS::IChannel& ic, const GS::InputFrame& iFrame);
		GSErrCode								WriteTWIsolatedStateVersionLast (GS::OChannel& oc, const GS::OutputFrame& oFrame) const;

		// ... XML serialization ...............................................................................................
	public:
		virtual GSErrCode						ReadIsolatedStateXML (GS::XMLIChannel& ix) override;
		virtual GSErrCode						WriteIsolatedStateXML (GS::XMLOChannel& ox) const override;
	};
	
	typedef ODB::Ref<CurrentStoryCutLevels> CurrentStoryCutLevelsRef;
	typedef ODB::ConstRef<CurrentStoryCutLevels> CurrentStoryCutLevelsConstRef;

	EDB::StoryWithRange VBDOCUMENT_DLL_EXPORT GetStoryWithRange (const FloorPlanCutLevelsConstRef floorPlanCutLevels, EDB::GeneralElemConstRef elem, short story);


	// --- Initializations -------------------------------------------------------------------------------------------------

	inline  CurrentStoryCutLevels::CurrentStoryCutLevels () : TW::Object (&classInfo),
		relativeCalculated (false),
		currCutLevelCalculated (0.0),
		topCutLevelCalculated (0.0),
		currentStoryLevel (0.0),
		topBase2Current (3.0),
		bottomBase2Current (-3.0),
		currentFloorIndex (-15000)
	{

	}

	inline  CurrentStoryCutLevels::CurrentStoryCutLevels (const FloorPlanCutLevelsRef floorPlanCutDataCore) : TW::Object (&classInfo),
		relativeCalculated (false),
		currCutLevelCalculated (0.0),
		topCutLevelCalculated (0.0),
		currentStoryLevel (0.0),
		topBase2Current (3.0),
		bottomBase2Current (-3.0),
		currentFloorIndex (-15000)
	{
		SetFloorPlanCutLevels (floorPlanCutDataCore);
	}

	// --- Get Information from the Current Floor Plan Cut -----------------------------------------------------------------

	inline  double  CurrentStoryCutLevels::GetCutPlaneAbsoluteLevel (void) const
	{
		DBASSERT (relativeCalculated);
		return (currentStoryLevel + currCutLevelCalculated);
	}

	inline  double  CurrentStoryCutLevels::GetAboveCutPlaneAbsoluteLevel (void) const
	{
		DBASSERT (relativeCalculated);
		return (currentStoryLevel + topBase2Current + topCutLevelCalculated);
	}

	inline  double  CurrentStoryCutLevels::GetBelowPlaneAbsoluteLevel (void) const
	{
		DBASSERT (relativeCalculated);
		return (currentStoryLevel + bottomBase2Current + GetFloorPlanCutLevels ()->bottomCutLevel);
	}

	inline  double  CurrentStoryCutLevels::GetCutPlaneLevel (void) const
	{
		DBASSERT (relativeCalculated);
		return (currCutLevelCalculated);
	}

	inline  double  CurrentStoryCutLevels::GetAboveCutPlaneLevel (void) const
	{
		DBASSERT (relativeCalculated);
		return (topBase2Current + topCutLevelCalculated);
	}

	inline  double  CurrentStoryCutLevels::GetBelowPlaneLevel (void) const
	{
		DBASSERT (relativeCalculated);
		return (bottomBase2Current + GetFloorPlanCutLevels ()->bottomCutLevel);
	}

	inline  double  CurrentStoryCutLevels::GetBelowFixLevel (void) const
	{
		DBASSERT (relativeCalculated);
		return (GetFloorPlanCutLevels ()->fixLevel2Absolute0 - currentStoryLevel);
	}

	inline  bool  CurrentStoryCutLevels::IsRelativeCalculated (void) const
	{
		return relativeCalculated;
	}


	inline bool	CurrentStoryCutLevels::operator == (const CurrentStoryCutLevels& rhs) const
	{
		return 		
			this->relativeCalculated		== rhs.relativeCalculated		&&
			this->currentFloorIndex			== rhs.currentFloorIndex		&&
			this->currCutLevelCalculated	== rhs.currCutLevelCalculated	&&
			this->topCutLevelCalculated		== rhs.topCutLevelCalculated	&&
			this->currentStoryLevel			== rhs.currentStoryLevel		&&
			this->topBase2Current			== rhs.topBase2Current			&&
			this->bottomBase2Current		== rhs.bottomBase2Current;
	}

	inline bool	CurrentStoryCutLevels::operator != (const CurrentStoryCutLevels& rhs) const
	{
		return !(*this == rhs);
	}


	class VBDOCUMENT_DLL_EXPORT FloorPlanCutLevelsGuard {
	private:
		FloorPlanCutLevelsRef floorPlanLevels;

	public:
		FloorPlanCutLevelsGuard ()
		{
			try {
				floorPlanLevels = new FloorPlanCutLevels ();
			}
			catch (...) {
				floorPlanLevels = nullptr;
			}
			DBASSERT (floorPlanLevels != nullptr);
		}

		FloorPlanCutLevelsGuard (FloorPlanCutLevelsGuard&& source) = default;

		FloorPlanCutLevelsGuard (const ODB::ConstRef<FloorPlanCutLevels>& source)
		{
			if (source != nullptr) {
				floorPlanLevels = new FloorPlanCutLevels (*source);
			} else {
				floorPlanLevels = new FloorPlanCutLevels ();
			}
		}

		~FloorPlanCutLevelsGuard ()
		{
			delete floorPlanLevels;
		}

		FloorPlanCutLevelsRef GetFloorPlanCutLevels (void) const
		{
			return floorPlanLevels;
		}
	};

	VBDOCUMENT_DLL_EXPORT double GetFloorCutAltitude (const EDB::FloorStructureConstRef& floorStructure, const VBD::FloorPlanCutLevelsConstRef& cutLevels, const short floorNumber);

} // namespace VBD

#endif