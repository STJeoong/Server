// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: MMO_protocol.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_MMO_5fprotocol_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_MMO_5fprotocol_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3021000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3021012 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
#include "MMO_enum.pb.h"
#include "MMO_struct.pb.h"
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_MMO_5fprotocol_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_MMO_5fprotocol_2eproto {
  static const uint32_t offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_MMO_5fprotocol_2eproto;
namespace protocol {
namespace mmo {
class EnterGame_Resp;
struct EnterGame_RespDefaultTypeInternal;
extern EnterGame_RespDefaultTypeInternal _EnterGame_Resp_default_instance_;
class PlayerEnter_Notify;
struct PlayerEnter_NotifyDefaultTypeInternal;
extern PlayerEnter_NotifyDefaultTypeInternal _PlayerEnter_Notify_default_instance_;
class PlayerExit_Notify;
struct PlayerExit_NotifyDefaultTypeInternal;
extern PlayerExit_NotifyDefaultTypeInternal _PlayerExit_Notify_default_instance_;
}  // namespace mmo
}  // namespace protocol
PROTOBUF_NAMESPACE_OPEN
template<> ::protocol::mmo::EnterGame_Resp* Arena::CreateMaybeMessage<::protocol::mmo::EnterGame_Resp>(Arena*);
template<> ::protocol::mmo::PlayerEnter_Notify* Arena::CreateMaybeMessage<::protocol::mmo::PlayerEnter_Notify>(Arena*);
template<> ::protocol::mmo::PlayerExit_Notify* Arena::CreateMaybeMessage<::protocol::mmo::PlayerExit_Notify>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace protocol {
namespace mmo {

// ===================================================================

class EnterGame_Resp final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:protocol.mmo.EnterGame_Resp) */ {
 public:
  inline EnterGame_Resp() : EnterGame_Resp(nullptr) {}
  ~EnterGame_Resp() override;
  explicit PROTOBUF_CONSTEXPR EnterGame_Resp(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  EnterGame_Resp(const EnterGame_Resp& from);
  EnterGame_Resp(EnterGame_Resp&& from) noexcept
    : EnterGame_Resp() {
    *this = ::std::move(from);
  }

  inline EnterGame_Resp& operator=(const EnterGame_Resp& from) {
    CopyFrom(from);
    return *this;
  }
  inline EnterGame_Resp& operator=(EnterGame_Resp&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const EnterGame_Resp& default_instance() {
    return *internal_default_instance();
  }
  static inline const EnterGame_Resp* internal_default_instance() {
    return reinterpret_cast<const EnterGame_Resp*>(
               &_EnterGame_Resp_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(EnterGame_Resp& a, EnterGame_Resp& b) {
    a.Swap(&b);
  }
  inline void Swap(EnterGame_Resp* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(EnterGame_Resp* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  EnterGame_Resp* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<EnterGame_Resp>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const EnterGame_Resp& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom( const EnterGame_Resp& from) {
    EnterGame_Resp::MergeImpl(*this, from);
  }
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _impl_._cached_size_.Get(); }

  private:
  void SharedCtor(::PROTOBUF_NAMESPACE_ID::Arena* arena, bool is_message_owned);
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(EnterGame_Resp* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "protocol.mmo.EnterGame_Resp";
  }
  protected:
  explicit EnterGame_Resp(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kPlayersFieldNumber = 3,
    kRespFieldNumber = 1,
    kIdFieldNumber = 2,
  };
  // repeated .protocol.mmo.PlayerInfo players = 3;
  int players_size() const;
  private:
  int _internal_players_size() const;
  public:
  void clear_players();
  ::protocol::mmo::PlayerInfo* mutable_players(int index);
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::protocol::mmo::PlayerInfo >*
      mutable_players();
  private:
  const ::protocol::mmo::PlayerInfo& _internal_players(int index) const;
  ::protocol::mmo::PlayerInfo* _internal_add_players();
  public:
  const ::protocol::mmo::PlayerInfo& players(int index) const;
  ::protocol::mmo::PlayerInfo* add_players();
  const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::protocol::mmo::PlayerInfo >&
      players() const;

  // .protocol.mmo.E_RespCode resp = 1;
  void clear_resp();
  ::protocol::mmo::E_RespCode resp() const;
  void set_resp(::protocol::mmo::E_RespCode value);
  private:
  ::protocol::mmo::E_RespCode _internal_resp() const;
  void _internal_set_resp(::protocol::mmo::E_RespCode value);
  public:

  // int32 id = 2;
  void clear_id();
  int32_t id() const;
  void set_id(int32_t value);
  private:
  int32_t _internal_id() const;
  void _internal_set_id(int32_t value);
  public:

  // @@protoc_insertion_point(class_scope:protocol.mmo.EnterGame_Resp)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  struct Impl_ {
    ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::protocol::mmo::PlayerInfo > players_;
    int resp_;
    int32_t id_;
    mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_MMO_5fprotocol_2eproto;
};
// -------------------------------------------------------------------

class PlayerEnter_Notify final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:protocol.mmo.PlayerEnter_Notify) */ {
 public:
  inline PlayerEnter_Notify() : PlayerEnter_Notify(nullptr) {}
  ~PlayerEnter_Notify() override;
  explicit PROTOBUF_CONSTEXPR PlayerEnter_Notify(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  PlayerEnter_Notify(const PlayerEnter_Notify& from);
  PlayerEnter_Notify(PlayerEnter_Notify&& from) noexcept
    : PlayerEnter_Notify() {
    *this = ::std::move(from);
  }

  inline PlayerEnter_Notify& operator=(const PlayerEnter_Notify& from) {
    CopyFrom(from);
    return *this;
  }
  inline PlayerEnter_Notify& operator=(PlayerEnter_Notify&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const PlayerEnter_Notify& default_instance() {
    return *internal_default_instance();
  }
  static inline const PlayerEnter_Notify* internal_default_instance() {
    return reinterpret_cast<const PlayerEnter_Notify*>(
               &_PlayerEnter_Notify_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(PlayerEnter_Notify& a, PlayerEnter_Notify& b) {
    a.Swap(&b);
  }
  inline void Swap(PlayerEnter_Notify* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(PlayerEnter_Notify* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  PlayerEnter_Notify* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<PlayerEnter_Notify>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const PlayerEnter_Notify& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom( const PlayerEnter_Notify& from) {
    PlayerEnter_Notify::MergeImpl(*this, from);
  }
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _impl_._cached_size_.Get(); }

  private:
  void SharedCtor(::PROTOBUF_NAMESPACE_ID::Arena* arena, bool is_message_owned);
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(PlayerEnter_Notify* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "protocol.mmo.PlayerEnter_Notify";
  }
  protected:
  explicit PlayerEnter_Notify(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kInfoFieldNumber = 1,
  };
  // .protocol.mmo.PlayerInfo info = 1;
  bool has_info() const;
  private:
  bool _internal_has_info() const;
  public:
  void clear_info();
  const ::protocol::mmo::PlayerInfo& info() const;
  PROTOBUF_NODISCARD ::protocol::mmo::PlayerInfo* release_info();
  ::protocol::mmo::PlayerInfo* mutable_info();
  void set_allocated_info(::protocol::mmo::PlayerInfo* info);
  private:
  const ::protocol::mmo::PlayerInfo& _internal_info() const;
  ::protocol::mmo::PlayerInfo* _internal_mutable_info();
  public:
  void unsafe_arena_set_allocated_info(
      ::protocol::mmo::PlayerInfo* info);
  ::protocol::mmo::PlayerInfo* unsafe_arena_release_info();

  // @@protoc_insertion_point(class_scope:protocol.mmo.PlayerEnter_Notify)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  struct Impl_ {
    ::protocol::mmo::PlayerInfo* info_;
    mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_MMO_5fprotocol_2eproto;
};
// -------------------------------------------------------------------

class PlayerExit_Notify final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:protocol.mmo.PlayerExit_Notify) */ {
 public:
  inline PlayerExit_Notify() : PlayerExit_Notify(nullptr) {}
  ~PlayerExit_Notify() override;
  explicit PROTOBUF_CONSTEXPR PlayerExit_Notify(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  PlayerExit_Notify(const PlayerExit_Notify& from);
  PlayerExit_Notify(PlayerExit_Notify&& from) noexcept
    : PlayerExit_Notify() {
    *this = ::std::move(from);
  }

  inline PlayerExit_Notify& operator=(const PlayerExit_Notify& from) {
    CopyFrom(from);
    return *this;
  }
  inline PlayerExit_Notify& operator=(PlayerExit_Notify&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const PlayerExit_Notify& default_instance() {
    return *internal_default_instance();
  }
  static inline const PlayerExit_Notify* internal_default_instance() {
    return reinterpret_cast<const PlayerExit_Notify*>(
               &_PlayerExit_Notify_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    2;

  friend void swap(PlayerExit_Notify& a, PlayerExit_Notify& b) {
    a.Swap(&b);
  }
  inline void Swap(PlayerExit_Notify* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(PlayerExit_Notify* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  PlayerExit_Notify* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<PlayerExit_Notify>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const PlayerExit_Notify& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom( const PlayerExit_Notify& from) {
    PlayerExit_Notify::MergeImpl(*this, from);
  }
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _impl_._cached_size_.Get(); }

  private:
  void SharedCtor(::PROTOBUF_NAMESPACE_ID::Arena* arena, bool is_message_owned);
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(PlayerExit_Notify* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "protocol.mmo.PlayerExit_Notify";
  }
  protected:
  explicit PlayerExit_Notify(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kIdFieldNumber = 1,
  };
  // int32 id = 1;
  void clear_id();
  int32_t id() const;
  void set_id(int32_t value);
  private:
  int32_t _internal_id() const;
  void _internal_set_id(int32_t value);
  public:

  // @@protoc_insertion_point(class_scope:protocol.mmo.PlayerExit_Notify)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  struct Impl_ {
    int32_t id_;
    mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_MMO_5fprotocol_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// EnterGame_Resp

// .protocol.mmo.E_RespCode resp = 1;
inline void EnterGame_Resp::clear_resp() {
  _impl_.resp_ = 0;
}
inline ::protocol::mmo::E_RespCode EnterGame_Resp::_internal_resp() const {
  return static_cast< ::protocol::mmo::E_RespCode >(_impl_.resp_);
}
inline ::protocol::mmo::E_RespCode EnterGame_Resp::resp() const {
  // @@protoc_insertion_point(field_get:protocol.mmo.EnterGame_Resp.resp)
  return _internal_resp();
}
inline void EnterGame_Resp::_internal_set_resp(::protocol::mmo::E_RespCode value) {
  
  _impl_.resp_ = value;
}
inline void EnterGame_Resp::set_resp(::protocol::mmo::E_RespCode value) {
  _internal_set_resp(value);
  // @@protoc_insertion_point(field_set:protocol.mmo.EnterGame_Resp.resp)
}

// int32 id = 2;
inline void EnterGame_Resp::clear_id() {
  _impl_.id_ = 0;
}
inline int32_t EnterGame_Resp::_internal_id() const {
  return _impl_.id_;
}
inline int32_t EnterGame_Resp::id() const {
  // @@protoc_insertion_point(field_get:protocol.mmo.EnterGame_Resp.id)
  return _internal_id();
}
inline void EnterGame_Resp::_internal_set_id(int32_t value) {
  
  _impl_.id_ = value;
}
inline void EnterGame_Resp::set_id(int32_t value) {
  _internal_set_id(value);
  // @@protoc_insertion_point(field_set:protocol.mmo.EnterGame_Resp.id)
}

// repeated .protocol.mmo.PlayerInfo players = 3;
inline int EnterGame_Resp::_internal_players_size() const {
  return _impl_.players_.size();
}
inline int EnterGame_Resp::players_size() const {
  return _internal_players_size();
}
inline ::protocol::mmo::PlayerInfo* EnterGame_Resp::mutable_players(int index) {
  // @@protoc_insertion_point(field_mutable:protocol.mmo.EnterGame_Resp.players)
  return _impl_.players_.Mutable(index);
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::protocol::mmo::PlayerInfo >*
EnterGame_Resp::mutable_players() {
  // @@protoc_insertion_point(field_mutable_list:protocol.mmo.EnterGame_Resp.players)
  return &_impl_.players_;
}
inline const ::protocol::mmo::PlayerInfo& EnterGame_Resp::_internal_players(int index) const {
  return _impl_.players_.Get(index);
}
inline const ::protocol::mmo::PlayerInfo& EnterGame_Resp::players(int index) const {
  // @@protoc_insertion_point(field_get:protocol.mmo.EnterGame_Resp.players)
  return _internal_players(index);
}
inline ::protocol::mmo::PlayerInfo* EnterGame_Resp::_internal_add_players() {
  return _impl_.players_.Add();
}
inline ::protocol::mmo::PlayerInfo* EnterGame_Resp::add_players() {
  ::protocol::mmo::PlayerInfo* _add = _internal_add_players();
  // @@protoc_insertion_point(field_add:protocol.mmo.EnterGame_Resp.players)
  return _add;
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::protocol::mmo::PlayerInfo >&
EnterGame_Resp::players() const {
  // @@protoc_insertion_point(field_list:protocol.mmo.EnterGame_Resp.players)
  return _impl_.players_;
}

// -------------------------------------------------------------------

// PlayerEnter_Notify

// .protocol.mmo.PlayerInfo info = 1;
inline bool PlayerEnter_Notify::_internal_has_info() const {
  return this != internal_default_instance() && _impl_.info_ != nullptr;
}
inline bool PlayerEnter_Notify::has_info() const {
  return _internal_has_info();
}
inline const ::protocol::mmo::PlayerInfo& PlayerEnter_Notify::_internal_info() const {
  const ::protocol::mmo::PlayerInfo* p = _impl_.info_;
  return p != nullptr ? *p : reinterpret_cast<const ::protocol::mmo::PlayerInfo&>(
      ::protocol::mmo::_PlayerInfo_default_instance_);
}
inline const ::protocol::mmo::PlayerInfo& PlayerEnter_Notify::info() const {
  // @@protoc_insertion_point(field_get:protocol.mmo.PlayerEnter_Notify.info)
  return _internal_info();
}
inline void PlayerEnter_Notify::unsafe_arena_set_allocated_info(
    ::protocol::mmo::PlayerInfo* info) {
  if (GetArenaForAllocation() == nullptr) {
    delete reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(_impl_.info_);
  }
  _impl_.info_ = info;
  if (info) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:protocol.mmo.PlayerEnter_Notify.info)
}
inline ::protocol::mmo::PlayerInfo* PlayerEnter_Notify::release_info() {
  
  ::protocol::mmo::PlayerInfo* temp = _impl_.info_;
  _impl_.info_ = nullptr;
#ifdef PROTOBUF_FORCE_COPY_IN_RELEASE
  auto* old =  reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(temp);
  temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  if (GetArenaForAllocation() == nullptr) { delete old; }
#else  // PROTOBUF_FORCE_COPY_IN_RELEASE
  if (GetArenaForAllocation() != nullptr) {
    temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  }
#endif  // !PROTOBUF_FORCE_COPY_IN_RELEASE
  return temp;
}
inline ::protocol::mmo::PlayerInfo* PlayerEnter_Notify::unsafe_arena_release_info() {
  // @@protoc_insertion_point(field_release:protocol.mmo.PlayerEnter_Notify.info)
  
  ::protocol::mmo::PlayerInfo* temp = _impl_.info_;
  _impl_.info_ = nullptr;
  return temp;
}
inline ::protocol::mmo::PlayerInfo* PlayerEnter_Notify::_internal_mutable_info() {
  
  if (_impl_.info_ == nullptr) {
    auto* p = CreateMaybeMessage<::protocol::mmo::PlayerInfo>(GetArenaForAllocation());
    _impl_.info_ = p;
  }
  return _impl_.info_;
}
inline ::protocol::mmo::PlayerInfo* PlayerEnter_Notify::mutable_info() {
  ::protocol::mmo::PlayerInfo* _msg = _internal_mutable_info();
  // @@protoc_insertion_point(field_mutable:protocol.mmo.PlayerEnter_Notify.info)
  return _msg;
}
inline void PlayerEnter_Notify::set_allocated_info(::protocol::mmo::PlayerInfo* info) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArenaForAllocation();
  if (message_arena == nullptr) {
    delete reinterpret_cast< ::PROTOBUF_NAMESPACE_ID::MessageLite*>(_impl_.info_);
  }
  if (info) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
        ::PROTOBUF_NAMESPACE_ID::Arena::InternalGetOwningArena(
                reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(info));
    if (message_arena != submessage_arena) {
      info = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, info, submessage_arena);
    }
    
  } else {
    
  }
  _impl_.info_ = info;
  // @@protoc_insertion_point(field_set_allocated:protocol.mmo.PlayerEnter_Notify.info)
}

// -------------------------------------------------------------------

// PlayerExit_Notify

// int32 id = 1;
inline void PlayerExit_Notify::clear_id() {
  _impl_.id_ = 0;
}
inline int32_t PlayerExit_Notify::_internal_id() const {
  return _impl_.id_;
}
inline int32_t PlayerExit_Notify::id() const {
  // @@protoc_insertion_point(field_get:protocol.mmo.PlayerExit_Notify.id)
  return _internal_id();
}
inline void PlayerExit_Notify::_internal_set_id(int32_t value) {
  
  _impl_.id_ = value;
}
inline void PlayerExit_Notify::set_id(int32_t value) {
  _internal_set_id(value);
  // @@protoc_insertion_point(field_set:protocol.mmo.PlayerExit_Notify.id)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace mmo
}  // namespace protocol

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_MMO_5fprotocol_2eproto
