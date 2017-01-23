#include "system.h"

#include "util/string.h"

inline char* strdup(const char *s)
{
	int len = strlen(s) + 1;
	uchar *newstr = (uchar*)malloc(len);
	strncpy((char*)newstr, s, len);
	newstr[len - 1] = '\0';
	return (char*)newstr; 
}

string_t* string_new()
{
	string_t *string = (string_t*)malloc(sizeof(string_t));
	string->capacity = STRING_LENGTH;
	string->len = 0;
	string->s = (uchar*)malloc(string->capacity + 1);
	return string;
}

string_t* string_new_count(size_t count)
{
	string_t *string = (string_t*)malloc(sizeof(string_t));
	string->capacity = count;
	string->len = 0;
	string->s = (uchar*)malloc(string->capacity + 1);
	return string;
}

string_t* string_new_string(const string_t* s)
{
	string_t *string = (string_t*)malloc(sizeof(string_t));
	string->capacity = s->capacity;
	string->len = s->len;
	string->s = (uchar*)strdup((const char*)s->s);
	return string;
}


string_t* string_new_char(const char* s)
{
	string_t *string = (string_t*)malloc(sizeof(string_t));
	string->capacity = strlen(s);
	string->len = string->capacity;
	string->s = (uchar*)strdup(s);
	return string;
}

void string_delete(string_t* s)
{
	free(s->s);
	free(s);
}

size_t string_size(const string_t* s)
{
	if (!s) {
		return 0;
	}
	return s->len;
}

void string_resize(string_t* str, size_t newSize)
{
	if (newSize > str->capacity) {
		uchar *newStr = (uchar*)realloc(str->s, sizeof(uchar) * newSize + 1);
		if (newStr) {
			str->s = newStr;
			str->capacity = newSize;
		}
	} else if (newSize < str->capacity) {
		free(str->s + newSize*sizeof(uchar*) + 1);
		str->capacity = newSize;
		str->s[newSize] = '\0';
		if (str->len > str->capacity) {
			str->len = str->capacity;
		}
	}
}

inline const char* string_cstr(const string_t* s)
{
	return (const char*)s->s;
}

inline uchar string_get(const string_t* s, size_t at)
{
	return s->s[at];
}

uchar string_at(const string_t* s, size_t at)
{
	if (at >= s->len) {
		return 0;
	}
	return s->s[at];
}

string_t* string_assign_char(string_t* s, const char* as)
{
	const int l = strlen(as);
	if (s->capacity < l) {
		string_resize(s, l);
	}
	memcpy(s->s, as, l);
	s->s[l] = '\0';
	s->len = l;
	return s;
}

string_t* string_append_char(string_t* s, const char* app)
{
	size_t l = strlen(app);
	if (s->capacity < s->len + l) {
		string_resize(s, s->len + l);
	}
	strncpy(((char*)(&s->s[s->len])), app, l);
	s->s[s->len + l] = '\0';
	s->len += l;
	return s;
}

string_t* string_append_string(string_t* s, const string_t* app)
{
	return string_append_char(s, (const char*)app->s);
}

void string_puch_back(string_t* s, const uchar c)
{
	if (s->len == s->capacity) {
		string_resize(s, s->capacity * 2);
	}
	s->s[s->len] = c;
	++s->len;
}

string_t* string_insert_char(string_t* s, size_t at, const char* ins)
{
	if (at >= s->len) { // special case
		string_append_char(s, ins);
	}
	size_t l = strlen(ins);
	if (s->capacity < s->len + l) {
		string_resize(s, s->len + l);
	}
	for (size_t i = s->len; i >= at; --i) {
		s->s[i + l] = s->s[i];
	}
	for (size_t i = at; i <= s->len; ++i) {
		s->s[i] = ins[i - at];
	}
	s->len += l;
	return s;
}

inline string_t* string_insert_string(string_t* s, size_t at, const string_t* ins)
{
	return string_insert_char(s, at, (const char*)ins->s);
}

string_t* string_erase(string_t* s, size_t at, size_t len)
{
	size_t count = 0;
	for (size_t i = at; i < s->len && i + len <= s->len; ++i) {
		s->s[i] = s->s[i + len];
		++count;
	}
	s->s[at + count] = '\0';
	s->len = strlen((char*)s->s);
	
	return s;
}

string_t* string_copy(string_t* s, const string_t* copy, size_t at, size_t len)
{
	if (s->capacity < len) {
		string_resize(s, len);
	}
	
	size_t count = 0;
	for (size_t i = at; i < at + len && i < copy->len; ++i) {
		s->s[count] = copy->s[i];
		++count;
	}
	
	s->s[count] = '\0';
	s->len = count;
	return s;
}

char string_find_c(const string_t* s, char c, size_t start)
{
	for (size_t i = start; i < s->len; ++i) {
		if (s->s[i] == (uchar)c) {
			return i;
		}
	}
	return STRING_NPOS;
}

size_t string_find_first_of_char(const string_t* s, const char* c, size_t start)
{
	size_t l = strlen(c);
	for (size_t i = start; i < s->len; ++i) {
		for (size_t j = 0; j < l; ++j) {
			if (s->s[i] == c[j]) {
				return i;
			}
		}
	}
	return STRING_NPOS;
}

string_t* string_substr(const string_t* s, size_t pos, size_t len)
{
	string_t *sub = string_new_count(s->len);
	if (len == STRING_NPOS) {
		len = s->len;
	}
	
	for (size_t i = pos; i < s->len && i < pos + len; ++i) {
		sub->s[i - pos] = s->s[i];
	}
	return sub;
}


