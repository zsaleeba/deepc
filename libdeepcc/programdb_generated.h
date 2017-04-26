// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_PROGRAMDB_DEEPC_H_
#define FLATBUFFERS_GENERATED_PROGRAMDB_DEEPC_H_

#include "flatbuffers/flatbuffers.h"

namespace deepC {

struct SourceModified;

struct SourceFile;

MANUALLY_ALIGNED_STRUCT(8) SourceModified FLATBUFFERS_FINAL_CLASS {
 private:
  uint64_t seconds_;
  uint32_t nanoseconds_;
  int32_t padding0__;

 public:
  SourceModified() {
    memset(this, 0, sizeof(SourceModified));
  }
  SourceModified(const SourceModified &_o) {
    memcpy(this, &_o, sizeof(SourceModified));
  }
  SourceModified(uint64_t _seconds, uint32_t _nanoseconds)
      : seconds_(flatbuffers::EndianScalar(_seconds)),
        nanoseconds_(flatbuffers::EndianScalar(_nanoseconds)),
        padding0__(0) {
    (void)padding0__;
  }
  uint64_t seconds() const {
    return flatbuffers::EndianScalar(seconds_);
  }
  uint32_t nanoseconds() const {
    return flatbuffers::EndianScalar(nanoseconds_);
  }
};
STRUCT_END(SourceModified, 16);

struct SourceFile FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_FILENAME = 4,
    VT_SOURCE = 6,
    VT_MODIFIED = 8
  };
  const flatbuffers::String *filename() const {
    return GetPointer<const flatbuffers::String *>(VT_FILENAME);
  }
  const flatbuffers::String *source() const {
    return GetPointer<const flatbuffers::String *>(VT_SOURCE);
  }
  const SourceModified *modified() const {
    return GetStruct<const SourceModified *>(VT_MODIFIED);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_FILENAME) &&
           verifier.Verify(filename()) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_SOURCE) &&
           verifier.Verify(source()) &&
           VerifyField<SourceModified>(verifier, VT_MODIFIED) &&
           verifier.EndTable();
  }
};

struct SourceFileBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_filename(flatbuffers::Offset<flatbuffers::String> filename) {
    fbb_.AddOffset(SourceFile::VT_FILENAME, filename);
  }
  void add_source(flatbuffers::Offset<flatbuffers::String> source) {
    fbb_.AddOffset(SourceFile::VT_SOURCE, source);
  }
  void add_modified(const SourceModified *modified) {
    fbb_.AddStruct(SourceFile::VT_MODIFIED, modified);
  }
  SourceFileBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  SourceFileBuilder &operator=(const SourceFileBuilder &);
  flatbuffers::Offset<SourceFile> Finish() {
    const auto end = fbb_.EndTable(start_, 3);
    auto o = flatbuffers::Offset<SourceFile>(end);
    return o;
  }
};

inline flatbuffers::Offset<SourceFile> CreateSourceFile(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> filename = 0,
    flatbuffers::Offset<flatbuffers::String> source = 0,
    const SourceModified *modified = 0) {
  SourceFileBuilder builder_(_fbb);
  builder_.add_modified(modified);
  builder_.add_source(source);
  builder_.add_filename(filename);
  return builder_.Finish();
}

inline flatbuffers::Offset<SourceFile> CreateSourceFileDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *filename = nullptr,
    const char *source = nullptr,
    const SourceModified *modified = 0) {
  return deepC::CreateSourceFile(
      _fbb,
      filename ? _fbb.CreateString(filename) : 0,
      source ? _fbb.CreateString(source) : 0,
      modified);
}

inline const deepC::SourceFile *GetSourceFile(const void *buf) {
  return flatbuffers::GetRoot<deepC::SourceFile>(buf);
}

inline bool VerifySourceFileBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<deepC::SourceFile>(nullptr);
}

inline void FinishSourceFileBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<deepC::SourceFile> root) {
  fbb.Finish(root);
}

}  // namespace deepC

#endif  // FLATBUFFERS_GENERATED_PROGRAMDB_DEEPC_H_
