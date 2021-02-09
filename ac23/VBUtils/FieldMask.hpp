// *********************************************************************************************************************
// Field mask for observer classes
//
// Module:			VBUtils
// Namespace:		-
// Contact person:	FI
//
// [SG compatible]
// *********************************************************************************************************************

#if !defined (FIELDMASK_H)
#define FIELDMASK_H

#pragma once


// --- Includes	--------------------------------------------------------------------------------------------------------

// GSRoot
#include "Definitions.hpp"
#include "HashSet.hpp"
#include "HashTable.hpp"
#include "Ref.hpp"

// own
#include "VBUtilsExport.hpp"
#include "FieldGroup.hpp"
#include "Field.hpp"
#include "SubFieldMaskId.hpp"


VBUTILS_DLL_EXPORT extern const GS::CustomClassProperty FieldMaskProperty;


// =============================================================================
// FieldMask
//
// =============================================================================
class FieldMask;
typedef GS::Ref<FieldMask> FieldMaskRef;

template<typename T>
class OrFieldMaskTable;

template<typename T>
class AndFieldMaskTable;


class VBUTILS_DLL_EXPORT FieldMask
{
private:
	template<typename T>
	friend class OrFieldMaskTable;

	template<typename T>
	friend class AndFieldMaskTable;
private:
	class FieldMaskData
	{
	private:
		GS::HashSet<Field>							fieldsStatic;
		GS::HashSet<DynamicField>					fieldsDynamic;
		bool										inverseMode;

	public:
		FieldMaskData (bool setAll = false);

		FieldMaskData (const FieldMaskData& source)													= default;
		FieldMaskData (FieldMaskData&& source)														= default;
		FieldMaskData&								operator=		(const FieldMaskData& source)	= default;
		FieldMaskData&								operator=		(FieldMaskData&& source)		= default;

		bool										IsEmpty			(void) const;
		bool										IsInverseMode	(void) const { return inverseMode; }

		void										Set				(const Field& field);
		void										Clear			(const Field& field);
		bool										Test			(const Field& field) const;

		void										Set				(const DynamicField& field);
		void										Clear			(const DynamicField& field);
		bool										Test			(const DynamicField& field) const;

		static FieldMaskData						FieldsOR		(const FieldMaskData& left, const FieldMaskData& right);
		static FieldMaskData						FieldsAND		(const FieldMaskData& left, const FieldMaskData& right);

		void										SetAll			(void);
		void										ClearAll		(void);

		void										SetGroup		(const FieldGroup& group, const GS::ClassInfo* classInfo);
		void										ClearGroup		(const FieldGroup& group, const GS::ClassInfo* classInfo);

		GS::HashSet<Field>							GetChangedFields		(const GS::ClassInfo* classInfo) const;
		GS::HashSet<DynamicField>					GetChangedDynamicFields	(void) const;
		
		void										RemoveInvalidFields		(const GS::HashSet<Field>& allowedFields);
	};

	const GS::ClassInfo*										classInfo;
	FieldMaskData												fieldMaskData;
	GS::HashTable<SubFieldMaskId, FieldMask*>					subFieldMasks;
	mutable GS::HashTable<DynamicSubFieldMaskId, FieldMask*>	dynamicSubFieldMasks;

protected:
	static void		FieldsOR (FieldMask& left, const FieldMask& right);
	static void		FieldsAND (FieldMask& left, const FieldMask& right);

	FieldMask (bool setAll = false, const GS::ClassInfo* classInfo = nullptr);
	FieldMask (const FieldMask& source, const GS::ClassInfo* classInfo);
	FieldMask (FieldMask&& source, const GS::ClassInfo* classInfo);
	
public:
	FieldMask (const FieldMask& source) = delete;
	~FieldMask();

	FieldMask&									operator=			(const FieldMask& source);
	FieldMask&									operator=			(FieldMask&& source);

	FieldMask&									operator|=			(const FieldMask& source);
	FieldMask&									operator&=			(const FieldMask& source);

	bool										IsEmpty				(void) const;

	void										Set					(const Field& field);
	void										Clear				(const Field& field);
	bool										Test				(const Field& field) const;

	void										Set					(const DynamicField& field);
	void										Clear				(const DynamicField& field);
	bool										Test				(const DynamicField& field) const;

	void										SetAll				(void);
	void										ClearAll			(void);

	const  FieldMask& 							GetSubFieldMask		(const SubFieldMaskId& subFieldMaskGuid) const;
	FieldMask& 									GetSubFieldMask		(const SubFieldMaskId& subFieldMaskGuid);

	bool			 							HasSubFieldMask		(const DynamicSubFieldMaskId& subFieldMaskGuid) const;
	const  FieldMask& 							GetSubFieldMask		(const DynamicSubFieldMaskId& subFieldMaskGuid) const;
	FieldMask& 									GetSubFieldMask		(const DynamicSubFieldMaskId& subFieldMaskGuid);

	void										SetGroup			(const FieldGroup& group);
	void										ClearGroup			(const FieldGroup& group);

	GS::HashSet<Field>							GetChangedFields	(void) const;
	GS::HashSet<DynamicField>					GetChangedDynamicFields	(void) const;

private:
	bool										IsInverseMode		(void) const;
	void										AddSubFieldMasks	(void);
	void										RemoveInvalidSubMasks (void);
	static FieldMask*							CreateFieldMask		(const GS::ClassInfo* classInfo, const FieldMask* source);
	void										ValidateAssignement	(const GS::ClassInfo* sourceClassInfo, bool freeMemory = false);
	void										FreeMemory			(void);
public:
	static FieldMask*							CreateFieldMask		(const GS::ClassInfo* classInfo);
	static FieldMask*							CopyFieldMask		(FieldMaskRef source);
	static FieldMask*							CopyFieldMask		(const FieldMask& source);
	static GS::Guid								FieldIdx2FieldId	(const GS::Guid& classId, GSIndex idx);
	static GS::Guid								FieldName2FieldId	(const GS::Guid& classId, const GS::UniString& name);
};



template<class T>
class FieldMaskT : public FieldMask
{
public:
	FieldMaskT (bool setFull = false) : 
		FieldMask (setFull, T::GetClassInfoStatic ())
	{
	}

	FieldMaskT (const FieldMaskT<T>& source) : 
		FieldMask (source, T::GetClassInfoStatic ())
	{
	}

	FieldMaskT (FieldMaskT<T>&& source) : 
		FieldMask (std::move (source), T::GetClassInfoStatic ())
	{
	}

	FieldMaskT<T>&	operator= (const FieldMaskT<T>& source)
	{
		return static_cast<FieldMaskT<T>&> (FieldMask::operator= (source));
	}

	FieldMaskT<T>&	operator= (FieldMaskT<T>&& source)
	{
		return static_cast<FieldMaskT<T>&> (FieldMask::operator= (std::move (source)));
	}

	FieldMaskT<T>&	operator= (const FieldMask& source)
	{
		return static_cast<FieldMaskT<T>&> (FieldMask::operator= (source));
	}

	FieldMaskT<T>&	operator= (FieldMask&& source)
	{
		return static_cast<FieldMaskT<T>&> (FieldMask::operator= (std::move (source)));
	}

	FieldMaskT<T>	operator|	(const FieldMask& source) const
	{
		if (this == &source)
			return *this;

		FieldMaskT<T> left = *this;
		FieldsOR (left, source);
		return left;
	}

	FieldMaskT<T>	operator&	(const FieldMask& source) const
	{
		if (this == &source)
			return *this;

		FieldMaskT<T> left = *this;
		FieldsAND (left, source);
		return left;
	}
};

typedef FieldMask* CCALL FieldMaskFunctionType (const FieldMask* source);

#define	DECLARE_FIELDMASK_FUNCTION(className)															\
	public:																								\
		static inline FieldMask* CCALL FieldMaskFunction (const FieldMask* source)						\
		{																								\
			if (source != nullptr)																		\
				return new FieldMaskT<className> (static_cast<const FieldMaskT<className>&>(*source));	\
			else																						\
				return new FieldMaskT<className> ();													\
		}																								\



#endif
