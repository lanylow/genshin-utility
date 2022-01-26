#include <Windows.h>
#include <stdio.h>
#include <stdint.h>

int open_process(const char* path, uint32_t* process_id, HANDLE* handle) {
  STARTUPINFO startup_info = { 0 };
  PROCESS_INFORMATION process_info = { 0 };

  int res = CreateProcessA(path, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &startup_info, &process_info);
  CloseHandle(process_info.hThread);

  *process_id = process_info.dwProcessId;
  *handle = process_info.hProcess;

  return res;
}

void throw_error(const char* message) {
  printf("%s", message);
  Sleep(5000);
  exit(-1);
}

void throw_error_and_terminate(const char* message, HANDLE process) {
  TerminateProcess(process, -1);
  throw_error(message);
}

int main(int argc, char** argv) {
  if (*++argv == NULL) {
    fprintf(stderr, "ERROR: genshin impact process path was not provided\n");
    exit(1);
  }

  char* process_path = *argv;
  uint32_t process_id = 0;
  HANDLE process_handle = 0;

  if (!open_process(process_path, &process_id, &process_handle))
    throw_error_and_terminate("Failed to create Genshin Impact process\n", process_handle);

  printf("Successfully created process GenshinImpact.exe with ID %d\n", process_id);

  char dll_path[MAX_PATH];
  if(!GetFullPathNameA("library.dll", MAX_PATH, dll_path, NULL))
    throw_error_and_terminate("Failed to get library.dll path\n", process_handle);

  printf("Injecting file: %s\n", dll_path);
  printf("Trying to find LoadLibraryA address\n");
  void* load_library = NULL;
  if (!(load_library = (void*)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA")))
    throw_error_and_terminate("Failed to get LoadLibraryA address\n", process_handle);

  printf("LoadLibraryA found at: 0x%p\n", load_library);
  printf("Trying to allocate memory\n");
  void* allocation = NULL;
  if (!(allocation = VirtualAllocEx(process_handle, NULL, strlen(dll_path), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE)))
    throw_error_and_terminate("Failed to allocate memory\n", process_handle);

  printf("Memory allocated at: 0x%p\n", allocation);
  printf("Writing path to the allocated memory\n");
  WriteProcessMemory(process_handle, allocation, dll_path, strlen(dll_path), NULL);

  printf("Creating LoadLibraryA thread\n");
  CreateRemoteThread(process_handle, NULL, 0, (LPTHREAD_START_ROUTINE)load_library, (void*)allocation, 0, NULL);

  CloseHandle(process_handle);
}