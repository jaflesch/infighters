#pragma once
#include <sstream>

enum Type {
	TYPE_UNKNOWN,
	TYPE_S64,
	TYPE_S32,
	TYPE_S16,
	TYPE_S8,
	TYPE_U64,
	TYPE_U32,
	TYPE_U16,
	TYPE_U8,
	TYPE_VEC2,
	TYPE_VEC3,
	TYPE_VEC4,
	TYPE_R32,
	TYPE_R64,
	TYPE_QUAT,

};

#define DBG_MAX_ENTRIES 2048
struct DebugTable_Entry {
	void* data = 0;
	char* name;
	Type type;
};

struct DebugTable {
	DebugTable_Entry entries[DBG_MAX_ENTRIES];
	int entry_index;

	DebugTable();
	void create_entry(char* name, void* address, Type type);
	void modify(char* name, void* value, int offset, Type t);
	DebugTable_Entry find_in_entries(char* name);
	void get_value(char* name, std::stringstream* ss);
};