use std::{
    ffi::{CStr, CString},
    env::consts::DLL_EXTENSION,
    ptr,
    mem,
    error
};

use winapi::{
    um::{
        tlhelp32::{PROCESSENTRY32, Process32First, Process32Next, CreateToolhelp32Snapshot, TH32CS_SNAPPROCESS},
        libloaderapi::{GetModuleHandleA, GetProcAddress},
        winnt::{PROCESS_CREATE_THREAD, PROCESS_VM_OPERATION, PROCESS_VM_WRITE, MEM_RESERVE, MEM_COMMIT, PAGE_READWRITE},
        handleapi::CloseHandle,  
        processthreadsapi::{OpenProcess, CreateRemoteThread}, memoryapi::{VirtualAllocEx, WriteProcessMemory}, 
    }, 
    shared::minwindef::{FALSE, LPVOID, DWORD}
};

use thiserror::Error;

#[derive(Debug, Error)]
enum LoaderError {
    #[error("Couldn't create a process list snapshot.")]
    ProcessListSnapshot,
    #[error("Couldn't find a module handle.")]
    ModuleHandle,
    #[error("Couldn't find a function address.")]
    FunctionAddress,
    #[error("OpenProcess returned 0, make sure Genshin Impact is opened before running this.")]
    OpenProcessNull,
    #[error("Couldn't allocate memory for the DLL path.")]
    AllocationFailed
}

type ThreadStartRoutine = unsafe extern "system" fn(LPVOID) -> DWORD;

unsafe fn get_process_id_by_name(name: &str) -> Result<u32, Box<dyn error::Error>> {
    let snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if snapshot.is_null() {
        return Err(LoaderError::ProcessListSnapshot.into());
    }

    let mut entry: PROCESSENTRY32 = mem::zeroed();
    entry.dwSize = mem::size_of::<PROCESSENTRY32>() as _;

    if Process32First(snapshot, &mut entry) != 0 {
        while Process32Next(snapshot, &mut entry) != 0 {
            let current = CStr::from_ptr(entry.szExeFile.as_ptr() as _).to_str()?;

            if current == name {
                CloseHandle(snapshot);
                return Ok(entry.th32ProcessID as _);
            }
        }
    }

    CloseHandle(snapshot);

    return Ok(0);
}

unsafe fn get_function_address<'a>(module: &str, function: &str) -> Result<u64, Box<dyn error::Error>> {
    let mod_cstr = CString::new(module)?;
    let fn_cstr = CString::new(function)?;

    let handle = GetModuleHandleA(mod_cstr.as_ptr());

    if handle.is_null() {
        return Err(LoaderError::ModuleHandle.into());
    }

    let func = GetProcAddress(handle, fn_cstr.as_ptr());

    if func.is_null() {
        return Err(LoaderError::FunctionAddress.into());
    }

    return Ok(func as _);
}

unsafe fn inject(process_id: u32, dll_path: &str) -> Result<(), Box<dyn error::Error>> {
    let path_cstr = CString::new(dll_path)?;
    let path_len = path_cstr.as_bytes_with_nul().len();

    let proc = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_WRITE, FALSE, process_id);

    if proc.is_null() {
        return Err(LoaderError::OpenProcessNull.into());
    }

    let allocation = VirtualAllocEx(proc, ptr::null_mut(), path_len, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    println!("Allocated memory for the library path at 0x{:x}", allocation as usize);

    if allocation.is_null() {
        return Err(LoaderError::AllocationFailed.into());
    }

    WriteProcessMemory(proc, allocation, path_cstr.as_ptr() as LPVOID, path_len, ptr::null_mut());

    let load_library: ThreadStartRoutine = mem::transmute(get_function_address("kernel32.dll", "LoadLibraryA")?);
    println!("LoadLibraryA found at 0x{:x}", load_library as usize);

    let thread = CreateRemoteThread(proc, ptr::null_mut(), 0, Some(load_library), allocation, 0, ptr::null_mut());
    CloseHandle(thread);

    return Ok(());
}

fn init() -> Result<(), Box<dyn error::Error>> {
    println!("Waiting for GenshinImpact.exe");

    let mut process_id;

    loop {
        process_id = unsafe { get_process_id_by_name("GenshinImpact.exe")? };

        if process_id != 0 {
            break;
        }
    }

    println!("Injecting into {}", process_id);

    let exe_path = std::env::current_dir()?;
    let dll_path = exe_path.join("library").with_extension(DLL_EXTENSION);

    println!("Library path: {}", dll_path.display());

    unsafe { inject(process_id, dll_path.to_str().unwrap())? };

    println!("Done.");

    return Ok(());
}

fn main() {
    init().unwrap_or_else(|e| println!("Failed to inject the library: {}", e));
}
