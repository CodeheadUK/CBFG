# Codehead's Bitmap Font Generator (CBFG)

CBFG is a tool I developed to help with my OpenGL projects. It generates bitmap font textures with kerning information so that reasonably pretty, variable width text can be displayed in OpenGL applications.

### Features

- DIB rendering of font gives best font output, regardless of users screen settings
- Global or per character position and width adjustment
- Texture Sizes from 16×16 up to 4096×4096
- Zoom up to 400% for accurate tweaking of character positions
- Anti-aliasing or ClearType (Win XP and up) for smoother looking fonts
- Control of font width
- Font preview option
- TGA export option
- BMP output option
- BFF output in 8, 24 and 32bit colour depths
- C++ example source code for loading and rendering BFF files
- Binary font data export
- Font info dump option
- User configurable colours and startup parameters
- Written for Win32 APIs (Windows only for now, although I'm told it runs fine under WINE)

### Open Release
I haven't updated the tool for a good while, but I still get plenty of email about it from people who find it useful, which is nice. However, some of those people want new features which I just don't have time to add. So, I'm releasing the code under the BSD 3-clause licence so that people can hack on the code and add the features they need.

