// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: MMO_struct.proto

#include "MMO_struct.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

PROTOBUF_PRAGMA_INIT_SEG

namespace _pb = ::PROTOBUF_NAMESPACE_ID;
namespace _pbi = _pb::internal;

namespace protocol {
namespace mmo {
PROTOBUF_CONSTEXPR TransformInt::TransformInt(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.x_)*/0
  , /*decltype(_impl_.y_)*/0
  , /*decltype(_impl_.dir_)*/0
  , /*decltype(_impl_._cached_size_)*/{}} {}
struct TransformIntDefaultTypeInternal {
  PROTOBUF_CONSTEXPR TransformIntDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~TransformIntDefaultTypeInternal() {}
  union {
    TransformInt _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 TransformIntDefaultTypeInternal _TransformInt_default_instance_;
PROTOBUF_CONSTEXPR ObjectInfo::ObjectInfo(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.transform_)*/nullptr
  , /*decltype(_impl_.id_)*/0
  , /*decltype(_impl_.state_)*/0
  , /*decltype(_impl_._cached_size_)*/{}} {}
struct ObjectInfoDefaultTypeInternal {
  PROTOBUF_CONSTEXPR ObjectInfoDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~ObjectInfoDefaultTypeInternal() {}
  union {
    ObjectInfo _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 ObjectInfoDefaultTypeInternal _ObjectInfo_default_instance_;
}  // namespace mmo
}  // namespace protocol
static ::_pb::Metadata file_level_metadata_MMO_5fstruct_2eproto[2];
static constexpr ::_pb::EnumDescriptor const** file_level_enum_descriptors_MMO_5fstruct_2eproto = nullptr;
static constexpr ::_pb::ServiceDescriptor const** file_level_service_descriptors_MMO_5fstruct_2eproto = nullptr;

const uint32_t TableStruct_MMO_5fstruct_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::protocol::mmo::TransformInt, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::protocol::mmo::TransformInt, _impl_.x_),
  PROTOBUF_FIELD_OFFSET(::protocol::mmo::TransformInt, _impl_.y_),
  PROTOBUF_FIELD_OFFSET(::protocol::mmo::TransformInt, _impl_.dir_),
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::protocol::mmo::ObjectInfo, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::protocol::mmo::ObjectInfo, _impl_.id_),
  PROTOBUF_FIELD_OFFSET(::protocol::mmo::ObjectInfo, _impl_.state_),
  PROTOBUF_FIELD_OFFSET(::protocol::mmo::ObjectInfo, _impl_.transform_),
};
static const ::_pbi::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, -1, sizeof(::protocol::mmo::TransformInt)},
  { 9, -1, -1, sizeof(::protocol::mmo::ObjectInfo)},
};

static const ::_pb::Message* const file_default_instances[] = {
  &::protocol::mmo::_TransformInt_default_instance_._instance,
  &::protocol::mmo::_ObjectInfo_default_instance_._instance,
};

const char descriptor_table_protodef_MMO_5fstruct_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\020MMO_struct.proto\022\014protocol.mmo\032\016MMO_en"
  "um.proto\"F\n\014TransformInt\022\t\n\001x\030\001 \001(\005\022\t\n\001y"
  "\030\002 \001(\005\022 \n\003dir\030\003 \001(\0162\023.protocol.mmo.E_Dir"
  "\"s\n\nObjectInfo\022\n\n\002id\030\001 \001(\005\022*\n\005state\030\002 \001("
  "\0162\033.protocol.mmo.E_ObjectState\022-\n\ttransf"
  "orm\030\003 \001(\0132\032.protocol.mmo.TransformIntb\006p"
  "roto3"
  ;
static const ::_pbi::DescriptorTable* const descriptor_table_MMO_5fstruct_2eproto_deps[1] = {
  &::descriptor_table_MMO_5fenum_2eproto,
};
static ::_pbi::once_flag descriptor_table_MMO_5fstruct_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_MMO_5fstruct_2eproto = {
    false, false, 245, descriptor_table_protodef_MMO_5fstruct_2eproto,
    "MMO_struct.proto",
    &descriptor_table_MMO_5fstruct_2eproto_once, descriptor_table_MMO_5fstruct_2eproto_deps, 1, 2,
    schemas, file_default_instances, TableStruct_MMO_5fstruct_2eproto::offsets,
    file_level_metadata_MMO_5fstruct_2eproto, file_level_enum_descriptors_MMO_5fstruct_2eproto,
    file_level_service_descriptors_MMO_5fstruct_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_MMO_5fstruct_2eproto_getter() {
  return &descriptor_table_MMO_5fstruct_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2 static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_MMO_5fstruct_2eproto(&descriptor_table_MMO_5fstruct_2eproto);
namespace protocol {
namespace mmo {

// ===================================================================

class TransformInt::_Internal {
 public:
};

TransformInt::TransformInt(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:protocol.mmo.TransformInt)
}
TransformInt::TransformInt(const TransformInt& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  TransformInt* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.x_){}
    , decltype(_impl_.y_){}
    , decltype(_impl_.dir_){}
    , /*decltype(_impl_._cached_size_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::memcpy(&_impl_.x_, &from._impl_.x_,
    static_cast<size_t>(reinterpret_cast<char*>(&_impl_.dir_) -
    reinterpret_cast<char*>(&_impl_.x_)) + sizeof(_impl_.dir_));
  // @@protoc_insertion_point(copy_constructor:protocol.mmo.TransformInt)
}

inline void TransformInt::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_.x_){0}
    , decltype(_impl_.y_){0}
    , decltype(_impl_.dir_){0}
    , /*decltype(_impl_._cached_size_)*/{}
  };
}

TransformInt::~TransformInt() {
  // @@protoc_insertion_point(destructor:protocol.mmo.TransformInt)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void TransformInt::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
}

void TransformInt::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void TransformInt::Clear() {
// @@protoc_insertion_point(message_clear_start:protocol.mmo.TransformInt)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  ::memset(&_impl_.x_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&_impl_.dir_) -
      reinterpret_cast<char*>(&_impl_.x_)) + sizeof(_impl_.dir_));
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* TransformInt::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // int32 x = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 8)) {
          _impl_.x_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // int32 y = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 16)) {
          _impl_.y_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // .protocol.mmo.E_Dir dir = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 24)) {
          uint64_t val = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
          _internal_set_dir(static_cast<::protocol::mmo::E_Dir>(val));
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* TransformInt::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:protocol.mmo.TransformInt)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // int32 x = 1;
  if (this->_internal_x() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteInt32ToArray(1, this->_internal_x(), target);
  }

  // int32 y = 2;
  if (this->_internal_y() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteInt32ToArray(2, this->_internal_y(), target);
  }

  // .protocol.mmo.E_Dir dir = 3;
  if (this->_internal_dir() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteEnumToArray(
      3, this->_internal_dir(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:protocol.mmo.TransformInt)
  return target;
}

size_t TransformInt::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:protocol.mmo.TransformInt)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // int32 x = 1;
  if (this->_internal_x() != 0) {
    total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(this->_internal_x());
  }

  // int32 y = 2;
  if (this->_internal_y() != 0) {
    total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(this->_internal_y());
  }

  // .protocol.mmo.E_Dir dir = 3;
  if (this->_internal_dir() != 0) {
    total_size += 1 +
      ::_pbi::WireFormatLite::EnumSize(this->_internal_dir());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData TransformInt::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    TransformInt::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*TransformInt::GetClassData() const { return &_class_data_; }


void TransformInt::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<TransformInt*>(&to_msg);
  auto& from = static_cast<const TransformInt&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:protocol.mmo.TransformInt)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (from._internal_x() != 0) {
    _this->_internal_set_x(from._internal_x());
  }
  if (from._internal_y() != 0) {
    _this->_internal_set_y(from._internal_y());
  }
  if (from._internal_dir() != 0) {
    _this->_internal_set_dir(from._internal_dir());
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void TransformInt::CopyFrom(const TransformInt& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:protocol.mmo.TransformInt)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool TransformInt::IsInitialized() const {
  return true;
}

void TransformInt::InternalSwap(TransformInt* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(TransformInt, _impl_.dir_)
      + sizeof(TransformInt::_impl_.dir_)
      - PROTOBUF_FIELD_OFFSET(TransformInt, _impl_.x_)>(
          reinterpret_cast<char*>(&_impl_.x_),
          reinterpret_cast<char*>(&other->_impl_.x_));
}

::PROTOBUF_NAMESPACE_ID::Metadata TransformInt::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_MMO_5fstruct_2eproto_getter, &descriptor_table_MMO_5fstruct_2eproto_once,
      file_level_metadata_MMO_5fstruct_2eproto[0]);
}

// ===================================================================

class ObjectInfo::_Internal {
 public:
  static const ::protocol::mmo::TransformInt& transform(const ObjectInfo* msg);
};

const ::protocol::mmo::TransformInt&
ObjectInfo::_Internal::transform(const ObjectInfo* msg) {
  return *msg->_impl_.transform_;
}
ObjectInfo::ObjectInfo(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:protocol.mmo.ObjectInfo)
}
ObjectInfo::ObjectInfo(const ObjectInfo& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  ObjectInfo* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.transform_){nullptr}
    , decltype(_impl_.id_){}
    , decltype(_impl_.state_){}
    , /*decltype(_impl_._cached_size_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  if (from._internal_has_transform()) {
    _this->_impl_.transform_ = new ::protocol::mmo::TransformInt(*from._impl_.transform_);
  }
  ::memcpy(&_impl_.id_, &from._impl_.id_,
    static_cast<size_t>(reinterpret_cast<char*>(&_impl_.state_) -
    reinterpret_cast<char*>(&_impl_.id_)) + sizeof(_impl_.state_));
  // @@protoc_insertion_point(copy_constructor:protocol.mmo.ObjectInfo)
}

inline void ObjectInfo::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_.transform_){nullptr}
    , decltype(_impl_.id_){0}
    , decltype(_impl_.state_){0}
    , /*decltype(_impl_._cached_size_)*/{}
  };
}

ObjectInfo::~ObjectInfo() {
  // @@protoc_insertion_point(destructor:protocol.mmo.ObjectInfo)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void ObjectInfo::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  if (this != internal_default_instance()) delete _impl_.transform_;
}

void ObjectInfo::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void ObjectInfo::Clear() {
// @@protoc_insertion_point(message_clear_start:protocol.mmo.ObjectInfo)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  if (GetArenaForAllocation() == nullptr && _impl_.transform_ != nullptr) {
    delete _impl_.transform_;
  }
  _impl_.transform_ = nullptr;
  ::memset(&_impl_.id_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&_impl_.state_) -
      reinterpret_cast<char*>(&_impl_.id_)) + sizeof(_impl_.state_));
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* ObjectInfo::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // int32 id = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 8)) {
          _impl_.id_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // .protocol.mmo.E_ObjectState state = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 16)) {
          uint64_t val = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
          _internal_set_state(static_cast<::protocol::mmo::E_ObjectState>(val));
        } else
          goto handle_unusual;
        continue;
      // .protocol.mmo.TransformInt transform = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 26)) {
          ptr = ctx->ParseMessage(_internal_mutable_transform(), ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* ObjectInfo::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:protocol.mmo.ObjectInfo)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // int32 id = 1;
  if (this->_internal_id() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteInt32ToArray(1, this->_internal_id(), target);
  }

  // .protocol.mmo.E_ObjectState state = 2;
  if (this->_internal_state() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteEnumToArray(
      2, this->_internal_state(), target);
  }

  // .protocol.mmo.TransformInt transform = 3;
  if (this->_internal_has_transform()) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(3, _Internal::transform(this),
        _Internal::transform(this).GetCachedSize(), target, stream);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:protocol.mmo.ObjectInfo)
  return target;
}

size_t ObjectInfo::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:protocol.mmo.ObjectInfo)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // .protocol.mmo.TransformInt transform = 3;
  if (this->_internal_has_transform()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *_impl_.transform_);
  }

  // int32 id = 1;
  if (this->_internal_id() != 0) {
    total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(this->_internal_id());
  }

  // .protocol.mmo.E_ObjectState state = 2;
  if (this->_internal_state() != 0) {
    total_size += 1 +
      ::_pbi::WireFormatLite::EnumSize(this->_internal_state());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData ObjectInfo::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    ObjectInfo::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*ObjectInfo::GetClassData() const { return &_class_data_; }


void ObjectInfo::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<ObjectInfo*>(&to_msg);
  auto& from = static_cast<const ObjectInfo&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:protocol.mmo.ObjectInfo)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (from._internal_has_transform()) {
    _this->_internal_mutable_transform()->::protocol::mmo::TransformInt::MergeFrom(
        from._internal_transform());
  }
  if (from._internal_id() != 0) {
    _this->_internal_set_id(from._internal_id());
  }
  if (from._internal_state() != 0) {
    _this->_internal_set_state(from._internal_state());
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void ObjectInfo::CopyFrom(const ObjectInfo& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:protocol.mmo.ObjectInfo)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool ObjectInfo::IsInitialized() const {
  return true;
}

void ObjectInfo::InternalSwap(ObjectInfo* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(ObjectInfo, _impl_.state_)
      + sizeof(ObjectInfo::_impl_.state_)
      - PROTOBUF_FIELD_OFFSET(ObjectInfo, _impl_.transform_)>(
          reinterpret_cast<char*>(&_impl_.transform_),
          reinterpret_cast<char*>(&other->_impl_.transform_));
}

::PROTOBUF_NAMESPACE_ID::Metadata ObjectInfo::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_MMO_5fstruct_2eproto_getter, &descriptor_table_MMO_5fstruct_2eproto_once,
      file_level_metadata_MMO_5fstruct_2eproto[1]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace mmo
}  // namespace protocol
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::protocol::mmo::TransformInt*
Arena::CreateMaybeMessage< ::protocol::mmo::TransformInt >(Arena* arena) {
  return Arena::CreateMessageInternal< ::protocol::mmo::TransformInt >(arena);
}
template<> PROTOBUF_NOINLINE ::protocol::mmo::ObjectInfo*
Arena::CreateMaybeMessage< ::protocol::mmo::ObjectInfo >(Arena* arena) {
  return Arena::CreateMessageInternal< ::protocol::mmo::ObjectInfo >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
