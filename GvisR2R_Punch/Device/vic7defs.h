/* VIC7DEFS.H -- Useful and necessary definitions for 
   Victor Image Processing Library for Windows
*/
#ifndef _VICDEFS_H_
#define _VICDEFS_H_

#if defined WIN32 || defined _WIN32
# define huge
# define far
# define _export
#endif

#if defined(__TCPLUSPLUS__) || defined(__TURBOC__) \
 || defined(__BCPLUSPLUS__) || defined(__BORLANDC__)
#  define BC_COMP_
#else
#  define MSC_COMP_
#endif

/* Limits of an image Victor will process */
#if defined WIN32 || defined _WIN32
# define  XLIMIT    32768U
# define  YLIMIT    32768U
#else
# if defined MSC_COMP_
#  define  XLIMIT     4048U
# else
#  define  XLIMIT     4048
# endif
# define  YLIMIT    32768U
#endif

/* Useful definitions, values */
#define UCHAR unsigned char
#define ULONG unsigned long

/* 32-bit platform defines */
#define WIN32SOS 1 
#define WINNTOS  2
#define WIN95OS  4
#define WIN98OS  8

/* Digitizer Models */
#define HRTDIGI  1
#define IDCDIGI  2
#define SVIADIGI 3
#define QVIADIGI 4
#define WVPRDIGI 5
#define GALDIGI  6

/* Macros */
#define inrange(a,x,b) ((a)<=(x)&&(x)<=(b)) /* Check range of x */
#define ofssetof(typ,mem) ((size_t)&(((typ *)NULL)->mem))
#define outrange(a,x,b) ((x)<(a)||(x)>(b))  /* Check range of x */
#define dim(x) (sizeof(x) / sizeof(x[0]))   /* Returns no. of elements */
#define BUFFER_BYTES(bits) ((((bits)+31) & ~31) >> 3) // Buffer width in bytes
#if defined WIN32 || defined _WIN32
# define BUFFER_SIZE(ptr)  (GlobalSize((HGLOBAL)GlobalHandle((ptr)))) // Buffer size in bytes
// If VirtualAlloc() is used:
# define BUFFER_SIZEV(ptr, bufAdr) \
   { MEMORY_BASIC_INFORMATION mem_Info_; \
   VirtualQuery(ptr, &mem_Info_, sizeof(mem_Info_)); \
   *bufAdr = mem_Info_.RegionSize; }
#else
# define BUFFER_SIZE(ptr)  (GlobalSize((HGLOBAL)GlobalHandle(SELECTOROF(ptr)))) // Buffer size in bytes
#endif
#define CALC_WIDTH(image)  ((image)->endx - (image)->stx + 1) // Width in pixels
#define CALC_HEIGHT(image) ((image)->endy - (image)->sty + 1) // Height in pixels

/* Image descriptor */
typedef struct {
   UCHAR *ibuff;             /* Image buffer address */
   unsigned stx, sty, endx, endy; /* Image area to be processed (pixel coords) */
   unsigned buffwidth;            /* Image buffer width in bytes */
   RGBQUAD  *palette; /* Address of palette associated with the image */
   int colors;           /* Palette colors */
   int imgtype;          /* Image type: bit 0 = 1 if image is grayscale */
   BITMAPINFOHEADER *bmh; /* BITMAPINFOHEADER header and DIB address */
   HBITMAP hBitmap;      /* Device-independent bitmap handle */
   } imgdes;

/* Bit definitions for imgtype */
#define IMGTYPE_GRAYSCALE 1      /* Image is grayscale */
// Returns TRUE if image is a palette color image
#define isPaletteColorImage(image) \
   (image->bmh->biBitCount == 8 && (image->imgtype & IMGTYPE_GRAYSCALE) == 0)
// Returns TRUE if image area is the entire image
#define isWholeImage(image) \
   (image->stx == 0 && image->sty == 0 &&\
   image->endx == (unsigned)image->bmh->biWidth - 1 &&\
   image->endy == (unsigned)image->bmh->biHeight - 1)

/* TIFF file format info structure definition (used by tiffinfo) */
typedef struct {
   unsigned ByteOrder;     /* Byte order -- II or MM */
   unsigned width, length; /* TIF image width, length */
   unsigned BitsPSample;   /* TIF bits per sample */
   unsigned Comp;          /* Compression scheme code */
   unsigned SamplesPPixel; /* Samples per pixel */
   unsigned PhotoInt;      /* Photo interpretation */
   unsigned PlanarCfg;     /* Planar configuration */
   int vbitcount;          /* Victor bits per pixel */
   } TiffData;

/* TIFF file format info structure definition (used by tiffinfo) */
typedef struct {
   unsigned ByteOrder;     /* Byte order -- II or MM */
   unsigned width, length; /* TIF image width, length */
   unsigned BitsPSample;   /* TIF bits per sample */
   unsigned Comp;          /* Compression scheme code */
   unsigned SamplesPPixel; /* Samples per pixel */
   unsigned PhotoInt;      /* Photo interpretation */
   unsigned PlanarCfg;     /* Planar configuration */
   int vbitcount;          /* Victor bits per pixel */
   unsigned xres, yres;    // Horiz, vert pixel density of image
   unsigned resunit;       // X/Y density units
   unsigned fillorder;
   unsigned rowsperstrip;
   unsigned orientation;
   unsigned long IFDofs;   // Bytes from file start to 1st IFD tag
   int page;               // Page label
   } TiffDataEx;

/* PCX file format info structure definition (used by pcxinfo) */
typedef struct {
   int PCXvers;      /* PCX version number */
   unsigned width, length; /* PCX image width, length */
   int BPPixel;      /* PCX bits per pixel */
   int Nplanes;      /* Number of planes */
   int BytesPerLine; /* Bytes per line */
   int PalInt;       /* Palette info interpretation: 1 = color, 2 = gray scale */
   int vbitcount;    /* Victor bits per pixel */
   } PcxData;

// Significant bits
typedef union { 
   char sb_char[4]; 
   DWORD sb_long; 
   } PNGSIGBITS;

// Transparency or background color data
typedef struct {
   BOOL isPresent; // TRUE if transparency data is present
   UCHAR data[6]; // Up to 6 bytes of transparency data
   int byteCount; // Bytes of transparency data
   } PNGTRANSINFO;

// Savepngex() modes
#define PNG_BASE  0
#define PNG_WRT_TRANSCOLOR 1

// Used by savepngex()
typedef struct {
   ULONG transcolor; // Transparent color for GS, RGB and palette color images
   ULONG reserved1;
   ULONG reserved2;
   ULONG reserved3;
   } PNG_DATA;

// PNG file format info structure definition (used by pnginfo)
typedef struct {
   unsigned width;  // PNG image width, length
   unsigned length;
   int bitDepth;    // Bits per pixel, 1, 2, 4, 8, or 16
   int vbitcount;   // Victor bits per pixel
   int colorType;   // Color types: 0=GS, 2=RGB, 3=PalCol, 4=GS+alpha, 6=RGB+alpha
   int interlaced;  // 0 => non-interlaced, 1 => interlaced
   int imageId;     // Unique identifier for colorType/bitDepth
   int channels;    // Number of channels of data per pixel
   int pixelDepth;  // Number of bits per pixel
   unsigned rowBytes;  // Bytes in one image row
   PNGTRANSINFO transData; // Transparency data
   PNGTRANSINFO backData;  // Background color data
   DWORD igamma;    // Gamma as float = igamma / 100000   
   DWORD physXres;  // X,Y axis pixels per unit
   DWORD physYres;
   DWORD physUnits;
   PNGSIGBITS sigBit;  // Significant bits in file
   DWORD offsXoffset; // X,Y axis offset
   DWORD offsYoffset;
   UCHAR offsUnits;
   } PngData;

/* GIF file format info structure definition (used by gifinfo) */
#define TransColor BckCol
typedef struct {
   unsigned width, length; /* GIF image width, length */
   int BitsColRes;         /* Bits of color resolution */
   int BitsPPixel;         /* Bits per pixel */
   int BckCol;             /* Transparent color */
   int Laceflag;           /* Interlace flag */
   int Codesize;           /* Code size */
   int GIFvers;            /* GIF version, 87 or 89 */
   int vbitcount;          /* Victor bits per pixel */
   } GifData;

// GIF global data used by savegifframe(), etc.
typedef struct {
   unsigned scrwidth;
   unsigned scrlength;
   BOOL hasColorMap; // Global color table is present!
   int bckColor;  // Color index of screen backgnd
   unsigned loop;
   } GifGlobalSaveData;

// GIF global data (used by loadgifframe(), etc.
typedef struct {
   GifGlobalSaveData saveData; // Global vars needed to save a GIF frame
   int BitsPPixel; // Bitcount
   int colorRes;
   int pixelAspectRatio;
   long commentOffset;   // Bytes from file start to first comment extension
   int colors;           // Number of colors in global color table
   long colorMapOffset;  // Global color table offset in file
   } GifGlobalData;

// GIF frame data used by savegifframe(), etc.
typedef struct {
   // Image desc
   unsigned startx;
   unsigned starty; // X,Y pixel position with respect to scrwidth, scrlength
   BOOL hasColorMap; // Local color table is present!
   // GCB
   unsigned delay;        // 100ths of a second to display frame
   int transColor;        // Transparent color index, -1 => none
   int removeBy;          // How graphic is to be treated after display
   BOOL waitForUserInput; // If true, expect user input
   } GifFrameSaveData;

// Data used by savegifmultiframetobuffer().
typedef struct {
    UCHAR *buffAddr;
    DWORD frameDataOfs;  // Offset of where the frame data starts
    DWORD commentOfs;  // Offset of where the image data starts
    DWORD terminatorOfs; // Offset of GIF terminator
   } MULTIFRAME_DATA;

// GIF frame data used by loadgifframe(), etc.
typedef struct {
   GifFrameSaveData saveData; // Frame vars needed to save a GIF frame
   // The following vars are not needed for the save fctn
   int vbitcount;        // Victor bits per pixel
   unsigned width;       // GIF image width, length
   unsigned length;      // GIF image width, length
   int frame;            // Frame number the data describes
   BOOL interlace;       // Interlaced image
   int codesize;         // Code size
   int colors;           // Number of colors in local color table
   long colorMapOffset;  // Local color table offset in file
   long rasterDataOffset; // Bytes from file start to start of raster data
   } GifFrameData;

/* Targa file format info structure definition (used by tgainfo) */
typedef struct {
   int IDfieldchars;       /* Chars in ID field */
   unsigned width, length; /* Targa image width, length */
   int ColorMapType;       /* Color map type: 0=no, 1=yes */
   int ImageType;          /* Image type */
   int ColorMapEntryBits;  /* No. bits in color map entry */
   int Xorigin, Yorigin;   /* X,Y origin of image */
   int BPerPix;            /* Bits per pixel */
   int ABPerPix;           /* Attribute bits per pixel */
   int ScreenOrigin;       /* 0=lower left corner, 2=upper left */
   int Interleave;         /* Data storage interleave: 0=none */
   int vbitcount;          /* Victor bits per pixel */
   } TgaData;

/* JPEG file format info structure definition (used by jpeginfo) */
typedef struct {
   int ftype;              /* JPEG file type: -1 or 0 - 15 */
   unsigned width, length; /* JPEG image width, length */
   unsigned comps;         /* Number of components */
   unsigned precision;     /* Sample precision, 8 or 16 */
   unsigned sampFac[4];    /* Horiz * 16 + Vert */
   int vbitcount;          /* Victor bits per pixel */
   } JpegData;

#define THUMB_CODE_JPEG 0x10  // JPEG-coded Thumbnail
#define THUMB_CODE_PAL  0x11  // Palette-coded Thumbnail
#define THUMB_CODE_RGB  0x13  // RGB-coded Thumbnail
#define THUMB_CODE_TIFUNC  0x14  // Uncompressed TIF Thumbnail (EXIF)
#define THUMB_CODE_TIFJPG  0x15  // TIF Comp=6 (JPG) Thumbnail (EXIF)

// Color conversion mode, used by loadjpgex()
typedef enum { YCC_TO_RGB=0, RGB_TO_RGB, DONT_CARE, YCCK_TO_RGB, CMYK_TO_RGB, RGBA_TO_RGB }  COLCON_MODE;

// Rotation definitions, used by rotateex()
typedef enum { ROTATE_INTERP=0, ROTATE_NO_INTERP }  ROT_MODE;

#pragma pack(1)
typedef struct {
   BOOL hasThumbNail;
   UCHAR width, length; // Thumbnail dimensions
   UCHAR bitcount;      // Bits per pixel of thumbnail--8 or 24
   UCHAR coding;        // THUMB_CODE_JPEG, THUMB_CODE_PAL, or THUMB_CODE_RGB
   } JPEG_THUMB_DATA;
#pragma pack()

/* JPEG file format info structure definition (used by jpeginfo) */
typedef struct {
   int ftype;              /* JPEG file type: -1 or 0 - 15 */
   unsigned width, length; /* JPEG image width, length */
   unsigned comps;         /* Number of components */
   unsigned precision;     /* Sample precision, 8 or 16 */
   unsigned sampFac[4];    /* Horiz * 16 + Vert */
   int vbitcount;          /* Victor bits per pixel */
   unsigned xres, yres; // Horiz, vert pixel density of image, NOT thumbnail
   unsigned resunit;         // X/Y density units
   JPEG_THUMB_DATA thumbNail; // Thumbnail data
   } JpegDataEx;

/* Victor version struct */
typedef struct {WORD version, flags; } VIC_VERSION_INFO;

/* Vicversionex flags */
#define VIC_VS_STATIC_RTL   1  // 1 => Uses static C run-time lib, 0 => uses MSVCRTxx.DLL
#define VIC_VS_THREAD_SAFE  2  // 2 => Multithread safe, 0=> not multithread safe
#define VIC_VS_RELEASE      4  // 4 => Release version, 0 => prerelease (beta) version
#define VIC_VS_EVAL_LIB     8  // 8 => Evaluation version, 0 => not evaluation version

/* OS version info struct */
typedef struct { UINT version, platformID; } MYVERSIONINFO;
#define VER_PLATFORM_WIN32s        0
#define VER_PLATFORM_WIN32_WINDOWS 1
#define VER_PLATFORM_WIN32_NT      2

/* HSV structure */
typedef struct tagHSVTRIPLE
{
   UCHAR hue;
   UCHAR saturation;
   UCHAR value;
   } HSVTRIPLE;


/* PALETTEPOINT structure */
typedef struct tagPALETTEPOINT
{
	UCHAR index;
   UCHAR red;
   UCHAR green;
   UCHAR blue;
   } PALETTEPOINT;


// Used by calcminmax()
typedef struct tagMINMAX
{
   int min;
   int max;
   int res1, res2;  // Possible future expansion
   } MINMAX;

// For sort routine
typedef struct { unsigned val, x, y; } COORD_VAL;

// Twain structs and definitions
typedef char TW_STR32[34];
// Data for Twain ONEVALUE-type container
typedef struct { short val; } TWAIN_ONEVALUE;
// Data for Twain ENUM-type container 
typedef struct
{
   short array[18];
   WORD nelems;  // Number of elements in array[]
   WORD currentIndex;  // The value that is currently in effect
   WORD defaultIndex;  // Power-up value
   } TWAIN_ENUMTYPE;
// Data for Twain RANGE-type container 
typedef struct
{
   short min;      // Starting value in the range
   short max;      // Final value in the range
   short stepSize; // Increment from min to max
   short currentVal;  // The value that is currently in effect
   short defaultVal;  // Power-up value
   } TWAIN_RANGE;

// Capability get/set struct
typedef struct {
   short conType;  // Container type, TWON_ONEVALUE, TWON_ENUMERATION, or TWON_RANGE,
   TWAIN_ONEVALUE oneValue; // Data for ONEVALUE-type container 
   TWAIN_ENUMTYPE enumType;
   TWAIN_RANGE range;
   } TWAIN_CAP_DATA;

// Container constants (4)
//typedef enum {TWON_ARRAY=3, TWON_ENUMERATION, TWON_ONEVALUE, TWON_RANGE}  TW_CONTAINER_VAR;
#define TWON_ARRAY 3
#define TWON_ENUMERATION 4
#define TWON_ONEVALUE 5
#define TWON_RANGE 6

// Pixel type constants (9)
//typedef enum {
//   TWPT_BW=0, TWPT_GRAY, TWPT_RGB, TWPT_PALETTE, 
//   TWPT_CMY, TWPT_CMYK, TWPT_YUV, TWPT_YUVK, TWPT_CIEXYZ, 
//   }  TW_PIXELTYPE_VAR;

#define TWPT_BW 0
#define TWPT_GRAY 1
#define TWPT_RGB 2
#define TWPT_PALETTE 3
#define TWPT_CMY 4
#define TWPT_CMYK 5
#define TWPT_YUV 6
#define TWPT_YUVK 7
#define TWPT_CIEXYZ 8 



// Units constants (6)
//typedef enum {
//   TWUN_INCHES=0, TWUN_CENTIMETERS, TWUN_PICAS, 
//   TWUN_POINTS, TWUN_TWIPS, TWUN_PIXELS,
//   }  TW_UNITS_VAR;

#define TWUN_INCHES 0
#define TWUN_CENTIMETERS 1
#define TWUN_PICAS 2
#define TWUN_POINTS 3
#define TWUN_TWIPS 4
#define TWUN_PIXELS 5



#define TW_DONTCARE 0xffff
#define TWSS_NONE        0

#ifdef _WIN32_WCE
// For allocations > 64Kb
# define BIG_ALLOC_AREA(dwSize)  (LocalAlloc(LPTR, (dwSize)))
# define BIG_FREE_AREA(ptr)      (LocalFree((ptr)))
#else // Not WinCE
# define BIG_ALLOC_AREA(dwSize)  (GlobalAllocPtr(GHND, (dwSize)))
# define BIG_FREE_AREA(ptr)      (GlobalFreePtr((ptr)))
#endif

#ifdef __cplusplus
   extern "C" {
#endif   /* __cplusplus */
/* Function declarations for Victor Image Processing Library */

INT64 _export WINAPI getbufferaddress64(UCHAR *bufferaddress);
INT64 _export WINAPI getbuffersize64(UCHAR *bufferaddress);


int  _export WINAPI addimage(imgdes  *srcimg, imgdes  *oprimg, imgdes  *desimg);
int  _export WINAPI allocimage(imgdes  *image, int width, int length, int bppixel);
int  _export WINAPI andimage(imgdes  *srcimg, imgdes  *oprimg, imgdes  *desimg);
int  _export WINAPI blur(imgdes  *srcimg, imgdes  *desimg);
int  _export WINAPI blurthresh(const int thres, imgdes  *srcimg, imgdes  *desimg);
int  _export WINAPI bmpinfo(LPCSTR fname, BITMAPINFOHEADER  *bdat);
int  _export WINAPI bmpinfoW(LPCWSTR fname, BITMAPINFOHEADER  *bdat);
int  _export WINAPI bmpinfofrombuffer(UCHAR *buff, BITMAPINFOHEADER  *bdat);
int  _export WINAPI brightenmidrange(imgdes  *srcimg, imgdes  *desimg);
int  _export WINAPI calcavglevel(imgdes *srcimg, int *redavg, int *grnavg, int *bluavg);
int  _export WINAPI calchisto(imgdes  *srcimg, long  *redtab, long  *grntab, long  *blutab);
int  _export WINAPI calchistorgb(imgdes  *srcimg, long  *redtab, long  *grntab, long  *blutab, int mode);
int  _export WINAPI calcminmax(imgdes *srcimg, MINMAX *redMinmax, MINMAX *grnMinmax, MINMAX *bluMinmax);
int  _export WINAPI changebright(const int amt, imgdes  *srcimg, imgdes  *desimg);
int  _export WINAPI clienttoimage(HWND hWnd, imgdes  *image);
int  _export WINAPI colordither(imgdes  *srcimg, imgdes  *desimg, int colorMode);
int  _export WINAPI colorscatter(imgdes  *srcimg, imgdes  *desimg, int colorMode);
int  _export WINAPI colortogray(imgdes  *srcimg, imgdes  *desimg);


int _export WINAPI addnoise(int amount, imgdes  *srcimg, imgdes  *desimg);
int _export WINAPI addtext(unsigned int color, int pointsize, LPCSTR font, LPCSTR atext, imgdes  *desimg);
int _export WINAPI addtextex(unsigned int color, LOGFONT *lfont, LPCSTR atext, imgdes  *desimg);
int _export WINAPI addtextexw(unsigned int color, LOGFONT *lfont, LPCTSTR text, imgdes  *desimg);

int _export WINAPI convert8bitto1bitex(int mode, imgdes  *srcimg, imgdes  *desimg, UCHAR *screen);
int _export WINAPI convertgray16to8ex(unsigned long min, unsigned long max, imgdes *srcimg, imgdes *desimg);
int _export WINAPI convertgray32to8(unsigned long min, unsigned long max, imgdes *srcimg, imgdes *desimg);
int _export WINAPI convertgray32to16(unsigned long min, unsigned long max, imgdes *srcimg, imgdes *desimg);
int _export WINAPI convertgray32to816(unsigned long min, unsigned long max, imgdes *srcimg, imgdes *desimg);
int _export WINAPI displace(int tiles, imgdes *srcimg,imgdes *oprimg, imgdes *desimg);      
int _export WINAPI dropbits(const int value, imgdes  *srcimg, imgdes  *desimg);
int _export WINAPI emboss(imgdes  *srcimg, imgdes  *desimg);
int _export WINAPI embossongray(imgdes  *srcimg, imgdes  *desimg);
int _export WINAPI extractcolorrange(RGBTRIPLE *rgbcolor, int extent, imgdes  *srcimg, imgdes  *desimg);
int _export WINAPI freebuffer(UCHAR *bufferaddress);
int _export WINAPI gaussianblur(int ksize, imgdes  *srcimg, imgdes  *desimg);
unsigned int  _export WINAPI getbuffersize(UCHAR *bufferaddress);
int _export WINAPI makepalette(imgdes  *image, PALETTEPOINT * ppa, int numelem);
int _export WINAPI outlineongray(imgdes  *srcimg, imgdes  *desimg);
int _export WINAPI rgbimagetocmykimage(imgdes  *srcimg, imgdes  *desimg);
int _export WINAPI cmykimagetorgbimage(imgdes  *srcimg, imgdes  *desimg);

int _export WINAPI screen(imgdes  *srcimg, imgdes *oprimg, imgdes  *desimg);
int _export WINAPI tile(imgdes  *srcimg, imgdes  *desimg);
int _export WINAPI wtaveragemask(imgdes *srcimg, imgdes *oprimg, imgdes *desimg,imgdes *masimg);


int _export WINAPI tiffgetdnstring(LPCSTR afname, BYTE *idStr, unsigned idStrLen, BYTE *tagStr, unsigned tagStrLen); 
int _export WINAPI tiffsetdnstring(BYTE *idStr, unsigned idStrLen, BYTE *tagStr, unsigned tagStrLen);



int  _export WINAPI convert1bitto8bit(imgdes  *srcimg, imgdes  *desimg);
int  _export WINAPI convert1bitto8bitsmooth(imgdes  *srcimg, imgdes  *desimg);
int  _export WINAPI convert8bitto1bit(int mode, imgdes  *srcimg, imgdes  *desimg);
int  _export WINAPI convertgray16to8(imgdes  *srcimg, imgdes  *desimg);
int  _export WINAPI convertgray8to16(imgdes  *srcimg, imgdes  *desimg);
int  _export WINAPI convertpaltorgb(imgdes  *srcimg, imgdes  *desimg);
int  _export WINAPI convertrgbtopal(int palcolors, imgdes  *srcimg, imgdes  *desimg);
int  _export WINAPI convertrgbtopalex(int palcolors, imgdes  *srcimg, imgdes  *desimg, int mode);
int  _export WINAPI copyimage(imgdes  *srcimg, imgdes  *desimg);
int  _export WINAPI copyimagebits(imgdes  *srcimg, imgdes  *desimg);
void _export WINAPI copyimagepalette(imgdes  *srcimg, imgdes  *desimg);
void _export WINAPI copyimgdes(imgdes  *srcimg, imgdes  *desimg);
int  _export WINAPI cover(const int thres, imgdes  *srcimg, imgdes  *oprimg, imgdes  *desimg);
int _export WINAPI coverclear(const long transColor, imgdes  *srcimg, imgdes  *oprimg, imgdes  *desimg);
int  _export WINAPI ddbtoimage(HBITMAP hBitmap, HPALETTE hPal, imgdes  *image);
int  _export WINAPI defaultpalette(imgdes  *srcimg);
int  _export WINAPI dibsecttoimage(HBITMAP hBitmap, imgdes  *image);
int  _export WINAPI dibtobitmap(HDC hdc, UCHAR  *dib, HBITMAP  *hBitmap);
int  _export WINAPI dibtoimage(UCHAR  *dib, imgdes  *desimg);
int  _export WINAPI dilate(const int amt, imgdes  *srcimg, imgdes  *desimg);
int  _export WINAPI divide(const int divsr, imgdes  *srcimg, imgdes  *desimg);
int  _export WINAPI drawhisto(HDC hdc, RECT  *rect, int bppixel, long  *redtab, long  *grntab, long  *blutab);
int  _export WINAPI erode(const int amt, imgdes  *srcimg, imgdes  *desimg);
int  _export WINAPI exchangelevel(int min, int max, const int newval, imgdes  *srcimg, imgdes  *desimg);
int  _export WINAPI expandcontrast(int min, int max, imgdes  *srcimg, imgdes  *desimg);
int  _export WINAPI extractplane(int plane, imgdes  *srcimg, imgdes  *desimg);
int  _export WINAPI flipimage(imgdes  *srcimg, imgdes  *desimg);
void _export WINAPI freeimage(imgdes  *image);
int  _export WINAPI gammabrighten(const double amt, imgdes  *srcimg, imgdes  *desimg);
int  _export WINAPI getgifcomment(LPCSTR fname, char  *buff, int maxbuf);
int  _export WINAPI getgifcommentW(LPCWSTR fname, char  *buff, int maxbuf);
int  _export WINAPI getpngcomment(LPCSTR fname, char  *keyword, char  *buff, int maxbuf);
int  _export WINAPI getpngcommentW(LPCWSTR fname, char  *keyword, char  *buff, int maxbuf);
long _export WINAPI getpixelcolor(imgdes  *image, int xcoord, int ycoord);
int  _export WINAPI gifinfo(LPCSTR fname, GifData  *ginfo);
int  _export WINAPI gifinfoW(LPCWSTR fname, GifData  *ginfo);
int  _export WINAPI gifinfofrombuffer(UCHAR  *buff, GifData  *ginfo);
int  _export WINAPI gifframecount(LPCSTR fname, int  *totalFrames);
int  _export WINAPI gifframecountW(LPCWSTR fname, int  *totalFrames);
int  _export WINAPI gifframecountfrombuffer(UCHAR  *buff, int  *totalFrames);
int  _export WINAPI gifinfoframe(LPCSTR fname, GifData  *gifdat, GifGlobalData  *gdata, GifFrameData  *fdata, int frameTarget);
int  _export WINAPI gifinfoframeW(LPCWSTR fname, GifData  *gifdat, GifGlobalData  *gdata, GifFrameData  *fdata, int frameTarget);
int  _export WINAPI gifinfoframefrombuffer(UCHAR  *buff, GifData  *ginfo, GifGlobalData  *gdata, GifFrameData  *fdata, int frameTarget);
int  _export WINAPI gifinfoallframes(LPCSTR fname, GifGlobalData  *gdata, GifFrameData  *fdata, int frameElem);
int  _export WINAPI gifinfoallframesW(LPCWSTR fname, GifGlobalData  *gdata, GifFrameData  *fdata, int frameElem);
int  _export WINAPI gifinfoallframesfrombuffer(UCHAR  *buff, GifGlobalData  *gdata, GifFrameData  *fdata, int frameElem);
int  _export WINAPI histobrighten(imgdes  *srcimg, imgdes  *desimg);
int  _export WINAPI histoequalize(imgdes  *srcimg, imgdes  *desimg);
void _export WINAPI hsv2rgb(HSVTRIPLE  *hsvpal, RGBQUAD  *rgbpal, int colors);
int  _export WINAPI hsvimagetorgbimage(imgdes  *srcimg, imgdes  *desimg);
void _export WINAPI imageareatorect(imgdes  *image, RECT  *rect);
int  _export WINAPI imagetodib(imgdes  *srcimg, UCHAR  *  *dib);
int  _export WINAPI insertplane(int plane, imgdes  *srcimg, imgdes  *desimg);
BOOL _export WINAPI isgrayscaleimage(imgdes  *image);
int  _export WINAPI jpeggeterror(void);
int  _export WINAPI jpeginfo(LPCSTR fname, JpegData  *jdat);
int  _export WINAPI jpeginfoW(LPCWSTR fname, JpegData  *jdat);
int  _export WINAPI jpeginfoex(LPCSTR fname, JpegDataEx  *jdat);
int  _export WINAPI jpeginfoexW(LPCWSTR fname, JpegDataEx  *jdat);
int  _export WINAPI jpeginfofrombuffer(UCHAR  *buff, JpegData  *jinfo);
int  _export WINAPI jpeginfofrombufferex(UCHAR  *buff, JpegDataEx  *jinfo);
void _export WINAPI jpegsetxyresolution(unsigned xres, unsigned yres, unsigned resUnit);
int  _export WINAPI jpegsetthumbnailsize(int longEdge);
int  _export WINAPI kodalith(const int thres, imgdes  *srcimg, imgdes  *desimg);
int  _export WINAPI limitlevel(const int thres, imgdes  *srcimg, imgdes  *desimg);
int  _export WINAPI loadbif(LPCSTR fname, imgdes  *desimg);
int  _export WINAPI loadbifW(LPCWSTR fname, imgdes  *desimg);
int  _export WINAPI loadbmp(LPCSTR fname, imgdes  *image);
int  _export WINAPI loadbmpW(LPCWSTR fname, imgdes  *image);
int  _export WINAPI loadbmpfrombuffer(UCHAR  *buff, imgdes  *desimg);
int  _export WINAPI loadbmppalette(LPCSTR fname, RGBQUAD  *paltab);
int  _export WINAPI loadbmppaletteW(LPCWSTR fname, RGBQUAD  *paltab);
int  _export WINAPI loadbmppalettefrombuffer(UCHAR  *buff, RGBQUAD  *paltab);
int  _export WINAPI loadgifframe(LPCSTR fname, imgdes  *desimg, GifGlobalData  *gdata, GifFrameData  *fdata);
int  _export WINAPI loadgifframeW(LPCWSTR fname, imgdes  *desimg, GifGlobalData  *gdata, GifFrameData  *fdata);
int  _export WINAPI loadgifframefrombuffer(UCHAR  *buff, imgdes  *desimg, GifGlobalData  *gdata, GifFrameData  *fdata);
int  _export WINAPI loadgif(LPCSTR fname, imgdes  *desimg);
int  _export WINAPI loadgifW(LPCWSTR fname, imgdes  *desimg);
int  _export WINAPI loadgiffrombuffer(UCHAR  *buff, imgdes  *desimg);
int  _export WINAPI loadgifpalette(LPCSTR fname, RGBQUAD  *paltab);
int  _export WINAPI loadgifpaletteW(LPCWSTR fname, RGBQUAD  *paltab);
int  _export WINAPI loadgifpalettefrombuffer(UCHAR  *buff, RGBQUAD  *paltab);
int  _export WINAPI loadgifglobalpalette(LPCSTR fname, RGBQUAD  *paltab);
int  _export WINAPI loadgifglobalpaletteW(LPCWSTR fname, RGBQUAD  *paltab);
int  _export WINAPI loadgifglobalpalettefrombuffer(UCHAR  *buff, RGBQUAD  *paltab);
int  _export WINAPI loadgifframepalette(LPCSTR fname, RGBQUAD  *paltab, int frame);
int  _export WINAPI loadgifframepaletteW(LPCWSTR fname, RGBQUAD  *paltab, int frame);
int  _export WINAPI loadgifframepalettefrombuffer(UCHAR  *buff, RGBQUAD  *paltab, int frame);
int  _export WINAPI loadjpg(LPCSTR fname, imgdes  *desimg);
int  _export WINAPI loadjpgex(LPCSTR fname, imgdes  *desimg, int mode);
int  _export WINAPI loadjpgW(LPCWSTR fname, imgdes  *desimg);
int  _export WINAPI loadjpgexW(LPCWSTR fname, imgdes  *desimg, int mode);
int  _export WINAPI loadjpgfrombuffer(UCHAR  *buff, imgdes  *desimg);
int  _export WINAPI loadjpgfrombufferex(UCHAR  *buff, imgdes  *desimg, int mode);
int  _export WINAPI loadjpgthumbnail(LPCSTR fname, imgdes  *desimg, BOOL createThumbNail);
int  _export WINAPI loadjpgthumbnailW(LPCWSTR fname, imgdes  *desimg, BOOL createThumbNail);
int  _export WINAPI loadjpgthumbnailfrombuffer(UCHAR  *buff, imgdes  *desimg, BOOL createThumbNail);
int  _export WINAPI loadpcx(LPCSTR fname, imgdes  *desimg);
int  _export WINAPI loadpcxW(LPCWSTR fname, imgdes  *desimg);
int  _export WINAPI loadpcxpalette(LPCSTR fname, RGBQUAD  *paltab);
int  _export WINAPI loadpcxpaletteW(LPCWSTR fname, RGBQUAD  *paltab);
int  _export WINAPI loadpng(LPCSTR fname, imgdes  *desimg);
int  _export WINAPI loadpngW(LPCWSTR fname, imgdes  *desimg);
int  _export WINAPI loadpngfrombuffer(UCHAR  *buff, imgdes  *desimg);
int  _export WINAPI loadpngpalette(LPCSTR fname, RGBQUAD  *paltab);
int  _export WINAPI loadpngpaletteW(LPCWSTR fname, RGBQUAD  *paltab);
int  _export WINAPI loadpngpalettefrombuffer(UCHAR  *buff, RGBQUAD  *qpal);

int  _export WINAPI loadpngwithalpha(LPCSTR afname, imgdes  *desimg, imgdes *alphaImg);
int  _export WINAPI loadpngwithalphaW(LPCWSTR wcfname, imgdes  *desimg, imgdes *alphaImg);

int  _export WINAPI loadtga(LPCSTR fname, imgdes  *desimg);
int  _export WINAPI loadtgaW(LPCWSTR fname, imgdes  *desimg);
int  _export WINAPI loadtgapalette(LPCSTR fname, RGBQUAD  *paltab);
int  _export WINAPI loadtgapaletteW(LPCWSTR fname, RGBQUAD  *paltab);

int  _export WINAPI loadtgawithalpha(LPCSTR afname, imgdes  *desimg, imgdes *alphaImg);
int  _export WINAPI loadtgawithalphaW(LPCWSTR wcfname, imgdes  *desimg, imgdes *alphaImg);

int  _export WINAPI loadtif(LPCSTR fname, imgdes  *desimg);
int  _export WINAPI loadtifW(LPCWSTR fname, imgdes  *desimg);
int  _export WINAPI loadtiffrombuffer(UCHAR  *buff, imgdes  *desimg);
int  _export WINAPI loadtifpage(LPCSTR fname, imgdes  *desimg, int page);
int  _export WINAPI loadtifpageW(LPCWSTR fname, imgdes  *desimg, int page);
int  _export WINAPI loadtifpagebyindex(LPCSTR fname, imgdes  *desimg, int pageIndex);
int  _export WINAPI loadtifpagebyindexW(LPCWSTR fname, imgdes  *desimg, int pageIndex);
int  _export WINAPI loadtifpagebyindexfrombuffer(UCHAR  *buff, imgdes  *desimg, int pageIndex);
int  _export WINAPI loadtifpagebyindexfrombufferwithalpha(UCHAR *buff, imgdes *desimg, int pageIndex, imgdes *alphaImg);
int  _export WINAPI loadtifpalette(LPCSTR fname, RGBQUAD  *paltab);
int  _export WINAPI loadtifpaletteW(LPCWSTR fname, RGBQUAD  *paltab);
int  _export WINAPI loadtifpalettepage(LPCSTR fname, RGBQUAD  *paltab, int page);
int  _export WINAPI loadtifpalettepageW(LPCWSTR fname, RGBQUAD  *paltab, int page);
int  _export WINAPI loadtifpalettepagebyindex(LPCSTR fname, RGBQUAD  *paltab, int pageIndex);
int  _export WINAPI loadtifpalettepagebyindexW(LPCWSTR fname, RGBQUAD  *paltab, int pageIndex);
int  _export WINAPI loadtifpalettefrombuffer(UCHAR  *buff, RGBQUAD  *paltab);
int  _export WINAPI loadtifwithalpha(LPCSTR afname, imgdes  *desimg, imgdes  *alphaImg);
int  _export WINAPI loadtifwithalphaW(LPCWSTR wcfname, imgdes  *desimg, imgdes  *alphaImg);
int  _export WINAPI matchcolorimage(imgdes  *srcimg, imgdes  *desimg);
int  _export WINAPI matchcolorimageex(imgdes  *srcimg, imgdes  *desimg, int mode);
int  _export WINAPI matrixconv(LPCSTR kernel, imgdes  *srcimg, imgdes  *desimg);
int  _export WINAPI matrixconvex(const int ksize, LPCSTR kernal, const int divsr, imgdes  *srcimg, imgdes  *desimg);
int  _export WINAPI medianfilter(const int ksize, imgdes  *srcimg, imgdes  *desimg);
int  _export WINAPI mirrorimage(imgdes  *srcimg, imgdes  *desimg);
int  _export WINAPI multiply(const int multr, imgdes  *srcimg, imgdes  *desimg);
int  _export WINAPI multiplyex(const double multr, imgdes  *srcimg, imgdes  *desimg);
int  _export WINAPI multiplyimage(imgdes  *srcimg, imgdes  *oprimg, imgdes  *desimg);
int  _export WINAPI negative(imgdes  *srcimg, imgdes  *desimg);
int  _export WINAPI orimage(imgdes  *srcimg, imgdes  *oprimg, imgdes  *desimg);
int  _export WINAPI outline(imgdes  *srcimg, imgdes  *desimg);
int  _export WINAPI pcxinfo(LPCSTR fname, PcxData  *pdata);
int  _export WINAPI pcxinfoW(LPCWSTR fname, PcxData  *pdata);
int  _export WINAPI pnggeterror(void);
int  _export WINAPI pnggetxyresolution(LPCSTR fname, unsigned  *xres, unsigned  *yres, unsigned  *resUnit);
int  _export WINAPI pnggetxyresolutionW(LPCWSTR fname, unsigned  *xres, unsigned  *yres, unsigned  *resUnit);
void _export WINAPI pngsetxyresolution(unsigned xres, unsigned yres, unsigned resUnit);
int  _export WINAPI pnginfo(LPCSTR fname, PngData  *pdat);
int  _export WINAPI pnginfoW(LPCWSTR fname, PngData  *pdat);
int  _export WINAPI pnginfofrombuffer(UCHAR  *buff, PngData  *pdat);
int  _export WINAPI pixelcount(int min, int max, long  *redct, long  *grnct, long  *bluct, imgdes  *srcimg);
int  _export WINAPI pixellize(const int factr, imgdes  *srcimg, imgdes  *desimg);
int  _export WINAPI printimage(HWND hwnd, HDC hdcprn, int mode, imgdes  *image, RECT  *rect, int boxsiz, int (WINAPI *dspfct)());
void _export WINAPI printimageenddoc(HDC hdcprn, BOOL ejectPage);
int  _export WINAPI printimagenoeject(HWND hwnd, HDC hdcprn, int mode, imgdes  *image, RECT  *rect, int boxsiz, int (WINAPI *dspfct)());
int  _export WINAPI printimagestartdoc(HDC hdcPrn, LPCSTR aDocname);
int  _export WINAPI printimagestartdocW(HDC hdcPrn, LPCWSTR wcDocname);
int  _export WINAPI rainbowpalette(imgdes  *srcimg);
void _export WINAPI recttoimagearea(RECT  *rect, imgdes  *image);
int  _export WINAPI reduceimagecolors(imgdes  *srcimg, imgdes  *desimg);
int  _export WINAPI removenoise(imgdes  *srcimg, imgdes  *desimg);
int  _export WINAPI resize(imgdes  *srcimg, imgdes  *desimg);
int  _export WINAPI resizeex(imgdes  *srcimg, imgdes  *desimg, int resizeMode);
void _export WINAPI rgb2hsv(RGBQUAD  *rgbpal, HSVTRIPLE  *hsvpal, int colors);
int  _export WINAPI rgbimagetohsvimage(imgdes  *srcimg, imgdes  *desimg);
int  _export WINAPI rotate(const double angle, imgdes  *srcimg, imgdes  *desimg);
int  _export WINAPI rotateex(const double angle, imgdes  *srcimg, imgdes  *desimg, int mode);
int  _export WINAPI rotate90(const int dir, imgdes  *srcimg, imgdes  *desimg);
int  _export WINAPI savebif(LPCSTR fname, imgdes  *srcimg);
int  _export WINAPI savebifW(LPCWSTR fname, imgdes  *srcimg);
int  _export WINAPI savebmp(LPCSTR fname, imgdes  *srcimg, int cmp);
int  _export WINAPI savebmpW(LPCWSTR fname, imgdes  *srcimg, int cmp);
int  _export WINAPI savebmptobuffer(UCHAR  *  *buffAddr, imgdes  *srcimg, int cmp);
int  _export WINAPI saveeps(LPCSTR fname, imgdes  *srcimg);
int  _export WINAPI saveepsW(LPCWSTR fname, imgdes  *srcimg);
int  _export WINAPI savegif(LPCSTR fname, imgdes  *srcimg);
int  _export WINAPI savegifW(LPCWSTR fname, imgdes  *srcimg);
int  _export WINAPI savegifex(LPCSTR fname, imgdes  *srcimg, int mode, int transColor);
int  _export WINAPI savegifexW(LPCWSTR fname, imgdes  *srcimg, int mode, int transColor);
int  _export WINAPI savegiftobufferex(UCHAR  *  *buffAddr, imgdes  *srcimg, int mode, int transColor);
int  _export WINAPI savegifframe(LPCSTR fname, imgdes  *srcimg, GifGlobalSaveData  *gdata, GifFrameSaveData  *fdata, int mode);
int  _export WINAPI savegifframeW(LPCWSTR fname, imgdes  *srcimg, GifGlobalSaveData  *gdata, GifFrameSaveData  *fdata, int mode);
int  _export WINAPI savegifframetobuffer(UCHAR  *  *buffAddr, imgdes  *srcimg, GifGlobalSaveData  *gdata, GifFrameSaveData  *fdata, int mode);
int  _export WINAPI savegifmultiframetobuffer(UCHAR  *  *buffAddr, imgdes *imageArr, GifGlobalSaveData *gdata, GifFrameSaveData *fdataArr, int mode, int nelem);
int  _export WINAPI savejpg(LPCSTR fname, imgdes  *srcimg, int qual);
int  _export WINAPI savejpgW(LPCWSTR fname, imgdes  *srcimg, int qual);
int  _export WINAPI savejpgex(LPCSTR fname, imgdes  *srcimg, int qual, int savemode);
int  _export WINAPI savejpgexW(LPCWSTR fname, imgdes  *srcimg, int qual, int savemode);
int  _export WINAPI savejpgtobuffer(UCHAR  *  *buffAddr, imgdes  *srcimg, int qual);
int  _export WINAPI savejpgtobufferex(UCHAR  *  *buffAddr, imgdes  *srcimg, int qual, int savemode);
int  _export WINAPI savepcx(LPCSTR fname, imgdes  *srcimg);
int  _export WINAPI savepcxW(LPCWSTR fname, imgdes  *srcimg);
int  _export WINAPI savepng(LPCSTR fname, imgdes *srcimg, int cmp);
int  _export WINAPI savepngexW(LPCWSTR wcfname, imgdes *srcimg, int compsn, int mode, PNG_DATA *pData);
int  _export WINAPI savepngex(LPCSTR afname, imgdes *srcimg, int compsn, int mode, PNG_DATA *pData);
int  _export WINAPI savepngW(LPCWSTR fname, imgdes *srcimg, int cmp);
int  _export WINAPI savepngtobuffer(UCHAR  *  *buffAddr, imgdes *srcimg, int compsn);
int  _export WINAPI savepngtobufferex(UCHAR  *  *buffAddr, imgdes *srcimg, int compsn, int mode, PNG_DATA *pData);
int  _export WINAPI savetga(LPCSTR fname, imgdes  *srcimg, int cmp);
int  _export WINAPI savetgaW(LPCWSTR fname, imgdes  *srcimg, int cmp);
int  _export WINAPI savetif(LPCSTR fname, imgdes  *srcimg, int cmp);
int  _export WINAPI savetifW(LPCWSTR fname, imgdes  *srcimg, int cmp);
int  _export WINAPI savetifpage(LPCSTR fname, imgdes  *srcimg, int cmprsn, int page);
int  _export WINAPI savetifpageW(LPCWSTR fname, imgdes  *srcimg, int cmprsn, int page);
int  _export WINAPI savetifappendW(LPCWSTR wcSrc, LPCWSTR wcOpr, LPCWSTR wcDes);
int  _export WINAPI savetifappend(LPCSTR aSrc, LPCSTR aOpr, LPCSTR aDes);
int  _export WINAPI savetiftobuffer(UCHAR  *  *buffAddr, imgdes  *srcimg, int cmprsn);
void _export WINAPI setgifcomment(int version, char  *comment);
void _export WINAPI setimagearea(imgdes  *image, unsigned stx, unsigned sty, unsigned endx, unsigned endy);
int  _export WINAPI setpixelcolor(imgdes  *image, int xcoord, int ycoord, ULONG level);
int  _export WINAPI setupimgdes(UCHAR  *dib, imgdes  *image);
int  _export WINAPI sharpen(imgdes  *srcimg, imgdes  *desimg);
int  _export WINAPI sharpengentle(imgdes  *srcimg, imgdes  *desimg);
int  _export WINAPI standardpalette(imgdes  *srcimg);
int  _export WINAPI subimage(imgdes  *srcimg, imgdes  *oprimg, imgdes  *desimg);
int  _export WINAPI tgainfo(LPCSTR fname, TgaData  *tdat);
int  _export WINAPI tgainfoW(LPCWSTR fname, TgaData  *tdat);
int  _export WINAPI threshold(const int thres, imgdes  *srcimg, imgdes  *desimg);

int  _export WINAPI tiffgetpageinfo(LPCSTR fname, int  *totalPages, int  *pageArray, int arrayElems);
int  _export WINAPI tiffgetpageinfoW(LPCWSTR fname, int  *totalPages, int  *pageArray, int arrayElems);
int  _export WINAPI tiffgetpageinfofrombuffer(UCHAR  *buff, int  *totalPages, int  *pageArray, int arrayElems);

int  _export WINAPI tiffgetSOIofspagebyindexW(LPCWSTR wcfname, unsigned long *soiOfs, int targetPage);
int  _export WINAPI tiffgetSOIofspagebyindex(LPCSTR afname, unsigned long *soiOfs, int targetPage);
int  _export WINAPI tiffgetSOIofspagebyindexfrombuffer(UCHAR  *buff, unsigned long *soiOfs, int targetPage);

int  _export WINAPI tiffgetxyresolution(LPCSTR fname, unsigned  *xres, unsigned  *yres, unsigned  *resUnit);
int  _export WINAPI tiffgetxyresolutionW(LPCWSTR fname, unsigned  *xres, unsigned  *yres, unsigned  *resUnit);
int  _export WINAPI tiffgetxyresolutionpagebyindex(LPCSTR fname, unsigned *xres, unsigned *yres, unsigned *resUnit, int targetPage);
int  _export WINAPI tiffgetxyresolutionpagebyindexW(LPCWSTR fname, unsigned *xres, unsigned *yres, unsigned *resUnit, int targetPage);
int  _export WINAPI tiffgeterror(void);

int  _export WINAPI tiffinfo(LPCSTR fname, TiffData  *tdat);
int  _export WINAPI tiffinfoW(LPCWSTR fname, TiffData  *tdat);
int  _export WINAPI tiffinfofrombuffer(UCHAR  *buff, TiffData  *tdat);

int  _export WINAPI tiffinfopage(LPCSTR fname, TiffData  *tdat, int page);
int  _export WINAPI tiffinfopageW(LPCWSTR fname, TiffData  *tdat, int page);

int  _export WINAPI tiffinfopagebyindex(LPCSTR fname, TiffData  *tdat, int pageIndex);
int  _export WINAPI tiffinfopagebyindexW(LPCWSTR fname, TiffData  *tdat, int pageIndex);
int  _export WINAPI tiffinfopagebyindexfrombuffer(UCHAR  *buff, TiffData  *tdat, int pageIndex);

int  _export WINAPI tiffinfopagebyindexex(LPCSTR fname, TiffDataEx  *tdatex, int pageIndex);
int  _export WINAPI tiffinfopagebyindexexW(LPCWSTR fname, TiffDataEx  *tdatex, int pageIndex);
int  _export WINAPI tiffinfopagebyindexfrombufferex(UCHAR  *buff, TiffDataEx  *tdatex, int pageIndex);

// int  _export WINAPI tiffinfofrombuffer(UCHAR  *buff, TiffData  *tdat);

void _export WINAPI tiffsetxyresolution(unsigned xres, unsigned yres, unsigned resUnit);
int  _export WINAPI twoimagefcts_(imgdes *srcimg, imgdes *oprimg, imgdes *desimg, void (*twofct)(UCHAR *,UCHAR *,UCHAR *,int,int,int), BOOL allowOneBitFctn, int data);
void _export WINAPI unlockLZW(DWORD dwKey);
int  _export WINAPI usetable(const UCHAR *redtab, const UCHAR *grntab, const UCHAR *blutab, imgdes  *srcimg, imgdes  *desimg);
WORD _export WINAPI Victorversion(void);
WORD _export WINAPI Victorversiondate(LPSTR desStr, int bufsiz);
WORD _export WINAPI VictorversiondateW(LPWSTR desStr, int bufsiz);
WORD _export WINAPI Victorversionex(VIC_VERSION_INFO  *vicVerInfo);
int  _export WINAPI victowinpal(imgdes  *srcimg, HPALETTE  *hPal);
int  _export WINAPI viewimage(HWND hwnd, HDC hdc, HPALETTE  *hPal, int xpos, int ypos, imgdes  *image);
int  _export WINAPI viewimageex(HWND hwnd, HDC hdc, HPALETTE  *hPal, int xpos, int ypos, imgdes  *image, int scrx, int scry, int colRedMode);
int  _export WINAPI windowtoimage(HWND hWnd, imgdes  *image);
int  _export WINAPI wintovicpal(HPALETTE hPal, imgdes  *desimg);
int  _export WINAPI wtaverage(const int prct, imgdes *srcimg, imgdes *oprimg, imgdes *desimg);
int  _export WINAPI xorimage(imgdes *srcimg, imgdes *oprimg, imgdes *desimg);
int  _export WINAPI zeroimage(const int level, imgdes *image);
void _export WINAPI zeroimgdes(imgdes  *image);

// VicStats Declarations
int _export WINAPI calcavglevelfloat(imgdes *srcimg, double *redavg, double *grnavg, double *bluavg);
int _export WINAPI correlationcoef(imgdes *srcimg, imgdes *oprimg, double *pCoef);
int _export WINAPI correlateimages(imgdes *srcimg, imgdes *oprimg, imgdes *desimg);
int _export WINAPI sortpixelsbyval(imgdes *srcimg, COORD_VAL *darray, int nelem);
void _export WINAPI clearAvgLevelCache(void);

#define isolate addimage // Set isolate() == add()

// VicFx Declarations
int _export WINAPI addtexture(imgdes *srcimg, imgdes *oprimg, imgdes *desimg);
int _export WINAPI bleach(imgdes *srcimg, imgdes *oprimg, imgdes *desimg);
int _export WINAPI darker(imgdes *srcimg, imgdes *oprimg, imgdes *desimg);
int _export WINAPI difference(imgdes *srcimg, imgdes *oprimg, imgdes *desimg);
int _export WINAPI differenceabs(imgdes *srcimg, imgdes *oprimg, imgdes *desimg);
int _export WINAPI divideimage(imgdes *srcimg, imgdes *oprimg, imgdes *desimg);
int _export WINAPI lighter(imgdes *srcimg, imgdes *oprimg, imgdes *desimg);
int _export WINAPI multiplynegative(imgdes *srcimg, imgdes *oprimg, imgdes *desimg);

#ifdef VIC_STATIC_LIB
// Static lib declarations
void WINAPI VicLibStart(void);
void WINAPI VicLibTerm(void);
void WINAPI VicStatsLibStart(void);
void WINAPI VicStatsLibTerm(void);
#endif

// Twain declarations
// Define LOAD_VICTWAIN_EXPLICIT if we load DLL with a call to LoadLibrary
#if !defined LOAD_VICTWAIN_EXPLICIT
void _export WINAPI TWclose(void);
int  _export WINAPI TWdetecttwain(HWND hWnd);
int  _export WINAPI TWgeterror(void);
int  _export WINAPI TWgetsourcenames(HWND hWnd, TW_STR32  *nameList, int  *nameCount);
int  _export WINAPI TWgetmeasureunit(HWND hWnd, TWAIN_CAP_DATA  *typeUnit);
int  _export WINAPI TWgetpendingxfers(HWND hWnd, int  *count);
int  _export WINAPI TWgetphysicalsize(HWND hWnd, int  *width, int  *height);
int  _export WINAPI TWgetpixeltype(HWND hWnd, TWAIN_CAP_DATA  *pixelType);
int  _export WINAPI TWgetbrightness(HWND hWnd, TWAIN_CAP_DATA  *brightness);
int  _export WINAPI TWgetcontrast(HWND hWnd, TWAIN_CAP_DATA  *contrast);
int  _export WINAPI TWgetfeeder(HWND hWnd, BOOL  *feederIsEnabled, BOOL  *feederHasPaper);
int  _export WINAPI TWgetsourcenames(HWND hWnd, TW_STR32  *nameList, int  *nameCount);
int  _export WINAPI TWgetxresolution(HWND hWnd, TWAIN_CAP_DATA  *xRes);
int  _export WINAPI TWgetyresolution(HWND hWnd, TWAIN_CAP_DATA  *yRes);
int  _export WINAPI TWopen(HWND hWnd);
int  _export WINAPI TWscancountimages(HWND hWnd, imgdes  *desimg, RECT  *pRect, BOOL showIU, int maxPages, int (WINAPI *saveScan)(imgdes *));
int  _export WINAPI TWscanimage(HWND hWnd, imgdes  *desimg);
int  _export WINAPI TWscanimageex(HWND hWnd, imgdes  *desimg, RECT  *pRect, BOOL showIU);
int  _export WINAPI TWscanmultipleimages(HWND hWnd, imgdes  *desimg, int (WINAPI *saveScan)(imgdes *));
int  _export WINAPI TWscanmultipleimagesex(HWND hWnd, imgdes  *desimg, RECT  *pRect, BOOL showIU, int (WINAPI *saveScan)(imgdes *));
int  _export WINAPI TWselectsource(HWND hWnd);
int  _export WINAPI TWselectsourcebyname(HWND hWnd, LPCSTR dsName);
int  _export WINAPI TWsetbrightness(HWND hWnd, TWAIN_CAP_DATA  *brightness);
int  _export WINAPI TWsetcontrast(HWND hWnd, TWAIN_CAP_DATA  *contrast);
int  _export WINAPI TWsetduplex(HWND hWnd, BOOL enableDuplex);
int  _export WINAPI TWsetfeeder(HWND hWnd, BOOL enableFeeder);
int  _export WINAPI TWsetpagesize(HWND hWnd, unsigned pageConst);
int  _export WINAPI TWsetpixeltype(HWND hWnd, TWAIN_CAP_DATA  *pixelType);
int  _export WINAPI TWsetmeasureunit(HWND hWnd, TWAIN_CAP_DATA  *typeUnit);
int  _export WINAPI TWsetxresolution(HWND hWnd, TWAIN_CAP_DATA  *xRes);
int  _export WINAPI TWsetyresolution(HWND hWnd, TWAIN_CAP_DATA  *yRes);
void _export WINAPI TWsetproductname(LPCSTR prodName);
WORD _export WINAPI TWvicversion(void);
// Static lib declarations
void _export WINAPI TWStaticLibStart(HINSTANCE hWnd);
void _export WINAPI TWStaticLibTerm(void);
#endif

int  _export WINAPI checkrange_(imgdes  *image);
int  _export WINAPI checkrange1_(imgdes  *image);
int  _export WINAPI checkrange16_(imgdes  *image);
int  _export WINAPI checkrange32_(imgdes  *image);
void _export WINAPI CreateDefaultPalette_(imgdes  *image);
void _export WINAPI Align1bppBits_(UCHAR  *dbuff, UCHAR  *src, int stx, int bits2mov);
void _export WINAPI Store1bppBits_(UCHAR  *des, UCHAR  *sbuff, int stx, int bits2mov);

#if defined WIN32 || defined _WIN32
int _export WINAPI allocDIB(imgdes  *image, int iwidth, int ilength, int bppixel);
int _export WINAPI updatebitmapcolortable(imgdes  *image);
#define fmemcpy_ memcpy
#define fmemset_ memset
#define fmemmove_ memmove
#define _fmemcmp memcmp
#define _fcalloc calloc
#define setGlobalCompactFlag(flag)
#else
/* String functions to handle  pointers */
void  * _export WINAPI fmemcpy_(void  *des, const void  *src, unsigned count);
void  * _export WINAPI fmemmove_(void  *des, const void  *src, unsigned count);
void  * _export WINAPI fmemset_(void  *des, int mchr, unsigned count);
void _export WINAPI setGlobalCompactFlag(BOOL flag);
void _export WINAPI chirp(void);
#define allocDIB allocimage
#if defined BC_COMP_
# define  FMEMCPY   _fmemcpy
# define  FMEMSET   _fmemset
# define  FMEMMOVE  _fmemmove
#else
# define  FMEMCPY   fmemcpy_
# define  FMEMSET   fmemset_
# define  FMEMMOVE  fmemmove_
#endif  // BC_COMP_
#define allocDIB allocimage
#endif

#ifdef __cplusplus
 }
#endif   /* __cplusplus */

/* Error codes for Victor Image Processing Library for Windows */
// NO_ERROR is already defined by WINDOWS.H
#if !defined(NO_ERROR)
#define  NO_ERROR     0       /* No error */
#endif
#define  BAD_RANGE    -1      /* Range error */
#define  NO_DIG       -2      /* Digitizer board not detected */
#define  BAD_DSK      -3      /* Disk full, file not written */
#define  BAD_OPN      -4      /* Filename not found */
#define  BAD_FAC      -5      /* Non-dimensional variable out of range */
#define  BAD_TIFF     -6      /* Unreadable TIF format */
#define  TIFF_NOPAGE -51      /* TIF page not found */
#define  TIFF_MOTYPE -69      /* Can't append an Intel-type TIF to a Motorola-type TIF */
#define  BAD_BPS      -8      /* TIF bits per sample not supported */
#define  BAD_CMP      -9      /* Unreadable compression scheme */
#define  BAD_CRT     -10      /* Cannot create file */
#define  BAD_FTPE    -11      /* Unknown file format */

#define  BAD_DIB     -12      /* Device independent bitmap is compressed */
#define  VMODE_ERR   -13      /* Invalid video mode */
#define  BAD_MEM     -14      /* Insufficient memory for function */
#define  BAD_PIW     -15      /* Not PIW format */
#define  BAD_PCX     -16      /* Unreadable PCX format */
#define  BAD_GIF     -17      /* Unreadable GIF format */
#define  GIF_NOFRAME -71      /* GIF frame not found */
#define  PRT_ERR     -18      /* Print error */
#define  PRT_BUSY    -41      /* Printimage() is busy */
#define  SCAN_ERR    -19      /* Scanner error */

/* Expanded/Extended/Conventional memory error codes */
#define  CM_ERR      -20      /* Conventional memory handle overflow */
#define  NO_EMM      -21      /* Expanded memory manager not found */
#define  EMM_ERR     -22      /* Expanded memory manager error */
#define  NO_XMM      -23      /* Expanded memory manager not found */
#define  XMM_ERR     -24      /* Expanded memory manager error */

#define  BAD_TGA     -25      /* Unreadable TGA format */
#define  BAD_BPP     -26      /* Bits per pixel not supported */
#define  BAD_BMP     -27      /* Unreadable BMP format */
#define  BAD_JPEG    -43      /* Unreadable JPEG format */
#define  TOO_CPLX    -44      /* Image is too complex to complete operation */
#define  NOT_AVAIL   -50      /* Function not available due to missing module */
#define  LZW_DISABLED -53     /* LZW functionality disabled */
#define  BAD_DATA     -62     /* File contains invalid data */
#define  BAD_PNG      -63     /* Unreadable PNG format */
#define  BAD_PNG_CMP  -64     /* PNG compressor error */
#define  NO_ACK       -65     /* No ACK from digitizer */
#define  BAD_HANDLE   -68     /* Handle not valid */
#define  BAD_TN_SIZE  -70     /* Thumbnail size out of range */
#define  BAD_DIGI_MEM -72     /* Insufficient digitizer memory for selected mode */
#define  BAD_DIM      -73     /* Image format does not support width or length > 65535 */

/* Scanner ADF error codes */
#define  SCAN_UNLOAD    -45   // paper could not be unloaded from ADF
#define  SCAN_LIDUP     -46   // ADF lid was opened
#define  SCAN_NOPAPER   -47   // ADF bin is empty
#define  SCAN_NOADF     -48   // ADF is not connected,
#define  SCAN_NOTREADY  -49   // ADF is connected but not ready

/* Device error codes */
#define  COM_ERR     -31      /* Serial data reception error */
#define  BAD_COM     -32      /* No data from COM port */
#define  NO_DEV_DATA -33      /* No data from device */
#define  TIMEOUT     -34      /* Function timed out */
// Twain image acquisition errors
#define  TWAIN_FIRST_ERR  TWAIN_NOWND
#define  TWAIN_LAST_ERR   TWAIN_STOP_SCAN
//  Critical errors
#define  TWAIN_NOWND -54      /* Could not create Twain parent window */
#define  TWAIN_NODSM -55      /* Could not open Twain Source Manager */
#define  TWAIN_NODS  -56      /* Could not open Twain Data Source */
#define  TWAIN_ERR   -57      /* Twain image acquisition error */
#define  TWAIN_NOMATCH -58    /* None of the elements in two lists were equal */
#define  TWAIN_BAD_DATATYPE -59 /* Data type mismatch */
//  Informational
#define  TWAIN_SCAN_CANCEL -60 /* User cancelled scan */
#define  TWAIN_BUSY  -61      /* Twain function is busy */
#define  TWAIN_NO_PAPER -66   /* Auto feeder is empty */
#define  TWAIN_STOP_SCAN -67  /* Stop scanning images */

// Png extended error codes (available when rcode == BAD_PNG)
#define PNG_ERR_UNK_CRIT_CHK  -100 // Unknown critical chunk
#define PNG_ERR_TOO_FEW_IDATS -101 // Not enough IDATs for image
#define PNG_ERR_INV_IHDR_CHK  -102 // Invalid IHDR chunk
#define PNG_ERR_INV_BITDEPTH  -103 // Invalid bit depth in IHDR
#define PNG_ERR_INV_COLORTYPE -104 // Invalid color type in IHDR
#define PNG_ERR_INV_BITCOL    -105 // Invalid color type/bit depth combo in IHDR
#define PNG_ERR_INV_INTERLACE -106 // Invalid interlace method in IHDR
#define PNG_ERR_INV_COMP      -107 // Invalid compression method in IHDR
#define PNG_ERR_INV_FILTER    -108 // Invalid filter method in IHDR
#define PNG_ERR_IMAGE_SIZE    -109 // Invalid image size in IHDR
#define PNG_ERR_BAD_SIG       -110 // Bad PNG signature
#define PNG_ERR_BAD_CRC       -111 // Bad CRC value
#define PNG_ERR_TOO_MUCH_DATA -112 // Extra data at end of file
#define PNG_ERR_EARLY_EOF     -113 // Unexpected End Of File
#define PNG_ERR_MEM_ERR       -114 // Memory error
#define PNG_ERR_DECOMPRESSION -115 // Decompression error
#define PNG_ERR_COMPRESSION   -116 // Compression error
#define PNG_ERR_NO_DISK_SPACE -117 // Out of disk space

// Jpeg extended error codes (available when rcode == BAD_JPEG)
#define JPG_BAD_PRECISION   -100 // Sample precision is not 8
#define JPG_BAD_EOF         -101 // Unexpected End Of File
#define JPG_BAD_RESTART     -102 // Reset marker could not be found
#define JPG_INVALID_MARKER  -103 // Invalid marker found in the image data
#define JPG_READ_ERR        -104 // Error reading data from the file
#define JPG_INVALID_DATA    -105 // Invalid data found in JPEG file
#define JPG_BAD_COMPINFO    -106 // Component info out of bounds
#define JPG_BAD_BLOCKNO     -107 // Blocks in an MCU is > 10
#define JPG_BAD_BPPIXEL     -108 // Bits per sample is not 8
#define JPG_BAD_COMPNO      -109 // Invalid number of components
#define JPG_BAD_FTYPE       -110 // File type not SOF0 or SOF1
#define JPG_BAD_EOI         -111 // Unexpected End Of Image
#define JPG_BAD_JFIF        -112 // File is not JPEG JFIF
#define JPG_BAD_SCAN_PARAM  -113 // Bad progressive JPEG scan parameter
#define JPG_BAD_MEM         -114 // Out of memory
#define JPG_NO_DISK_SPACE   -115 // Out of disk space

// TIFF extended error codes (available when rcode == BAD_TIFF)
# define TIF_INVALID_DATA -100   // Invalid data found in TIF file     
# define TIF_READ_ERR     -101   // Error reading data from the file   
# define TIF_BAD_EOF      -102   // Unexpected End Of File             
# define TIF_G4_COMPLEX   -103   // Trans point arrays not large enough

/* Windows related */
/* Could not lock memory (invalid handle or memory discarded) */
#define BAD_LOCK    -40
#define BAD_IBUF    -42       // Invalid image buffer address
#define BAD_PTR     -52       // Pointer does not point at readable/writable memory

/* Error codes specifically for TI Graphics Architecture (TIGA) interface */
#define TIGA_BAD_BPP -37      /* Bits per pixel of TIGA mode not 8 */
#define TIGA_BAD_MEM -38      /* Could not allocate enough GSP memory */
#define TIGA_NO_EXT  -39      /* Could not load TIGA extended primitives */

/* Printimage() print modes */
#define PRTDEFAULT  0
#define PRTHALFTONE 1
#define PRTSCATTER  2

/* Convert8bitto1bit modes */
#define CONV8TO1SCATTER   0
#define CONV8TO1DITHER    1
#define CONV8TO1THRESHOLD 2

/* Colordither, colorscatter modes */
#define COLORSCATTER16  0
#define COLORSCATTER256 1
#define COLORDITHER16  COLORSCATTER16
#define COLORDITHER256 COLORSCATTER256

/* convertrgbtopalmode() modes */
typedef enum {
   CR_OCTREENODIFF, // Use octree color method without error diffusion
   CR_OCTREEDIFF,   // Use octree color method with error diffusion
   CR_TSDNODIFF,    // Use TSD color method without error diffusion
   CR_TSDDIFF,      // Use TSD color method with error diffusion
   }  CR_VAR;
#define CR_LOW  CR_OCTREENODIFF
#define CR_HIGH CR_TSDDIFF

/* 24-bit RGB display methods (viewimageex()) */
#define VIEWOPTPAL  0
#define VIEWDITHER  1
#define VIEWSCATTER 2

/* calchistorgb modes */
#define PALETTEIMAGEASGRAY 0
#define PALETTEIMAGEASRGB  1

/* Resize modes */
#define RESIZEFAST 0
#define RESIZEBILINEAR 1

/* File save defines */
#define TIFUNC 0
#define TIFLZW 1
#define TIFPB  2
#define TIFG3  3
#define TIFG4  4
#define TIFFAX 5
#define TIFJPG6 6
#define TIFJPG 7
#define BMPUNC 0
#define BMPRKE 1
#define TGAUNC 0
#define TGARLE 1
#define PIWUNC 0
#define PIWRLE 1
#define PNGINTERLACE   1
#define PNGALLFILTER   0
#define PNGNOFILTER    2
#define PNGSUBFILTER   4
#define PNGUPFILTER    6
#define PNGAVGFILTER   8
#define PNGPAETHFILTER 10
#define PNGALLFILTERS PNGALLFILTER
// Savejpgex() modes
#define JPGSEQ    0
#define JPGPROG   1
#define JPGSEQOPT 2
#define JPGSAVETHUMBNAIL 0x0010
#define JPGSEQTN    0x0010
#define JPGPROGTN   0x0011
#define JPGSEQOPTTN 0x0012
// Savegifex() mode flags
#define GIFLZWCOMP     0
#define GIFINTERLACE   1
#define GIFTRANSPARENT 2
#define GIFWRITE4BIT   4
#define GIFNOCOMP      8
#define GIFMULTIFRAME  16

#define GIFWRITE_1BIT   0x00010000
#define GIFWRITE_2BIT   0x00020000
#define GIFWRITE_3BIT   0x00030000
#define GIFWRITE_4BIT   0x00040000
#define GIFWRITE_5BIT   0x00050000
#define GIFWRITE_6BIT   0x00060000
#define GIFWRITE_7BIT   0x00070000
#define GIFWRITE_8BIT   0x00080000

#define TIFF_WRITE_ONE_STRIP 0x0010  // Write TIFF file as a single strip
#define TIFF_WRITE_FILLORDER_TWO 0x0020  // Write TIFF file with FillOrder = 2
#define TIFF_WRITE_MULTIFILES_NO_BACKUP 0x0040 // Write multipage TIFF file without making a backup first

// Savepngex() modes
#define PNGREG          0
#define PNGTRANSCOLOR   1

/* ScanJet models */
#define SCNJET 0
#define SJPLUS 1
#define SJIIC  2
#define SJIIP  3
#define SJIICX 4
#define SJIIIC 5
#define SJIIIP 6
/* Image output data types */
#define LINEART 0
#define BWDITH  3
#define BWGRAY  4
#define COL24   5
#define COLDITH 7
/* ScanJet parameter codes */
#define SJXRES       0      /* X resolution */
#define SJYRES       1      /* Y resolution */
#define SJXEXTENT    2      /* X extent */
#define SJYEXTENT    3      /* Y extent */
#define SJDATATYPE   4      /* Output data type */
#define SJINTENSITY  5      /* Intensity */
#define SJCONTRAST   6      /* Contrast */
#define SJRESET      7      /* Reset scanner to default conditions */
#define SJBPLINE     8      /* Bytes per scanline */
#define SJSLINES     9      /* Scanline in a window */
#define SJDWIDTH     10     /* Data width */
#define SJMODEL1     11     /* Model 1 */
#define SJMODEL2     12     /* Model 2 */
#define SJERRSTACK   13     /* Get error stack depth */
#define SJGETERRNO   14     /* Get error number */
#define SJCLRERR     15     /* Clear last error */
#define SJSCNWIN     16     /* Scan the window */
#define SJSTDWID     17     /* Set data width */
#define SJSTDTYP     18     /* Set output data type, invert image */
#define SJSTXYRS     19     /* Set X,Y resolution */
#define SJSTINTENS   20     /* Set intensity */
#define SJSTCONTRAST 21     /* Set contrast */
#define SJWINSIZ     22     /* Set scan window size */
#define SJERR        23     /* No match, return string to generate error */
#define SJADFSCNWIN    24  // ADF scan window
#define SJADFCHANGE    25  // ADF change doc
#define SJADFUNLOAD    26  // ADF unload doc
#define SJADFREADY     27  // ADF is ready
#define SJADFCONNECTED 28  // ADF is connected
#define SJADFHASPAPER  29  // ADF has paper
#define SJADFOPENED    30  // ADF opened since last change/scan
#define SJADFREADYTOUNLOAD 31 // ADF is ready to unload
#define SJMAXSTR     31

// Typedefs for scanner routine function pointers
typedef int (WINAPI *LPTWDETECTTWAIN)(HWND hWnd);
typedef int (WINAPI *LPTWGETERROR)(void);
typedef int (WINAPI *LPTWSCANIMAGE)(HWND hWnd, imgdes  *desimg);
typedef int (WINAPI *LPTWSELECTSOURCE)(HWND hWnd);
typedef int (WINAPI *LPTWSETPRODUCTNAME)(LPCSTR prodName);
typedef void (WINAPI *LPSJCLOSE)(int handle);
typedef int (WINAPI *LPSJDETECT)(int  *handle);
typedef int (WINAPI *LPSJDETECTDRIVER)(void);
typedef int (WINAPI *LPSJGETMAX)(int handle, int code);
typedef int (WINAPI *LPSJGETMIN)(int handle, int code);
typedef int (WINAPI *LPSJGETSCANBYTES)(int handle);
typedef int (WINAPI *LPSJGETSCANROWS)(int handle);
typedef int (WINAPI *LPSJMODEL)(int handle);
typedef int (WINAPI *LPSJSCANIMAGE)(int handle, imgdes  *desimg);
typedef int (WINAPI *LPSJSETCONTRAST)(int handle, int contrast);
typedef int (WINAPI *LPSJSETINTENSITY)(int handle, int contrast);
typedef int (WINAPI *LPSJSETDATATYPE)(int handle, int type);
typedef int (WINAPI *LPSJSETDATAWIDTH)(int handle, int width);
typedef int (WINAPI *LPSJWINSIZE)(int handle, RECT  *scanrect);
typedef int (WINAPI *LPSJXYRES)(int handle, int xval, int yval);

// Wide char string functions (functions without 'W' are ANSI)
#ifdef UNICODE
#define bmpinfo  bmpinfoW
#define getgifcomment  getgifcommentW
#define getpngcomment  getpngcommentW
#define gifinfo  gifinfoW
#define gifframecount  gifframecountW
#define gifinfoframe  gifinfoframeW
#define gifinfoallframes  gifinfoallframesW
#define jpeginfo  jpeginfoW
#define jpeginfoex  jpeginfoexW
#define loadbif  loadbifW
#define loadbmp  loadbmpW
#define loadbmppalette  loadbmppaletteW
#define loadgifframe  loadgifframeW
#define loadgif  loadgifW
#define loadgifpalette  loadgifpaletteW
#define loadgifglobalpalette  loadgifglobalpaletteW
#define loadgifframepalette  loadgifframepaletteW
#define loadjpg  loadjpgW
#define loadjpgthumbnail  loadjpgthumbnailW
#define loadpcx  loadpcxW
#define loadpcxpalette  loadpcxpaletteW
#define loadpng  loadpngW
#define loadpngpalette  loadpngpaletteW
#define loadtga  loadtgaW
#define loadtgapalette  loadtgapaletteW
#define loadtif  loadtifW
#define loadtifpage  loadtifpageW
#define loadtifpagebyindex  loadtifpagebyindexW
#define loadtifpalette  loadtifpaletteW
#define loadtifpalettepage  loadtifpalettepageW
#define loadtifpalettepagebyindex  loadtifpalettepagebyindexW
#define pcxinfo  pcxinfoW
#define printimagestartdoc  printimagestartdocW
#define pnggetxyresolution  pnggetxyresolutionW
#define pnginfo  pnginfoW
#define savebif  savebifW
#define savebmp  savebmpW
#define saveeps  saveepsW
#define savegif  savegifW
#define savegifex  savegifexW
#define savegifframe  savegifframeW
#define savejpg  savejpgW
#define savejpgex  savejpgexW
#define savepcx  savepcxW
#define savepng  savepngW
#define savepngex  savepngexW
#define savetga  savetgaW
#define savetif  savetifW
#define savetifpage  savetifpageW
#define savetifappend  savetifappendW
#define tgainfo  tgainfoW
#define tiffgetpageinfo  tiffgetpageinfoW
#define tiffgetSOIofspagebyindex tiffgetSOIofspagebyindexW
#define tiffgetxyresolution  tiffgetxyresolutionW
#define tiffgetxyresolutionpagebyindex  tiffgetxyresolutionpagebyindexW
#define tiffinfo  tiffinfoW
#define tiffinfopage  tiffinfopageW
#define tiffinfopagebyindex  tiffinfopagebyindexW
#define tiffinfopagebyindexex  tiffinfopagebyindexexW
#define savegifframe  savegifframeW 
#define loadgifframe  loadgifframeW 
#define gifframecount  gifframecountW 
#define gifinfoallframes  gifinfoallframesW 
#define gifinfoframe   gifinfoframeW 
#define loadgifglobalpalette  loadgifglobalpaletteW 
#define loadgifframepalette  loadgifframepaletteW 
#define Victorversiondate  VictorversiondateW
#endif // UNICODE

#endif  /* _VICDEFS_H_ */
