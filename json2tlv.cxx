#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using nlohmann::json;
using namespace std;

// write primitive types in binary representation to output stream
template <typename T> void writebin(ostream &s, const T v) {
  s.write((const char *)&v, sizeof(v));
}

void writestr(ostream &s, const string &v) {
  uint32_t len(v.size());
  writebin(s, len);
  s.write((const char *)v.data(), v.size());
}

int main() {
  ofstream out("out.bin", ofstream::binary);
  // dictionary
  // map will be used for fast index lookup
  map<string, unsigned> key2number;
  // vector will be used to write dictionary in correct order at EOF
  vector<string> keys;
  for (;;) {
    // read lines from stdin stopping at empty line
    string line;
    getline(cin, line);
    if (line.empty())
      break;
    json j(json::parse(line));
    // collect list of keys and number them
    for (const auto &el : j.items()) {
      if (key2number.find(el.key()) == key2number.end()) {
        key2number[el.key()] = 1 + key2number.size();
        keys.emplace_back(el.key());
      }
    }
    // iterate once again over JSON items and write them to TLV
    for (const auto &el : j.items()) {
      // write key number
      writebin<uint16_t>(out, key2number[el.key()]);
      // write value
      switch (el.value().type()) {
      case json::value_t::null:
        writebin<uint8_t>(out, 1);
        break;
      case json::value_t::string:
        writebin<uint8_t>(out, 2);
        writestr(out, el.value().get<string>());
        break;
      case json::value_t::boolean:
        writebin<uint8_t>(out, 3);
        writebin<uint8_t>(out, el.value().get<bool>());
        break;
      case json::value_t::number_integer:
        writebin<uint8_t>(out, 4);
        writebin(out, el.value().get<int32_t>());
        break;
      case json::value_t::number_unsigned:
        writebin<uint8_t>(out, 5);
        writebin(out, el.value().get<uint32_t>());
        break;
      case json::value_t::number_float:
        writebin<uint8_t>(out, 6);
        writebin(out, el.value().get<float>());
        break;
      default:
        uint8_t type(static_cast<uint8_t>(el.value().type()));
        throw runtime_error(string("Unsupported type: ") + to_string(type));
      }
    }
  }
  // 0 type means end of records and start of dictionary
  writebin<uint8_t>(out, 0);
  // write number of items in dictionary
  writebin<uint16_t>(out, keys.size());
  // write all dictionary items
  for (const auto &k : keys)
    writestr(out, k);
  return 0;
}
