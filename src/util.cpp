#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>


void* memory_alloc(u32 size)
{
	return calloc(1, size);
}

void memory_free(void* block) 
{
	free(block);
}

void memory_copy(void* destination, void* origin, u32 size)
{
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