// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: authentication.proto

#ifndef PROTOBUF_authentication_2eproto__INCLUDED
#define PROTOBUF_authentication_2eproto__INCLUDED

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
namespace authentication {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_authentication_2eproto();
void protobuf_AssignDesc_authentication_2eproto();
void protobuf_ShutdownFile_authentication_2eproto();

class Login;

// ===================================================================

class Login : public ::google::protobuf::Message {
 public:
  Login();
  virtual ~Login();
  
  Login(const Login& from);
  
  inline Login& operator=(const Login& from) {
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
  static const Login& default_instance();
  
  void Swap(Login* other);
  
  // implements Message ----------------------------------------------
  
  Login* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Login& from);
  void MergeFrom(const Login& from);
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
  
  // @@protoc_insertion_point(class_scope:grids.message.authentication.Login)
 private:
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  mutable int _cached_size_;
  
  ::grids::Message* base_;
  friend void  protobuf_AddDesc_authentication_2eproto();
  friend void protobuf_AssignDesc_authentication_2eproto();
  friend void protobuf_ShutdownFile_authentication_2eproto();
  
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];
  
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
  static Login* default_instance_;
};
// ===================================================================


// ===================================================================

// Login

// required .grids.Message base = 1;
inline bool Login::has_base() const {
  return _has_bit(0);
}
inline void Login::clear_base() {
  if (base_ != NULL) base_->::grids::Message::Clear();
  _clear_bit(0);
}
inline const ::grids::Message& Login::base() const {
  return base_ != NULL ? *base_ : *default_instance_->base_;
}
inline ::grids::Message* Login::mutable_base() {
  _set_bit(0);
  if (base_ == NULL) base_ = new ::grids::Message;
  return base_;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace authentication
}  // namespace message
}  // namespace grids

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_authentication_2eproto__INCLUDED
