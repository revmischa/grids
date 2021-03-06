// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "program.pb.h"
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace grids {
namespace message {
namespace program {

namespace {

const ::google::protobuf::Descriptor* CompileAndExecute_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  CompileAndExecute_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_program_2eproto() {
  protobuf_AddDesc_program_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "program.proto");
  GOOGLE_CHECK(file != NULL);
  CompileAndExecute_descriptor_ = file->message_type(0);
  static const int CompileAndExecute_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CompileAndExecute, base_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CompileAndExecute, source_storage_key_),
  };
  CompileAndExecute_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      CompileAndExecute_descriptor_,
      CompileAndExecute::default_instance_,
      CompileAndExecute_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CompileAndExecute, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CompileAndExecute, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(CompileAndExecute));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_program_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    CompileAndExecute_descriptor_, &CompileAndExecute::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_program_2eproto() {
  delete CompileAndExecute::default_instance_;
  delete CompileAndExecute_reflection_;
}

void protobuf_AddDesc_program_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::grids::protobuf_AddDesc_base_2eproto();
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\rprogram.proto\022\025grids.message.program\032\n"
    "base.proto\"M\n\021CompileAndExecute\022\034\n\004base\030"
    "\001 \002(\0132\016.grids.Message\022\032\n\022source_storage_"
    "key\030\002 \002(\t", 129);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "program.proto", &protobuf_RegisterTypes);
  CompileAndExecute::default_instance_ = new CompileAndExecute();
  CompileAndExecute::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_program_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_program_2eproto {
  StaticDescriptorInitializer_program_2eproto() {
    protobuf_AddDesc_program_2eproto();
  }
} static_descriptor_initializer_program_2eproto_;


// ===================================================================

const ::std::string CompileAndExecute::_default_source_storage_key_;
#ifndef _MSC_VER
const int CompileAndExecute::kBaseFieldNumber;
const int CompileAndExecute::kSourceStorageKeyFieldNumber;
#endif  // !_MSC_VER

CompileAndExecute::CompileAndExecute()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void CompileAndExecute::InitAsDefaultInstance() {
  base_ = const_cast< ::grids::Message*>(&::grids::Message::default_instance());
}

CompileAndExecute::CompileAndExecute(const CompileAndExecute& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void CompileAndExecute::SharedCtor() {
  _cached_size_ = 0;
  base_ = NULL;
  source_storage_key_ = const_cast< ::std::string*>(&_default_source_storage_key_);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

CompileAndExecute::~CompileAndExecute() {
  SharedDtor();
}

void CompileAndExecute::SharedDtor() {
  if (source_storage_key_ != &_default_source_storage_key_) {
    delete source_storage_key_;
  }
  if (this != default_instance_) {
    delete base_;
  }
}

void CompileAndExecute::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* CompileAndExecute::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return CompileAndExecute_descriptor_;
}

const CompileAndExecute& CompileAndExecute::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_program_2eproto();  return *default_instance_;
}

CompileAndExecute* CompileAndExecute::default_instance_ = NULL;

CompileAndExecute* CompileAndExecute::New() const {
  return new CompileAndExecute;
}

void CompileAndExecute::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (_has_bit(0)) {
      if (base_ != NULL) base_->::grids::Message::Clear();
    }
    if (_has_bit(1)) {
      if (source_storage_key_ != &_default_source_storage_key_) {
        source_storage_key_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool CompileAndExecute::MergePartialFromCodedStream(
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
        if (input->ExpectTag(18)) goto parse_source_storage_key;
        break;
      }
      
      // required string source_storage_key = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_source_storage_key:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_source_storage_key()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->source_storage_key().data(), this->source_storage_key().length(),
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

void CompileAndExecute::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required .grids.Message base = 1;
  if (_has_bit(0)) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      1, this->base(), output);
  }
  
  // required string source_storage_key = 2;
  if (_has_bit(1)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->source_storage_key().data(), this->source_storage_key().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      2, this->source_storage_key(), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* CompileAndExecute::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required .grids.Message base = 1;
  if (_has_bit(0)) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        1, this->base(), target);
  }
  
  // required string source_storage_key = 2;
  if (_has_bit(1)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->source_storage_key().data(), this->source_storage_key().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        2, this->source_storage_key(), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int CompileAndExecute::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required .grids.Message base = 1;
    if (has_base()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->base());
    }
    
    // required string source_storage_key = 2;
    if (has_source_storage_key()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->source_storage_key());
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

void CompileAndExecute::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const CompileAndExecute* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const CompileAndExecute*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void CompileAndExecute::MergeFrom(const CompileAndExecute& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from._has_bit(0)) {
      mutable_base()->::grids::Message::MergeFrom(from.base());
    }
    if (from._has_bit(1)) {
      set_source_storage_key(from.source_storage_key());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void CompileAndExecute::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void CompileAndExecute::CopyFrom(const CompileAndExecute& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool CompileAndExecute::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000003) != 0x00000003) return false;
  
  if (has_base()) {
    if (!this->base().IsInitialized()) return false;
  }
  return true;
}

void CompileAndExecute::Swap(CompileAndExecute* other) {
  if (other != this) {
    std::swap(base_, other->base_);
    std::swap(source_storage_key_, other->source_storage_key_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata CompileAndExecute::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = CompileAndExecute_descriptor_;
  metadata.reflection = CompileAndExecute_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace program
}  // namespace message
}  // namespace grids

// @@protoc_insertion_point(global_scope)
