# nfx-stringbuilder TODO

Project roadmap and task tracking for the nfx-stringbuilder library.

### Todo

- [ ] Add convenience methods for common operations
  - [ ] `appendLine(str)` / `appendLn(str)` - append with newline character
  - [ ] `join(container, delimiter)` - join collection elements with delimiter
  - [ ] `substr(pos, len)` for zero-copy access to portions
  - [ ] `replace(pos, len, str)` - in-place replacement
  - [ ] `insert(pos, str)` - insert at position
  - [ ] `truncate(size)` - limit output size
- [ ] Consider SIMD optimization for numeric conversions (may not be worth it - `std::to_chars` is already optimal)
- [ ] Consider compile-time format string validation (explore integration with `std::format` concepts)

### In Progress

- NIL

### Done ✓

- NIL
