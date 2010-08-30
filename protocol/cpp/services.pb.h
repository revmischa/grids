// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: services.proto

#ifndef PROTOBUF_services_2eproto__INCLUDED
#define PROTOBUF_services_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2003000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2003000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_message_reflection.h>
#include "base.pb.h"
// @@protoc_insertion_point(includes)

namespace grids {
namespace message {
namespace services {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_services_2eproto();
void protobuf_AssignDesc_services_2eproto();
void protobuf_ShutdownFile_services_2eproto();

class List;

// ===================================================================

class List : public ::google::protobuf::Message {
 public:
  List();
  virtual ~List();
  
  List(const List& from);
  
  inline List& operator=(const List& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const List& default_instance();
  
  void Swap(List* other);
  
  // implements Message ----------------------------------------------
  
  List* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const List& from);
  void MergeFrom(const List& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // required .grids.Message base = 1;
  inline bool has_base() const;
  inline void clear_base();
  static const int kBaseFieldNumber = 1;
  inline const ::grids::Message& base() const;
  inline ::grids::Message* mutable_base();
  
  // repeated string services = 2;
  inline int services_size() const;
  inline void clear_services();
  static const int kServicesFieldNumber = 2;
  inline const ::std::string& services(int index) const;
  inline ::std::string* mutable_services(int index);
  inline void set_services(int index, const ::std::string& value);
  inline void set_services(int index, const char* value);
  inline void set_services(int index, const char* value, size_t size);
  inline ::std::string* add_services();
  inline void add_services(const ::std::string& value);
  inline void add_services(const char* value);
  inline void add_services(const char* value, size_t size);
  inline const ::google::protobuf::RepeatedPtrField< ::std::string>& services() const;
  inline ::google::protobuf::RepeatedPtrField< ::std::string>* mutable_services();
  
  // @@protoc_insertion_point(class_scope:grids.message.services.List)
 private:
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  mutable int _cached_size_;
  
  ::grids::Message* base_;
  ::google::protobuf::RepeatedPtrField< ::std::string> services_;
  friend void  protobuf_AddDesc_services_2eproto();
  friend void protobuf_AssignDesc_services_2eproto();
  friend void protobuf_ShutdownFile_services_2eproto();
  
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];
  
  // WHY DOES & HAVE LOWER PRECEDENCE THAN != !?
  inline bool _has_bit(int index) const {
    return (_has_bits_[index / 32] & (1u << (index % 32))) != 0;
  }
  inline void _set_bit(int index) {
    _has_bits_[index / 32] |= (1u << (index % 32));
  }
  inline void _clear_bit(int index) {
    _has_bits_[index / 32] &= ~(1u << (index % 32));
  }
  
  void InitAsDefaultInstance();
  static List* default_instance_;
};
// ===================================================================


// ===================================================================

// List

// required .grids.Message base = 1;
inline bool List::has_base() const {
  return _has_bit(0);
}
inline void List::clear_base() {
  if (base_ != NULL) base_->::grids::Message::Clear();
  _clear_bit(0);
}
inline const ::grids::Message& List::base() const {
  return base_ != NULL ? *base_ : *default_instance_->base_;
}
inline ::grids::Message* List::mutable_base() {
  _set_bit(0);
  if (base_ == NULL) base_ = new ::grids::Message;
  return base_;
}

// repeated string services = 2;
inline int List::services_size() const {
  return services_.size();
}
inline void List::clear_services() {
  services_.Clear();
}
inline const ::std::string& List::services(int index) const {
  return services_.Get(index);
}
inline ::std::string* List::mutable_services(int index) {
  return services_.Mutable(index);
}
inline void List::set_services(int index, const ::std::string& value) {
  services_.Mutable(index)->assign(value);
}
inline void List::set_services(int index, const char* value) {
  services_.Mutable(index)->assign(value);
}
inline void List::set_services(int index, const char* value, size_t size) {
  services_.Mutable(index)->assign(
    reinterpret_cast<const char*>(value), size);
}
inline ::std::string* List::add_services() {
  return services_.Add();
}
inline void List::add_services(const ::std::string& value) {
  services_.Add()->assign(value);
}
inline void List::add_services(const char* value) {
  services_.Add()->assign(value);
}
inline void List::add_services(const char* value, size_t size) {
  services_.Add()->assign(reinterpret_cast<const char*>(value), size);
}
inline const ::google::protobuf::RepeatedPtrField< ::std::string>&
List::services() const {
  return services_;
}
inline ::google::protobuf::RepeatedPtrField< ::std::string>*
List::mutable_services() {
  return &services_;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace services
}  // namespace message
}  // namespace grids

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_services_2eproto__INCLUDED
