#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED

/*----------------------------------------------------------------------------
--  Includes
----------------------------------------------------------------------------*/

#include <string.h>

/*----------------------------------------------------------------------------
-- Defines
----------------------------------------------------------------------------*/

#define STRING_LENGTH 32
#define STRING_NPOS ((size_t)0xFFFFFFFF)

/*----------------------------------------------------------------------------
-- Types
----------------------------------------------------------------------------*/

typedef char uchar;

typedef struct string_s {
	uchar *s;
	size_t len;
	size_t capacity;
} string_t;

/*----------------------------------------------------------------------------
--  Functions
----------------------------------------------------------------------------*/

inline char* strdup(const char *s);

string_t* string_new();
string_t* string_new_count(size_t count);
string_t* string_new_string(const string_t *s);
string_t* string_new_char(const char *s);
void string_delete(string_t *s);

size_t string_size(const string_t *s);

inline const char* string_cstr(const string_t *s);
inline uchar string_get(const string_t *s, size_t at);
uchar string_at(const string_t *s, size_t at);

string_t* string_assign_char(string_t* s, const char* as);
#define string_assign_string(_s, _as) string_assign_char((_s), (const char*)((_as)->s))
string_t* string_append_char(string_t *s, const char *app);
inline string_t* string_append_string(string_t *s, const string_t *app);
void string_push_back(string_t *s, const uchar c);
string_t* string_insert_char(string_t *s, size_t at, const char *ins);
inline string_t* string_insert_string(string_t *s, size_t at, const string_t *ins);
string_t* string_erase(string_t *s, size_t at, size_t len);
string_t* string_copy(string_t *s, const string_t *copy, size_t at, size_t len);

size_t string_find_c(const string_t *s, char c, size_t start);
size_t string_find_first_of_char(const string_t *s, const char *c, size_t start);
string_t* string_substr(const string_t *s, size_t pos, size_t len);


#endif // STRING_H_INCLUDED

