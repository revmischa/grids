// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "authentication.pb.h"
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace grids {
namespace message {
namespace authentication {

namespace {

const ::google::protobuf::Descriptor* Login_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  Login_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_authentication_2eproto() {
  protobuf_AddDesc_authentication_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "authentication.proto");
  GOOGLE_CHECK(file != NULL);
  Login_descriptor_ = file->message_type(0);
  static const int Login_offsets_[1] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Login, base_),
  };
  Login_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      Login_descriptor_,
      Login::default_instance_,
      Login_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Login, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Login, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(Login));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_authentication_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    Login_descriptor_, &Login::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_authentication_2eproto() {
  delete Login::default_instance_;
  delete Login_reflection_;
}

void protobuf_AddDesc_authentication_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::grids::protobuf_AddDesc_base_2eproto();
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\024authentication.proto\022\034grids.message.au"
    "thentication\032\nbase.proto\"%\n\005Login\022\034\n\004bas"
    "e\030\001 \002(\0132\016.grids.Message", 103);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "authentication.proto", &protobuf_RegisterTypes);
  Login::default_instance_ = new Login();
  Login::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_authentication_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_authentication_2eproto {
  StaticDescriptorInitializer_authentication_2eproto() {
    protobuf_AddDesc_authentication_2eproto();
  }
} static_descriptor_initializer_authentication_2eproto_;


// ===================================================================

#ifndef _MSC_VER
const int Login::kBaseFieldNumber;
#endif  // !_MSC_VER

Login::Login()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void Login::InitAsDefaultInstance() {
  base_ = const_cast< ::grids::Message*>(&::grids::Message::default_instance());
}

Login::Login(const Login& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void Login::SharedCtor() {
  _cached_size_ = 0;
  base_ = NULL;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

Login::~Login() {
  SharedDtor();
}

void Login::SharedDtor() {
  if (this != default_instance_) {
    delete base_;
  }
}

void Login::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Login::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Login_descriptor_;
}

const Login& Login::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_authentication_2eproto();  return *default_instance_;
}

Login* Login::default_instance_ = NULL;

Login* Login::New() const {
  return new Login;
}

void Login::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (_has_bit(0)) {
      if (base_ != NULL) base_->::grids::Message::Clear();
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool Login::MergePartialFromCodedStream(
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

void Login::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required .grids.Message base = 1;
  if (_has_bit(0)) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      1, this->base(), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* Login::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required .grids.Message base = 1;
  if (_has_bit(0)) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        1, this->base(), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int Login::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required .grids.Message base = 1;
    if (has_base()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->base());
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

void Login::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const Login* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const Login*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void Login::MergeFrom(const Login& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from._has_bit(0)) {
      mutable_base()->::grids::Message::MergeFrom(from.base());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void Login::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Login::CopyFrom(const Login& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Login::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;
  
  if (has_base()) {
    if (!this->base().IsInitialized()) return false;
  }
  return true;
}

void Login::Swap(Login* other) {
  if (other != this) {
    std::swap(base_, other->base_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata Login::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = Login_descriptor_;
  metadata.reflection = Login_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace authentication
}  // namespace message
}  // namespace grids

// @@protoc_insertion_point(global_scope)
