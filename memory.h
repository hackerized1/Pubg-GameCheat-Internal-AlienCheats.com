#pragma once
#include "sdk.h"
using address64 = uintptr_t;
using offset64 = uintptr_t;
class Memory
{
public:
	template <typename Type, typename Base, typename Offset>
	static inline Type Ptr(Base base, Offset offset)
	{
		static_assert(std::is_pointer<Type>::value || std::is_integral<Type>::value, "Type must be a pointer or address");
		static_assert(std::is_pointer<Base>::value || std::is_integral<Base>::value, "Base must be a pointer or address");
		static_assert(std::is_pointer<Offset>::value || std::is_integral<Offset>::value, "Offset must be a pointer or address");

		return reinterpret_cast<Type>(base + offset);
	}
	static bool IsValidPtr(int64_t ptr)
	{
		return (ptr && (long long)ptr > 0x220000001 && (long long)ptr < 0xFFFFFFFFFFFF);
	}
	static address64 GetOffset(const address64 MainInstance, const address64 current_address, const std::int32_t relative = 4)
	{
		const auto relative_address = current_address + *reinterpret_cast<std::int32_t*>(current_address) + relative;
		return relative_address - MainInstance;
	}
};