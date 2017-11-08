#include "debug_table.h"
#include <string.h>
#include "util.h"
#include <hmath.h>
#include "quaternion.h"

typedef long long s64;
typedef int s32;
typedef short s16;
typedef char s8;

typedef unsigned long long u64;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

DebugTable::DebugTable()
{
	memset(entries, 0, DBG_MAX_ENTRIES * sizeof(DebugTable_Entry));
	entry_index = 0;
}

void DebugTable::create_entry(char* name, void* address, Type type)
{
	entries[entry_index].data = address;
	entries[entry_index].name = name;
	entries[entry_index].type = type;
	entry_index++;
}

DebugTable_Entry DebugTable::find_in_entries(char* name) {
	for (int i = 0; i < entry_index; ++i) {
		if (is_string_equal((u8*)name, (u8*)entries[i].name)) {
			return entries[i];
		}
	}
	return DebugTable_Entry();
}

void DebugTable::modify(char* name, void* value, int offset, Type t)
{
	using namespace hm;
	DebugTable_Entry val = find_in_entries(name);
	if (!val.data) return;

	switch (t) {
		case TYPE_S64: *((s64*)val.data + offset) = *(s64*)value;					 break;
		case TYPE_S32: *((s32*)val.data + offset) = *(s32*)value;					 break;
		case TYPE_S16: *((s16*)val.data + offset) = *(s16*)value;					 break;
		case TYPE_S8:  *((s8*)val.data + offset) = *(s8*)value;						 break;

		case TYPE_U64: *((u64*)val.data + offset) = *(u64*)value;					 break;
		case TYPE_U32: *((u32*)val.data + offset) = *(u32*)value;					 break;
		case TYPE_U16: *((u16*)val.data + offset) = *(u16*)value;					 break;
		case TYPE_U8:  *((u8*)val.data  + offset) = *(u8*)value;					 break;

		case TYPE_R32: *((float*)val.data  + offset) = *(float*)value;				 break;
		case TYPE_R64: *((double*)val.data + offset) = *(double*)value;				 break;

		case TYPE_VEC2: *((vec2*)val.data + offset) = *(vec2*)value;				 break;
		case TYPE_VEC3: *((vec3*)val.data + offset) = *(vec3*)value;				 break;
		case TYPE_VEC4: *((vec4*)val.data + offset) = *(vec4*)value;				 break;

		case TYPE_QUAT: *((Quaternion*)val.data + offset) = *(Quaternion*)value;	 break;
	}
}

void DebugTable::get_value(char* name, std::stringstream* ss) {
	DebugTable_Entry val = find_in_entries(name);
	if (!val.data) return;

	switch (val.type) {
	case TYPE_S64:	*ss << "s64" << *(s64*)val.data; break;
	case TYPE_S32:	*ss << "s32" << *(s32*)val.data; break;
	case TYPE_S16:	*ss << "s16" << *(s16*)val.data; break;
	case TYPE_S8:	*ss << "s8" << *(s8*)val.data; break;

	case TYPE_U64:	*ss << "u64" << *(u64*)val.data; break;
	case TYPE_U32:	*ss << "u32" << *(u32*)val.data; break;
	case TYPE_U16:	*ss << "u16" << *(u16*)val.data; break;
	case TYPE_U8:	*ss << "u8" << *(u8*)val.data; break;

	case TYPE_R32:	*ss << "float" << *(float*)val.data; break;
	case TYPE_R64:	*ss << "double" << *(double*)val.data; break;

	case TYPE_VEC2:	*ss << "vec2 { " << (*(vec2*)val.data).x << ", " << (*(vec2*)val.data).y << " }";	break;
	case TYPE_VEC3:	*ss << "vec3 { " << (*(vec3*)val.data).x << ", " << (*(vec3*)val.data).y << ", " << (*(vec3*)val.data).z << " }"; break;
	case TYPE_VEC4:	*ss << "vec4 { " << (*(vec4*)val.data).x << ", " << (*(vec4*)val.data).y << ", " << (*(vec4*)val.data).z << ", " << (*(vec4*)val.data).w << " }"; break;

	case TYPE_QUAT:	*ss << "Quaternion { " << (*(Quaternion*)val.data).x << ", " << (*(Quaternion*)val.data).y << ", " << (*(Quaternion*)val.data).z << ", " << (*(Quaternion*)val.data).w << " }"; break;
	}
}