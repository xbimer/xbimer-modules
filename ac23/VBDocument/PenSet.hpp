// *********************************************************************************************************************
// Description:		PenSet definition
//
// Module:			VBDocument
// Namespace:		VBD
// Contact person:	VaLa
//
// SG compatible
// *********************************************************************************************************************


#if !defined PENSET_HPP
#define PENSET_HPP

#include "VBDocumentExport.hpp"
#include "TWObject.hpp"

#include "VBDViewSetting.hpp"

namespace VBAttr {
	class PenTable;
};

namespace VBD {
	class VBDOCUMENT_DLL_EXPORT PenSet : public TW::Object,
										 public ViewSettingBase
	{
		DECLARE_DYNAMIC_CLASS_INFO (PenSet)

	private:
	#define _PENSET_DATA_MEMBERS_					\
		GSTime			lastConnectionTime;			\
		GS::UniString	storedName;					\

		_PENSET_DATA_MEMBERS_

// ... Transaction management class ....................................................................................
	protected:
		class IsolatedState : public TW::Object::IsolatedState
		{
		private:
			friend class PenSet;

			_PENSET_DATA_MEMBERS_

		public:
			virtual ~IsolatedState ();

		};

// ... Constructors ....................................................................................................
	private:
	public:
		PenSet ();
		PenSet (const PenSet& source);
		PenSet (GS::ClassInfo* finalClassInfo);
		PenSet (ODB::Ref<VBAttr::PenTable> ls, VBD::AttributeType type);

		virtual ~PenSet ();
	public:
		static ODB::Ref<VBD::PenSet> CreateVBDAttributeWithMissingAttribute (const GS::UniString& value);

	private:
		void									InitOwn (void);
		void									ResetOwn (void);
// ... operator= .......................................................................................................
	public:
		PenSet&									operator=	(const PenSet& source) = delete;

// ... Overridden virtual methods (GS::Object) .........................................................................
		PenSet*									Clone () const override;

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
		static ODB::ConstRef<VBAttr::PenTable>	GetPenSet (ODB::ConstRef<PenSet> penSet);
		static ODB::Ref<VBAttr::PenTable>		GetPenSet (ODB::Ref<PenSet> penSet);

		GSErrCode							SetPenSet (ODB::Ref<VBAttr::PenTable> ls, VBD::AttributeType type);
		VBD::AttributeType					GetType () const;
		static VBD::AttributeType			GetType (ODB::ConstRef<VBAttr::PenTable> ps);

// ... Link management .............................................................................................
	public:
		static ODB::Association11 <VBD::ViewSetting, PenSet>&		GetViewSettingAssociationID (void);
		GS::UniString						GetStoredName (void) const { return storedName; }

		static ODB::ConstRef<VBAttr::PenTable>	GetAttribute (ODB::ConstRef<PenSet> penSet);
		static ODB::Ref<VBAttr::PenTable>		GetAttribute (ODB::Ref<PenSet> penSet);

		void									SetLastConnectionTime (const GSTime& value);
		const GSTime&							GetLastConnectionTime (void) const { return lastConnectionTime; }
		virtual bool							HasAttribute (void) const override;
		void									SetAttribute (ODB::Ref<VBAttr::PenTable> ls, VBD::AttributeType type);


	};
}     // namespace VBD
#endif