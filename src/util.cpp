#include "util.h"
#include <stdlib.h>

u8* get_file_name_from_file_path(u8* file_path)
{
	u32 i = 0;
	u8* file_name_pos = file_path;

	while (file_path[i] != 0)
	{
		if (file_path[i] == '/' || file_path[i] == '\\')
			file_name_pos = file_path + i + 1;

		++i;
	}

	return file_name_pos;
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

#define LOG_LEVEL_FATAL 0
#define LOG_LEVEL_ERROR 1
#define LOG_LEVEL_WARN 2
#define LOG_LEVEL_SUCCESS 3

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

void log_success(char* msg)
{
	HANDLE cout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	int written = 0;
	s16 att = set_error_color(LOG_LEVEL_SUCCESS, cout_handle);
	WriteConsoleA(cout_handle, msg, (DWORD)strlen(msg), (LPDWORD)&written, 0);
	reset_error_color(att, cout_handle);
}

void log_msg(char* msg) {
	HANDLE cout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	int written = 0;
	WriteConsoleA(cout_handle, msg, (DWORD)strlen(msg), (LPDWORD)&written, 0);
}

void log_msg_size(char* msg, s32 size)
{
	HANDLE cout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	int written = 0;
	WriteConsoleA(cout_handle, msg, size, (LPDWORD)&written, 0);
}

int u32_to_str_base10(u32 val, char* buffer)
{
	u32 aux = val;
	u32 divisor = 1;
	char numbuffer[32] = { 0 };
	char* at = &numbuffer[31];
	int count = 0;

	if (val == 0) {
		*buffer = '0';
		count++;
		return count;
	}

	u32 accumulated = aux;
	while (accumulated) {
		u32 v = (aux / divisor) % 10;
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

int u64_to_str_base10(u64 val, char* buffer)
{
	u64 aux = val;
	u64 divisor = 1;
	char numbuffer[64] = { 0 };
	char* at = &numbuffer[63];
	int count = 0;

	if (val == 0) {
		*buffer = '0';
		count++;
		return count;
	}

	u64 accumulated = aux;
	while (accumulated) {
		u64 v = (aux / divisor) % 10;
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

int s64_to_str_base10(s64 val, char* buffer)
{
	s64 aux = (val < 0) ? -val : val;
	s64 divisor = 1;
	if (val < 0) *buffer++ = '-';

	char numbuffer[64] = { 0 };
	char* at = &numbuffer[63];
	int count = 0;

	if (val == 0) {
		*buffer = '0';
		count++;
		return count;
	}

	s64 accumulated = aux;
	while (accumulated) {
		s64 v = (aux / divisor) % 10;
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

int u64_to_str_base16(u64 val, bool leading_zeros, char* buffer)
{
	char numbuffer[64] = { 0 };
	char* at = &numbuffer[63];
	int count = 0;

	if (val == 0 && !leading_zeros) {
		*buffer = '0';
		count++;
		return count;
	}

	u64 mask = 0x0000000f;
	u64 auxmask = 0xffffffffffffffff;
	while (count < 16) {
		if (!(auxmask & val) && !leading_zeros) break;
		u32 v = (val & (mask << (count * 4))) >> (count * 4);
		if (v >= 0x0A) v += 0x37;
		else v += 0x30;
		*at-- = v;
		auxmask &= ~mask << (count * 4);
		count++;
	}
	for (int i = 0; i < count; ++i) {
		*buffer++ = *(++at);
	}
	return count;
}

int u8_to_str_base16(u8 val, bool leading_zeros, char* buffer)
{
	int count = 0;
	u8 v1 = val & 0x0f;
	u8 v2 = (val & 0xf0) >> 4;
	if (v2 == 0 && leading_zeros) {
		*buffer++ = 0x30;
		count++;
	}
	else {
		*buffer++ = (v2 >= 0x0A) ? v2 + 0x37 : v2 + 0x30;
		count++;
	}
	*buffer++ = (v1 >= 0x0A) ? v1 + 0x37 : v1 + 0x30;
	count++;
	return count;
}

int u32_to_str_base16(u32 val, bool leading_zeros, char* buffer)
{
	char numbuffer[64] = { 0 };
	char* at = &numbuffer[63];
	int count = 0;

	if (val == 0) {
		*buffer = '0';
		count++;
		return count + 1;
	}

	u32 mask = 0x0000000f;
	u32 auxmask = 0xffffffff;
	while (count < 8) {
		if (!(auxmask & val) && !leading_zeros) break;
		u32 v = (val & (mask << (count * 4))) >> (count * 4);
		if (v >= 0x0A) v += 0x37;
		else v += 0x30;
		*at-- = v;
		auxmask &= ~mask << (count * 4);
		count++;
	}
	for (int i = 0; i < count; ++i) {
		*buffer++ = *(++at);
	}
	return count;
}

bool is_mem_equal(u8* str1, u8* str2, s64 size)
{
	s64 i;
	for (i = 0; i<size; i++)
		if (str1[i] != str2[i])
			return false;

	return true;
}

bool is_string_equal(u8* str1, u8* str2)
{
	u64 i = 0;
	char c1, c2;

	do
	{
		c1 = str1[i];
		c2 = str2[i];
		if (c1 != c2) return false;
		++i;
	} while (c1 != 0 && c2 != 0);

	return true;
}

static double frequency_counter = 0.0;

void init_timer() {
	LARGE_INTEGER li;
	QueryPerformanceFrequency(&li);
	frequency_counter = (double)li.QuadPart;
}

double get_time() {
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return (double)li.QuadPart / frequency_counter;
}

string::string()
{
	capacity = 0;
	length = 0;
	data = 0;
}

string::string(s64 capac, size_t strlen, char* str)
{
	capacity = capac;
	length = strlen;
	data = (char*)malloc(capacity);
	if (strlen) strncpy(data, str, strlen);
}

string::~string()
{
	capacity = 0;
	length = 0;
	if (capacity > 0) free(data);
	data = 0;
}

void string::cat(string& r)
{
	if (length + r.length >= capacity)
	{
		size_t newcap = capacity * 2 + r.length;
		capacity = newcap;
		data = (char*)realloc(data, newcap);
	}
	memcpy(&data[length], r.data, r.length);
	length += r.length;
}

void string::cat(char* str, size_t len)
{
	if (length + len >= capacity)
	{
		size_t newcap = capacity * 2 + len;
		capacity = newcap;
		data = (char*)realloc(data, newcap);
	}
	memcpy(&data[length], str, len);
	length += len;
}

void string::cat(s64 num)
{
	char buffer[128];
	s64 m = 10;
	int i = 0;
	for (; num; ++i)
	{
		s64 res = num % m;
		s64 nres = res / (m / 10);
		buffer[64 - i] = nres + 0x30;
		num -= res;
		m *= 10;
	}
	int len = 64 - i;

	if (length + len >= capacity)
	{
		size_t newcap = capacity * 2 + len;
		capacity = newcap;
		data = (char*)realloc(data, newcap);
	}
	memcpy(&data[length], buffer, len);
	length += len;
}

bool string::is_mutable()
{
	if (capacity == -1) return false;
	return true;
}


string make_new_string(s64 capacity)
{
	string res;
	res.capacity = capacity;
	res.length = 0;
	res.data = (char*)malloc(res.capacity);
	return res;
}

string make_new_string(const char* val)
{
	string res;
	size_t len = strlen(val);
	res.capacity = len + 1;
	res.length = len;
	if (len) {
		res.data = (char*)malloc(res.capacity);
		strncpy(res.data, val, len);
	}
	return res;
}

void make_immutable_string(string& s, const char* val, s64 length)
{
	s.capacity = -1;
	// yes i dont give a damn I say when it is or it is'nt const
	s.data = (char*)val;
	s.length = length;
}

void make_immutable_string(string& s, const char* val)
{
	s.capacity = -1;
	// yes i dont give a damn I say when it is or it is'nt const
	s.data = (char*)val;
	s.length = strlen(val);
}

void make_immutable_string(string& dest, string& src)
{
	dest.capacity = -1;
	dest.data = src.data;
	dest.length = src.length;
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