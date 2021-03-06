// Copyright (c) 2015 Pavel Novy. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __EXAMPLE_TYPEINFO_H__
#define __EXAMPLE_TYPEINFO_H__

#include "example/example_export.h"

#include <stddef.h>
#include <string>

#ifdef EXAMPLE_TYPE_INFO_FULL_COMPARE
# include <cstring>
#endif

#define EXAMPLE_TYPE_INFO_TABLE_MAX_PARAM 5

namespace example {

/**
 * TypeInfoTable
 * holds type name and it's generic parameters.
 * TODO also store the sizeof for blind allocations.
 */
struct TypeInfoTable {
  const char *Name;
  const TypeInfoTable *Parameter[EXAMPLE_TYPE_INFO_TABLE_MAX_PARAM];
};

inline bool operator==(TypeInfoTable const &a, TypeInfoTable const &b) {
#ifndef EXAMPLE_TYPE_INFO_FULL_COMPARE
  /* it's safe to compare by pointer values only when TypeInfo
   * is declared once for each type. */
  return (&a == &b);
#else
  /* bail out if both points to same location */
  if (&a == &b) return true;

  /* compare names */
  if (strcmp(a.Name, b.Name) != 0) return false;

  /* types are both generic, so check their parameters */
  TypeInfoTable const *const *ap = &a.Parameter[0];
  TypeInfoTable const *const *bp = &a.Parameter[0];

  while (*ap && *bp) {
    if (**ap != **bp) return false;
    ++ap;
    ++bp;
  }
  return *ap == *bp
#endif  // EXAMPLE_TYPE_INFO_FULL_COMPARE
}

inline bool operator!=(TypeInfoTable const &a, TypeInfoTable const &b) {
  return !(a == b);
}

template <typename T, typename Any>
struct TypeInfoModel;

/**
 * @internal TypeInfoModel
 * generic TypeInfoTable generator.
 */
template <typename T, typename Any = void>
struct TypeInfoModel {
  static const TypeInfoTable Value;
};

template <typename T, typename Any>
const TypeInfoTable TypeInfoModel<T, Any>::Value = {"undefined", {0, }};

// TypeInfoModel array specializations

#define _ARRAY_NAME                                                  \
  {                                                                  \
    'a', 'r', 'r', 'a', 'y', '[', Size / 1000000000UL % 10 + '0',    \
        Size / 100000000UL % 10 + '0', Size / 10000000UL % 10 + '0', \
        Size / 1000000UL % 10 + '0', Size / 100000UL % 10 + '0',     \
        Size / 10000UL % 10 + '0', Size / 1000UL % 10 + '0',         \
        Size / 100UL % 10 + '0', Size / 10UL % 10 + '0',             \
        Size / 1UL % 10 + '0', ']'                                   \
  }

/* non-const array specializations */
template <typename Any, typename T, std::size_t Size>
struct TypeInfoModel<T[Size], Any> {
  static const TypeInfoTable Value;
  static const char Name[256];
};

template <typename Any, typename T, std::size_t Size>
const char TypeInfoModel<T[Size], Any>::Name[256] = _ARRAY_NAME;

template <typename Any, typename T, std::size_t Size>
const TypeInfoTable TypeInfoModel<T[Size], Any>::Value = {
    &Name[0], {&TypeInfoModel<T>::Value}};

/* const array specialization */
template <typename Any, typename T, std::size_t Size>
struct TypeInfoModel<const T[Size], Any> {
  static const TypeInfoTable Value;
  static const char Name[256];
};

template <typename Any, typename T, std::size_t Size>
const char TypeInfoModel<const T[Size], Any>::Name[256] = _ARRAY_NAME;

template <typename Any, typename T, std::size_t Size>
const TypeInfoTable TypeInfoModel<const T[Size], Any>::Value = {
    &Name[0], {&TypeInfoModel<const T>::Value}};

#undef _ARRAY_NAME

/**
 * EmptyType
 * empty type.
 */
struct EmptyType {
  inline bool operator==(const EmptyType &) const { return true; }
};

/*----------------------------------------------------------------------------*/

/**
 * TypeInfo
 * runtime type name information independent of std's RTTI, with support
 * for template arguments.
 * For each type you whish to indentify at runtime, you first
 * need to define its name with EXAMPLE_NAME_TYPE_<N> macro where N is number
 * of template arguments.
 * For example type 'int' has 0 arguments, thus <code>EXAMPLE_NAME_TYPE_0 ("int",
 *int)
 * </code>. Type 'std::vector' usually takes one argument, this <code>
 * EXAMPLE_NAME_TYPE_1 ("std::vector", std::vector<ARG1>)</code>.
 *
 * Note:
 * Since comparing two TypeInfo instances can occur quite frequently,
 * you should make sure, that each TypeInfoTable is instantiated only once
 * by explicit template instantiation, see tests/sharedlib
 * and tests/test-shared for example. This isn't (by default) true
 * when dealing with two instances from different compilation units, since
 * each unit generates it's own local TypeInfoTable. If you can't guarantee
 * this is not true, for whatever reason, make sure you compile with
 * EXAMPLE_TYPE_INFO_FULL_COMPARE defined.
 */
class TypeInfo {
 public:

  explicit TypeInfo(TypeInfoTable const *x) : m_InfoTable(x) {}

  TypeInfo(TypeInfo const &c) : m_InfoTable(c.m_InfoTable) {}

  /** @return name of the type */
  char const *GetName() const { return m_InfoTable->Name; }

  /** @return type information table */
  TypeInfoTable const *GetTypeInfoTable() const { return m_InfoTable; }

  inline bool operator==(TypeInfo const &y) const {
    return *m_InfoTable == *y.m_InfoTable;
  }

  inline bool operator!=(TypeInfo const &y) const {
    return *m_InfoTable != *y.m_InfoTable;
  }

 private:
  TypeInfoTable const *m_InfoTable;
};

/*----------------------------------------------------------------------------*/

#define EXAMPLE_NAME_TYPE_0(export_decl, name, ...)                             \
  namespace example {                                                           \
  template <typename Any>                                                   \
  struct TypeInfoModel<__VA_ARGS__, Any> {                                  \
    static const TypeInfoTable Value;                                       \
  };                                                                        \
  template <typename Any>                                                   \
  const TypeInfoTable TypeInfoModel<__VA_ARGS__, Any>::Value = {name, {0}}; \
  extern template export_decl TypeInfo TypeInfoOf<__VA_ARGS__>();           \
  }  // namespace example

#define EXAMPLE_NAME_TYPE_1(name, ...)                               \
  namespace example {                                                \
  template <typename Any, typename ARG1>                         \
  struct TypeInfoModel<__VA_ARGS__, Any> {                       \
    static const TypeInfoTable Value;                            \
  };                                                             \
  template <typename Any, typename ARG1>                         \
  const TypeInfoTable TypeInfoModel<__VA_ARGS__, Any>::Value = { \
      name,                                                      \
      {&TypeInfoModel<ARG1>::Value}};                            \
  }  // namespace example

#define EXAMPLE_NAME_TYPE_2(name, ...)                                        \
  namespace example {                                                         \
  template <typename Any, typename ARG1, typename ARG2>                  \
  struct TypeInfoModel<__VA_ARGS__, Any> {                               \
    static const TypeInfoTable Value;                                    \
  };                                                                     \
  template <typename Any, typename ARG1, typename ARG2>                  \
  const TypeInfoTable TypeInfoModel<__VA_ARGS__, Any>::Value = {         \
      name, {&TypeInfoModel<ARG1>::Value, &TypeInfoModel<ARG2>::Value}}; \
  }  // namespace example

#define EXAMPLE_NAME_INSTANCE(...)                 \
  namespace example {                              \
  template TypeInfo TypeInfoOf<__VA_ARGS__>(); \
  }  // namespace example

template <typename T>
inline TypeInfo TypeInfoOf() {
  return TypeInfo(&TypeInfoModel<T>::Value);
}

template <typename T>
inline TypeInfo TypeInfoOf(T &) {
  return TypeInfoOf<T &>();
}

template <typename T>
inline TypeInfo TypeInfoOf(T const &) {
  return TypeInfoOf<T const &>();
}

}  // namespace example

#include <string>
#include <vector>

EXAMPLE_NAME_TYPE_0(EXAMPLE_EXPORT, "bool", bool)
EXAMPLE_NAME_TYPE_0(EXAMPLE_EXPORT, "int", int)
EXAMPLE_NAME_TYPE_0(EXAMPLE_EXPORT, "float", float)
EXAMPLE_NAME_TYPE_0(EXAMPLE_EXPORT, "double", double)
EXAMPLE_NAME_TYPE_0(EXAMPLE_EXPORT, "long", long)
EXAMPLE_NAME_TYPE_0(EXAMPLE_EXPORT, "string", std::string)
EXAMPLE_NAME_TYPE_0(EXAMPLE_EXPORT, "empty", EmptyType)

EXAMPLE_NAME_TYPE_1("ptr", ARG1 *)
EXAMPLE_NAME_TYPE_1("const", ARG1 const)
EXAMPLE_NAME_TYPE_1("ref", ARG1 &)
EXAMPLE_NAME_TYPE_1("array", std::vector<ARG1>)

#endif /* __EXAMPLE_TYPEINFO_H__ */
