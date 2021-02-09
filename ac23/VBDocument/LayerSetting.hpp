// *********************************************************************************************************************
// Description:		LayerSetting definition
//
// Module:			VBDocument
// Namespace:		VBD
// Contact person:	VaLa
//
// SG compatible
// *********************************************************************************************************************


#if !defined VBDLAYERSETTING_HPP
#define VBDLAYERSETTING_HPP

#include "VBDocumentExport.hpp"
#include "TWObject.hpp"

#include "VBDViewSetting.hpp"
#include "LayerStatusList.hpp"

namespace VBAttr {
	class LayerComb;
};

namespace VBD {
	class VBDOCUMENT_DLL_EXPORT LayerSetting : public TW::Object,
											   public ViewSettingBase
	{
		DECLARE_DYNAMIC_CLASS_INFO (LayerSetting)

	private:
	#define _VBDLAYERSETTING_DATA_MEMBERS_			\
		GSTime			lastConnectionTime;			\
		GS::UniString	storedName;					\

		_VBDLAYERSETTING_DATA_MEMBERS_

// ... Transaction management class ....................................................................................
	protected:
		class IsolatedState : public TW::Object::IsolatedState
		{
		private:
			friend class LayerSetting;

			_VBDLAYERSETTING_DATA_MEMBERS_

		public:
			virtual ~IsolatedState ();

		};

// ... Constructors ....................................................................................................
	private:
	public:
		LayerSetting ();
		LayerSetting (const LayerSetting& source);
		LayerSetting (GS::ClassInfo* finalClassInfo);
		LayerSetting (ODB::Ref<VBAttr::LayerComb> ls, VBD::AttributeType type);

		virtual ~LayerSetting ();
	public:
		static ODB::Ref<VBD::LayerSetting> CreateVBDAttributeWithMissingAttribute (const GS::UniString& value);

	private:
		void									InitOwn (void);
		void									ResetOwn (void);
	public:
		LayerSetting&							operator=	(const LayerSetting& source) = delete;

		VBAttr::LayerStatusList					GetDifference (ODB::ConstRef<VBD::LayerSetting> other) const;

// ... Overridden virtual methods (GS::Object) .........................................................................
		LayerSetting*							Clone () const override;

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
		static ODB::ConstRef<VBAttr::LayerComb>	GetLayerCombination (ODB::ConstRef<LayerSetting> layerSettting);
		static ODB::Ref<VBAttr::LayerComb>		GetLayerCombination (ODB::Ref<LayerSetting> layerSettting);
		GSErrCode								SetLayerCombination (ODB::Ref<VBAttr::LayerComb> layerRef, VBD::AttributeType type);
		VBD::AttributeType						GetType () const;
		static VBD::AttributeType				GetType (const ODB::ConstRef<VBAttr::LayerComb> layerComb);

		bool									operator== (const VBD::LayerSetting& rhs) const;
		bool									operator!= (const VBD::LayerSetting& rhs) const;

// ... Link management .............................................................................................
	public:
		static ODB::Association11 <VBD::ViewSetting, LayerSetting>&		GetViewSettingAssociationID (void);
		GS::UniString						GetStoredName (void) const { return storedName; }

		static ODB::ConstRef<VBAttr::LayerComb>	GetAttribute (ODB::ConstRef<LayerSetting> layerSettting);
		static ODB::Ref<VBAttr::LayerComb>		GetAttribute (ODB::Ref<LayerSetting> layerSettting);

		void								SetLastConnectionTime (const GSTime& value);
		const GSTime&						GetLastConnectionTime (void) const { return lastConnectionTime; }
		virtual bool						HasAttribute (void) const override;
		void								SetAttribute (ODB::Ref<VBAttr::LayerComb> layerRef, VBD::AttributeType type);

	};
}     // namespace VBD
#endif