#ifndef ICONS_H
#define ICONS_H 1

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#	include <wx/wx.h>
#endif

extern void initialize_images(void);

extern wxBitmap *_img_launcher_pi;
extern wxBitmap *_img_launcher;

#ifdef LAUNCHER_USE_SVG
extern wxString _svg_launcher;
extern wxString _svg_launcher_rollover;
extern wxString _svg_launcher_toggled;
#endif

#endif /* ICONS_H */
