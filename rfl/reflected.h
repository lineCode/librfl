// Copyright (c) 2015 Pavel Novy. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RFL_REFLECTED__
#define __RFL_REFLECTED__

#include "rfl/rfl_export.h"
#include "rfl/types.h"
#include "rfl/type_info.h"
#include "rfl/any_var.h"

#include <string>
#include <vector>
#include <map>

// non copyable
// string constants
// perfect hashes
// ? dynamic and static variants

namespace rfl {

class Namespace;
typedef std::vector<Namespace *> Namespaces;

class Enum;
typedef std::vector<Enum *> Enums;

class Class;
typedef std::vector<Class *> Classes;

class Property;
typedef std::vector<Property *> Properties;

class Argument;
typedef std::vector<Argument *> Arguments;

class Method;
typedef std::vector<Method *> Methods;

class PackageFile;
typedef std::vector<PackageFile *> PackageFiles;

class Package;

class RFL_EXPORT Annotation {
public:
  // TODO proper get/setters
  std::string value_;
  std::string file_;
  int line_;

  Annotation() {}
  Annotation(std::string const &value, char const *file, int line);
  Annotation(Annotation const &x);
  Annotation &operator= (Annotation const &x);

  void AddEntry(std::string const &key, std::string const &value);
  char const *GetEntry(std::string const &key) const;
private:
  typedef std::map<std::string, std::string> EntryMap;
  EntryMap entries_;
};

class RFL_EXPORT Reflected {
public:
  Reflected() {}
  Reflected(std::string const &name);
  Reflected(std::string const &name, Annotation const &anno);

  std::string const &name() const { return name_; }
  Annotation const &annotation() const { return annotation_; }

private:
  std::string name_;
  Annotation annotation_;
};

// XXX rename to Field
class RFL_EXPORT Property : public Reflected {
public:
  Property() {}

  Property(std::string const &name,
           std::string const &type,
           uint32 offset,
           Annotation const &anno);

  Property(Property const &x);

  Property &operator=(Property const &x);

  Class *parent_class() const { return class_; }
  std::string const &type() const { return type_; }

  uint32 offset() const { return offset_ ;}
private:
  std::string type_;
  uint32 offset_;
  friend class Class;
  void set_parent_class(Class *clazz) { class_ = clazz; }
  Class *class_;
};

class RFL_EXPORT Enum : public Reflected {
public:
  struct RFL_EXPORT EnumItem {
    long value_;
    std::string id_;
    std::string name_;
  };
public:
  Enum(std::string const &name,
       std::string const &type,
       PackageFile *pkg_file,
       Annotation const &anno,
       Namespace *ns,
       Class *parent);

  Namespace *enum_namespace() const { return namespace_; }
  Class *parent_class() const { return parent_class_; }

  void AddEnumItem(EnumItem const &item);
  void RemoveEnumItem(EnumItem const &item);
  EnumItem const &GetEnumItemAt(size_t idx) const;
  size_t GetNumEnumItems() const;

  std::string const &type() const { return type_; }
  PackageFile *package_file() const;

private:
  Namespace *namespace_;
  Class *parent_class_;
  std::string type_;
  std::vector<EnumItem> items_;
  PackageFile *pkg_file_;
};

class RFL_EXPORT Argument : public Reflected {
public:
  enum Kind {
    kReturn_Kind,
    kInput_Kind,
    kOutput_Kind,
    kInOut_Kind
  };
  Argument() {}
  Argument(std::string name,
           Kind kind,
           std::string const &type,
           Annotation const &anno)
      : Reflected(name, anno), kind_(kind), type_(type) {}

  Kind kind() const { return kind_; }
  std::string const &type() const { return type_; }

private:
  Kind kind_;
  std::string type_;
};

class RFL_EXPORT Method : public Reflected {
public:
  Method() {}
  Method(std::string const &name, Annotation const &anno)
    : Reflected(name, anno) {}

  Class *parent_class() const { return class_; }

  void AddArgument(Argument *arg);
  void RemoveArgument(Argument *arg);

  Argument *GetArgumentAt(size_t idx) const;
  size_t GetNumArguments() const;

private:
  friend class Class;
  void set_parent_class(Class *klass) { class_ = klass; }
  Class *class_;
  Arguments arguments_;
};

class RFL_EXPORT EnumContainer {
public:
  EnumContainer();
  void AddEnum(Enum *e);
  void RemoveEnum(Enum *e);
  Enum *FindEnum(char const *enum_name) const;
  size_t GetNumEnums() const;
  Enum *GetEnumAt(size_t idx) const;
private:
  Enums enums_;
};

class RFL_EXPORT Class : public Reflected, public EnumContainer {
public:
  Class() {}

  Class(std::string const &name,
        PackageFile *pkg_file,
        Annotation const &anno,
        Class *super = nullptr,
        Property **props = nullptr,
        Class **nested = nullptr
        );

  Namespace *class_namespace() const { return namespace_; }
  Class *parent_class() const { return parent_; }
  Class *super_class() const { return super_; }
  std::string const &header_file() const;
  PackageFile *package_file() const;

  void AddProperty(Property *prop);
  void RemoveProperty(Property *prop);
  size_t GetNumProperties() const;
  Property *GetPropertyAt(size_t idx) const;
  Property *FindProperty(char const *name) const;

  void AddMethod(Method *method);
  void RemoveMethod(Method *method);
  size_t GetNumMethods() const;
  Method *GetMethodAt(size_t idx) const;
  Method *FindMethod(char const *name) const;

  void AddClass(Class *klass);
  void RemoveClass(Class *klass);
  Class *FindClass(char const *class_name) const;
  size_t GetNumClasses() const;
  Class *GetClassAt(size_t idx) const;

private:
  friend class Namespace;
  void set_class_namespace(Namespace *ns) { namespace_ = ns; }
  void set_parent_class(Class *parent) { parent_ = parent; }

private:
  Namespace *namespace_;
  Class *parent_;
  Class *super_;
  Properties properties_;
  Classes classes_;
  Methods methods_;
  PackageFile *pkg_file_;
};

class RFL_EXPORT Namespace : public Reflected, public EnumContainer {
public:
  Namespace(std::string const &name,
            Class **classes = nullptr,
            Namespace **namespaces = nullptr);

  void AddClass(Class *klass);
  void RemoveClass(Class *klass);
  Class *FindClass(char const *class_name) const;
  size_t GetNumClasses() const;
  Class *GetClassAt(size_t idx) const;

  void AddNamespace(Namespace *ns);
  void RemoveNamespace(Namespace *ns);
  Namespace *FindNamespace(char const *ns) const;
  size_t GetNumNamespaces() const;
  Namespace *GetNamespaceAt(size_t idx) const;

  Namespace *parent_namespace() const { return parent_namespace_; }

private:
  void set_parent_namespace(Namespace *ns) { parent_namespace_ = ns; }
  Namespace *parent_namespace_;
  Classes classes_;
  Namespaces namespaces_;
};

class RFL_EXPORT PackageFile : public EnumContainer {
public:
  PackageFile(std::string const &path);

  std::string const &source_path() const;
  std::string filename() const;
  bool is_dependency() const;
  void set_is_dependecy(bool is);

  void AddClass(Class *klass);
  void RemoveClass(Class *klass);
  size_t GetNumClasses() const;
  Class *GetClassAt(size_t idx) const;

private:
  std::string source_path_;
  bool is_dependency_;
  Classes classes_;
};

class RFL_EXPORT Package : public Namespace {
public:
  Package(std::string const &name,
          std::string const &version,
          Namespace **nested = nullptr);

  void AddImport(std::string const &import);
  std::string const &GetImportAt(int idx) const;
  int GetImportNum() const;

  void AddLibrary(std::string const &import);
  std::string const &GetLibraryAt(int idx) const;
  int GetLibraryNum() const;

  std::string const &version() const { return version_; }

  PackageFile *GetOrCreatePackageFile(std::string const &path);

  void AddPackageFile(PackageFile *pkg_file);
  void RemovePackageFile(PackageFile *pkg_file);
  size_t GetNumPackageFiles() const;
  PackageFile *GetPackageFileAt(size_t idx) const;

private:
  std::vector<std::string> imports_;
  std::vector<std::string> libs_;
  std::string version_;
  PackageFiles files_;
};

class RFL_EXPORT PackageManifest {
public:
  bool Load(char const *filename);
  bool Save(char const *filename);
  char const *GetEntry(char const *entry) const;
  void SetEntry(char const *key, char const *value);
private:
  typedef std::map<std::string, std::string> EntryMap;
  EntryMap entry_map_;
};

typedef Package const *(*LoadPackageFunc)();

} // namespace rfl

#endif /* __RFL_REFLECTED__ */
