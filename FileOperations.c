/*
 * FileOperations.c
 *
 *  Created on: 16.06.2013
 *      Author: daniel
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

// #include "SDL.h"

#if defined(__WIN32__) || defined(__WINRT__)
#	include <direct.h>
#	include <io.h>
#   include "SDL_stdinc.h"

#define WIN_UTF8ToString(S) (wchar_t *)SDL_iconv_string("UTF-16LE", "UTF-8", (char *)(S), SDL_strlen(S)+1)

#else
#	include <unistd.h>
#   include <errno.h>
#endif

#include "FileOperations.h"

#ifdef __ANDROID__
#include <android/asset_manager.h>
#include <android/log.h>
#endif

static int        file_exists_plain(const char *name);
static FILEHANDLE file_open_plain(const char *name, const char *mode);
static long       file_read_plain(char *buffer, long size, FILEHANDLE handle);
static long       file_write_plain(const char *buffer, long size, FILEHANDLE handle);
static long       file_print_plain(FILEHANDLE handle, const char *format, ...);
static int        file_putc_plain(int character, FILEHANDLE handle);
static int        file_seek_plain(FILEHANDLE handle, off_t offset, int whence);
static long       file_tell_plain(FILEHANDLE handle);
static int        file_error_plain(FILEHANDLE handle);
static int        file_rename_plain(const char *oldname, const char *newname);
static int        file_remove_plain(const char *name);
static int        file_flush_plain(FILEHANDLE handle);
static int        file_close_plain(FILEHANDLE handle);

#ifdef __ANDROID__

extern AAssetManager *assetManager;

static int        file_exists_android_asset(const char *name);
static FILEHANDLE file_open_android_asset(const char *name, const char *mode);
static long       file_read_android_asset(char *buffer, long size, FILEHANDLE handle);
static long       file_write_android_asset(const char *buffer, long size, FILEHANDLE handle);
static long       file_print_android_asset(FILEHANDLE handle, const char *format, ...);
static int        file_putc_android_asset(int character, FILEHANDLE handle);
static int        file_seek_android_asset(FILEHANDLE handle, off_t offset, int whence);
static long       file_tell_android_asset(FILEHANDLE handle);
static int        file_error_android_asset(FILEHANDLE handle);
static int        file_rename_android_asset(const char *oldname, const char *newname);
static int        file_remove_android_asset(const char *name);
static int        file_flush_android_asset(FILEHANDLE handle);
static int        file_close_android_asset(FILEHANDLE handle);

static int        file_exists_android_obb_plain(const char *name);
static FILEHANDLE file_open_android_obb_plain(const char *name, const char *mode);
static long       file_read_android_obb_plain(char *buffer, long size, FILEHANDLE handle);
static long       file_write_android_obb_plain(const char *buffer, long size, FILEHANDLE handle);
static long       file_print_android_obb_plain(FILEHANDLE handle, const char *format, ...);
static int        file_putc_android_obb_plain(int character, FILEHANDLE handle);
static int        file_seek_android_obb_plain(FILEHANDLE handle, off_t offset, int whence);
static long       file_tell_android_obb_plain(FILEHANDLE handle);
static int        file_error_android_obb_plain(FILEHANDLE handle);
static int        file_rename_android_obb_plain(const char *oldname, const char *newname);
static int        file_remove_android_obb_plain(const char *name);
static int        file_flush_android_obb_plain(FILEHANDLE handle);
static int        file_close_android_obb_plain(FILEHANDLE handle);

static int        file_exists_android_obb_mount(const char *name);
static FILEHANDLE file_open_android_obb_mount(const char *name, const char *mode);
static long       file_read_android_obb_mount(char *buffer, long size, FILEHANDLE handle);
static long       file_write_android_obb_mount(const char *buffer, long size, FILEHANDLE handle);
static long       file_print_android_obb_mount(FILEHANDLE handle, const char *format, ...);
static int        file_putc_android_obb_mount(int character, FILEHANDLE handle);
static int        file_seek_android_obb_mount(FILEHANDLE handle, off_t offset, int whence);
static long       file_tell_android_obb_mount(FILEHANDLE handle);
static int        file_error_android_obb_mount(FILEHANDLE handle);
static int        file_rename_android_obb_mount(const char *oldname, const char *newname);
static int        file_remove_android_obb_mount(const char *name);
static int        file_flush_android_obb_mount(FILEHANDLE handle);
static int        file_close_android_obb_mount(FILEHANDLE handle);

#endif


generic_file_ops file_ops_plain =
{
	file_exists_plain,
	file_open_plain,
	file_read_plain,
	file_write_plain,
	file_print_plain,
	file_putc_plain,
	file_seek_plain,
	file_tell_plain,
	file_error_plain,
	file_rename_plain,
	file_remove_plain,
	file_flush_plain,
	file_close_plain
};

#ifdef __ANDROID__

generic_file_ops file_ops_android_asset =
{
  .file_exists = file_exists_android_asset,
  .file_open   = file_open_android_asset,
  .file_read   = file_read_android_asset,
  .file_write  = file_write_android_asset,
  .file_print  = file_print_android_asset,
  .file_putc   = file_putc_android_asset,
  .file_seek   = file_seek_android_asset,
  .file_tell   = file_tell_android_asset,
  .file_error  = file_error_android_asset,
  .file_rename = file_rename_android_asset,
  .file_remove = file_remove_android_asset,
  .file_flush  = file_flush_android_asset,
  .file_close  = file_close_android_asset
};

generic_file_ops file_ops_android_obb_plain =
{
  .file_exists = file_exists_android_obb_plain,
  .file_open   = file_open_android_obb_plain,
  .file_read   = file_read_android_obb_plain,
  .file_write  = file_write_android_obb_plain,
  .file_print  = file_print_android_obb_plain,
  .file_putc   = file_putc_android_obb_plain,
  .file_seek   = file_seek_android_obb_plain,
  .file_tell   = file_tell_android_obb_plain,
  .file_error  = file_error_android_obb_plain,
  .file_rename = file_rename_android_obb_plain,
  .file_remove = file_remove_android_obb_plain,
  .file_flush  = file_flush_android_obb_plain,
  .file_close  = file_close_android_obb_plain
};

generic_file_ops file_ops_android_obb_mount =
{
  .file_exists = file_exists_android_obb_mount,
  .file_open   = file_open_android_obb_mount,
  .file_read   = file_read_android_obb_mount,
  .file_write  = file_write_android_obb_mount,
  .file_print  = file_print_android_obb_mount,
  .file_putc   = file_putc_android_obb_mount,
  .file_seek   = file_seek_android_obb_mount,
  .file_tell   = file_tell_android_obb_mount,
  .file_error  = file_error_android_obb_mount,
  .file_rename = file_rename_android_obb_mount,
  .file_remove = file_remove_android_obb_mount,
  .file_flush  = file_flush_android_obb_mount,
  .file_close  = file_close_android_obb_mount
};

#endif

generic_file_ops *get_file_operations(file_access_variant access_variant)
{
  if (access_variant == FILE_ACCESS_VARIANT_PLAIN)
  {
#ifdef __ANDROID__
	// __android_log_print(ANDROID_LOG_VERBOSE, "org.libsdl.app", "FileOperations: Requested PLAIN access.");
#endif
    return &file_ops_plain;
  }

#ifdef __ANDROID__

  if (access_variant == FILE_ACCESS_VARIANT_ANDROID_ASSET)
  {
    // __android_log_print(ANDROID_LOG_VERBOSE, "org.libsdl.app", "FileOperations: Requested ANDROID ASSET access.");
    return &file_ops_android_asset;
  }
  if (access_variant == FILE_ACCESS_VARIANT_ANDROID_OBB_PLAIN)
  {
    // __android_log_print(ANDROID_LOG_VERBOSE, "org.libsdl.app", "FileOperations: Requested OBB PLAIN access.");
    return &file_ops_android_obb_plain;
  }
  if (access_variant == FILE_ACCESS_VARIANT_ANDROID_OBB_MOUNT)
  {
    return &file_ops_android_obb_mount;
  }

#endif

  return NULL;
}

static int        file_exists_plain(const char *name)
{
#if defined(__WIN32__) || defined(__WINRT__)
	return _waccess(WIN_UTF8ToString(name), 0);
#else
	return access(name, R_OK);
#endif
}

static FILEHANDLE file_open_plain(const char *name, const char *mode)
{
#if defined(__WIN32__) || defined(__WINRT__)
	return (FILEHANDLE) _wfopen(WIN_UTF8ToString(name), WIN_UTF8ToString(mode));
#else
	return (FILEHANDLE) fopen(name, mode);
#endif
}

static long       file_read_plain(char *buffer, long size, FILEHANDLE handle)
{
	return fread(buffer, 1, size, (FILE*) handle);
}

static long       file_write_plain(const char *buffer, long size, FILEHANDLE handle)
{
	return fwrite(buffer, 1, size, (FILE*) handle);
}

static long       file_print_plain(FILEHANDLE handle, const char *format, ...)
{
	int retval;
	va_list arglist;
	va_start( arglist, format );
	retval = vfprintf((FILE*) handle, format, arglist);
	va_end(arglist);

	return retval;
}

static int        file_putc_plain(int character, FILEHANDLE handle)
{
	return fputc(character, (FILE*) handle);
}

static int        file_seek_plain(FILEHANDLE handle, off_t offset, int whence)
{
#if defined(__MINGW32__) || defined(WIN32) || defined(__WINRT__)
	return fseek((FILE*) handle, offset, whence);
#else
	return fseeko((FILE*) handle, offset, whence);
#endif
}

static long       file_tell_plain(FILEHANDLE handle)
{
	return ftell((FILE*) handle);
}

static int        file_error_plain(FILEHANDLE handle)
{
	return ferror((FILE*) handle);
}

static int        file_rename_plain(const char *oldname, const char *newname)
{
#if defined(__WIN32__) || defined(__WINRT__)
	return _wrename(WIN_UTF8ToString(oldname), WIN_UTF8ToString(newname));
#else
	return rename(oldname, newname);
#endif
}

static int        file_remove_plain(const char *name)
{
#if defined(__WIN32__) || defined(__WINRT__)
	return _wremove(WIN_UTF8ToString(name));
#else
	return remove(name);
#endif
}

static int        file_flush_plain(FILEHANDLE handle)
{
	int flush_status;
	flush_status = fflush(handle);
	if (flush_status != 0)
	{
		flush_status = errno;
	}

	// explicitly call fsync to make sure data makes it to disk
	int fsync_status;
#if defined(__WIN32__) || defined(__WINRT__)
	int descr = _fileno(handle);
#else
	int descr = fileno(handle);
#endif
	if (descr != -1)
	{
#if defined(__WIN32__) || defined(__WINRT__) || defined(__MINGW32__)
		fsync_status = _commit(descr);
#else
		fsync_status = fsync(descr);
#endif
		if (fsync_status != 0)
		{
			fsync_status = errno;
		}
	}
	else
	{
		fsync_status = 0;
	}

	return (flush_status != 0) ? flush_status : (fsync_status != 0) ? fsync_status : 0;
}

static int        file_close_plain(FILEHANDLE handle)
{
	return fclose((FILE*)handle);
}

#ifdef __ANDROID__

char buffer[32768];

static int        file_exists_android_asset(const char *name)
{
	int i;
	int retval = 0;

    // skip the "asset://" prefix and remove a possible trailing slash
	int len = strlen(name);
	strcpy(buffer, name + 8);
	len = len - 8;
	if ((buffer[len - 1] == '/') || (buffer[len - 1] == '\\'))
	{
		buffer[len - 1] = 0;
	}
	for (i = 0; i < len; i++)
	{
		if (buffer[i] == '\\')
		{
			buffer[i] = '/';
		}
	}

	AAsset *asset = AAssetManager_open(assetManager, buffer, AASSET_MODE_BUFFER);
    if (asset == NULL)
    {
    	// __android_log_print(ANDROID_LOG_VERBOSE, "org.libsdl.app", "AssetFile: Request to open asset at path: %s FAILED", buffer);
       retval = -1;
    }
    else
    {
    	// __android_log_print(ANDROID_LOG_VERBOSE, "org.libsdl.app", "AssetFile: Request to open asset at path: %s OK", buffer);
        AAsset_close(asset);
    }

    return retval;
}

static FILEHANDLE file_open_android_asset(const char *name, const char *mode)
{
    // ignore the supplied mode flags

    // skip the "asset://" prefix and remove a possible trailing slash
	int i;
	int len = strlen(name);
	strcpy(buffer, name + 8);
	len = len - 8;
	if ((buffer[len - 1] == '/') || (buffer[len - 1] == '\\'))
	{
		buffer[len - 1] = 0;
	}
	for (i = 0; i < len; i++)
	{
		if (buffer[i] == '\\')
		{
			buffer[i] = '/';
		}
	}

	FILEHANDLE handle = (FILEHANDLE) AAssetManager_open(assetManager, buffer, AASSET_MODE_BUFFER);
	// __android_log_print(ANDROID_LOG_VERBOSE, "org.libsdl.app", "AssetFile: Request to open asset at path: %s success=%s", buffer, (handle == NULL) ? "FALSE" : "TRUE");
    return handle;
}

static long       file_read_android_asset(char *buffer, long size, FILEHANDLE handle)
{
	long retval = AAsset_read((AAsset *) handle, buffer, size);
	// __android_log_print(ANDROID_LOG_VERBOSE, "org.libsdl.app", "AssetFile: read=%ld", retval);
    return retval;
}

static long       file_write_android_asset(const char *buffer, long size, FILEHANDLE handle)
{
    // writing is not possible
    return 0;
}

static long       file_print_android_asset(FILEHANDLE handle, const char *format, ...)
{
    // writing is not possible
    return 0;
}

static int        file_putc_android_asset(int character, FILEHANDLE handle)
{
    // writing is not possible
    return 0;
}

static int        file_seek_android_asset(FILEHANDLE handle, off_t offset, int whence)
{
    int retval = AAsset_seek((AAsset *) handle, offset, whence);
	// __android_log_print(ANDROID_LOG_VERBOSE, "org.libsdl.app", "AssetFile: seek=%d", retval);

	// need to align to original fseek() return value
	if (retval >= 0)
	{
		retval = 0;
	}

	return retval;
}

static long       file_tell_android_asset(FILEHANDLE handle)
{
	off_t length;
	off_t remainingLength;
	long retval;

	length = AAsset_getLength((AAsset *) handle);
	remainingLength = AAsset_getRemainingLength((AAsset *) handle);

	retval = length - remainingLength;

	/*
	__android_log_print(ANDROID_LOG_VERBOSE, "org.libsdl.app", "AssetFile: tell len=%d remainingLen=%d tell=%ld",
			length, remainingLength, retval);
	*/

	return retval;
}

static int        file_error_android_asset(FILEHANDLE handle)
{
	return 0;
}

static int        file_rename_android_asset(const char *oldname, const char *newname)
{
	// renaming is not possible
	return 0;
}

static int        file_remove_android_asset(const char *name)
{
	// removing is not possible
	return 0;
}

static int        file_flush_android_asset(FILEHANDLE handle)
{
	// flushing not possible
	return 0;
}

static int        file_close_android_asset(FILEHANDLE handle)
{
	AAsset_close((AAsset *)handle);
	return 0;
}

static int        file_exists_android_obb_plain(const char *name)
{
  // skip the "obbplain://" prefix and remove a possible trailing slash
  int i;
  int len = strlen(name);
  strcpy(buffer, name + 11);
  len = len - 11;
  if ((buffer[len - 1] == '/') || (buffer[len - 1] == '\\'))
  {
    buffer[len - 1] = 0;
  }
  for (i = 0; i < len; i++)
  {
    if (buffer[i] == '\\')
    {
      buffer[i] = '/';
    }
  }

  return access(buffer, R_OK);
}

static FILEHANDLE file_open_android_obb_plain(const char *name, const char *mode)
{
  // skip the "obbplain://" prefix and remove a possible trailing slash
  int i;
  int len = strlen(name);
  strcpy(buffer, name + 11);
  len = len - 11;
  if ((buffer[len - 1] == '/') || (buffer[len - 1] == '\\'))
  {
    buffer[len - 1] = 0;
  }
  for (i = 0; i < len; i++)
  {
    if (buffer[i] == '\\')
    {
      buffer[i] = '/';
    }
  }

  // do not open the file for writing or appending, only reading is good
  if ((mode[0] != 'r') && (mode[0] != 'R'))
  {
    return NULL;
  }

  return (FILEHANDLE) fopen(buffer, mode);
}

static long       file_read_android_obb_plain(char *buffer, long size, FILEHANDLE handle)
{
  return fread(buffer, 1, size, (FILE*) handle);
}

static long       file_write_android_obb_plain(const char *buffer, long size, FILEHANDLE handle)
{
  // OBB files shall never be written to
  return 0;
}

static long       file_print_android_obb_plain(FILEHANDLE handle, const char *format, ...)
{
  // OBB files shall never be written to
  return 0;
}

static int        file_putc_android_obb_plain(int character, FILEHANDLE handle)
{
  // OBB files shall never be written to
  return 0;
}

static int        file_seek_android_obb_plain(FILEHANDLE handle, off_t offset, int whence)
{
  return fseeko((FILE*) handle, offset, whence);
}

static long       file_tell_android_obb_plain(FILEHANDLE handle)
{
  return ftell((FILE*) handle);
}

static int        file_error_android_obb_plain(FILEHANDLE handle)
{
  return ferror((FILE*) handle);
}

static int        file_rename_android_obb_plain(const char *oldname, const char *newname)
{
	// renaming is not possible
	return 0;
}

static int        file_remove_android_obb_plain(const char *name)
{
	// removing is not possible
	return 0;
}

static int        file_flush_android_obb_plain(FILEHANDLE handle)
{
	// flushing not possible
	return 0;
}

static int        file_close_android_obb_plain(FILEHANDLE handle)
{
  return fclose((FILE*) handle);
}

static int        file_exists_android_obb_mount(const char *name)
{
  // skip the "obbmount://" prefix and remove a possible trailing slash
  int i;
  int len = strlen(name);
  strcpy(buffer, name + 11);
  len = len - 11;
  if ((buffer[len - 1] == '/') || (buffer[len - 1] == '\\'))
  {
    buffer[len - 1] = 0;
  }
  for (i = 0; i < len; i++)
  {
    if (buffer[i] == '\\')
    {
      buffer[i] = '/';
    }
  }

  return access(buffer, R_OK);
}

static FILEHANDLE file_open_android_obb_mount(const char *name, const char *mode)
{
  // skip the "obbmount://" prefix and remove a possible trailing slash
  // in case a path starts with this prefix, the path is already the path
  // of the "mounted" obb (all logic is in the Java part)
  int i;
  int len = strlen(name);
  strcpy(buffer, name + 11);
  len = len - 11;
  if ((buffer[len - 1] == '/') || (buffer[len - 1] == '\\'))
  {
    buffer[len - 1] = 0;
  }
  for (i = 0; i < len; i++)
  {
    if (buffer[i] == '\\')
    {
      buffer[i] = '/';
    }
  }

  // do not open the file for writing or appending, only reading is good
  if ((mode[0] != 'r') && (mode[0] != 'R'))
  {
    return NULL;
  }

  return (FILEHANDLE) fopen(buffer, mode);
}

static long       file_read_android_obb_mount(char *buffer, long size, FILEHANDLE handle)
{
  return fread(buffer, 1, size, (FILE*) handle);
}

static long       file_write_android_obb_mount(const char *buffer, long size, FILEHANDLE handle)
{
  // OBB files shall never be written to
  return 0;
}

static long       file_print_android_obb_mount(FILEHANDLE handle, const char *format, ...)
{
  // OBB files shall never be written to
  return 0;
}

static int        file_putc_android_obb_mount(int character, FILEHANDLE handle)
{
  // OBB files shall never be written to
  return 0;
}

static int        file_seek_android_obb_mount(FILEHANDLE handle, off_t offset, int whence)
{
  return fseeko((FILE*) handle, offset, whence);
}

static long       file_tell_android_obb_mount(FILEHANDLE handle)
{
  return ftell((FILE*) handle);
}

static int        file_error_android_obb_mount(FILEHANDLE handle)
{
  return ferror((FILE*) handle);
}

static int        file_rename_android_obb_mount(const char *oldname, const char *newname)
{
	// renaming is not possible
	return 0;
}

static int        file_remove_android_obb_mount(const char *name)
{
	// removing is not possible
	return 0;
}

static int        file_flush_android_obb_mount(FILEHANDLE handle)
{
	// flushing not possible
	return 0;
}

static int        file_close_android_obb_mount(FILEHANDLE handle)
{
  return fclose((FILE*) handle);
}

#endif
