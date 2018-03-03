#include "stdafx.h"

/**
 * Single firmware
 */
class FirmwareRelease {
public:
	wxString file;
	wxString name;
	wxString downloadUrl;
	wxString description;
	wxString published;
	wxString date;
	int downloadCount;
};

// Declare dynamic array
WX_DECLARE_OBJARRAY(FirmwareRelease, FirmwareReleases);

/**
 * For CURL download, binary safe
 */
class MemBlock {
public:
	void* ptr;
	unsigned int size;

	MemBlock() {
		ptr = NULL;
		size = 0;
	}

	wxString handoff() {
		char* s = (char*) calloc(size, 1);
		memcpy(s, ptr, size);
		wxString str = wxString::FromUTF8(s, size);
		if (str.Length() == 0) {
			wxLogWarning("Failed to parse as UTF8: %s", s);
		}
		free(ptr);
		free(s);
		size = 0;
		ptr = NULL;
		return str;
	}
};
