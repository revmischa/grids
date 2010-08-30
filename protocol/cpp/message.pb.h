// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: message.proto

#ifndef PROTOBUF_message_2eproto__INCLUDED
#define PROTOBUF_message_2eproto__INCLUDED

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
#include "grids.pb.h"
// @@protoc_insertion_point(includes)

namespace grids {
namespace message {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_message_2eproto();
void protobuf_AssignDesc_message_2eproto();
void protobuf_ShutdownFile_message_2eproto();

class Encrypted;
class Unencrypted;
class Echo;
class Connected;

// ===================================================================

class Encrypted : public ::google::protobuf::Message {
 public:
  Encrypted();
  virtual ~Encrypted();
  
  Encrypted(const Encrypted& from);
  
  inline Encrypted& operator=(const Encrypted& from) {
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
  static const Encrypted& default_instance();
  
  void Swap(Encrypted* other);
  
  // implements Message ----------------------------------------------
  
  Encrypted* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Encrypted& from);
  void MergeFrom(const Encrypted& from);
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
  
  // @@protoc_insertion_point(class_scope:grids.message.Encrypted)
 private:
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  mutable int _cached_size_;
  
  ::grids::Message* base_;
  friend void  protobuf_AddDesc_message_2eproto();
  friend void protobuf_AssignDesc_message_2eproto();
  friend void protobuf_ShutdownFile_message_2eproto();
  
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
  static Encrypted* default_instance_;
};
// -------------------------------------------------------------------

class Unencrypted : public ::google::protobuf::Message {
 public:
  Unencrypted();
  virtual ~Unencrypted();
  
  Unencrypted(const Unencrypted& from);
  
  inline Unencrypted& operator=(const Unencrypted& from) {
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
  static const Unencrypted& default_instance();
  
  void Swap(Unencrypted* other);
  
  // implements Message ----------------------------------------------
  
  Unencrypted* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Unencrypted& from);
  void MergeFrom(const Unencrypted& from);
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
  
  // @@protoc_insertion_point(class_scope:grids.message.Unencrypted)
 private:
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  mutable int _cached_size_;
  
  ::grids::Message* base_;
  friend void  protobuf_AddDesc_message_2eproto();
  friend void protobuf_AssignDesc_message_2eproto();
  friend void protobuf_ShutdownFile_message_2eproto();
  
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
  static Unencrypted* default_instance_;
};
// -------------------------------------------------------------------

class Echo : public ::google::protobuf::Message {
 public:
  Echo();
  virtual ~Echo();
  
  Echo(const Echo& from);
  
  inline Echo& operator=(const Echo& from) {
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
  static const Echo& default_instance();
  
  void Swap(Echo* other);
  
  // implements Message ----------------------------------------------
  
  Echo* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Echo& from);
  void MergeFrom(const Echo& from);
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
  
  // optional string echo_string = 2;
  inline bool has_echo_string() const;
  inline void clear_echo_string();
  static const int kEchoStringFieldNumber = 2;
  inline const ::std::string& echo_string() const;
  inline void set_echo_string(const ::std::string& value);
  inline void set_echo_string(const char* value);
  inline void set_echo_string(const char* value, size_t size);
  inline ::std::string* mutable_echo_string();
  
  // @@protoc_insertion_point(class_scope:grids.message.Echo)
 private:
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  mutable int _cached_size_;
  
  ::grids::Message* base_;
  ::std::string* echo_string_;
  static const ::std::string _default_echo_string_;
  friend void  protobuf_AddDesc_message_2eproto();
  friend void protobuf_AssignDesc_message_2eproto();
  friend void protobuf_ShutdownFile_message_2eproto();
  
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
  static Echo* default_instance_;
};
// -------------------------------------------------------------------

class Connected : public ::google::protobuf::Message {
 public:
  Connected();
  virtual ~Connected();
  
  Connected(const Connected& from);
  
  inline Connected& operator=(const Connected& from) {
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
  static const Connected& default_instance();
  
  void Swap(Connected* other);
  
  // implements Message ----------------------------------------------
  
  Connected* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Connected& from);
  void MergeFrom(const Connected& from);
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
  
  // optional string peer_name = 2;
  inline bool has_peer_name() const;
  inline void clear_peer_name();
  static const int kPeerNameFieldNumber = 2;
  inline const ::std::string& peer_name() const;
  inline void set_peer_name(const ::std::string& value);
  inline void set_peer_name(const char* value);
  inline void set_peer_name(const char* value, size_t size);
  inline ::std::string* mutable_peer_name();
  
  // @@protoc_insertion_point(class_scope:grids.message.Connected)
 private:
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  mutable int _cached_size_;
  
  ::grids::Message* base_;
  ::std::string* peer_name_;
  static const ::std::string _default_peer_name_;
  friend void  protobuf_AddDesc_message_2eproto();
  friend void protobuf_AssignDesc_message_2eproto();
  friend void protobuf_ShutdownFile_message_2eproto();
  
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
  static Connected* default_instance_;
};
// ===================================================================


// ===================================================================

// Encrypted

// required .grids.Message base = 1;
inline bool Encrypted::has_base() const {
  return _has_bit(0);
}
inline void Encrypted::clear_base() {
  if (base_ != NULL) base_->::grids::Message::Clear();
  _clear_bit(0);
}
inline const ::grids::Message& Encrypted::base() const {
  return base_ != NULL ? *base_ : *default_instance_->base_;
}
inline ::grids::Message* Encrypted::mutable_base() {
  _set_bit(0);
  if (base_ == NULL) base_ = new ::grids::Message;
  return base_;
}

// -------------------------------------------------------------------

// Unencrypted

// required .grids.Message base = 1;
inline bool Unencrypted::has_base() const {
  return _has_bit(0);
}
inline void Unencrypted::clear_base() {
  if (base_ != NULL) base_->::grids::Message::Clear();
  _clear_bit(0);
}
inline const ::grids::Message& Unencrypted::base() const {
  return base_ != NULL ? *base_ : *default_instance_->base_;
}
inline ::grids::Message* Unencrypted::mutable_base() {
  _set_bit(0);
  if (base_ == NULL) base_ = new ::grids::Message;
  return base_;
}

// -------------------------------------------------------------------

// Echo

// required .grids.Message base = 1;
inline bool Echo::has_base() const {
  return _has_bit(0);
}
inline void Echo::clear_base() {
  if (base_ != NULL) base_->::grids::Message::Clear();
  _clear_bit(0);
}
inline const ::grids::Message& Echo::base() const {
  return base_ != NULL ? *base_ : *default_instance_->base_;
}
inline ::grids::Message* Echo::mutable_base() {
  _set_bit(0);
  if (base_ == NULL) base_ = new ::grids::Message;
  return base_;
}

// optional string echo_string = 2;
inline bool Echo::has_echo_string() const {
  return _has_bit(1);
}
inline void Echo::clear_echo_string() {
  if (echo_string_ != &_default_echo_string_) {
    echo_string_->clear();
  }
  _clear_bit(1);
}
inline const ::std::string& Echo::echo_string() const {
  return *echo_string_;
}
inline void Echo::set_echo_string(const ::std::string& value) {
  _set_bit(1);
  if (echo_string_ == &_default_echo_string_) {
    echo_string_ = new ::std::string;
  }
  echo_string_->assign(value);
}
inline void Echo::set_echo_string(const char* value) {
  _set_bit(1);
  if (echo_string_ == &_default_echo_string_) {
    echo_string_ = new ::std::string;
  }
  echo_string_->assign(value);
}
inline void Echo::set_echo_string(const char* value, size_t size) {
  _set_bit(1);
  if (echo_string_ == &_default_echo_string_) {
    echo_string_ = new ::std::string;
  }
  echo_string_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* Echo::mutable_echo_string() {
  _set_bit(1);
  if (echo_string_ == &_default_echo_string_) {
    echo_string_ = new ::std::string;
  }
  return echo_string_;
}

// -------------------------------------------------------------------

// Connected

// required .grids.Message base = 1;
inline bool Connected::has_base() const {
  return _has_bit(0);
}
inline void Connected::clear_base() {
  if (base_ != NULL) base_->::grids::Message::Clear();
  _clear_bit(0);
}
inline const ::grids::Message& Connected::base() const {
  return base_ != NULL ? *base_ : *default_instance_->base_;
}
inline ::grids::Message* Connected::mutable_base() {
  _set_bit(0);
  if (base_ == NULL) base_ = new ::grids::Message;
  return base_;
}

// optional string peer_name = 2;
inline bool Connected::has_peer_name() const {
  return _has_bit(1);
}
inline void Connected::clear_peer_name() {
  if (peer_name_ != &_default_peer_name_) {
    peer_name_->clear();
  }
  _clear_bit(1);
}
inline const ::std::string& Connected::peer_name() const {
  return *peer_name_;
}
inline void Connected::set_peer_name(const ::std::string& value) {
  _set_bit(1);
  if (peer_name_ == &_default_peer_name_) {
    peer_name_ = new ::std::string;
  }
  peer_name_->assign(value);
}
inline void Connected::set_peer_name(const char* value) {
  _set_bit(1);
  if (peer_name_ == &_default_peer_name_) {
    peer_name_ = new ::std::string;
  }
  peer_name_->assign(value);
}
inline void Connected::set_peer_name(const char* value, size_t size) {
  _set_bit(1);
  if (peer_name_ == &_default_peer_name_) {
    peer_name_ = new ::std::string;
  }
  peer_name_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* Connected::mutable_peer_name() {
  _set_bit(1);
  if (peer_name_ == &_default_peer_name_) {
    peer_name_ = new ::std::string;
  }
  return peer_name_;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace message
}  // namespace grids

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_message_2eproto__INCLUDED
