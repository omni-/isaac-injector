#include "stdafx.h"
#include <stdio.h>
#include <Windows.h>
#include <string>
#include <cstring>
#include <fstream>
#include <Psapi.h>
#include "sigscan.h"

/* There is no ANSI ustrncpy */
unsigned char* ustrncpy(unsigned char *dest, const unsigned char *src, int len) {
	while (len--)
		dest[len] = src[len];

	return dest;
}

unsigned char* base_addr_;
size_t CSigScan::base_len;
std::ofstream outfile;

/* Initialize the Signature Object */
void CSigScan::Init(unsigned char *sig, char *mask, size_t len) 
{
	is_set = 0;
	base_addr_ = (unsigned char*)GetModuleHandle(NULL);

	sig_len = len;
	sig_str = new unsigned char[sig_len];
	ustrncpy(sig_str, sig, sig_len);

	sig_mask = new char[sig_len + 1];
	strncpy(sig_mask, mask, sig_len);
	sig_mask[sig_len + 1] = 0;

	if (!base_addr_)
	{
		outfile << "[err] failed to get base address" << std::endl;
		return;
	}

	if ((sig_addr = FindSignature()) == NULL)
	{
		outfile << "[err] failed to get signature" << std::endl;
		return;
	}

	is_set = 1;
	// SigScan Successful!
}

/* Destructor frees sig-string allocated memory */
CSigScan::~CSigScan(void) {
	delete[] sig_str;
	delete[] sig_mask;
}

/* Scan for the signature in memory then return the starting position's address */
void* CSigScan::FindSignature(void)
{
	MODULEINFO modinfo;

	GetModuleInformation(GetCurrentProcess(), GetModuleHandle(NULL), &modinfo, sizeof(MODULEINFO));

	base_len = modinfo.SizeOfImage;

	MEMORY_BASIC_INFORMATION meminfo;

	while (true)
	{
		if (VirtualQuery((void*)base_len, &meminfo, sizeof(MEMORY_BASIC_INFORMATION)))
			if (!(meminfo.Protect &PAGE_EXECUTE_WRITECOPY))
				break;
	}
	unsigned char *pBasePtr = base_addr_;
	unsigned char *pEndPtr = base_addr_ + base_len;
	size_t i;
	while (pBasePtr < pEndPtr) {
		for (i = 0; i < sig_len; i++) {
			if ((sig_mask[i] != '?') && (sig_str[i] != pBasePtr[i]))
				break;
		}

		// If 'i' reached the end, we know we have a match!
		if (i == sig_len)
			return (void*)pBasePtr;

		pBasePtr++;
	}
	return NULL;
}