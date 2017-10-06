#pragma once
#include "common.h"

#ifdef __linux__
#include <signal.h>
#define __debugbreak() raise(SIGTRAP)
#endif
#define assert(X) if(!(X)) __debugbreak()

#define MAX(X, Y) ((X < Y) ? Y : X)
#define MIN(X, Y) ((X < Y) ? X : Y)

// Alloc memory 'size' bytes
extern void* memory_alloc(u32 size);
// Free alloc'd memory block
extern void memory_free(void* block);
// Copy 'size' bytes from origin to destination
extern void memory_copy(void* destination, void* origin, u32 size);
// Copy string from origin to destination, with \0
extern void string_copy(s8* destination, s8* origin);
// Compare two strings, 0 if equal
extern s32 string_compare(s8* str1, s8* str2);
// Print to stdout
extern void print(const char *fmt, ...);

struct string {
	s64 length;
	u8* data;

	string() {
		length = 0;
		data = 0;
	}
};

extern r32 str_to_r32(string str);
extern r64 str_to_r64(string str);

string make_string(s8* v);
string make_string(s8* v, s64 length);
void free_string(string* s);

string concat(string* left, string* right, bool _free = false);
string concat(string* left, const s8* right, bool _free = false);
string concat(s8* left, string* right, bool _free = false);
string concat(s8* left, s8* right);

bool equal(string* left, string* right);
bool equal(string* left, const s8* right);
bool equal(s8* left, string* right);
bool equal(s8* left, s8* right);

bool is_white_space(char str);
bool str_equal(const char* str1, int str1_size, const char* str2, int str2_size);

u8 hex_from_ascii(u8 c);