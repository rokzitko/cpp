int main()
{
  [[maybe_unused]] int x = 12;
  [[maybe_unused]] int y {}, z {}, w {};
  switch (x) {
  case 11:
  case 12:
    y = 1;
    break;
  case 13:
    z = 2;
    [[fallthrough]];
  case 14:
    w = 3;
    break;
  case 15:
    z = 2;
    // fallthrough here, need -Wextra to report it!
  case 16:
    w = 4;
    break;
  }
}
