// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: programdb.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "programdb.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace {

const ::google::protobuf::Descriptor* SourceFileList_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  SourceFileList_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_programdb_2eproto() GOOGLE_ATTRIBUTE_COLD;
void protobuf_AssignDesc_programdb_2eproto() {
  protobuf_AddDesc_programdb_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "programdb.proto");
  GOOGLE_CHECK(file != NULL);
  SourceFileList_descriptor_ = file->message_type(0);
  static const int SourceFileList_offsets_[1] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SourceFileList, sources_),
  };
  SourceFileList_reflection_ =
    ::google::protobuf::internal::GeneratedMessageReflection::NewGeneratedMessageReflection(
      SourceFileList_descriptor_,
      SourceFileList::default_instance_,
      SourceFileList_offsets_,
      -1,
      -1,
      -1,
      sizeof(SourceFileList),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SourceFileList, _internal_metadata_),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SourceFileList, _is_default_instance_));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_programdb_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) GOOGLE_ATTRIBUTE_COLD;
void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
      SourceFileList_descriptor_, &SourceFileList::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_programdb_2eproto() {
  delete SourceFileList::default_instance_;
  delete SourceFileList_reflection_;
}

void protobuf_AddDesc_programdb_2eproto() GOOGLE_ATTRIBUTE_COLD;
void protobuf_AddDesc_programdb_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\017programdb.proto\"!\n\016SourceFileList\022\017\n\007s"
    "ources\030\001 \003(\tb\006proto3", 60);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "programdb.proto", &protobuf_RegisterTypes);
  SourceFileList::default_instance_ = new SourceFileList();
  SourceFileList::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_programdb_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_programdb_2eproto {
  StaticDescriptorInitializer_programdb_2eproto() {
    protobuf_AddDesc_programdb_2eproto();
  }
} static_descriptor_initializer_programdb_2eproto_;

// ===================================================================

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int SourceFileList::kSourcesFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

SourceFileList::SourceFileList()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:SourceFileList)
}

void SourceFileList::InitAsDefaultInstance() {
  _is_default_instance_ = true;
}

SourceFileList::SourceFileList(const SourceFileList& from)
  : ::google::protobuf::Message(),
    _internal_metadata_(NULL) {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:SourceFileList)
}

void SourceFileList::SharedCtor() {
    _is_default_instance_ = false;
  ::google::protobuf::internal::GetEmptyString();
  _cached_size_ = 0;
}

SourceFileList::~SourceFileList() {
  // @@protoc_insertion_point(destructor:SourceFileList)
  SharedDtor();
}

void SourceFileList::SharedDtor() {
  if (this != default_instance_) {
  }
}

void SourceFileList::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* SourceFileList::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return SourceFileList_descriptor_;
}

const SourceFileList& SourceFileList::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_programdb_2eproto();
  return *default_instance_;
}

SourceFileList* SourceFileList::default_instance_ = NULL;

SourceFileList* SourceFileList::New(::google::protobuf::Arena* arena) const {
  SourceFileList* n = new SourceFileList;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void SourceFileList::Clear() {
// @@protoc_insertion_point(message_clear_start:SourceFileList)
  sources_.Clear();
}

bool SourceFileList::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:SourceFileList)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // repeated string sources = 1;
      case 1: {
        if (tag == 10) {
         parse_sources:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->add_sources()));
          DO_(::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
            this->sources(this->sources_size() - 1).data(),
            this->sources(this->sources_size() - 1).length(),
            ::google::protobuf::internal::WireFormatLite::PARSE,
            "SourceFileList.sources"));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(10)) goto parse_sources;
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormatLite::SkipField(input, tag));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:SourceFileList)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:SourceFileList)
  return false;
#undef DO_
}

void SourceFileList::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:SourceFileList)
  // repeated string sources = 1;
  for (int i = 0; i < this->sources_size(); i++) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->sources(i).data(), this->sources(i).length(),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "SourceFileList.sources");
    ::google::protobuf::internal::WireFormatLite::WriteString(
      1, this->sources(i), output);
  }

  // @@protoc_insertion_point(serialize_end:SourceFileList)
}

::google::protobuf::uint8* SourceFileList::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:SourceFileList)
  // repeated string sources = 1;
  for (int i = 0; i < this->sources_size(); i++) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->sources(i).data(), this->sources(i).length(),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "SourceFileList.sources");
    target = ::google::protobuf::internal::WireFormatLite::
      WriteStringToArray(1, this->sources(i), target);
  }

  // @@protoc_insertion_point(serialize_to_array_end:SourceFileList)
  return target;
}

int SourceFileList::ByteSize() const {
// @@protoc_insertion_point(message_byte_size_start:SourceFileList)
  int total_size = 0;

  // repeated string sources = 1;
  total_size += 1 * this->sources_size();
  for (int i = 0; i < this->sources_size(); i++) {
    total_size += ::google::protobuf::internal::WireFormatLite::StringSize(
      this->sources(i));
  }

  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void SourceFileList::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:SourceFileList)
  if (GOOGLE_PREDICT_FALSE(&from == this)) {
    ::google::protobuf::internal::MergeFromFail(__FILE__, __LINE__);
  }
  const SourceFileList* source = 
      ::google::protobuf::internal::DynamicCastToGenerated<const SourceFileList>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:SourceFileList)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:SourceFileList)
    MergeFrom(*source);
  }
}

void SourceFileList::MergeFrom(const SourceFileList& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:SourceFileList)
  if (GOOGLE_PREDICT_FALSE(&from == this)) {
    ::google::protobuf::internal::MergeFromFail(__FILE__, __LINE__);
  }
  sources_.MergeFrom(from.sources_);
}

void SourceFileList::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:SourceFileList)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void SourceFileList::CopyFrom(const SourceFileList& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:SourceFileList)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool SourceFileList::IsInitialized() const {

  return true;
}

void SourceFileList::Swap(SourceFileList* other) {
  if (other == this) return;
  InternalSwap(other);
}
void SourceFileList::InternalSwap(SourceFileList* other) {
  sources_.UnsafeArenaSwap(&other->sources_);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  std::swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata SourceFileList::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = SourceFileList_descriptor_;
  metadata.reflection = SourceFileList_reflection_;
  return metadata;
}

#if PROTOBUF_INLINE_NOT_IN_HEADERS
// SourceFileList

// repeated string sources = 1;
int SourceFileList::sources_size() const {
  return sources_.size();
}
void SourceFileList::clear_sources() {
  sources_.Clear();
}
 const ::std::string& SourceFileList::sources(int index) const {
  // @@protoc_insertion_point(field_get:SourceFileList.sources)
  return sources_.Get(index);
}
 ::std::string* SourceFileList::mutable_sources(int index) {
  // @@protoc_insertion_point(field_mutable:SourceFileList.sources)
  return sources_.Mutable(index);
}
 void SourceFileList::set_sources(int index, const ::std::string& value) {
  // @@protoc_insertion_point(field_set:SourceFileList.sources)
  sources_.Mutable(index)->assign(value);
}
 void SourceFileList::set_sources(int index, const char* value) {
  sources_.Mutable(index)->assign(value);
  // @@protoc_insertion_point(field_set_char:SourceFileList.sources)
}
 void SourceFileList::set_sources(int index, const char* value, size_t size) {
  sources_.Mutable(index)->assign(
    reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:SourceFileList.sources)
}
 ::std::string* SourceFileList::add_sources() {
  // @@protoc_insertion_point(field_add_mutable:SourceFileList.sources)
  return sources_.Add();
}
 void SourceFileList::add_sources(const ::std::string& value) {
  sources_.Add()->assign(value);
  // @@protoc_insertion_point(field_add:SourceFileList.sources)
}
 void SourceFileList::add_sources(const char* value) {
  sources_.Add()->assign(value);
  // @@protoc_insertion_point(field_add_char:SourceFileList.sources)
}
 void SourceFileList::add_sources(const char* value, size_t size) {
  sources_.Add()->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_add_pointer:SourceFileList.sources)
}
 const ::google::protobuf::RepeatedPtrField< ::std::string>&
SourceFileList::sources() const {
  // @@protoc_insertion_point(field_list:SourceFileList.sources)
  return sources_;
}
 ::google::protobuf::RepeatedPtrField< ::std::string>*
SourceFileList::mutable_sources() {
  // @@protoc_insertion_point(field_mutable_list:SourceFileList.sources)
  return &sources_;
}

#endif  // PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)
