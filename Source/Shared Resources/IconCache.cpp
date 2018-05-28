#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "IconCache.h"

IconCache::IconCache(const char* cachePath)
{
    struct stat fileStat;
    stat(cachePath, &fileStat);
    fileLen = fileStat.st_size;
    if(fileLen <= 0)
    {
        DBG("IconCache::IconCache: Failed to find cache file " << cachePath);
	return;
    }
    fd = open(cachePath, O_RDONLY, 0);
    if(fd <= 0)
    {
        DBG("IconCache::IconCache: Failed to open cache file " << cachePath);
	return;
    }
    fileMap = mmap(nullptr, fileLen, PROT_READ, MAP_PRIVATE | MAP_POPULATE,
            fd, 0);
    if(fileMap == MAP_FAILED)
    {
        DBG("IconCache::IconCache: Failed to map cache file " << cachePath);
	return;
    }
    DBG("IconCache::IconCache: mapped cache file" << cachePath << ", size " 
            << String(fileLen));
    bigEndian == (htonl(47) == 47);
    uint32 hashOffset = read<uint32>(4);
    uint32 dirListOffset = read<uint32>(8);
        
    DBG("Reading dir list from offset " << String(dirListOffset));
    uint32 numDirs = read<uint32>(dirListOffset);
    dirListOffset += 4; 
    DBG("Finding " << String(numDirs) << " directories");
    for(int i = 0; i < numDirs; i++)
    {
        uint32 offset = read<uint32>(dirListOffset + i * 4);
	directories.add(readString(offset));
	DBG("Directory " << i << ":" << directories[i]);
    }

    hashBuckets = read<uint32>(hashOffset);
    DBG("Hash bucket count: " << String(hashBuckets));       
    hashOffset+=4;
    for(int i = 0; i < hashBuckets; i++)
    {
        hashOffsets.add(read<uint32>(hashOffset + i * 4));
    }

    for(int i = 0; i < hashBuckets; i++)
    {
        DBG("Bucket " << i << ":");
	uint32 first = read<uint32>(hashOffsets[i]);
	for(uint32 offset = first;
	        offset > 0 && offset < fileLen;
		offset = read<uint32>(offset))
	{
	    String name = readString(read<uint32>(offset + 4));
	    DBG("\t" << String(hashValue(name.toRawUTF8())) << ": "
			    << name);
	}
    }
}

IconCache::~IconCache()
{
    if(fileMap != MAP_FAILED)
    {
        int rc = munmap(fileMap, fileLen);
	jassert(rc == 0);
    }
    if(fd > 0)
    {
        close(fd);
    }
}

String IconCache::readString(uint32 offset)
{
    if(fileMap == MAP_FAILED || offset >= fileLen)
    {
        jassertfalse;
	return String();
    }
    int length = 0;
    while(read<char>((uint32) offset + length) != '\0')
    {
        length++;
	if((length + offset) >= fileLen)
	{
	    jassertfalse;
	    return String();
	}
    } 
    return String((char *) fileMap + offset);
}

uint32 IconCache::hashValue(const char* icon)
{
    if(icon == nullptr)
    {
	jassertfalse;
        return 0;
    }
    uint32 val = (int) * icon;
    for(const char* ch = icon + 1; *ch != '\0'; ch++)
    {
        val = (val << 5) - val + *ch;
    }
    return val % hashBuckets;
}

/*
  https://raw.githubusercontent.com/GNOME/gtk/master/docs/iconcache.txt
 Temporarily added here for reference- remove when no longer needed
 
 Information about the icon theme cache format used by GTK+
for more information, see the mailing list threads at 

http://mail.gnome.org/archives/gtk-devel-list/2004-April/msg00065.html
http://lists.freedesktop.org/archives/xdg/2004-October/005140.html


Back in May, Owen Taylor proposed [1] caching scheme for icon theme
information, to reduce the amount of stating and disk seeking at
application startup, and to reduce the memory overhead if each app
allocates all the icon theme data separately.

The proposal is to keep the information about the icons in the 
directory tree below each icon theme directory in an mmap()able 
cache file (There is basically one cache file per index.theme file). 

The cache doesn't try to hold all information from the index.theme files
that you would need to do lookups; it turns out that with the icon theme
specification, this isn't even sensible - you can install a 

 $HOME/.local/share/icons/Bluecurve/index.theme

That overrides

 /usr/share/icons/Bluecurve/index.theme

and chances how lookup happens for icons in /usr/share/icons/Bluecurve.

We would like to propose the cache file format as an appendix to the
icon theme specification. One thing which still needs to be investigated
is caching of the actual image data; the file format is has an
IMAGE_DATA_OFFSET member to allow adding that compatibly. An
implementation of the caching scheme for GTK+ can be found at [2]. The
cache generator which is included in the patch depends only on glib, and
it may be a good idea to move it to freedesktop.org as well. 


Regards, Matthias Clasen


The cache file format:

Header:
2			CARD16		MAJOR_VERSION	1	
2			CARD16		MINOR_VERSION	0	
4			CARD32		HASH_OFFSET		
4			CARD32		DIRECTORY_LIST_OFFSET

DirectoryList:
4			CARD32		N_DIRECTORIES		
4*N_DIRECTORIES		CARD32		DIRECTORY_OFFSET	

Hash:
4			CARD32		N_BUCKETS		
4*N_BUCKETS		CARD32		ICON_OFFSET	

Icon:
4			CARD32		CHAIN_OFFSET		
4			CARD32		NAME_OFFSET		
4			CARD32		IMAGE_LIST_OFFSET	

ImageList:
4			CARD32		N_IMAGES		
8*N_IMAGES		Image		IMAGES			

Image:
2			CARD16		DIRECTORY_INDEX	
2			ICON_FLAGS	FLAGS			
4			CARD32		IMAGE_DATA_OFFSET	

ICON_FLAGS
HAS_SUFFIX_PNG	1
HAS_SUFFIX_XPM	2
HAS_SUFFIX_SVG	4
HAS_ICON_FILE	8

ImageData:
4			CARD32		IMAGE_PIXEL_DATA_OFFSET
4			CARD32		IMAGE_META_DATA_OFFSET

4			CARD32		IMAGE_PIXEL_DATA_TYPE
4			CARD32		IMAGE_PIXEL_DATA_LENGTH
N/A			N/A		PIXEL_DATA

IMAGE_PIXEL_DATA_TYPE
0 GdkPixdata format

MetaData:
4			CARD32		EMBEDDED_RECT_OFFSET
4			CARD32		ATTACH_POINT_LIST_OFFSET
4			CARD32		DISPLAY_NAME_LIST_OFFSET

EmbeddedRect:
2			CARD16		X0
2			CARD16		Y0
2			CARD16		X1
2			CARD16		Y1

AttachPointList:
4			CARD32		N_ATTACH_POINTS
4*N_ATTACH_POINTS	AttachPoint

AttachPoint:
2			CARD16		X
2			CARD16		Y

DisplayNameList:
4			CARD32		N_DISPLAY_NAMES
4*N_DISPLAY_NAMES	DisplayName

DisplayName:
4			CARD32		DISPLAY_LANG_OFFSET
4			CARD32		DISPLAY_NAME_OFFSET
		

Notes:

* All offsets are from in bytes from the beginning of the file

* Strings are zero-terminated

* Directories are stored as relative paths.

* All numbers are in network (big-endian) order. This is
  necessary because the data will be stored in arch-independent
  directories like /usr/share/icons or even in user's 
  home directories.

* The hash function is that used by g_str_hash()

  unsigned int
  icon_str_hash (gconstpointer key)
  {
    const char *p = key;
    unsigned int h = *p;

    if (h)
      for (p += 1; *p != '\0'; p++)
        h = (h << 5) - h + *p;

    return h;
  }

 This should not be implemented by calling g_str_hash(). For
 optimal results, N_BUCKETS should be typically be prime.

* The same file format is used for icon themes (e.g.,
  /usr/share/icons/Bluecurve) and for unthemed icon directories
  (e.g., /usr/share/pixmaps)

  For an unthemed directory, N_DIRECTORIES==0 and each
  image has a DIRECTORY_INDEX field of 0xFFFF.

* Up-to-dateness of a cache file is determined simply:

    If the mod-time on the directory where the cache file
    lives is newer than the mod-time of the cache file,
    the cache file is out of date.

* Cache files have to be written atomically - write to a
  temporary name, then move over the old file - so that
  clients that have the old cache file open and mmap'ed
  won't get corrupt data.
 */
