#ifndef ATTRIBUTECHANGESCOMMAND_WITH_BIMDATA 
#define ATTRIBUTECHANGESCOMMAND_WITH_BIMDATA

#include "PropertyOperationsExport.hpp"
#include "ApplyAttributeChangesCommand.hpp"
#include "Project.hpp"
#include "ClassificationTypes.hpp"
#include "PropertyTypes.hpp"
#include "AttributeEnvironment.hpp"

namespace PropertyOperations
{

class PROPERTY_OPERATIONS_DLL_EXPORT ApplyAttributeChangesCommandWithBIMData : public VBAttrOperations::ApplyAttributeChangesCommand
{
	DECLARE_DYNAMIC_CLASS_INFO (ApplyAttributeChangesCommandWithBIMData)

public:	
	explicit ApplyAttributeChangesCommandWithBIMData (const EDB::ProjectRef& edbProject);
	~ApplyAttributeChangesCommandWithBIMData ();

	virtual GSErrCode		Read	(GS::IChannel& ic) override;
	virtual GSErrCode		Write	(GS::OChannel& oc) const override;

	CLS::ClassificationSystemContainerConstRef					GetClassificationSystemContainerInOwnIdSpace ();
	Property::UserDefinedPropertyDefinitionContainerConstRef	GetPropertyDefinitionContainerInOwnIdSpace ();

protected:
	ApplyAttributeChangesCommandWithBIMData ();

	virtual GS::Owner<VBAttrOperations::ReMergeableAttributeEnvironment>	CreateAttributeEnvironment () const override;

private:
	BIMData::BIMDataContainerConstRef							GetConstBIMDataContainer () const;

	EDB::ProjectRef originalEdbProject;
};

}

#endif
