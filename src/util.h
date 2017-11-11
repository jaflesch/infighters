#pragma once
#include "common.h"

#define FLAG(N) (1 << N)

#define FOR(N) for(int i = 0; i < N; ++i)
#define MAX(L, R) ((L > R) ? L : R)
#define MIN(L, R) ((L < R) ? L : R)
#define assert(EXP) if(!(EXP)) { printf("Assertion failed at %s (%d)", __FILE__, __LINE__); DebugBreak(); }

#define E_32 2.7182818284f
#define MINS32 -2147483648
#define MAXS32 2147483647
#define MAXS64 9223372036854775807
#define MINS64 -9223372036854775808
#define MAXU64 18446744073709551615

u8* read_entire_file(u8* filename, s64* out_size);

void error_fatal(char* error_type, char* buffer);
void error_warning(char* error);
void log_success(char* msg);

#define CLAMP_DOWN(V, MIN) ((V < MIN) ? MIN : V)
#define CLAMP_UP(V, MAX) ((V > MAX) ? MAX : V)
#define CLAMP(V, MIN, MAX) ((V < MIN) ? MIN : (V > MAX) ? MAX : V)

// misc

int u32_to_str_base10(u32 val, char* buffer);
int u64_to_str_base10(u64 val, char* buffer);
int s32_to_str_base10(s32 val, char* buffer);
int s64_to_str_base10(s64 val, char* buffer);
int u64_to_str_base16(u64 val, bool leading_zeros, char* buffer);
int u8_to_str_base16(u8 val, bool leading_zeros, char* buffer);

#define internal static

internal bool is_white_space(char str)
{
	if (str == ' ' ||
		str == '\t' ||
		//str == '\n' ||
		str == '\v' ||
		str == '\f' ||
		str == '\r')
	{
		return true;
	}
	return false;
}

internal bool is_number(char c)
{
	if (c >= '0' && c <= '9')
		return true;
	return false;
}

internal bool is_letter(char c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return true;
	return false;
}

bool is_string_equal(u8* str1, u8* str2);

//
//	capacity -1 means immutable string
//
struct string
{
	s64 capacity;
	size_t length;

	char* data;

	string();
	string(s64 capac, size_t strlen, char* str);
	~string();
	void cat(string& r);
	void cat(char* str, size_t len);
	void cat(s64 num);

	bool is_mutable();
};

string make_new_string(const char*);
string make_new_string(s64 capacity);
void make_immutable_string(string& s, const char* val, s64 length);
void make_immutable_string(string& s, const char* val);
void make_immutable_string(string& dest, string& src);

bool str_equal(const char* str1, int str1_size, const char* str2, int str2_size);

#undef internal

void init_timer();
double get_time();
