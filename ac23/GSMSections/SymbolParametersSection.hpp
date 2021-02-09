#ifndef	SYMBOLPARAMETERSSECTION_HPP
#define	SYMBOLPARAMETERSSECTION_HPP

#pragma once

#include	"ISectionContent.hpp"
#include	"SectionFactory.hpp"
#include	"SymbSect.h"
#include	"Owner.hpp"

class SymbolParametersSection : public LPF::ISectionContent
{
public:
	SymbolParametersSection (LPF::SectionInfo& sectionInfo);
	SymbolParametersSection (const SymbolParametersSection& other);
	virtual ~SymbolParametersSection ();

	virtual bool					IsLoaded () const override;
	virtual GS::Object*				GetContent () override;
	virtual const GS::Object*		GetContent () const override;
	virtual void					SetContent (GS::Object* newObject, short sectVersion = UseSectCurrVersion) override;
	virtual void					ClearContent () override;

	virtual LPF::ISectionContent*	Clone () const override;

	virtual GSErrCode				Update () override;
	virtual GSErrCode				Convert (GS::PlatformSign platform) override;
	virtual GSErrCode				Check () override;
	virtual short					GetCurrentVersion () override;

	virtual GSErrCode				Read (GS::IChannel& ic, USize sectLen, GS::PlatformSign symbolPlatform, short sectVersion, GSType symbolType) override;
	virtual GSErrCode				Write (GS::OChannel& oc, GS::PlatformSign symbolPlatform, short sectVersion) override;

	virtual GSErrCode				Dump (GS::OChannel& oChannel, bool detailed) override;

	SymbolParametersSection&		operator = (const SymbolParametersSection& other);

private:
	GS::Owner<SymbolParametersSectionContent>		mContent;
};



class SymbolParametersSectionCreator : public LPF::ISectionContentCreator
{
public:
	LPF::ISectionContent*		Create (LPF::SectionInfo& sectionInfo);
};



class SymbolParametersSectionLegacyConversion
{
public:
	static GSHandle										ExportToHandle (const SymbolParametersSectionContent* syParSect, GS::PlatformSign symbolPlatform, short sectVersion);
	static GS::Owner<SymbolParametersSectionContent>	ImportFromHandle (GSConstHandle handle, GS::PlatformSign symbolPlatform, short sectVersion, GSType symbolType);
};

#endif
