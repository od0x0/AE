#ifndef __LUAGL_UTIL_H__
#define __LUAGL_UTIL_H__


typedef struct luaglConst  
{
  const char *str;
  unsigned int value;
} luaglConst;

void luagl_initconst(lua_State *L, const luaglConst *gl_const);
unsigned int luagl_get_enum(lua_State *L, int index, const luaglConst* gl_const);
unsigned int luagl_get_gl_enum(lua_State *L, int index);
int luagl_checkboolean (lua_State *L, int narg);

#define LUAGL_ENUM_ERROR (unsigned int)-2

/*#define __luagl_debug_memleak 1*/

void *luagl_malloc(size_t s);
void luagl_free(void *v);

/* returns an parray with given type and size */
#define LUAGL_NEW_ARRAY(type, size) ( (type *)luagl_malloc((size) * sizeof(type)) )

/* returns a bi-dimensional parray with given type and size */
#define LUAGL_NEW_ARRAY2(type, size1, size2) ( (type *)luagl_malloc((size1) * (size2) * sizeof(type)) )

/* frees the space for the given parray, must be called together with LUAGL_NEW_ARRAY or LUAGL_NEW_ARRAY2 */
#define LUAGL_DELETE_ARRAY(parray) { if(parray) {luagl_free(parray);} }

/* Returns an array in a lua table. */
void luagl_push_arrayuc(lua_State *L, GLubyte* parray, int size);
void luagl_push_arrayb(lua_State *L, GLubyte* parray, int size);  /* boolean */
void luagl_push_arrayui(lua_State *L, GLuint* parray, int size);
void luagl_push_arrayi(lua_State *L, GLint* parray, int size);
void luagl_push_arrayf(lua_State *L, GLfloat* parray, int size);
void luagl_push_arrayd(lua_State *L, GLdouble* parray, int size);

/* Gets an parray from a lua table, store it in 'parray' and returns the no. of elems of the parray
   index refers to where the table is in stack. */
int luagl_get_arrayuc(lua_State *L, int index, GLubyte **parray);
int luagl_get_arrayb(lua_State *L, int index, GLubyte **parray);  /* boolean */
int luagl_get_arrayi(lua_State *L, int index, GLint **parray);
int luagl_get_arrayui(lua_State *L, int index, GLuint **parray);
int luagl_get_arrayd(lua_State *L, int index, GLdouble **parray);
int luagl_get_arrayf(lua_State *L, int index, GLfloat **parray);

int luagl_get_array2uc(lua_State *L, int index, GLubyte **parray, int *size);
int luagl_get_array2d(lua_State *L, int index, GLdouble **parray, int *size);
int luagl_get_array2f(lua_State *L, int index, GLfloat **parray, int *size);

int luagl_get_arrayb_pointer(lua_State *L, int index, GLubyte **parray);  /* boolean */
int luagl_get_arrayd_pointer(lua_State *L, int index, GLdouble **parray);
int luagl_get_array2d_pointer(lua_State *L, int index, GLdouble **parray, int *size);

int luagl_str2mask(const char *str);
const char *luagl_mask2str(int mask);

#endif