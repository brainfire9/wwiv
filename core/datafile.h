/**************************************************************************/
/*                                                                        */
/*                          WWIV Version 5.x                              */
/*            Copyright (C)2014-2020, WWIV Software Services              */
/*                                                                        */
/*    Licensed  under the  Apache License, Version  2.0 (the "License");  */
/*    you may not use this  file  except in compliance with the License.  */
/*    You may obtain a copy of the License at                             */
/*                                                                        */
/*                http://www.apache.org/licenses/LICENSE-2.0              */
/*                                                                        */
/*    Unless  required  by  applicable  law  or agreed to  in  writing,   */
/*    software  distributed  under  the  License  is  distributed on an   */
/*    "AS IS"  BASIS, WITHOUT  WARRANTIES  OR  CONDITIONS OF ANY  KIND,   */
/*    either  express  or implied.  See  the  License for  the specific   */
/*    language governing permissions and limitations under the License.   */
/**************************************************************************/
#ifndef __INCLUDED_CORE_DATAFILE_H__
#define __INCLUDED_CORE_DATAFILE_H__

#include <vector>
#include "core/file.h"
#include <filesystem>

namespace wwiv {
namespace core {

/**
 * File: Provides a high level, cross-platform common wrapper for file
 * of repeating structs (like Pascal records).  Many of the common WWIV
 * datafiles are multiple copies of the same struct type.
 *
 * Example:
 *   vector<mailrec> emails;
 *   DataFile<mailrec> f(PathFilePath("/home/wwiv/bbs/data", "email.dat"));
 *   if (!f) { LOG(FATAL) << "email.dat does not exist!" << endl; }
 *   if (!f.ReadVector(emails) { LOG(FATAL) << "unable to load email.dat"; }
 *   // No need to close f since when f goes out of scope it'll close automatically.
 */
template <typename RECORD, std::size_t SIZE = sizeof(RECORD)> class DataFile final {
public:
  DataFile(const std::filesystem::path& full_file_name,
           int nFileMode = File::modeDefault,
           int nShareMode = File::shareUnknown)
    : file_(full_file_name) {
    file_.Open(nFileMode, nShareMode);
  }

  ~DataFile() = default;

  [[nodiscard]] File& file() { return file_; }

  void Close() { file_.Close(); }

  [[nodiscard]] bool ok() const { return file_.IsOpen(); }

  bool ReadVector(std::vector<RECORD>& records, std::size_t max_records = 0) {
    auto num_to_read = number_of_records();
    if (num_to_read == 0) {
      // Reading nothing is always successful.
      return true;
    }
    if (max_records != 0 && max_records < num_to_read) {
      num_to_read = max_records;
    }
    if (records.size() < num_to_read) {
      records.resize(num_to_read);
    }
    return Read(&records[0], num_to_read);
  }

  bool Read(RECORD* record, int num_records = 1) {
    if (num_records == 0) {
      // Reading nothing is always successful.
      return true;
    }
    return file_.Read(record, num_records * SIZE) == static_cast<int>(num_records * SIZE);
  }

  bool Read(int record_number, RECORD* record) {
    if (!Seek(record_number)) {
      return false;
    }
    return Read(record);
  }

  bool WriteVector(const std::vector<RECORD>& records, std::size_t max_records = 0) {
    auto num = records.size();
    if (max_records != 0 && max_records < num) {
      num = max_records;
    }
    return Write(&records[0], num);
  }

  bool Write(const RECORD* record, int num_records = 1) {
    return file_.Write(record, num_records * SIZE) == static_cast<ssize_t>(num_records * SIZE);
  }

  bool Write(int record_number, const RECORD* record) {
    if (!Seek(record_number)) {
      return false;
    }
    return Write(record);
  }

  bool Seek(int record_number) {
    return file_.Seek(record_number * SIZE, File::Whence::begin) ==
           static_cast<File::size_type>(record_number * SIZE);
  }

  [[nodiscard]] std::size_t number_of_records() const noexcept {
    return static_cast<std::size_t>(file_.length() / SIZE);
  }

  explicit operator bool() const noexcept { return file_.IsOpen(); }

private:
  File file_;
};

}
}

#endif  // __INCLUDED_CORE_DATAFILE_H__
