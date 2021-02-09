#ifndef CREATE_OPENINGS_FROM_SELECTION_PARAMETERS_HPP
#define CREATE_OPENINGS_FROM_SELECTION_PARAMETERS_HPP
#pragma once

#include "VBElemOperationsExport.hpp"


namespace VBEO {
namespace Opening {


	class VB_ELEM_OPERATIONS_DLL_EXPORT CreateOpeningsFromSelectionParameters : public GS::Object {
		DECLARE_CLASS_INFO
	public:
		enum class PreferredShape {
			Automatic,
			Rectangle,
			Circle
		};

		enum class NumberOfOpenings {
			OneOpeningPerElement,
			CombineOpeningsIfCloserThanLowerBound
		};

		enum class ElementIdType {
			Inherit,
			ToolDefaults
		};

	private:
		UInt32 mSelectedOriginalElementsNumber;
		PreferredShape mOpeningShape;
		double mOffset;
		bool mKeepOriginalElements;
		NumberOfOpenings mNumberOfOpenings;
		double mCombineOpeningsLowerBound;
		ElementIdType mElementIdType;

	public:
		CreateOpeningsFromSelectionParameters ();
		CreateOpeningsFromSelectionParameters (UInt32 inSelectedOriginalElementsNumber, PreferredShape inShape,
			double inOffset, bool inKeepOriginalElements, NumberOfOpenings inNumberOfOpenings, double inCombineOpeningsLowerBound, ElementIdType inElementId);

		UInt32 GetSelectedOriginalElementsNumber () const;
		PreferredShape GetOpeningShape () const;
		double GetOffset () const;
		bool IsOriginalElementsKept () const;
		NumberOfOpenings GetNumberOfOpenings () const;
		double GetCombineOpeningsLowerBound () const;
		ElementIdType GetElementIdType () const;

		void SetSelectedOriginalElementsNumber (const UInt32& value);
		void SetOpeningShape (const PreferredShape& value);
		void SetOffset (const double& value);
		void KeepOriginalElementsOn ();
		void KeepOriginalElementsOff ();
		void SetNumberOfOpenings (const NumberOfOpenings& value);
		void SetCombineOpeningsLowerBound (const double& value);
		void SetElementIdType (const ElementIdType& value);

		virtual	GSErrCode	Read (GS::IChannel& ic) override;
		virtual	GSErrCode	Write (GS::OChannel& oc) const override;

	private:
		GSErrCode ReadVersion1 (GS::IChannel& ic, const GS::InputFrame& frame);
		GSErrCode WriteVersion1 (GS::OChannel& oc) const;

	};
}
}


#endif