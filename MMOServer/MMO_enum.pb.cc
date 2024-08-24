// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: MMO_enum.proto

#include "MMO_enum.pb.h"

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
}  // namespace mmo
}  // namespace protocol
static const ::_pb::EnumDescriptor* file_level_enum_descriptors_MMO_5fenum_2eproto[5];
static constexpr ::_pb::ServiceDescriptor const** file_level_service_descriptors_MMO_5fenum_2eproto = nullptr;
const uint32_t TableStruct_MMO_5fenum_2eproto::offsets[1] = {};
static constexpr ::_pbi::MigrationSchema* schemas = nullptr;
static constexpr ::_pb::Message* const* file_default_instances = nullptr;

const char descriptor_table_protodef_MMO_5fenum_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\016MMO_enum.proto\022\014protocol.mmo*\205\001\n\nE_Pac"
  "ketID\022\022\n\016ENTER_GAME_REQ\020\000\022\023\n\017ENTER_GAME_"
  "RESP\020\001\022\027\n\023OBJECT_ENTER_NOTIFY\020\002\022\026\n\022OBJEC"
  "T_EXIT_NOTIFY\020\003\022\014\n\010MOVE_REQ\020\004\022\017\n\013MOVE_NO"
  "TIFY\020\005*\034\n\nE_RespCode\022\006\n\002OK\020\000\022\006\n\002NO\020\001*-\n\r"
  "E_ObjectState\022\010\n\004NONE\020\000\022\010\n\004IDLE\020\001\022\010\n\004MOV"
  "E\020\002*9\n\005E_Dir\022\n\n\006BOTTOM\020\000\022\010\n\004LEFT\020\001\022\006\n\002UP"
  "\020\002\022\t\n\005RIGHT\020\003\022\007\n\003MAX\020\004*\'\n\tE_KeyCode\022\005\n\001Q"
  "\020\000\022\005\n\001W\020\001\022\005\n\001E\020\002\022\005\n\001R\020\003b\006proto3"
  ;
static ::_pbi::once_flag descriptor_table_MMO_5fenum_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_MMO_5fenum_2eproto = {
    false, false, 351, descriptor_table_protodef_MMO_5fenum_2eproto,
    "MMO_enum.proto",
    &descriptor_table_MMO_5fenum_2eproto_once, nullptr, 0, 0,
    schemas, file_default_instances, TableStruct_MMO_5fenum_2eproto::offsets,
    nullptr, file_level_enum_descriptors_MMO_5fenum_2eproto,
    file_level_service_descriptors_MMO_5fenum_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_MMO_5fenum_2eproto_getter() {
  return &descriptor_table_MMO_5fenum_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2 static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_MMO_5fenum_2eproto(&descriptor_table_MMO_5fenum_2eproto);
namespace protocol {
namespace mmo {
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* E_PacketID_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_MMO_5fenum_2eproto);
  return file_level_enum_descriptors_MMO_5fenum_2eproto[0];
}
bool E_PacketID_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
      return true;
    default:
      return false;
  }
}

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* E_RespCode_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_MMO_5fenum_2eproto);
  return file_level_enum_descriptors_MMO_5fenum_2eproto[1];
}
bool E_RespCode_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
      return true;
    default:
      return false;
  }
}

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* E_ObjectState_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_MMO_5fenum_2eproto);
  return file_level_enum_descriptors_MMO_5fenum_2eproto[2];
}
bool E_ObjectState_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
      return true;
    default:
      return false;
  }
}

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* E_Dir_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_MMO_5fenum_2eproto);
  return file_level_enum_descriptors_MMO_5fenum_2eproto[3];
}
bool E_Dir_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
      return true;
    default:
      return false;
  }
}

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* E_KeyCode_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_MMO_5fenum_2eproto);
  return file_level_enum_descriptors_MMO_5fenum_2eproto[4];
}
bool E_KeyCode_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
      return true;
    default:
      return false;
  }
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace mmo
}  // namespace protocol
PROTOBUF_NAMESPACE_OPEN
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
