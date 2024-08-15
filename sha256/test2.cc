#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>

using namespace std;

#include <openssl/sha.h>
string sha256file(const string &filename)
{
  unsigned char hash[SHA256_DIGEST_LENGTH];
  SHA256_CTX sha256;
  SHA256_Init(&sha256);
  std::ifstream t(filename);
  std::stringstream buf;
  buf << t.rdbuf();
  SHA256_Update(&sha256, buf.str().c_str(), buf.str().size());
  SHA256_Final(hash, &sha256);
  stringstream ss;
  for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
  {
    ss << hex << setw(2) << setfill('0') << (int)hash[i];
  }
  return ss.str();
}

int main() {
  cout << sha256file("file1") << endl;
  cout << sha256file("file2") << endl;
  cout << sha256file("file3") << endl;
  cout << sha256file("file4") << endl;
  return 0;
}
