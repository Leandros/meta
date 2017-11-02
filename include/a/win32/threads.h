/*
 * Copyright (c) Microsoft Corporation. All rights reserved.
 * Copyright (c) Arvid Gerstmann. All rights reserved.
 */
#ifndef WINDOWS_THREADS_H
#define WINDOWS_THREADS_H

/* Disable all warnings */
#if defined(_MSC_VER)
    #pragma warning(push, 0)
#endif

#ifndef WINDOWS_BASE_H
#include "windows_base.h"
#endif
#if defined(__cplusplus)
extern "C" {
#endif

/* WaitForSingleObject, WaitForMultipleObjects, etc: */
#define INFINITE                0xffffffff

#define STATUS_WAIT_0               ((DWORD)0x00000000L)
#define STATUS_ABANDONED_WAIT_0     ((DWORD)0x00000080L)
#define STATUS_TIMEOUT              ((DWORD)0x00000102L)
#define STATUS_PENDING              ((DWORD)0x00000103L)

#define WAIT_FAILED                 ((DWORD)0xffffffff)
#define WAIT_OBJECT_0               ((STATUS_WAIT_0) + 0)
#define WAIT_ABANDONED              ((STATUS_ABANDONED_WAIT_0) + 0)
#define WAIT_TIMEOUT                258L

/* TLS: */
#define TLS_OUT_OF_INDEXES          ((DWORD)0xFFFFFFFF)

/* Thread Access Rights: */
#define THREAD_GET_CONTEXT          (0x0008)
#define THREAD_QUERY_INFORMATION    (0x0040)
#define THREAD_SUSPEND_RESUME       (0x0002)
#define THREAD_TERMINATE            (0x0001)
#define THREAD_ALL_ACCESS           \
    (STANDARD_RIGHTS_REQUIRED | \
     SYNCHRONIZE | \
     0xFFFF)


/* ========================================================================== */
/* Structures: */
/* Magic for Thread Local Storage: */
typedef void (NTAPI *PIMAGE_TLS_CALLBACK)(
        PVOID DllHandle,
        DWORD Reason,
        PVOID Reserved);
typedef struct _IMAGE_TLS_DIRECTORY32 {
    DWORD       StartAddressOfRawData;
    DWORD       EndAddressOfRawData;
    DWORD       AddressOfIndex;
    DWORD       AddressOfCallbacks;
    DWORD       SizeOfZeroFill;
    DWORD       Characteristics;
} IMAGE_TLS_DIRECTORY32, *PIMAGE_TLS_DIRECTORY32;
typedef struct _IMAGE_TLS_DIRECTORY64 {
    ULONGLONG   StartAddressOfRawData;
    ULONGLONG   EndAddressOfRawData;
    ULONGLONG   AddressOfIndex;
    ULONGLONG   AddressOfCallbacks;
    DWORD       SizeOfZeroFill;
    DWORD       Characteristics;
} IMAGE_TLS_DIRECTORY64, *PIMAGE_TLS_DIRECTORY64;
#if defined(_WIN64)
typedef IMAGE_TLS_DIRECTORY64       IMAGE_TLS_DIRECTORY;
typedef PIMAGE_TLS_DIRECTORY64      PIMAGE_TLS_DIRECTORY;
#else
typedef IMAGE_TLS_DIRECTORY32       IMAGE_TLS_DIRECTORY;
typedef PIMAGE_TLS_DIRECTORY32      PIMAGE_TLS_DIRECTORY;
#endif

/* Linked List */
typedef struct _LIST_ENTRY {
   struct _LIST_ENTRY *Flink;
   struct _LIST_ENTRY *Blink;
} LIST_ENTRY, *PLIST_ENTRY;

typedef struct _RTL_CRITICAL_SECTION_DEBUG {
    WORD        Type;
    WORD        CreatorBackTraceIndex;
    struct _RTL_CRITICAL_SECTION *CriticalSection;
    LIST_ENTRY  ProcessLocksList;
    DWORD       EntryCount;
    DWORD       ContentionCount;
    DWORD       Flags;
    WORD        CreatorBackTraceIndexHigh;
    WORD        SpareWORD;
} RTL_CRITICAL_SECTION_DEBUG, *PRTL_CRITICAL_SECTION_DEBUG;

#pragma pack(push, 8)
typedef struct _RTL_CRITICAL_SECTION {
    PRTL_CRITICAL_SECTION_DEBUG DebugInfo;
    LONG LockCount;
    LONG RecursionCount;
    HANDLE OwningThread;
    HANDLE LockSemaphore;
    ULONG_PTR SpinCount;
} RTL_CRITICAL_SECTION, *PRTL_CRITICAL_SECTION;
typedef RTL_CRITICAL_SECTION    CRITICAL_SECTION;
typedef PRTL_CRITICAL_SECTION   PCRITICAL_SECTION;
typedef PRTL_CRITICAL_SECTION   LPCRITICAL_SECTION;
#pragma pack(pop)


/* ========================================================================== */
/* Waiting Functions: */
DWORD WINAPI WaitForSingleObject(
        HANDLE  hHandle,
        DWORD   dwMilliseconds);
DWORD WINAPI WaitForMultipleObjects(
        DWORD   nCount,
  const HANDLE *lpHandles,
        BOOL    bWaitAll,
        DWORD   dwMilliseconds);

/* ========================================================================== */
/* Threading Functions: */
typedef DWORD (WINAPI *PTHREAD_START_ROUTINE)(LPVOID);
typedef PTHREAD_START_ROUTINE LPTHREAD_START_ROUTINE;
HANDLE WINAPI CreateThread(
        LPSECURITY_ATTRIBUTES lpThreadAttributes,
        SIZE_T dwStackSize,
        LPTHREAD_START_ROUTINE lpStartAddress,
        LPVOID lpParameter,
        DWORD dwCreationFlags,
        LPDWORD lpThreadId);
HANDLE WINAPI GetCurrentThread(void);
DWORD WINAPI GetCurrentThreadId(void);
DWORD_PTR WINAPI SetThreadAffinityMask(
        HANDLE hThread,
        DWORD_PTR dwThreadAffinityMask);
HANDLE WINAPI OpenThread(
        DWORD dwDesiredAccess,
        BOOL bInheritHandle,
        DWORD dwThreadId);
DWORD WINAPI SuspendThread(
        HANDLE hThread);
void WINAPI ExitThread(
        DWORD dwExitCode);
BOOL WINAPI GetProcessAffinityMask(
        HANDLE hProcess,
        PDWORD_PTR lpProcessAffinityMask,
        PDWORD_PTR lpSystemAffinityMask);

/* ========================================================================== */
/* CriticalSection Functions: */
void WINAPI InitializeCriticalSection(
        LPCRITICAL_SECTION lpCriticalSection);
BOOL WINAPI InitializeCriticalSectionAndSpinCount(
        LPCRITICAL_SECTION lpCriticalSection,
        DWORD dwSpinCount);
DWORD WINAPI SetCriticalSectionSpinCount(
        LPCRITICAL_SECTION lpCriticalSection,
        DWORD dwSpinCount);
void WINAPI EnterCriticalSection(
        LPCRITICAL_SECTION lpCriticalSection);
BOOL WINAPI TryEnterCriticalSection(
        LPCRITICAL_SECTION lpCriticalSection);
void WINAPI LeaveCriticalSection(
        LPCRITICAL_SECTION lpCriticalSection);
void WINAPI DeleteCriticalSection(
        LPCRITICAL_SECTION lpCriticalSection);

/* ========================================================================== */
/* Mutex Functions: */
HANDLE WINAPI CreateMutexA(
        LPSECURITY_ATTRIBUTES lpMutexAttributes,
        BOOL bInitialOwner,
        LPCSTR lpName);
HANDLE WINAPI CreateMutexW(
        LPSECURITY_ATTRIBUTES lpMutexAttributes,
        BOOL bInitialOwner,
        LPCWSTR lpName);

BOOL WINAPI ReleaseMutex(
        HANDLE hMutex);

/* ========================================================================== */
/* Semaphore Functions: */
HANDLE WINAPI CreateSemaphoreA(
        LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
        LONG lInitialCount,
        LONG lMaximumCount,
        LPCSTR lpName);
HANDLE WINAPI CreateSemaphoreW(
        LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
        LONG lInitialCount,
        LONG lMaximumCount,
        LPCWSTR lpName);

BOOL WINAPI ReleaseSemaphore(
        HANDLE hSemaphore,
        LONG lReleaseCount,
        LPLONG lpPreviousCount);

/* ========================================================================== */
/* Thread-Local Storage: */
DWORD WINAPI TlsAlloc(void);
BOOL WINAPI TlsSetValue(
        DWORD   dwTlsIndex,
        LPVOID  lpTlsValue);
LPVOID WINAPI TlsGetValue(
        DWORD   dwTlsIndex);
DWORD WINAPI TlsFree(
        DWORD   dwTlsIndex);

#if defined(__cplusplus)
}
#endif

/* Enable all warnings */
#if defined(_MSC_VER)
    #pragma warning(pop)
#endif

#endif /* WINDOWS_THREADS_H */

