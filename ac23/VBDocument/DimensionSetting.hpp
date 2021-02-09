// *********************************************************************************************************************
// Description:		DimensionSetting definition
//
// Module:			VBDocument
// Namespace:		VBD
// Contact person:	VaLa
//
// SG compatible
// *********************************************************************************************************************


#if !defined DIMENSIONSETTING_HPP
#define DIMENSIONSETTING_HPP

#include "VBDocumentExport.hpp"
#include "TWObject.hpp"

#include "VBDViewSetting.hpp"
#include "DimensionStandards.hpp"

namespace VBD {
	class VBDOCUMENT_DLL_EXPORT DimensionSetting : public TW::Object,
												   public ViewSettingBase
	{
		DECLARE_DYNAMIC_CLASS_INFO (DimensionSetting)

	private:
	#define _DIMENSIONSETTING_DATA_MEMBERS_			\
		GSTime			lastConnectionTime;			\
		GS::UniString	storedName;					\

		_DIMENSIONSETTING_DATA_MEMBERS_

// ... Transaction management class ....................................................................................
	protected:
		class IsolatedState : public TW::Object::IsolatedState
		{
		private:
			friend class DimensionSetting;

			_DIMENSIONSETTING_DATA_MEMBERS_

		public:
			virtual ~IsolatedState ();

		};

// ... Constructors ....................................................................................................
	private:

	public:
		DimensionSetting ();
		DimensionSetting (const DimensionSetting& source);
		DimensionSetting (ODB::Ref<VBD::DimensionStandards> ls, AttributeType type);

		virtual ~DimensionSetting ();
	public:
		static ODB::Ref<VBD::DimensionSetting> CreateVBDAttributeWithMissingAttribute (const GS::UniString& value);
	private:
		void									InitOwn (void);
		void									ResetOwn (void);
// ... operator= .......................................................................................................
	public:
		DimensionSetting&						operator=	(const DimensionSetting& source) = delete;

// ... Overridden virtual methods (GS::Object) .........................................................................
		DimensionSetting*						Clone () const override;

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
		virtual GSErrCode						ReadXML (GS::XMLIChannel& ix) override;
		virtual GSErrCode						WriteXML (GS::XMLOChannel& ox) const override;

// ... Accessors .......................................................................................................
	private:
		void								SetStoredName (const GS::UniString& value);

	public:
		static ODB::ConstRef<VBD::DimensionStandards>	GetDimension (ODB::ConstRef<DimensionSetting> dimensionSetting);
		static ODB::Ref<VBD::DimensionStandards>		GetDimension (ODB::Ref<DimensionSetting> dimensionSetting);
		GSErrCode								SetDimension (ODB::Ref<VBD::DimensionStandards> ls, AttributeType type);

		VBD::AttributeType						GetType () const;
		static VBD::AttributeType				GetType (const ODB::ConstRef<VBD::DimensionStandards>& dim);

// ... Link management .............................................................................................
	public:
		static ODB::Association11 <VBD::ViewSetting, DimensionSetting>&		GetViewSettingAssociationID (void);
		GS::UniString							GetStoredName (void) const { return storedName; }

		static ODB::ConstRef<VBD::DimensionStandards>	GetAttribute (ODB::ConstRef<DimensionSetting> dimensionSetting);
		static ODB::Ref<VBD::DimensionStandards>		GetAttribute (ODB::Ref<DimensionSetting> dimensionSetting);

		void									SetLastConnectionTime (const GSTime& value);
		const GSTime&							GetLastConnectionTime (void) const { return lastConnectionTime; }
		virtual bool							HasAttribute (void) const override;
		void									SetAttribute (ODB::Ref<VBD::DimensionStandards> ls, AttributeType type);
	};
}     // namespace VBD
#endif