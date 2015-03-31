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
	int downloadCount;
};

// Declare dynamic array
WX_DECLARE_OBJARRAY(FirmwareRelease, FirmwareReleases);
