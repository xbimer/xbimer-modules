#pragma once

#ifndef __CAST_COMMON_H__
#define __CAST_COMMON_H__

#include "./types/point.h"
#include "./types/rect.h"

#include "pybind11.h"
#include "stl.h"

#include "Point.hpp" //GS::Point
#include "DG.h"
#include "DGUtility.hpp" // DG::Point
#include "Location.hpp"

#define POINT_CASTER_FACTORY(type,dcls,scls)							\
template<> class type_caster<dcls> {									\
public:																	\
	bool load(handle src, bool /*convert*/) {							\
		if (!src) {														\
			return false;												\
		}																\
		auto obj = reinterpret_borrow<object>(src);						\
		auto Type = module::import("xbimer").attr("cast").attr(#scls);	\
		if (!obj.get_type().is(Type)) {									\
			return false;												\
		}																\
		auto ncls=obj.cast<scls>();										\
		type x = static_cast<type>(ncls.GetX());						\
		type y = static_cast<type>(ncls.GetY());						\
		this->value = dcls(x, y);										\
		return true;													\
	}																	\
	static handle cast(dcls src, return_value_policy /* policy */, handle /* parent */) {	\
		auto Type = module::import("xbimer").attr("cast").attr(#scls);	\
		type x = static_cast<type>(src.GetX());							\
		type y = static_cast<type>(src.GetY());							\
		return Type(x, y).release();									\
	}																	\
	PYBIND11_TYPE_CASTER(dcls, _("xbimer.cast."#scls));					\
}

#define NPOINT_CASTER_FACTORY(type1,type2,dcls,scls)					\
template<> class type_caster<dcls> {									\
public:																	\
	bool load(handle src, bool /*convert*/) {							\
		if (!src) {														\
			return false;												\
		}																\
		auto obj = reinterpret_borrow<object>(src);						\
		auto Type = module::import("xbimer").attr("cast").attr(#scls);	\
		if (!obj.get_type().is(Type)) {									\
			return false;												\
		}																\
		auto ncls=obj.cast<scls>();										\
		type1 x = static_cast<type1>(ncls.GetX());						\
		type1 y = static_cast<type1>(ncls.GetY());						\
		this->value = dcls(type2(x), type2(y));							\
		return true;													\
	}																	\
	static handle cast(dcls src, return_value_policy /* policy */, handle /* parent */) {	\
		auto Type = module::import("xbimer").attr("cast").attr(#scls);	\
		type2 x = static_cast<type2>(src.GetX());						\
		type2 y = static_cast<type2>(src.GetY());						\
		return Type(type1(x.GetValue()), type1(y.GetValue())).release();\
	}																	\
	PYBIND11_TYPE_CASTER(dcls, _("xbimer.cast."#scls));					\
}

#define RECT_CASTER_FACTORY(type,dcls,scls)								\
template<> class type_caster<dcls> {									\
public:																	\
	bool load(handle src, bool /*convert*/) {							\
		if (!src) {														\
			return false;												\
		}																\
		auto obj = reinterpret_borrow<object>(src);						\
		auto Type = module::import("xbimer").attr("cast").attr(#scls);	\
		if (!obj.get_type().is(Type)) {									\
			return false;												\
		}																\
		auto ncls=obj.cast<scls>();										\
		type left = static_cast<type>(ncls.GetLeft());					\
		type top = static_cast<type>(ncls.GetTop());					\
		type right = static_cast<type>(ncls.GetRight());				\
		type bottom = static_cast<type>(ncls.GetBottom());				\
		this->value = dcls(left,top,right,bottom);						\
		return true;													\
	}																	\
	static handle cast(dcls src, return_value_policy /* policy */, handle /* parent */) {	\
		auto Type = module::import("xbimer").attr("cast").attr(#scls);	\
		type left = static_cast<type>(src.GetLeft());					\
		type top = static_cast<type>(src.GetTop());						\
		type right = static_cast<type>(src.GetRight());					\
		type bottom = static_cast<type>(src.GetBottom());				\
		return Type(left, top,right,bottom).release();					\
	}																	\
	PYBIND11_TYPE_CASTER(dcls, _("xbimer.cast."#scls));					\
}

#define NRECT_CASTER_FACTORY(type1,type2,dcls,scls)					\
template<> class type_caster<dcls> {									\
public:																	\
	bool load(handle src, bool /*convert*/) {							\
		if (!src) {														\
			return false;												\
		}																\
		auto obj = reinterpret_borrow<object>(src);						\
		auto Type = module::import("xbimer").attr("cast").attr(#scls);	\
		if (!obj.get_type().is(Type)) {									\
			return false;												\
		}																\
		auto ncls=obj.cast<scls>();										\
		type1 left = static_cast<type1>(ncls.GetLeft());				\
		type1 top = static_cast<type1>(ncls.GetTop());					\
		type1 right = static_cast<type1>(ncls.GetRight());				\
		type1 bottom = static_cast<type1>(ncls.GetBottom());			\
		this->value = dcls(type2(left), type2(top),type2(right),type2(bottom));				\
		return true;													\
	}																	\
	static handle cast(dcls src, return_value_policy /* policy */, handle /* parent */) {	\
		auto Type = module::import("xbimer").attr("cast").attr(#scls);	\
		type2 left = static_cast<type2>(src.GetLeft());				\
		type2 top = static_cast<type2>(src.GetTop());					\
		type2 right = static_cast<type2>(src.GetRight());				\
		type2 bottom = static_cast<type2>(src.GetBottom());			\
		return Type(type1(left.GetValue()), type1(top.GetValue()),type1(right.GetValue()),type1(bottom.GetValue())).release();	\
	}																	\
	PYBIND11_TYPE_CASTER(dcls, _("xbimer.cast."#scls));					\
}


using namespace xir::cast;

namespace pybind11 {
	namespace detail {
		POINT_CASTER_FACTORY(float, GS::Point<float>, FPoint);
		POINT_CASTER_FACTORY(double, GS::Point<double>, FPoint);
		POINT_CASTER_FACTORY(short, DG::Point, Point);
		NPOINT_CASTER_FACTORY(int64_t, DG::NativeUnit, DG::NativePoint, Point);
		RECT_CASTER_FACTORY(float, GS::Rect<float>, FRect);
		RECT_CASTER_FACTORY(double, GS::Rect<double>, FRect);
		RECT_CASTER_FACTORY(Int32, GS::Rect<Int32>, Rect);
		RECT_CASTER_FACTORY(short, DG::Rect, Rect);
		NRECT_CASTER_FACTORY(int64_t, DG::NativeUnit, DG::NativeRect, Rect);

		template <> class type_caster<GS::UniString> {
		public:
			bool load(handle src, bool /*convert*/) {
				if (!src) {
					return false;
				}
				else if (!PyUnicode_Check(src.ptr()) && PYBIND11_BYTES_CHECK(src.ptr())) {
					const char *bytes = PYBIND11_BYTES_AS_STRING(src.ptr());
					if (bytes) {
						value = GS::UniString(bytes, (size_t)PYBIND11_BYTES_SIZE(src.ptr()), GSCharCode::CC_System);
						return true;
					}
				}

				object utfNbytes = reinterpret_steal<object>(PyUnicode_AsEncodedString(src.ptr(), "utf-8", nullptr));
				if (!utfNbytes) {
					PyErr_Clear();
					return false;
				}
				const char *buffer = reinterpret_cast<const char *>(PYBIND11_BYTES_AS_STRING(utfNbytes.ptr()));
				size_t length = (size_t)PYBIND11_BYTES_SIZE(utfNbytes.ptr()) / sizeof(char);
				value = GS::UniString(buffer, (GS::USize)length, CC_UTF8);
				return true;
			}

			static handle cast(GS::UniString src, return_value_policy /* policy */, handle /* parent */) {
				const char *buffer = reinterpret_cast<const char *>(src.ToCStr().Get());
				GS::Int32 nbytes = strlen(buffer);
				return PyUnicode_DecodeUTF8(buffer, nbytes, nullptr);
			}

			PYBIND11_TYPE_CASTER(GS::UniString, _(PYBIND11_STRING_NAME));
		};

		template <> class type_caster<IO::Location> {
		public:
			bool load(handle src, bool /*convert*/) {
				if (!src) {
					return false;
				}
				else if (!PyUnicode_Check(src.ptr()) && PYBIND11_BYTES_CHECK(src.ptr())) {
					const char *bytes = PYBIND11_BYTES_AS_STRING(src.ptr());
					if (bytes) {
						GS::UniString path(bytes, (size_t)PYBIND11_BYTES_SIZE(src.ptr()), GSCharCode::CC_System);
						this->value = IO::Location(path);
						return true;
					}
				}

				object utfNbytes = reinterpret_steal<object>(PyUnicode_AsEncodedString(src.ptr(), "utf-8", nullptr));
				if (!utfNbytes) {
					PyErr_Clear();
					return false;
				}
				const char *buffer = reinterpret_cast<const char *>(PYBIND11_BYTES_AS_STRING(utfNbytes.ptr()));
				size_t length = (size_t)PYBIND11_BYTES_SIZE(utfNbytes.ptr()) / sizeof(char);
				GS::UniString path(buffer, (GS::USize)length, CC_UTF8);
				this->value= IO::Location(path);
				return true;
			}

			static handle cast(IO::Location src, return_value_policy /* policy */, handle /* parent */) {
				const char *buffer = reinterpret_cast<const char *>(src.ToDisplayText().ToCStr().Get());
				GS::Int32 nbytes = strlen(buffer);
				return PyUnicode_DecodeUTF8(buffer, nbytes, nullptr);
			}

			PYBIND11_TYPE_CASTER(IO::Location, _(PYBIND11_STRING_NAME));
		};

		template <typename ArrayType, typename Value, bool Resizable, USize Size = 0> struct gs_array_caster {
			using value_conv = make_caster<Value>;
		private:
			template <bool R = Resizable>
			bool require_size(enable_if_t<R, USize> size) {
				if (value.GetSize() != size)
					value.EnsureSize(size);
				return true;
			}
			template <bool R = Resizable>
			bool require_size(enable_if_t<!R, USize> size) {
				return size == Size;
			}
		public:
			bool load(handle src, bool convert) {
				if (!isinstance<sequence>(src))
					return false;
				auto l = reinterpret_borrow<sequence>(src);
				if (!require_size(l.size()))
					return false;
				USize ctr = 0;
				for (auto it : l) {
					value_conv conv;
					if (!conv.load(it, convert))
						return false;
					value[ctr++] = cast_op<Value &&>(std::move(conv));
				}
				return true;
			}

			template <typename T>
			static handle cast(T &&src, return_value_policy policy, handle parent) {
				list l(src.GetSize());
				size_t index = 0;
				for (auto &&value : src) {
					auto value_ = reinterpret_steal<object>(value_conv::cast(forward_like<T>(value), policy, parent));
					if (!value_)
						return handle();
					PyList_SET_ITEM(l.ptr(), (ssize_t)index++, value_.release().ptr()); // steals a reference
				}
				return l.release();
			}

			PYBIND11_TYPE_CASTER(ArrayType, _("List[") + value_conv::name + _<Resizable>(_(""), _("[") + _<Size>() + _("]")) + _("]"));
		};
	
		template <typename Type> struct type_caster<GS::Array<Type>>
			: gs_array_caster<GS::Array<Type>, Type, true> {
		};
	}
}


#undef POINT_CASTER_FACTORY
#undef NPOINT_CASTER_FACTORY
#undef RECT_CASTER_FACTORY
#undef NRECT_CASTER_FACTORY
#endif // !__CAST_COMMON_H__