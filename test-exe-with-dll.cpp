#include <MemoryModule.h>
#include <windows.h>
#include <iostream>
#include <tchar.h>
#include <fstream>
#include <vector>

typedef void(*dofooType)();
typedef void(*dobarType)();

void load_dofoo() {
    HINSTANCE handle = LoadLibraryA("foolib");
    if (handle != NULL) {
        dofooType func = (dofooType)GetProcAddress(handle, "dofoo");
        if (func != NULL) {
            func();
        }
        else {
            std::cerr << "getprocaddress failed" << std::endl;
        }
    }
    else {
        std::cerr << "loadlibrary foolib failed" << std::endl;
    }
}

void load_dobar() {
    HINSTANCE handle = LoadLibraryA("barlib");
    if (handle != NULL) {
        dobarType func = (dobarType)GetProcAddress(handle, "dobar");
        if (func != NULL) {
            func();
        }
        else {
            std::cerr << "getprocaddress failed" << std::endl;
        }
    }
    else {
        std::cerr << "loadlibrary barlib failed" << std::endl;
    }
}

HCUSTOMMODULE CustomLoadLibrary(const char* path, void* _) {
    return LoadLibraryA(path);
}

FARPROC CustomGetProcAddress(HCUSTOMMODULE target, const char* path, void* _) {
    return GetProcAddress((HMODULE)target, path);
}

auto LoadDll(const char* path)
{
    std::ifstream dll(path, std::ios::binary);
    std::vector<char> dllRead(std::istreambuf_iterator<char>(dll), {});

    size_t size = dllRead.size();
    char* allocatedMem = (char*)malloc(size);
    memcpy(allocatedMem, &dllRead[0], size);

    return MemoryLoadLibraryEx(allocatedMem, size, MemoryDefaultAlloc, MemoryDefaultFree,
        CustomLoadLibrary, CustomGetProcAddress, MemoryDefaultFreeLibrary,
        nullptr);
}

void loadmm_dofoo() {
    const char* path = OUTDIR"foolib.dll";
    std::cout << "LoadDLL(" << path << ")" << std::endl;
    auto handle = LoadDll(path);
    if (handle != NULL) {
        dofooType func = (dofooType)MemoryGetProcAddress(handle, "dofoo");
        if (func != NULL) {
            func();
        }
        else {
            std::cerr << "getprocaddress failed" << std::endl;
        }
    }
    else {
        std::cerr << "loadlibrary foolib failed" << std::endl;
    }
}

void loadmm_dobar() {
    const char* path = OUTDIR"barlib.dll";
    std::cout << "LoadDLL(" << path << ")" << std::endl;
    auto handle = LoadDll(path);
    if (handle != NULL) {
        dobarType func = (dobarType)MemoryGetProcAddress(handle, "dobar");
        if (func != NULL) {
            func();
        }
        else {
            std::cerr << "getprocaddress failed" << std::endl;
        }
    }
    else {
        std::cerr << "loadlibrary barlib failed" << std::endl;
    }
}

int main() {
    std::cerr << "start" << std::endl;
    //load_dofoo();
    //load_dobar();
    //loadmm_dofoo();
    loadmm_dobar();
}
