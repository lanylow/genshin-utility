#include <Windows.h>
#include <stdio.h>
#include <stdint.h>

void create_console() {
  AllocConsole();
  freopen_s((FILE**)stdout, "conout$", "w", stdout);
}

int compare(const uint8_t* data, const uint8_t* sig, uint32_t size) {
  for (uint32_t i = 0; i < size; i++)
    if (data[i] != sig[i] && sig[i] != 0x00)
      return 0;
  return 1;
}

uint8_t* find_signature(void* start, void* end, const char* sig, uint32_t size) {
  for (uint8_t* i = (uint8_t*)start; i < (uint8_t*)end - size; i++)
    if (compare(i, (uint8_t*)sig, size))
      return i;
  return 0;
}

void* find_pointer(void* module, const char* sig, uint32_t size) {
  IMAGE_DOS_HEADER* dos_header = (IMAGE_DOS_HEADER*)module;
  IMAGE_NT_HEADERS* nt_headers = (IMAGE_NT_HEADERS*)((uint8_t*)module + dos_header->e_lfanew);
  void* end = ((uint8_t*)module + nt_headers->OptionalHeader.SizeOfImage);

  uint8_t* address = find_signature(module, end, sig, size);
  int32_t i;
  for (i = 0; sig[i]; i++);
  int32_t offset = *(int32_t*)(address + i);
  return address + i + 4 + offset;
}