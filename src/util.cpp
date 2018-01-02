#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>


void* memory_alloc(s64 size)
{
	if(size > 0)
		return calloc(1, size);
	return 0;
}

void memory_free(void* block) 
{
	free(block);
}

void memory_copy(void* destination, void* origin, s64 size) {
	if(size > 0)
		memcpy(destination, origin, size);
}

void string_copy(s8* destination, s8* origin)
{
	strcpy(destination, origin);
}

s32 string_compare(s8* str1, s8* str2)
{
	return strcmp(str1, str2);
}

void print(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
}

string make_string(s8* v) {
	string res;
	s64 length = strlen(v);
	res.data = (u8*)memory_alloc(length + 1);
	memcpy(res.data, v, length + 1);
	res.length = length;
	res.data[length] = 0;
	return res;
}

string make_string(s8* v, s64 length) {
	string res;
	res.data = (u8*)memory_alloc(length + 1);
	memcpy(res.data, v, length + 1);
	res.length = length;
	res.data[length] = 0;
	return res;
}

string make_string(Memory_Arena* arena, s8* v) {
	string res;
	s64 length = strlen(v);
	res.data = (u8*)arena_alloc(arena, length + 1);
	memcpy(res.data, v, length + 1);
	res.length = length;
	res.data[length] = 0;
	return res;
}

string make_string(Memory_Arena* arena, s8* v, s64 length) {
	string res;
	res.data = (u8*)arena_alloc(arena, length + 1);
	memcpy(res.data, v, length + 1);
	res.length = length;
	res.data[length] = 0;
	return res;
}

void free_string(string* s) {
	memory_free(s->data);
	s->data = 0;
	s->length = 0;
}

string concat(string* left, string* right, bool _free) {
	string res;
	s64 length = left->length + right->length;
	res.data = (u8*)memory_alloc(length + 1);
	res.length = length;
	res.data[length] = 0;
	memory_copy(res.data, left->data, left->length);
	memory_copy(res.data + left->length, right->data, right->length);
	if (_free) {
		free(left->data);
		free(right->data);
		left->data = 0;
		right->data = 0;
		left->length = 0;
		right->length = 0;
	}
	return res;
}

string concat(string* left, const s8* right, bool _free) {
	string res;
	s64 right_length = strlen(right);
	s64 length = left->length + right_length;
	res.data = (u8*)memory_alloc(length + 1);
	res.length = length;
	res.data[length] = 0;
	memory_copy(res.data, left->data, left->length);
	memory_copy(res.data + left->length, (void*)right, right_length);
	if (_free) {
		free(left->data);
		left->data = 0;
		left->length = 0;
	}
	return res;
}

string concat(s8* left, string* right, bool _free) {
	string res;
	s64 left_length = strlen(left);
	s64 length = right->length + left_length;
	res.data = (u8*)memory_alloc(length + 1);
	res.length = length;
	res.data[length] = 0;
	memory_copy(res.data, left, left_length);
	memory_copy(res.data + left_length, right->data, right->length);
	if (_free) {
		free(right->data);
		right->data = 0;
		right->length = 0;
	}
	return res;
}

string concat(s8* left, s8* right) {
	string res;
	s64 left_length = strlen(left);
	s64 right_length = strlen(right);
	s64 length = left_length + right_length;
	res.data = (u8*)memory_alloc(length + 1);
	res.length = length;
	res.data[length] = 0;
	memory_copy(res.data, left, left_length);
	memory_copy(res.data + left_length, right, right_length);
	return res;
}

bool equal(string* left, string* right) {
	s64 left_length = left->length;
	s64 right_length = right->length;
	if (left_length == right_length) {
		for (int i = 0; i < left_length; ++i) {
			if (left->data[i] != right->data[i])
				return false;
		}
		return true;
	}
	return false;
}

bool equal(string* left, const s8* right) {
	s64 left_length = left->length;
	s64 right_length = strlen(right);
	if (left_length == right_length) {
		for (int i = 0; i < left_length; ++i) {
			if (left->data[i] != right[i])
				return false;
		}
		return true;
	}
	return false;
}

bool equal(s8* left, string* right) {
	s64 left_length = strlen(left);
	s64 right_length = right->length;
	if (left_length == right_length) {
		for (int i = 0; i < left_length; ++i) {
			if (left[i] != right->data[i])
				return false;
		}
		return true;
	}
	return false;
}

bool equal(s8* left, s8* right) {
	s8* ptr = left;
	s8* ptr2 = right;
	while (*ptr) {
		if (*ptr++ != *ptr2++)
			return false;
	}

	if (*ptr2 == 0) return true;
	else return false;
}

bool is_white_space(char str)
{
	if (str == ' ' ||
		str == '\t' ||
		str == '\n' ||
		str == '\v' ||
		str == '\f' ||
		str == '\r')
	{
		return true;
	}
	return false;
}

bool is_number(char c) {
	if (c >= '0' && c <= '9') return true;
	return false;
}

bool is_letter(char c) {
	if (c >= 'A' && c <= 'Z')
		return true;
	if (c >= 'a' && c <= 'z')
		return true;
	return false;
}

bool str_equal(const char* str1, int str1_size, const char* str2, int str2_size)
{
	if (str1_size == str2_size) {
		for (int i = 0; i < str1_size; ++i) {
			if (str1[i] != str2[i])
				return false;
		}
		return true;
	}
	return false;
}

s32 str_to_s32(char* text, int length)
{
	s32 result = 0;
	s32 tenths = 1;
	for (int i = length - 1; i >= 0; --i, tenths *= 10)
		result += (text[i] - 0x30) * tenths;
	return result;
}

int s32_to_str_base10(s32 val, char* buffer)
{
	s32 aux = (val < 0) ? (u64)-val : (u64)val;
	s32 divisor = 1;
	if (val < 0) *buffer++ = '-';

	char numbuffer[32] = { 0 };
	char* at = &numbuffer[31];
	int count = 0;

	if (val == 0) {
		*buffer = '0';
		count++;
		return count;
	}

	s32 accumulated = aux;
	while (accumulated) {
		s32 v = (aux / divisor) % 10;
		accumulated -= v * divisor;
		*at-- = v + 0x30;
		divisor *= 10;
		count++;
	}
	for (int i = 0; i < count; ++i) {
		*buffer++ = *(++at);
	}
	if (val < 0) count++;
	return count;
}


r64 str_to_r64(string s)
{
	r64 result = 0.0;
	r64 tenths = 1.0;
	r64 frac_tenths = 0.1;
	int point_index = 0;

	while (s.data[point_index] != '.') {
		++point_index;
		if (!is_number(s.data[point_index]))
			break;
	}

	for (int i = point_index - 1; i >= 0; --i, tenths *= 10.0)
		result += (s.data[i] - 0x30) * tenths;
	for (int i = point_index + 1; i < s.length; ++i, frac_tenths *= 0.1)
		result += (s.data[i] - 0x30) * frac_tenths;
	return result;
}

r32 str_to_r32(string s)
{
	return (r32)str_to_r64(s);
}

u8 hex_from_ascii(u8 c) {
	if (c >= '0' && c <= '9')
		return c - 0x30;
	if (c >= 'A' && c <= 'F')
		return c - 0x37;
	if (c >= 'a' && c <= 'f')
		return c - 0x57;
	return 0;
}

s64 next_2_pow(s64 num) {
	s64 res = 2;
	while (res < num) {
		res <<= 1;
	}
	return res;
}


#define LOG_LEVEL_FATAL 0
#define LOG_LEVEL_ERROR 1
#define LOG_LEVEL_WARN 2
#define LOG_LEVEL_SUCCESS 3
#if defined (_WIN32) || defined (_WIN64)
s16 set_error_color(u32 level, HANDLE hConsole)
{
	CONSOLE_SCREEN_BUFFER_INFO console_info;
	GetConsoleScreenBufferInfo(hConsole, &console_info);

	switch (level) {
	case LOG_LEVEL_SUCCESS:
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;
	case LOG_LEVEL_FATAL:
		SetConsoleTextAttribute(hConsole, BACKGROUND_RED | BACKGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;
	case LOG_LEVEL_ERROR:
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
		break;
	case LOG_LEVEL_WARN:
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;
	}
	return console_info.wAttributes;
}

void reset_error_color(s16 attribute, HANDLE hConsole)
{
	SetConsoleTextAttribute(hConsole, attribute);
}

void error_fatal(char* error_type, char* buffer)
{
	HANDLE error_handle = GetStdHandle(STD_ERROR_HANDLE);
	int written = 0;
	s16 att = set_error_color(LOG_LEVEL_FATAL, error_handle);
	WriteConsoleA(error_handle, error_type, (DWORD)strlen(error_type), (LPDWORD)&written, 0);
	if (buffer) {
		WriteConsoleA(error_handle, buffer, (DWORD)strlen(buffer), (LPDWORD)&written, 0);
	}
	reset_error_color(att, error_handle);
	MessageBoxA(0, "A fatal error occurred, check log.\n", "Fatal Error", 0);
	//DebugBreak();
	ExitProcess(-1);
}

void error_warning(char* error)
{
	HANDLE error_handle = GetStdHandle(STD_ERROR_HANDLE);
	int written = 0;
	s16 att = set_error_color(LOG_LEVEL_WARN, error_handle);
	WriteConsoleA(error_handle, error, (DWORD)strlen(error), (LPDWORD)&written, 0);
	reset_error_color(att, error_handle);
}

u8* read_entire_file(u8* filename, s64* out_size)
{
	/* get file size */
	WIN32_FIND_DATA info;
	HANDLE search_handle = FindFirstFileEx((LPCSTR)filename, FindExInfoStandard, &info, FindExSearchNameMatch, NULL, FIND_FIRST_EX_LARGE_FETCH);
	if (search_handle == INVALID_HANDLE_VALUE) return 0;
	FindClose(search_handle);
	u64 file_size = (u64)info.nFileSizeLow | ((u64)info.nFileSizeHigh << 32);
	if (out_size) *out_size = file_size;
	/* open file */
	HANDLE fhandle = CreateFile((LPCSTR)filename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	int error = GetLastError();

	u64 maxs32 = 0x7fffffff;
	DWORD bytes_read = 0;
	if (file_size == 0) return 0;				// error file is empty
	void* memory = malloc(file_size);
	if (INVALID_HANDLE_VALUE == fhandle) return 0;

	if (file_size > maxs32) {
		void* mem_aux = memory;
		s64 num_reads = file_size / maxs32;
		s64 rest = file_size % maxs32;
		DWORD bytes_read;
		for (s64 i = 0; i < num_reads; ++i) {
			ReadFile(fhandle, mem_aux, (DWORD)maxs32, &bytes_read, 0);
			mem_aux = (char*)mem_aux + maxs32;
		}
		ReadFile(fhandle, mem_aux, (DWORD)rest, &bytes_read, 0);
	}
	else {
		ReadFile(fhandle, memory, (DWORD)file_size, &bytes_read, 0);
	}
	CloseHandle(fhandle);
	return (u8*)memory;
}
#elif defined(__linux__)
void error_fatal(char* error_type, char* buffer)
{
	printf("%s %s", error_type, buffer);
	exit(-1);
}

void error_warning(char* error)
{
	printf("%s", error);
	exit(-1);
}

#endif
