#if !defined __GDL_GDLIPDICTIONARYPATHCACHE_HPP__
#define __GDL_GDLIPDICTIONARYPATHCACHE_HPP__

#include "GDLExport.h"

#include "GSUnID.hpp"
#include "Ref.hpp"
#include "HashTable.hpp"
#include "GDLDictionaryPath.hpp"


namespace GDL
{

class GDL_DLL_EXPORT DictionaryPathCache
{
public:
	struct GDL_DLL_EXPORT CodeAreaIdIndices
	{
		GS::Array<Int32>	idArray;
	
		ULong		GenerateHashValue (void) const;
		bool		operator== (const CodeAreaIdIndices& rightOp) const;
	};

	class GDL_DLL_EXPORT InstanceForLibPart
	{
	public:
		DictionaryPath* GetVolatileEditableDictionaryPath (const CodeAreaIdIndices&, const std::function<FieldName (Int32)>& resolveIdFn);

	private:
		FieldName		ResolveCodeAreaIdIndex (Int32, const std::function<FieldName (Int32)>& resolveIdFn);

		GS::HashTable<Int32,FieldName>							fieldNames;
		GS::HashTable<CodeAreaIdIndices,DictionaryPath>			paths;
	};

	GS::Ref<InstanceForLibPart> CreateInstanceForLibPart (const GS::UnID& libPartUnID);

private:
	GS::HashTable<GS::UnID, GS::Ref<InstanceForLibPart>>	libPartDictionaryPathCaches;
};


inline ULong GenerateHashValue (const GDL::DictionaryPathCache::CodeAreaIdIndices& target)
{
	return target.GenerateHashValue ();
}

}


#endif