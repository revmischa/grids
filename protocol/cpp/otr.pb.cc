// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "otr.pb.h"
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace grids {
namespace message {
namespace OTR {

namespace {

const ::google::protobuf::Descriptor* SMPRequest_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  SMPRequest_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_otr_2eproto() {
  protobuf_AddDesc_otr_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "otr.proto");
  GOOGLE_CHECK(file != NULL);
  SMPRequest_descriptor_ = file->message_type(0);
  static const int SMPRequest_offsets_[3] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SMPRequest, base_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SMPRequest, peer_name_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SMPRequest, question_),
  };
  SMPRequest_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      SMPRequest_descriptor_,
      SMPRequest::default_instance_,
      SMPRequest_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SMPRequest, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SMPRequest, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(SMPRequest));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_otr_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    SMPRequest_descriptor_, &SMPRequest::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_otr_2eproto() {
  delete SMPRequest::default_instance_;
  delete SMPRequest_reflection_;
}

void protobuf_AddDesc_otr_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::grids::protobuf_AddDesc_base_2eproto();
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\totr.proto\022\021grids.message.OTR\032\nbase.pro"
    "to\"O\n\nSMPRequest\022\034\n\004base\030\001 \002(\0132\016.grids.M"
    "essage\022\021\n\tpeer_name\030\002 \001(\t\022\020\n\010question\030\003 "
    "\002(\t", 123);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "otr.proto", &protobuf_RegisterTypes);
  SMPRequest::default_instance_ = new SMPRequest();
  SMPRequest::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_otr_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_otr_2eproto {
  StaticDescriptorInitializer_otr_2eproto() {
    protobuf_AddDesc_otr_2eproto();
  }
} static_descriptor_initializer_otr_2eproto_;


// ===================================================================

const ::std::string SMPRequest::_default_peer_name_;
const ::std::string SMPRequest::_default_question_;
#ifndef _MSC_VER
const int SMPRequest::kBaseFieldNumber;
const int SMPRequest::kPeerNameFieldNumber;
const int SMPRequest::kQuestionFieldNumber;
#endif  // !_MSC_VER

SMPRequest::SMPRequest()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void SMPRequest::InitAsDefaultInstance() {
  base_ = const_cast< ::grids::Message*>(&::grids::Message::default_instance());
}

SMPRequest::SMPRequest(const SMPRequest& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void SMPRequest::SharedCtor() {
  _cached_size_ = 0;
  base_ = NULL;
  peer_name_ = const_cast< ::std::string*>(&_default_peer_name_);
  question_ = const_cast< ::std::string*>(&_default_question_);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

SMPRequest::~SMPRequest() {
  SharedDtor();
}

void SMPRequest::SharedDtor() {
  if (peer_name_ != &_default_peer_name_) {
    delete peer_name_;
  }
  if (question_ != &_default_question_) {
    delete question_;
  }
  if (this != default_instance_) {
    delete base_;
  }
}

void SMPRequest::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* SMPRequest::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return SMPRequest_descriptor_;
}

const SMPRequest& SMPRequest::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_otr_2eproto();  return *default_instance_;
}

SMPRequest* SMPRequest::default_instance_ = NULL;

SMPRequest* SMPRequest::New() const {
  return new SMPRequest;
}

void SMPRequest::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (_has_bit(0)) {
      if (base_ != NULL) base_->::grids::Message::Clear();
    }
    if (_has_bit(1)) {
      if (peer_name_ != &_default_peer_name_) {
        peer_name_->clear();
      }
    }
    if (_has_bit(2)) {
      if (question_ != &_default_question_) {
        question_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool SMPRequest::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required .grids.Message base = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_base()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_peer_name;
        break;
      }
      
      // optional string peer_name = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_peer_name:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_peer_name()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->peer_name().data(), this->peer_name().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(26)) goto parse_question;
        break;
      }
      
      // required string question = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_question:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_question()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->question().data(), this->question().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }
      
      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void SMPRequest::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required .grids.Message base = 1;
  if (_has_bit(0)) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      1, this->base(), output);
  }
  
  // optional string peer_name = 2;
  if (_has_bit(1)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->peer_name().data(), this->peer_name().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      2, this->peer_name(), output);
  }
  
  // required string question = 3;
  if (_has_bit(2)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->question().data(), this->question().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      3, this->question(), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* SMPRequest::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required .grids.Message base = 1;
  if (_has_bit(0)) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        1, this->base(), target);
  }
  
  // optional string peer_name = 2;
  if (_has_bit(1)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->peer_name().data(), this->peer_name().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        2, this->peer_name(), target);
  }
  
  // required string question = 3;
  if (_has_bit(2)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->question().data(), this->question().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        3, this->question(), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int SMPRequest::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required .grids.Message base = 1;
    if (has_base()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->base());
    }
    
    // optional string peer_name = 2;
    if (has_peer_name()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->peer_name());
    }
    
    // required string question = 3;
    if (has_question()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->question());
    }
    
  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void SMPRequest::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const SMPRequest* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const SMPRequest*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void SMPRequest::MergeFrom(const SMPRequest& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from._has_bit(0)) {
      mutable_base()->::grids::Message::MergeFrom(from.base());
    }
    if (from._has_bit(1)) {
      set_peer_name(from.peer_name());
    }
    if (from._has_bit(2)) {
      set_question(from.question());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void SMPRequest::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void SMPRequest::CopyFrom(const SMPRequest& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool SMPRequest::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000005) != 0x00000005) return false;
  
  if (has_base()) {
    if (!this->base().IsInitialized()) return false;
  }
  return true;
}

void SMPRequest::Swap(SMPRequest* other) {
  if (other != this) {
    std::swap(base_, other->base_);
    std::swap(peer_name_, other->peer_name_);
    std::swap(question_, other->question_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata SMPRequest::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = SMPRequest_descriptor_;
  metadata.reflection = SMPRequest_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace OTR
}  // namespace message
}  // namespace grids

// @@protoc_insertion_point(global_scope)
