#include <windows.h>
#include <tlhelp32.h>

#include <print>
#include <iostream>
#include <string>
#include <filesystem>

namespace {

struct AutoRelease {
  HANDLE handle;

  AutoRelease(HANDLE handle)
    : handle(handle) {}

  ~AutoRelease() {
    CloseHandle(handle);
  }
};

uint32_t GetProcessIdByName(const char* name) {
  const auto snapshot = AutoRelease(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0));

  if (snapshot.handle == INVALID_HANDLE_VALUE)
    throw std::exception("Couldn't create a process list snapshot.");

  auto entry = PROCESSENTRY32();
  entry.dwSize = sizeof(entry);

  while (Process32Next(snapshot.handle, &entry))
    if (entry.szExeFile == name)
      return entry.th32ProcessID;

  return 0u;
}

void Inject(uint32_t process_id, const std::string& path) {
  const auto proc = AutoRelease(OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_WRITE, false, process_id));

  if (proc.handle == INVALID_HANDLE_VALUE)
    throw std::exception("OpenProcess returned 0, make sure the game is open before running this.");

  const auto alloc = VirtualAllocEx(proc.handle, nullptr, path.length() + 1, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
  std::println("Allocated memory for the library path at 0x{:x}.", (uintptr_t)alloc);

  if (alloc == nullptr)
    throw std::exception("Couldn't allocate memory for the DLL path.");

  WriteProcessMemory(proc.handle, alloc, path.c_str(), path.length() + 1, nullptr);
  AutoRelease(CreateRemoteThread(proc.handle, nullptr, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, alloc, 0, nullptr));
}

void Init() {
  std::println("Waiting for Genshin Impact or Honkai: Star Rail to start.");

  auto process_id = 0u;

  for (;;) {
    for (const auto name : {"GenshinImpact.exe", "YuanShen.exe", "StarRail.exe"})
      if (const auto id = GetProcessIdByName(name); id != 0u)
        process_id = id;

    if (process_id != 0u)
      break;
  }

  std::println("Injecting into {}.", process_id);

  const auto exe_path = std::filesystem::current_path();
  const auto dll_path = exe_path / "library.dll";

  std::println("Library path: {}.", dll_path.string());

  Inject(process_id, dll_path.string());

  std::println("Done.");
}

}

int main() {
  try {
    Init();
  }
  catch (const std::exception& e) {
    std::println(std::cerr, "{}", e.what());
  }
}
