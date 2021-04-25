#ifndef AREG_BASE_CEFILE_HPP
#define AREG_BASE_CEFILE_HPP

/************************************************************************
 * \file        areg/src/base/CEFile.hpp
 * \ingroup     AREG SDK, Asynchronous Event Generator Software Development Kit 
 * \author      Artak Avetyan (mailto:artak@aregtech.com)
 * \brief       AREG Platform, File class to work with
 *              files on File System. Also can be used as an object
 *              for data streaming.
 *
 ************************************************************************/
/************************************************************************
 * Include files.
 ************************************************************************/
#include "areg/src/base/GEGlobal.h"
#include "areg/src/base/CEFileBase.hpp"
#include "areg/src/base/CEString.hpp"

class CEStringList;

/**
 * \brief       File class to work with files on File System.
 *              Supports all functionalities for data streaming.
 * 
 * \details     This class derived from file base class. The class
 *              supports main functionalities with files on File System.
 *              Also functions to support data streaming and can be used
 *              as a streaming object.
 *
 **/
//////////////////////////////////////////////////////////////////////////
// CEFile class declaration
//////////////////////////////////////////////////////////////////////////
class AREG_API CEFile    : public CEFileBase
{
//////////////////////////////////////////////////////////////////////////
// defined constants
//////////////////////////////////////////////////////////////////////////
public:

    /**
     * \brief   Special folder definitions
     **/
    typedef enum E_SpecialFolder
    {
          SpecialUserHome       //<! Current user home folder path.
        , SpecialPersonal       //<! The user's document folder path.
        , SpecialAppData        //<! Application data folder path.
        , SpecialTemp           //<! USER or System temp folder path.

    } eSpecialFolder;

    /**
     * \brief   CEFile::SPEACIAL_MASKS
     *          The list of masked names to replace with special folders.
     */
    static const char * const   SPEACIAL_MASKS[]    /* = {"%home%", "%personal", "%appdata%", "%temp%"} */;

    /**
     * \brief   CEFile::LEN_SPECIAL_MASKS
     *          The special masked names array length.
     */
    static const int            LEN_SPECIAL_MASKS   /* = MACRO_SIZEOF_ARRAY(SPEACIAL_MASKS) */;

    /**
     * \brief   CEFile::TEMP_FILE_PREFIX
     *          The default prefix for temporary file. By default it is "_areg"
     **/
    static const char * const   TEMP_FILE_PREFIX    /*= "_areg"*/;

    /**
     * \brief   CEFile::TIMESTAMP_FILE_MASK
     *          Mask used in file name to set timestamp
     **/
    static const char * const   FILE_MASK_TIMESTAMP /*= "%time%"*/;

    /**
     * \brief   The length of FILE_MASK_TIMESTAMP string.
     **/
    static const int            FILE_MASK_TIMESTAMP_LEN;

    /**
     * \brief   CEFile::TIMESTAMP_FORMAT
     *          Default timestamp format, used as yyyy_mm_dd_hh_mm_ss_ms
     **/
    static const char * const   TIMESTAMP_FORMAT   /*= "%04d_%02d_%02d_%02d_%02d_%02d_%03d"*/; // yyyy_mm_dd_hh_mm_ss_ms

    /**
     * \brief   The name of application to add in file name or path
     **/
    static const char * const   FILE_MASK_APPNAME   /*= "%appname%"*/;

    /**
     * \brief   The length of FILE_MASK_APPNAME string.
     **/
    static const int            FILE_MASK_APPNAME_LEN;

    /**
     * \brief   CEFile::EXTENSION_SEPARATOR
     *          File extension separator.
     **/
    static const char           EXTENSION_SEPARATOR /*= '.'*/;

    /**
     * \brief   CEFile::UNIX_SEPARATOR
     *          Unix style path separator.
     **/
    static const char           UNIX_SEPARATOR      /* = '/' */;

    /**
     * \brief   CEFile::DOS_SEPARATOR
     *          DOS style path separator.
     **/
    static const char           DOS_SEPARATOR       /* = '\\' */;

    /**
     * \brief   CEFile::PATH_SEPARATOR
     *          The file path separator
     **/
    static const char           PATH_SEPARATOR      /* OS specific */;

    /**
     * \brief   CEFile::MAXIMUM_PATH
     *          The maximum path of file.
     **/
    static const int            MAXIMUM_PATH        /* OS specific */;

    /**
     * \brief   CEFile::INVALID_HANDLE
     *          Invalid file handle
     **/
    static FILEHANDLE const     INVALID_HANDLE       /* OS specific */;

//////////////////////////////////////////////////////////////////////////
// Constructors / destructor
//////////////////////////////////////////////////////////////////////////
public:
    /**
     * \brief   Default constructor
     **/
    CEFile( void );

    /**
     * \brief	Constructor to set file name / path and file open mode
     * \param	fileName	relative or absolute file path
     * \param	mode	    file open mode. 
     *                      For modes, see description in CEFileBase class 
     **/
    CEFile(const char* fileName, int mode = (CEFileBase::FO_MODE_WRITE | CEFileBase::FO_MODE_BINARY | CEFileBase::FOB_SHARE_READ));

    /**
     * \brief   Destructor
     **/
    virtual ~CEFile( void );


//////////////////////////////////////////////////////////////////////////
// Overrides
//////////////////////////////////////////////////////////////////////////
public:
/************************************************************************/
// CEFileBase class overrides
/************************************************************************/
    /**
     * \brief   Returns normalized and full path of file name.
     * \return  Returns the given name of file.
     **/
    virtual const char* GetName( void ) const;

    /**
     * \brief   Returns the file open mode (bits)
     **/
    virtual int GetMode( void ) const;

    /**
     * \brief   Opens the file. No operation can be performed before file is opened.
     *          For memory buffered file at least the file open mode should be set.
     *          For file system file object additionally the file name (or full / relative path) should be set.
     *          If mentioned parameters are not set, the open operation will fail and return false.
     *          If file object was already opened, second call to open file will fail.
     *
     * \return  Returns true if file object was opened with success.
     **/
    virtual bool Open( void );

    /**
     * \brief	Opens the file object. For memory buffered file the file name can be NULL.
     *          Any other name for memory buffered file object will have only symbolic meaning
     *          and will be ignored during open operation.
     *          For file system file object the file name should contain one of paths:
     *              a. only file name   -- means the file is located in current directory;
     *              b. relative path    -- mens relative to current directory file path;
     *              c. full path (including drive letter or network path) -- means full path to file object.
     *          For file system file object this also can be directory name.
     *
     * \param	fileName	The name or path (relative or full path) of file object. 
     *                      Can be either file object or directory.
     *
     * \param	mode	    The opening modes. The value should be combined with bitwise OR operation.
     *                      Before opening, the conflicting bits are removed.
     *                      For example, mode cannot contain (FO_MODE_ATTACH | FO_MODE_DETACH) at once.
     *                      One of bits will be ignored.
     *                      For more details see description of eFileOpenMode and eFileOpenBits.
     *
     * \return	Returns true if file was opened with success.
     **/
    virtual bool Open(const char* fileName, int mode);

    /**
     * \brief   Call to close file object.
     *          If file was opened in FO_MODE_ATTACH or FO_MODE_DETACH modes, on close the file object will not be deleted
     *          except if mode is combined with values FO_FORCE_DELETE or FO_MODE_CREATE_TEMP. Attach and Detach modes are
     *          valid and meaningful only for memory buffered file object. It has no meaning for File System file object.
     *          If FO_MODE_CREATE_TEMP is set, file object is always deleted on close.
     *          If FO_FORCE_DELETE is set, file object is deleted only for memory buffered file even if file was opened with attach mode.
     **/
    virtual void Close( void );

    /**
     * \brief	Delete opened file. This will force to delete file object even if it is attached memory buffered file
     * \return	Returns true if succeeded.
     **/
    virtual bool Remove( void );

    /**
     * \brief	Sets the file pointer position and returns current position. 
     *          The positive value of offset means move pointer forward.
     *          The negative value of offset means move pointer back.
     *          For memory buffered file the pointer cannot move more than IECursorPosition::POSITION_END.
     *
     * \param	offset	The offset in bytes to move. Positive value means moving forward. Negative value means moving back.
     * \param	startAt	Specifies the starting position of pointer and should have one of values:
     *                  IECursorPosition::POSITION_BEGIN   -- position from beginning of file
     *                  IECursorPosition::POSITION_CURRENT -- from current pointer position
     *                  IECursorPosition::POSITION_END     -- from end of file
     *
     * \return	If succeeds, returns the current position of pointer in bytes or value INVALID_POINTER_POSITION if fails.
     **/
    virtual unsigned int SetPosition(int offset, IECursorPosition::eCursorPosition startAt) const;

    /**
     * \brief	If succeeds, returns the current position of pointer in bytes or value INVALID_POINTER_POSITION if fails.
     *          Before calling function, the file object should be opened.
     * \return	If succeeds, returns the current position of pointer in bytes or value INVALID_POINTER_POSITION if fails.
     **/
    virtual unsigned int GetPosition( void ) const;

    /**
     * \brief	If succeeds, returns the current valid length of file data. otherwise returns INVALID_SIZE value.
     **/
    virtual unsigned int GetLength( void ) const;

    /**
     * \brief   Returns the current open status of file object. If file is opened, returns true
     **/
    virtual bool IsOpened() const;

    /**
     * \brief	Call to reserve space or set new size of file object and returns the current position of pointer.
     *          If new size is positive value, the size will be increased. Otherwise the reserved size will set size zero.
     *          If new size is more than the current size of file object, no data will be lost, no pointer position will be changed.
     *          If new size is less than the current size of file object, data will be truncated until the new size, 
     *          and if the new size is less than the current pointer position, the pointer will be move at the end of file.
     * 
     * \param	size	New Size is bytes to reserve or set. Positive value will set the size. Negative value will set size zero.
     *
     * \return  If succeeds, returns the current position of file pointer. Otherwise it returns value INVALID_POINTER_POSITION.
     **/
    virtual unsigned int Reserve(int newSize);

    /**
     * \brief   Purge file object data, sets the size zero and if succeeds, return true
     **/
    virtual bool PurgeData( void );

/************************************************************************/
// IEInStream interface overrides
/************************************************************************/
    /**
     * \brief	Read data from input stream object, copies into given buffer and
     *          returns the size of copied data. In this class it reads data from
     *          opened file.
     * \param	buffer	The pointer to buffer to copy data from input object
     * \param	size	The size in bytes of available buffer
     * \return	Returns the size in bytes of copied data
     **/
    virtual unsigned int Read( unsigned char* buffer, unsigned int size ) const;

    /**
     * \brief   Reads data from input stream object, copies into give Byte Buffer object
     *          and returns the size of copied data. Overwrite this method if copy behavior
     *          changed for certain buffer. For other buffers it should have simple behavior
     *          as copying to raw buffer
     * \param   buffer  The instance of Byte Buffer object to stream data from Input Stream object
     * \return	Returns the size in bytes of copied data
     **/
    virtual unsigned int Read( IEByteBuffer & buffer ) const;

    /**
     * \brief   Reads string data from Input Stream object and copies into given ASCII String.
     *          Overwrite method if need to change behavior of streaming string.
     * \param   asciiString     The buffer of ASCII String to stream data from Input Stream object.
     * \return  Returns the size in bytes of copied string data.
     **/
    virtual unsigned int Read( CEString & asciiString ) const;

    /**
     * \brief   Reads string data from Input Stream object and copies into given Wide String.
     *          Overwrite method if need to change behavior of streaming string.
     * \param   wideString      The buffer of Wide String to stream data from Input Stream object.
     * \return  Returns the size in bytes of copied string data.
     **/
    virtual unsigned int Read( CEWideString & wideString ) const;

/************************************************************************/
// IEOutStream interface overrides
/************************************************************************/
    /**
     * \brief	Write data to output stream object from given buffer
     *          and returns the size of written data. In this class 
     *          writes data into opened file.
     * \param	buffer	The pointer to buffer to read data and 
     *          copy to output stream object
     * \param	size	The size in bytes of data buffer
     * \return	Returns the size in bytes of written data
     **/
    virtual unsigned int Write( const unsigned char* buffer, unsigned int size );

    /**
     * \brief	Writes Binary data from Byte Buffer object to Output Stream object
     *          and returns the size of written data. Overwrite this method if need 
     *          to change behavior of streaming buffer.
     * \param	buffer	The instance of Byte Buffer object containing data to stream to Output Stream.
     * \return	Returns the size in bytes of written data
     **/
    virtual unsigned int Write( const IEByteBuffer & buffer );

    /**
     * \brief   Writes string data from given ASCII String object to output stream object.
     *          Overwrite method if need to change behavior of streaming string.
     * \param   asciiString     The buffer of String containing data to stream to Output Stream.
     * \return  Returns the size in bytes of copied string data.
     **/
    virtual unsigned int Write( const CEString & asciiString );

    /**
     * \brief   Writes string data from given wide-char String object to output stream object.
     *          Overwrite method if need to change behavior of streaming string.
     * \param   wideString  The buffer of String containing data to stream to Output Stream.
     * \return  Returns the size in bytes of copied string data.
     **/
    virtual unsigned int Write( const CEWideString & wideString );

    /**
     * \brief   Clears the buffers for the file and causes all buffered data 
     *          to be written to the file.
     **/
    virtual void Flush( void );

protected:
/************************************************************************/
// IEInStream interface overrides
/************************************************************************/
    /**
     * \brief	Returns size in bytes of available data that can be read, 
     *          i.e. remaining readable size.
     *          No necessarily that this size is equal to size of streamable buffer.
     *          For example, if the size of buffer is 'n' and 'x' bytes of data was
     *          already read from stream, the available readable size is 'n - x'.
     **/
    virtual unsigned int GetReadableSize( void ) const;

/************************************************************************/
// IEOutStream interface overrides
/************************************************************************/
    /**
     * \brief	Returns size in bytes of available space that can be written, 
     *          i.e. remaining writable size.
     *          No necessarily that this size is equal to size of streamable buffer.
     *          For example, if the size of buffer is 'n' and 'x' bytes of data was
     *          already written to stream, the available writable size is 'n - x'.
     **/
    virtual unsigned int GetWritableSize( void ) const;

//////////////////////////////////////////////////////////////////////////
// Static operations
//////////////////////////////////////////////////////////////////////////
public:
    /**
     * \brief	Returns the absolute path to file / folder
     * \param	filePath	Absolute or relative file / folder name
     **/
    static CEString GetFileFullPath(const char* filePath);

    /**
     * \brief   Returns the full path of directory part of file path, including backslash
     * \param   filePath    Absolute or relative file path
     **/
    static CEString GetFileDirectory(const char* filePath);

    /**
     * \brief	Returns only file name with extension of the given file path.
     *          If path is a folder, the file name is empty.
     * \param	filePath	Absolute or relative file path
     **/
    static CEString GetFileName(const char* filePath);

    /**
     * \brief   Returns only file extension of the given file path.
     *          If path is a folder, the file name is empty.
     * \param   filePath    Absolute or relative file path
     **/
    static CEString GetFileExtension(const char* filePath);

    /**
     * \brief   Returns only file name without extension of the given file path.
     *          If path is a folder, the file name is empty.
     * \param   filePath    Absolute or relative file path
     **/
    static CEString ExtractFileName(const char * filePath);

    /**
     * \brief	Delete given file. Returns true if succeeds.
     * \param	filePath	Absolute or relative file path to delete.
     **/
    static bool FileDelete(const char* filePath);

    /**
     * \brief	Creates folder. Returns true if succeeds.
     * \param	dirPath	    Absolute or relative path of directory
     **/
    static bool DirectoryCreate(const char* dirPath);

    /**
    * \brief	Deletes folder. Returns true if succeeds.
    * \param	dirPath	    Absolute or relative path of directory
     **/
    static bool DirectoryDelete(const char* dirPath);

    /**
     * \brief	Move file or folder from old location to new location
     * \param	oldPath	    Old absolute or relative path of file or folder
     * \param	newPath	    New absolute or relative path of file or folder
     **/
    static bool FileMove(const char* oldPath, const char* newPath);

    /**
     * \brief	Returns absolute path of current directory
     **/
    static CEString GetCurrentDir( void );

    /**
     * \brief	Sets current directory. Returns true if succeeds.
     * \param	dirPath	    Absolute or relative path to directory to set as current
     **/
    static bool SetCurrentDir(const char* dirPath);

    /**
     * \brief	Copies file from old location to new location. File in original location will remain unchanged
     * \param	originPath	Relative or absolute path of original file
     * \param	newPath	    Relative or absolute path to new location to copy
     * \param	copyForce	If new location already contains file:
     *                          - if true, file will be overwritten
     *                          - if false, operation will fail
     * \return	Returns true if operation succeeds.
     **/
    static bool FileCopy(const char* originPath, const char* newPath, bool copyForce);

    /**
     * \brief	Returns full path of system defined temporary folder
     **/
    static CEString GetTempFolder( void );

    /**
     * \brief	Generates temp file name in current or in temporary folder
     * \param	prefix	        The prefix that should contain temp file. If NULL TEMP_FILE_PREFIX (i.e. "_cz") will be used
     * \param	unique	        If true, the system will generate own unique name. 
     *                          Otherwise it will use tick count and will not guaranty uniqueness.
     * \param	inTempFolder	If true, generated file path will be in system defined temp folder, otherwise in current folder. 
     *                          File path in current folder will have relative path
     * \return	Absolute or relative path to temp file.
     **/
    static CEString CreateTempFileName(const char* prefix, bool unique, bool inTempFolder);
    /**
     * \brief   Generate unique temp file name in temporary folder with prefix TEMP_FILE_PREFIX ("_cz")
     **/
    static CEString CreateTempFileName();

    /**
     * \brief	Checks whether the given path is an existing directory or not.
     * \param	dirPath     The relative or absolute path of directory to check
     * \return	If directory exists, returns true. Otherwise returns false
     **/
    static bool IsDirectoryExist(const char* dirPath);

    /**
     * \brief	Checks whether the given path is an existing file or not.
     * \param	filePath    The relative or absolute path of file to check
     * \return	If file exists, returns true. Otherwise returns false
     **/
    static bool IsFileExist(const char* filePath);

    /**
     * \brief   1.  Normalizes file path, replaces current and parent folder symbols like "." or "..".
     *          2.  Generates file name setting timestamp on the name of file 
     *              timestamp in format yyyy_mm_dd_hh_mm_ss_ms keeping the file extension.
     *              If passed file name has keyword "%time%" it will replace with mentioned
     *              timestamp format.
     *          3.  Replaces special folder masks such as user home or temp.
     *
     *          Examples:
     *              a. "./dir1/dir2/..\\dir4\\file.dat" ==> converted to "<current dir>/dir1/dir4/file.dat",
     *                  where <current dir> is current working directory.
     *
     *              2.  ".logs/logs_%timestamp%.dat" ==> converted to "<current dir>/logs_yyyy_mm_dd_hh_mm_ss_ms.dat",
     *                  where <current dir> is current working directory and "yyyy_mm_dd_hh_mm_ss_ms" is a data-time format.
     *
     *              3.  "%personal%/my_file.dat" ==> converted to "<user document dir>/my_file.dat", where
     *                  <user document dir> is a special folder in user profile folder.
     *
     * \param   fileName    The file path and name to normalize.
     * \return  Returns generated unique and normalized file name.
     **/
    static CEString NormalizeFileName( const char * fileName );

    /**
     * \brief	Create Directories cascaded (nested) and returns true if succeeded or directories were already existing
     *
     * \param	dirPath	    Absolute or relative path to directory (cascaded) that should be created.
     *                      For example: if dirPath is "C:\\dir1\\dir2\\dir3\\" and "C:\\" does not contain such folders,
     *                      It will create all 3 folders.
     *                      Note, if dirPath will be "C:\\folder\\file.dat", it will assume that "file.dat" is a folder name 
     *                      and will try to create directory inside of "C:\\folder\\"
     *
     * \return	Returns true if succeeds.
     **/
    static bool CreateFoldersCascaded(const char* dirPath);

    /**
     * \brief   Returns folder location of current executable.
     **/
    static const char * GetExecutableFolder( void );

    /**
     * \brief   Returns special folder path. The type of required folder is defined in
     *          specialFolder parameter.
     * \param   specialFolder   The type of special folder in the system.
     * \return  If function succeeds, the return value is full path of special folder.
     *          Otherwise, it returns empty string.
     **/
    static CEString GetSpecialFolder(const eSpecialFolder specialFolder);

    static bool FindParent( const char * filePath, const char ** nextPos, const char * lastPos = NULL );

    static CEString GetParent( const char * filePath );

    static int SplitFilePath(const char * filePath, CEStringList & outList);

private:

    /**
     * \brief   Returns true if specified file path starts with current folder,
     *          i.e. starts with "./" or ".\". The path of name will be
     *          considered as relative to the path of executable file location.
     * \param   filePath    File path to check.
     * \param   skipSep     If true, it ignores checking path separator at the end of
     *                      "current directory". Otherwise, the filePath should contain
     *                      the path separator symbol.
     * \return  Returns true if specified file path starts with current folder
     **/
    static inline bool _nameHasCurrentFolder( const char * filePath, bool skipSep );

    /**
     * \brief   Returns true if specified file path starts with current folder,
     *          i.e. starts with "../" or "..\". The path of name will be
     *          considered as relative to the parent folder.
     * \param   filePath    File path to check.
     * \param   skipSep     If true, it ignores checking path separator at the end of
     *                      "parent directory". Otherwise, the filePath should contain
     *                      the path separator symbol.
     * \return  Returns true if specified file path starts with current folder
     **/
    static inline bool _nameHasParentFolder( const char * filePath, bool skipSep );

    /**
     * \brief   OS specific wrapper to create directory.
     * \param   dirPath     The full path to directory to create.
     *                      The parent directory must already exist.
     * \return  If succeeds, returns true.
     **/
    static bool _createFolder( const char * dirPath );

//////////////////////////////////////////////////////////////////////////
// OS specific methods
//////////////////////////////////////////////////////////////////////////
    /**
     * \brief   Free OS specific resources and close currently opened file.
     */
    void _closeFile( void );

//////////////////////////////////////////////////////////////////////////
// Member variables
//////////////////////////////////////////////////////////////////////////
protected:
    /**
     * \brief   Absolute or relative file path
     **/
    CEString                mFileName;

    /**
     * \brief   File open mode
     **/
    int                    mFileMode;

    /**
     * \brief   File System object handle.
     **/
    mutable FILEHANDLE      mFileHandle;

//////////////////////////////////////////////////////////////////////////
// Hidden / Forbidden methods
//////////////////////////////////////////////////////////////////////////
private:
    CEFile( const CEFile & /*src*/ );
    const CEFile & operator = ( const CEFile & /*src*/ );
};

//////////////////////////////////////////////////////////////////////////
// CEFile class inline functions
//////////////////////////////////////////////////////////////////////////

#endif  // AREG_BASE_CEFILE_HPP
