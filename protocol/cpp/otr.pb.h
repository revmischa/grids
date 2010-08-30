// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: otr.proto

#ifndef PROTOBUF_otr_2eproto__INCLUDED
#define PROTOBUF_otr_2eproto__INCLUDED

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
namespace OTR {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_otr_2eproto();
void protobuf_AssignDesc_otr_2eproto();
void protobuf_ShutdownFile_otr_2eproto();

class SMPRequest;

// ===================================================================

class SMPRequest : public ::google::protobuf::Message {
 public:
  SMPRequest();
  virtual ~SMPRequest();
  
  SMPRequest(const SMPRequest& from);
  
  inline SMPRequest& operator=(const SMPRequest& from) {
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
  static const SMPRequest& default_instance();
  
  void Swap(SMPRequest* other);
  
  // implements Message ----------------------------------------------
  
  SMPRequest* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const SMPRequest& from);
  void MergeFrom(const SMPRequest& from);
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
  
  // required string question = 3;
  inline bool has_question() const;
  inline void clear_question();
  static const int kQuestionFieldNumber = 3;
  inline const ::std::string& question() const;
  inline void set_question(const ::std::string& value);
  inline void set_question(const char* value);
  inline void set_question(const char* value, size_t size);
  inline ::std::string* mutable_question();
  
  // @@protoc_insertion_point(class_scope:grids.message.OTR.SMPRequest)
 private:
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  mutable int _cached_size_;
  
  ::grids::Message* base_;
  ::std::string* peer_name_;
  static const ::std::string _default_peer_name_;
  ::std::string* question_;
  static const ::std::string _default_question_;
  friend void  protobuf_AddDesc_otr_2eproto();
  friend void protobuf_AssignDesc_otr_2eproto();
  friend void protobuf_ShutdownFile_otr_2eproto();
  
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];
  
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
  static SMPRequest* default_instance_;
};
// ===================================================================


// ===================================================================

// SMPRequest

// required .grids.Message base = 1;
inline bool SMPRequest::has_base() const {
  return _has_bit(0);
}
inline void SMPRequest::clear_base() {
  if (base_ != NULL) base_->::grids::Message::Clear();
  _clear_bit(0);
}
inline const ::grids::Message& SMPRequest::base() const {
  return base_ != NULL ? *base_ : *default_instance_->base_;
}
inline ::grids::Message* SMPRequest::mutable_base() {
  _set_bit(0);
  if (base_ == NULL) base_ = new ::grids::Message;
  return base_;
}

// optional string peer_name = 2;
inline bool SMPRequest::has_peer_name() const {
  return _has_bit(1);
}
inline void SMPRequest::clear_peer_name() {
  if (peer_name_ != &_default_peer_name_) {
    peer_name_->clear();
  }
  _clear_bit(1);
}
inline const ::std::string& SMPRequest::peer_name() const {
  return *peer_name_;
}
inline void SMPRequest::set_peer_name(const ::std::string& value) {
  _set_bit(1);
  if (peer_name_ == &_default_peer_name_) {
    peer_name_ = new ::std::string;
  }
  peer_name_->assign(value);
}
inline void SMPRequest::set_peer_name(const char* value) {
  _set_bit(1);
  if (peer_name_ == &_default_peer_name_) {
    peer_name_ = new ::std::string;
  }
  peer_name_->assign(value);
}
inline void SMPRequest::set_peer_name(const char* value, size_t size) {
  _set_bit(1);
  if (peer_name_ == &_default_peer_name_) {
    peer_name_ = new ::std::string;
  }
  peer_name_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* SMPRequest::mutable_peer_name() {
  _set_bit(1);
  if (peer_name_ == &_default_peer_name_) {
    peer_name_ = new ::std::string;
  }
  return peer_name_;
}

// required string question = 3;
inline bool SMPRequest::has_question() const {
  return _has_bit(2);
}
inline void SMPRequest::clear_question() {
  if (question_ != &_default_question_) {
    question_->clear();
  }
  _clear_bit(2);
}
inline const ::std::string& SMPRequest::question() const {
  return *question_;
}
inline void SMPRequest::set_question(const ::std::string& value) {
  _set_bit(2);
  if (question_ == &_default_question_) {
    question_ = new ::std::string;
  }
  question_->assign(value);
}
inline void SMPRequest::set_question(const char* value) {
  _set_bit(2);
  if (question_ == &_default_question_) {
    question_ = new ::std::string;
  }
  question_->assign(value);
}
inline void SMPRequest::set_question(const char* value, size_t size) {
  _set_bit(2);
  if (question_ == &_default_question_) {
    question_ = new ::std::string;
  }
  question_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* SMPRequest::mutable_question() {
  _set_bit(2);
  if (question_ == &_default_question_) {
    question_ = new ::std::string;
  }
  return question_;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace OTR
}  // namespace message
}  // namespace grids

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_otr_2eproto__INCLUDED
