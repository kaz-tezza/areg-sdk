#ifndef AREG_BASE_TEHASHMAP_HPP
#define AREG_BASE_TEHASHMAP_HPP
/************************************************************************
 * \file        areg/src/base/TEHashMap.hpp
 * \ingroup     AREG SDK, Asynchronous Event Generator Software Development Kit 
 * \author      Artak Avetyan (mailto:artak@aregtech.com)
 * \brief       AREG Platform, Hash Map class template.
 *              Hash Map object to store elements by its hash value.
 *              Every element is accessed by unique Key value.
 *
 ************************************************************************/
/************************************************************************
 * Include files.
 ************************************************************************/
#include "areg/src/base/GEGlobal.h"
#include "areg/src/base/ETemplateBase.hpp"
#include "areg/src/base/TEPair.hpp"
#include "areg/src/base/IEIOStream.hpp"
#include "areg/src/base/NEMemory.hpp"

//////////////////////////////////////////////////////////////////////////
// TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement> class template declaration
//////////////////////////////////////////////////////////////////////////

/**
 * \brief       Hash Map class template declaration. Derives private
 *              class CETemplateConstants to have defined constants.
 *              The Hash Map binds Value with its Key. The Key element
 *              is unique. Values are accessed by Key. Key and Value
 *              can be of different types.
 *
 * \details	    Every element is stored in Block containing Key, Value
 *              Hash value and pointer to next Block within Hash Table.
 *              The Hash value is calculated on Key value. By hash value
 *              elements are accessed in Hash Table. This makes accessing
 *              elements faster. Every element also has Position, which
 *              makes accessing elements faster. For better performance,
 *              if large number of element is going to be saved in Hash
 *              Map, it is recommended to increase the size of Hash Table.
 *              By default, the size of Hash Table is
 *              CETemplateConstants::MAP_DEFAULT_HASH_SIZE. The maximum
 *              size of Hash Table is CETemplateConstants::MAP_MAX_TABLE_SIZE.
 *              To access first element in Hash Map, get Start Position,
 *              which is called by function GetStartPosition()
 *              The type KEY should be possible to convert to
 *              unsigned int type. This is required to calculate Hash Value.
 *              KEY and VALUE types should have at least default constructor
 *              and valid assigning operator. By default KEY_TYPE is equal
 *              to type KEY and VALUE_TYPE is equal to type VALUE.
 *              If a Key element has different logic to calculate
 *              Hash value, make own implementation of base class and pass
 *              it in type parameter list.
 *              The HashMap object is not thread safe and data access 
 *              should be  synchronized manually.
 *
 * \tparam  KEY         The type of Key to identify values in hash map.
 *                      Either should be primitive or should have at least
 *                      default constructor, initialization or copy 
 *                      constructor depending KEY and KEY_TYPE types.
 *                      Should be possible to convert to unsigned int
 *                      type to calculate hash value.
 * \tparam  VALUE       The type of stored items. Either should be 
 *                      primitive or should have default constructor 
 *                      and valid assigning operator. Also, should be 
 *                      possible to convert to type VALUE_TYPE.
 * \tparam  KEY_TYPE    The type of Key. By default is same as KEY.
 *                      If different, should be possible to convert 
 *                      and assign to KEY type.
 * \tparam  VALUE_TYPE  By default same as VALUE, but can be any other
 *                      type, which is converted and assign to VALUE type.
 * \tparam  Implement   The class that contains methods to get hash key value,
 *                      compare keys and values of the map. Pass own implementation
 *                      if default methods needs to be changed.
 **/

template < typename KEY, typename VALUE, typename KEY_TYPE = KEY, typename VALUE_TYPE = VALUE, class Implement = TEHashMapImpl<VALUE_TYPE, KEY_TYPE> >
class TEHashMap     : private   CETemplateConstants
                    , protected Implement
{
//////////////////////////////////////////////////////////////////////////
// Internal objects and types declaration
//////////////////////////////////////////////////////////////////////////
public:
    /**
     * \brief   Hidden / Protected class declaration.
     *          The Block class contains information
     *          of element in Hash Map -- value, pointers to next element,
     *          key value and hash value of Block object.
     **/
    //////////////////////////////////////////////////////////////////////////
    // TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::CEBlock class declaration
    //////////////////////////////////////////////////////////////////////////
    class CEBlock   : public TEPair<KEY, VALUE, KEY_TYPE, VALUE_TYPE>
    {
    //////////////////////////////////////////////////////////////////////////
    // constructor / destructor
    //////////////////////////////////////////////////////////////////////////
    public:
        /**
         * \brief   Default constructor
         **/
        CEBlock( void );
        /**
         * \brief   Initialization constructor.
         **/
        CEBlock(unsigned int hash, KEY_TYPE key, VALUE_TYPE value);
        /**
         * \brief   Destructor.
         **/
        ~CEBlock();

    //////////////////////////////////////////////////////////////////////////
    // member variables
    //////////////////////////////////////////////////////////////////////////
    public:
        /**
         * \brief   Pointer to next block in hash map
         **/
        CEBlock *       mNext;
        /**
         * \brief   Hash value of element, 
         *          it cannot be different for every element key, as well as it identifies
         *          the index in Hash Table, which value cannot be more than the Maximum Hash Size
         *          of Hash Table. The maximum size is no more than MAP_MAX_TABLE_SIZE (1024)
         **/
        unsigned int    mHash;
    };

//////////////////////////////////////////////////////////////////////////
// Constructor / Destructor
//////////////////////////////////////////////////////////////////////////
public:
    /**
     * \brief   Default constructor. 
     *          By default, the block size is MAP_DEFAULT_BLOCK_SIZE (48),
     *          Hash Table size is MAP_DEFAULT_HASH_SIZE (40)
     **/
    TEHashMap( void );

    /**
     * \brief	Constructor, initialization
     * \param	blockSize	The size of blocks in hash map to create at once.
     *                      If this is negative value, it creates MAP_DEFAULT_BLOCK_SIZE blocks.
     *                      It cannot be more than MAP_MAX_BLOCK_SIZE (1024)
     * \param	hashSize	The size of has map table. 
     *                      If it is negative, the size is MAP_DEFAULT_HASH_SIZE (64).
     *                      It cannot be more than MAP_MAX_TABLE_SIZE (1024)
     **/
    TEHashMap( int blockSize, int hashSize );

    /**
     * \brief   Copy constructor.
     * \param   src     The source to copy data.
     **/
    TEHashMap( const TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement> & src );

    /**
     * \brief   Destructor
     **/
    virtual ~TEHashMap( void );

//////////////////////////////////////////////////////////////////////////
// Operators
//////////////////////////////////////////////////////////////////////////
public:
/************************************************************************/
// Basic operators
/************************************************************************/

    /**
     * \brief	Assigning operator. It copies all elements from source map
     **/
    const TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>& operator = ( const TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement> & src );

    /**
     * \brief   Checks equality of 2 hash-map objects, and returns true if they are equal.
     *          There should be possible to compare KEY and VALUE types of hash map.
     * \param   other   The hash-map object to compare
     **/
    bool operator == ( const TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement> & other ) const;

    /**
     * \brief   Checks inequality of 2 hash-map objects, and returns true if they are not equal.
     *          There should be possible to compare KEY and VALUE types of hash map.
     * \param   other   The hash-map object to compare
     **/
    bool operator != ( const TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement> & other ) const;

    /**
     * \brief	Subscript operator. Returns reference to value of element by given key.
     *          If the key does not exist, inserts an element into Hash Map with specified key value. 
     *          Used on the left of an assignment statement.
     **/
    VALUE & operator [] ( KEY_TYPE Key );

    /**
     * \brief	Subscript operator. Returns value of element by given key.
     *          If the key does not exist, inserts an element into Hash Map with specified key value.
     *          Modification of value is not possible.
     **/
    inline VALUE_TYPE operator [] ( KEY_TYPE Key ) const;

/************************************************************************/
// Friend global operators to make Hash Map streamable
/************************************************************************/

    /**
     * \brief   Reads out from the stream Hash Map Key and Value pair Elements and saves in Hash Map.
     *          If Hash Map previously had Elements, they will be lost.
     *          The Elements in the Hash Map will be initialized in the same sequence
     *          as they were written.
     *          There should be possibility to initialize Value and Key pairs from streaming object and
     *          if KEY and VALUE are not primitives, but objects, they should have implemented streaming operator.
     * \param   stream  The streaming object for reading values
     * \param   input   The Hash Map object to save initialized values.
     **/
     template <typename K, typename V, typename KT, typename VT>
    friend const IEInStream & operator >> ( const IEInStream & stream, TEHashMap<K, V, KT, VT> & input);
    /**
     * \brief   Writes to the stream Hash Map values.
     *          The Elements of Hash Map will be written to the stream starting from start position.
     *          There should be possibility to stream every Element of Hash Map and if KEY and VALUE 
     *          are not primitives, but objects, they should have implemented streaming operator.
     * \param   stream  The streaming object to write values
     * \param   input   The Hash Map object to read out values.
     **/
     template <typename K, typename V, typename KT, typename VT>
    friend IEOutStream & operator << ( IEOutStream & stream, const TEHashMap<K, V, KT, VT> & output );

//////////////////////////////////////////////////////////////////////////
// Operations
//////////////////////////////////////////////////////////////////////////
public:
/************************************************************************/
// Attributes
/************************************************************************/

    /**
     * \brief	Returns the size of Hash Map
     **/
    inline int GetSize( void ) const;

    /**
     * \brief	Returns the size of hash table
     **/
    inline int GetHashTableSize( void ) const;

    /**
     * \brief	Returns true if Hash Map is empty
     **/
    inline bool IsEmpty( void ) const;

    /**
     * \brief	If Hash Map is not empty, returns the starting position (0xFFFFFFFF).
     *          Otherwise returns NULL
     **/
    inline MAPPOS GetStartPosition( void ) const;


/************************************************************************/
// Operations
/************************************************************************/

    /**
     * \brief	Searches element by given key. 
     *          If found element, return true and on exit returns the value of element
     * \param	Key	        Key to search.
     * \param	out_Value   On output, contains value of found element
     * \return	Returns true if finds element with specified key.
     **/
    inline bool Find( KEY_TYPE Key, VALUE & out_Value ) const;

    /**
     * \brief	Search an element by given key and returns position.
     *          If could not find element, returns NULL (INVALID_POSITION)
     * \param	Key	    Key to search.
     * \return	If finds, return position in Hash Map, otherwise returns NULL (INVALID_POSITION)
     **/
    inline MAPPOS Find( KEY_TYPE Key ) const;

    /**
     * \brief	Update existing element value or inserts new element in the Hash Map.
     *          If searchBeforeInsert is true, it will search for element first, 
     *          and if finds, it will update existing. Otherwise, it inserts new element.
     *          if searchBeforeInsert is false, it will insert new element without searching
     *          and no uniqueness of key values in hash map can be guarantied.
     * \param	Key	                The key of element to search or to create
     * \param	newValue	        The value of element to set or insert
     * \param	searchBeforeInsert	If true, it will search before creating new element and if finds
     *                              update value of existing element. Otherwise create new element
     *                              without checking uniqueness of keys in hash map
     * \return  Returns position of updated or new inserted element.
     **/
    MAPPOS SetKey( KEY_TYPE Key, VALUE_TYPE newValue, bool searchBeforeInsert = true );
    /**
     * \brief	Update existing element value or inserts new element in the Hash Map.
     *          If searchBeforeInsert is true, it will search for element first, 
     *          and if finds, it will update existing. Otherwise, it inserts new element.
     *          if searchBeforeInsert is false, it will insert new element without searching
     *          and no uniqueness of key values in hash map can be guarantied.
     * \param	newElement	        The Key and Value pair of element to set or insert
     * \param	searchBeforeInsert	If true, it will search before creating new element and if finds
     *                              update value of existing element. Otherwise create new element
     *                              without checking uniqueness of keys in hash map
     * \return  Returns position of updated or new inserted element.
     **/
    inline MAPPOS SetKey( const TEPair<KEY, VALUE, KEY_TYPE, VALUE_TYPE> & newElement, bool searchBeforeInsert = true );

    /**
     * \brief   Updates Existing Key and returns the position in the map.
     *          If Key does not exit, no new key will be set and function
     *          will return NULL.
     * \param   Key         The key of map to update. The function will update
     *                      only existing key. 
     * \param   newValue    New value to set on existing key.
     * \return  Returns non-NULL position value of existing key.
     *          And returns NULL if key does not exit.
     **/
    MAPPOS UpdateKey( KEY_TYPE Key, VALUE_TYPE newValue );

    /**
     * \brief	Remove existing key and if key exists, returns true.
     * \param	Key	    The Key to search and remove
     * \return	Return true if key successfully removed from hash map
     **/
    bool RemoveKey( KEY_TYPE Key );

    /**
     * \brief	Remove existing key and if succeeded, on output contains value of removed element
     * \param	Key	            The key to remove from Hash Map
     * \param	out_Value       If removed with success, on output it contains value of removed element
     * \return	Return true if successfully removed key. If key does not exist, return false.
     **/
    bool RemoveKey( KEY_TYPE Key, VALUE & out_Value );

    /**
     * \brief	Update value of an element by given position and return position to next element
     * \param	atPosition      Position of element to update value
     * \param	newValue	    New value to set for existing element.
     * \return  Returns position of next element. If given position was position of last valid element, it will return NULL
     **/
    MAPPOS SetAt( MAPPOS atPosition, VALUE_TYPE newValue );

    /**
     * \brief	Removes element by given position, and on output key and value 
     *          parameters contain value of remove element.
     *          The function returns next position of element in hash map.
     * \param	curPos      This should contain valid position of hash map.
     * \param	out_Key     On output, this contains key of removed element
     * \param	out_Value   On output, this contains value of removed element.
     * \return  Next position in hash map or NULL if reached the end.
     **/
    MAPPOS RemoveAt( MAPPOS curPos, KEY & out_Key, VALUE & out_Value );

    /**
     * \brief	Removes element by given valid position and returns value of removed element
     * \param	atPosition	Valid position of element in hash map
     * \return	Value of removed element.
     **/
    VALUE RemoveAt( MAPPOS atPosition );

    /**
     * \brief   Removes all elements in hash map, sets size zero
     **/
    void RemoveAll( void );

    /**
     * \brief	By given position value, retrieves key and value of element, and returns next position
     * \param	atPosition  Starting Position to get next position of element, and retrieve value and key
     * \param	out_Key     On output, this contains key of given position
     * \param	out_Value   On output, this contains value of given position
     * \return	Next position of element or next if it is last element in hash map.
     **/
    MAPPOS GetNextPosition( MAPPOS atPosition, KEY & out_Key, VALUE & out_Value ) const;
    /**
     * \brief	By given position value, retrieves key and value pair of element, and returns next position
     * \param	atPosition  Starting Position to get next position of element, and retrieve value and key pair
     * \param	out_Element On output, this element contains pair of Key and Value
     * \return	Next position of element or next if it is last element in hash map.
     **/
    inline MAPPOS GetNextPosition( MAPPOS atPosition, TEPair<KEY, VALUE, KEY_TYPE, VALUE_TYPE> & out_Element ) const;
    /**
     * \brief	By given position value, returns next position
     * \param	atPosition  Starting Position to get next position of element
     * \return	Next position of element or next if it is last element in hash map.
     **/
    inline MAPPOS GetNextPosition( MAPPOS atPosition ) const;
    /**
     * \brief	By given position value, returns the value of element. On return, the given position indicates next element position in the map.
     * \param	out_Position    Position to get element. On return, this value indicates value of next position.
     * \return	Return element value at given position.
     **/
    VALUE_TYPE GetElement( MAPPOS & out_Position ) const;
    /**
     * \brief   Returns reverence of element value at given valid position.
     *          If index is not valid, assertion is raised.
     **/
    inline VALUE& GetAt( const MAPPOS atPosition );
    /**
     * \brief   Returns element value at given valid position.
     *          If index is not valid, assertion is raised.
     **/
    inline VALUE_TYPE GetAt( const MAPPOS atPosition ) const;
    /**
     * \brief	Returns reference to value of element by given key.
     *          If the key does not exist, inserts an element into Hash Map with specified key value.
     **/
    VALUE & GetAt(KEY_TYPE Key);
    /**
     * \brief	Finds element by given Key and returns Value. The key should exist in hash map.
     *          If not element with give key found, assertion raises.
     * \param	Key	    The Key to search
     * \return	Value of element
     **/
    inline VALUE_TYPE GetAt( KEY_TYPE Key ) const;

    /**
     * \brief	Retrieves key and value of element by given position
     * \param	atPosition	The position of element to retrieve key and value
     * \param	out_Key	    On output, contains key of element of given position
     * \param	out_Value   On output, contains value of element of given position
     **/
    inline void GetAt( MAPPOS atPosition, KEY & out_Key, VALUE & out_Value ) const;
    /**
     * \brief	Retrieves key and value pair of element by given position
     * \param	atPosition	The position of element to retrieve key and value
     * \param	out_Element On output, contains Key and Value pair of element of given position
     **/
    inline void GetAt( MAPPOS atPosition, TEPair<KEY, VALUE, KEY_TYPE, VALUE_TYPE> & out_Element) const;

    /**
     * \brief   Returns the Key object value at the given position.
     *          The Key should be used only for reading. Avoid modifications of Key value.
     * \param   atPosition  The position to return Key object value.
     **/
    inline KEY_TYPE GetKeyAt( MAPPOS atPosition ) const;

    /**
     * \brief   Returns the Value object value at the given position.
     * \param   atPosition  The position to return Value object value.
     **/
    inline VALUE_TYPE GetValueAt( MAPPOS atPosition ) const;

    /**
     * \brief	Gets next element in hash map by given valid position and on output, 
     *          returns position, key and value of next element
     * \param	in_out_NextPosition	On input this should be valid position,
     *                              on output, this contain position to next element in hash map
     * \param	out_NextKey	        On output, this contains key of next element in hash map
     * \param	out_NextValue       On output, this contain value of next element in hash map
     * \return	Returns true, if next element was found and values on output are valid.
     **/
    bool GetNextElem( MAPPOS & in_out_NextPosition, KEY & out_NextKey, VALUE & out_NextValue ) const;

//////////////////////////////////////////////////////////////////////////
// Overrides
//////////////////////////////////////////////////////////////////////////
protected:

    /**
     * \brief	Calculates 32-bit and return the Hash Key value
     *          of the given Key object. The Hash Key value differs
     *          from Hash value stored in every element.
     *          The hash key value is used to calculate hash index
     *          of element in Hash Table.
     * \param	Key	    The key object to get hash key value
     * \return	Returns 32-bit hash key value
     **/
    inline unsigned int GetHashKey( KEY_TYPE Key ) const;

    /**
     * \brief   Called when comparing 2 keys.
     *          Overwrite method when need to change comparison.
     * \param   key1    Key on left side to compare
     * \param   key2    Key on right side to compare
     * \return  If function returns true, 2 keys are equal.
     *          Otherwise, they are not equal.
     **/
    inline bool EqualKeys( KEY_TYPE key1, KEY_TYPE key2 ) const;

    /**
     * \brief   Called when comparing 2 values of element.
     *          Overwrite method when need to change comparison.
     * \param   value1  Value on left side to compare.
     * \param   value2  Value on right side to compare.
     * \return  If function returns true, 2 values are equal.
     *          Otherwise, they are not equal.
     **/
    inline bool EqualValues( VALUE_TYPE value1, VALUE_TYPE value2) const;

//////////////////////////////////////////////////////////////////////////
// Protected / internal operations
//////////////////////////////////////////////////////////////////////////
protected:
    /**
     * \brief	Initialize the Hash Table object.
     * \param	sizeHashTable	The initial size of hash table. By default it is MAP_DEFAULT_HASH_SIZE (48)
     **/
    void InitHashTable( int sizeHashTable = CETemplateConstants::MAP_DEFAULT_HASH_SIZE );

    /**
     * \brief   Creates block list (mBlockSize elements contained in Block List)
     *          and initialize entries of elements in Block List
     **/
    void CreateBlockList( void );

    /**
     * \brief   Delete Block List entries
     **/
    void DeleteBlockList( void );

    /**
     * \brief   Free given block and place in Free List
     * \param block The block to free and link with Free List
     **/
    void FreeBlock( CEBlock * block );

    /**
     * \brief	Finds and returns pointer to block object by given Key if element by key exists.
     *          Otherwise it returns NULL.
     *          On output, 'outHash' contains hash value, which is defining index in Hash Table.
     * \param	Key	        The Key to search
     * \param	out_Hash    The Hash value of entry within Hash Table
     * \return	If found Key, returns pointer to Block element, otherwise returns NULL.
     **/
    CEBlock * GetBlockAt( KEY_TYPE Key, unsigned int & out_Hash ) const
    {
        CEBlock * result = NULL;
        out_Hash = GetHashKey(Key);

        if ( mHashTable != NULL )
        {
            for ( result  = mHashTable[out_Hash % mHashTableSize]; result != NULL; result = result->mNext)
            {
                if ( out_Hash == result->mHash && EqualKeys(result->mKey, Key) )
                    break;
            }
        }

        return result;
    }

    /**
     * \brief	Finds and returns pointer to pointer of block object by given Key if element by key exists.
     *          Otherwise it returns NULL.
     * \param	Key     The Key to search
     **/
    CEBlock ** GetBlockAt( KEY_TYPE Key ) const
    {
        CEBlock ** result = NULL;

        if ( mHashTable != NULL )
        {
            unsigned int hash = GetHashKey(Key);
            for ( result = &mHashTable[hash % mHashTableSize]; *result != NULL; result = &(*result)->mNext)
            {
                if ( hash == (*result)->mHash && EqualKeys((*result)->mKey, Key) )
                    break;
            }
        }

        return result;
    }

    /**
     * \brief   Returns Block object at the given position.
     *          If the position object is a first position, it will return first valid block.
     * \param atPosition    The position of Block
     **/
    CEBlock * GetBlockAt( MAPPOS atPosition ) const
    {
        ASSERT(atPosition	!= static_cast<MAPPOS>(NULL));
        return (atPosition == CETemplateConstants::START_POSITION ? GetFirstValidBlock() : static_cast<CEBlock *>(atPosition));
    }

    /**
     * \brief   Finds and returns reference (pointer to pointer)
     *          of given block within hash map that changes on Block
     *          have direct impact on Hash Table elements.
     *          This speeds up remove procedure.
     * \param   block   Block to find reference.
     * \return  Address of pointer of block within hash map space.
     **/
    CEBlock ** GetBlockReference( const CEBlock & block )
    {

        CEBlock ** result  = mHashTable != NULL ? &mHashTable[block.mHash % mHashTableSize] : NULL;

        for ( ; *result != NULL; result = &(*result)->mNext)
        {
            if ( block.mHash == (*result)->mHash && EqualKeys((*result)->mKey, block.mKey) )
                break;
        }
        return result;
    }

    /**
     * \brief   Gets the very first valid Block within Hash Table.
     *          Called if function was called with position parameter
     *          equal to START_POSITION (0xFFFFFFFF)
     **/
    CEBlock * GetFirstValidBlock( void ) const
    {
        CEBlock* result = NULL;
        for ( int idx = 0; (result == NULL) && (idx < mHashTableSize); ++ idx )
            result = mHashTable[idx];

        // must find something
        ASSERT(result != static_cast<CEBlock *>(NULL));
        return result;
    }

    /**
     * \brief   Gets the next valid Block within Hash Table bay given valid pointer to block object.
     * \param startAt   The pointer to Block object to start searching next entry. It must not be NULL,
     *                  otherwise assertion raised
     **/
    CEBlock * GetNextValidBlock( CEBlock * startAt ) const
    {
        ASSERT(startAt != NULL);
        ASSERT(mElemCount != 0);
        CEBlock* result = startAt->mNext;
        if (result == NULL)
        {
            for ( int idx = static_cast<int>(startAt->mHash % mHashTableSize) + 1; result == NULL && idx < mHashTableSize; ++ idx )
                result = mHashTable[idx];
        }
        
        return result;
    }

    /**
     * \brief	Initialize new Block entry taken from Free List
     * \return	Returns pointer to new Block entry
     **/
    CEBlock * InitNewBlock( void )
    {
        if (mFreeList == NULL)
            CreateBlockList();

        ASSERT(mFreeList != NULL);
        CEBlock* block= mFreeList;
        mFreeList   = mFreeList->mNext;
        mElemCount ++;
        ASSERT(mElemCount > 0);
        new(&block->mKey)	KEY;
        new(&block->mValue)VALUE;

        return block;
    }

    /**
     * \brief   Removes block in Hash Map
     * \param   block   Address of pointer of block
     *                  within Hash Map. Value returned
     *                  by GetBlockReference() function.
     **/
    void RemoveBlock( CEBlock ** block)
    {
        CEBlock* nextBlock = (*block)->mNext;
        FreeBlock(*block);
        *block = nextBlock;
        if (mElemCount == 0)
            DeleteAllBlocks();
    }

    /**
     * \brief   Deletes all blocks, reset all data.
     **/
    void DeleteAllBlocks( void )
    {
        DeleteBlockList();

        NEMemory::ZeroElements<CEBlock *>(mHashTable, mHashTableSize);
        mElemCount	= 0;
        mFreeList	= static_cast<CEBlock *>(NULL);
        mBlockList	= static_cast<CEBlock *>(NULL);
    }


//////////////////////////////////////////////////////////////////////////
// Member Variables
//////////////////////////////////////////////////////////////////////////
protected:
    /**
     * \brief   Hash Table object. 
     *          By default the Hash Table size is MAP_DEFAULT_HASH_SIZE (64)
     *          and cannot be more than MAP_MAX_TABLE_SIZE (1024)
     **/
    CEBlock **    mHashTable;
    /**
     * \brief   Size of Hash Map Table. By default it is MAP_DEFAULT_HASH_SIZE (64)
     *          and cannot be more than MAP_MAX_TABLE_SIZE (1024)
     **/
    int         mHashTableSize;

    /**
     * \brief   Current Block List. 
     *          By default the size of every block list is MAP_DEFAULT_BLOCK_SIZE (48)
     *          and cannot be more than MAP_MAX_BLOCK_SIZE (1024).
     *          Block List is created every time when List of Free Blocks is empty
     **/
    CEBlock *   mBlockList;
    /**
     * \brief   The size of single Block List. By default it is MAP_DEFAULT_BLOCK_SIZE (48)
     *          and cannot be more than MAP_MAX_BLOCK_SIZE (1024).
     **/
    int         mBlockSize;

    /**
     * \brief   Number of elements in Hash Map
     **/
    int         mElemCount;

    /**
     * \brief   List of Free Blocks
     **/
    CEBlock *   mFreeList;
};

//////////////////////////////////////////////////////////////////////////
// Function Implement
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::CEBlock class Implement
//////////////////////////////////////////////////////////////////////////

template < typename KEY, typename VALUE, typename KEY_TYPE /*= KEY*/, typename VALUE_TYPE /*= VALUE */, class Implement /* = CEHashMapBase */ >
inline TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::CEBlock::CEBlock( void )
    : TEPair<KEY, VALUE, KEY_TYPE, VALUE_TYPE>( )
    , mNext (NULL)
    , mHash (0)
{   ;   }

template < typename KEY, typename VALUE, typename KEY_TYPE /*= KEY*/, typename VALUE_TYPE /*= VALUE */, class Implement /* = CEHashMapBase */ >
inline TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::CEBlock::CEBlock(unsigned int hash, KEY_TYPE key, VALUE_TYPE value)
    : TEPair<KEY, VALUE, KEY_TYPE, VALUE_TYPE>(key, value)
    , mNext (NULL)
    , mHash (hash)
{   ;   }

template < typename KEY, typename VALUE, typename KEY_TYPE /*= KEY*/, typename VALUE_TYPE /*= VALUE */, class Implement /* = CEHashMapBase */ >
TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::CEBlock::~CEBlock( void )
{   ;   }

//////////////////////////////////////////////////////////////////////////
// TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement> class template Implement
//////////////////////////////////////////////////////////////////////////
template < typename KEY, typename VALUE, typename KEY_TYPE /*= KEY*/, typename VALUE_TYPE /*= VALUE */, class Implement /* = CEHashMapBase */ >
TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::TEHashMap( void )
    : mHashTable    (NULL)
    , mHashTableSize(0)
    , mBlockList    (NULL)
    , mBlockSize    (CETemplateConstants::MAP_DEFAULT_BLOCK_SIZE)
    , mElemCount    (0)
    , mFreeList     (NULL)
{
    InitHashTable(CETemplateConstants::MAP_DEFAULT_HASH_SIZE);
}

template < typename KEY, typename VALUE, typename KEY_TYPE /*= KEY*/, typename VALUE_TYPE /*= VALUE */, class Implement /* = CEHashMapBase */ >
TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::TEHashMap(int blockSize, int hashSize)
    : mHashTable    (NULL)
    , mHashTableSize(0)
    , mBlockList    (NULL)
    , mBlockSize    (blockSize > 0 && blockSize <= CETemplateConstants::MAP_MAX_BLOCK_SIZE ? blockSize : (blockSize < 0 ? CETemplateConstants::MAP_DEFAULT_BLOCK_SIZE : CETemplateConstants::MAP_MAX_BLOCK_SIZE))
    , mElemCount    (0)
    , mFreeList     (NULL)
{
    InitHashTable(hashSize > 0 && hashSize <= CETemplateConstants::MAP_MAX_TABLE_SIZE ? hashSize : hashSize < 0 ? CETemplateConstants::MAP_DEFAULT_HASH_SIZE : CETemplateConstants::MAP_MAX_TABLE_SIZE);
}

template < typename KEY, typename VALUE, typename KEY_TYPE /*= KEY*/, typename VALUE_TYPE /*= VALUE */, class Implement /* = CEHashMapBase */ >
TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::TEHashMap(const TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>& src)
    : mHashTable    (NULL)
    , mHashTableSize(0)
    , mBlockList    (NULL)
    , mBlockSize    (src.mBlockSize)
    , mElemCount    (0)
    , mFreeList     (NULL)
{
    InitHashTable(src.mHashTableSize);
    MAPPOS pos = src.GetStartPosition();
    KEY key;
    VALUE value;
    while (pos != CETemplateConstants::INVALID_POSITION)
    {
        pos = src.GetNextPosition(pos, key, value);
        SetKey(key, value, false);
    }
}

template < typename KEY, typename VALUE, typename KEY_TYPE /*= KEY*/, typename VALUE_TYPE /*= VALUE */, class Implement /* = CEHashMapBase */ >
TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::~TEHashMap( void )
{
    ASSERT(mHashTable!= (CEBlock**)NULL);

    RemoveAll();
    if (mHashTable != NULL) 
        delete [] mHashTable;
    mHashTable      = NULL;
    mHashTableSize  = 0;
}

template < typename KEY, typename VALUE, typename KEY_TYPE /*= KEY*/, typename VALUE_TYPE /*= VALUE */, class Implement /* = CEHashMapBase */ >
const TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>& TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::operator = ( const TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement> & src )
{
    if (static_cast<const TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement> *>(this) != static_cast<const TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement> *>(&src))
    {
        RemoveAll();
        mBlockSize  = src.mBlockSize;
        InitHashTable(src.mHashTableSize);

        MAPPOS pos = src.GetStartPosition();
        KEY key;
        VALUE value;

        while (pos != CETemplateConstants::INVALID_POSITION)
        {
            pos = src.GetNextPosition(pos, key, value);
            SetKey(key, value, false);
        }
    }
    return (*this);
}

template < typename KEY, typename VALUE, typename KEY_TYPE /*= KEY*/, typename VALUE_TYPE /*= VALUE */, class Implement /* = CEHashMapBase */ >
bool TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::operator == (const TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>& other) const
{
    bool result = true;
    if ( this != &other )
    {
        result = false;
        if ( other.mElemCount == mElemCount )
        {
            result = true;
            KEY otherKey;
            VALUE otherValue, thisValue;

            for ( MAPPOS otherPos = other.GetStartPosition( ); result && (otherPos != CETemplateConstants::INVALID_POSITION); )
            {
                otherPos = other.GetNextPosition( otherPos, otherKey, otherValue );
                result = Find( otherKey, thisValue ) ? EqualValues( otherValue, thisValue ) : false;
            }
        }
    }
    return result;
}

template < typename KEY, typename VALUE, typename KEY_TYPE /*= KEY*/, typename VALUE_TYPE /*= VALUE */, class Implement /* = CEHashMapBase */ >
bool TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::operator != ( const TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>& other ) const
{
    bool result = false;
    if (this != &other )
    {
        result = true;
        if (other.mElemCount == mElemCount)
        {
            result = false;
            KEY otherKey;
            VALUE otherValue, thisValue;

            for ( MAPPOS otherPos = other.GetStartPosition(); (result == false) && (otherPos != CETemplateConstants::INVALID_POSITION); )
            {
                otherPos = other.GetNextPosition(otherPos, otherKey, otherValue);
                result = Find(otherKey, thisValue) ? EqualValues( otherValue, thisValue ) == false : true;
            }
        }
    }
    return result;
}

template < typename KEY, typename VALUE, typename KEY_TYPE /*= KEY*/, typename VALUE_TYPE /*= VALUE */, class Implement /* = CEHashMapBase */ >
VALUE & TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::operator [] (KEY_TYPE Key)
{
    return GetAt(Key);
}

template < typename KEY, typename VALUE, typename KEY_TYPE /*= KEY*/, typename VALUE_TYPE /*= VALUE */, class Implement /* = CEHashMapBase */ >
inline VALUE_TYPE TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::operator [] ( KEY_TYPE Key ) const
{
    return GetAt(Key);
}

template < typename KEY, typename VALUE, typename KEY_TYPE /*= KEY*/, typename VALUE_TYPE /*= VALUE */, class Implement /* = CEHashMapBase */ >
inline int TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::GetSize( void ) const
{   return mElemCount;          }

template < typename KEY, typename VALUE, typename KEY_TYPE /*= KEY*/, typename VALUE_TYPE /*= VALUE */, class Implement /* = CEHashMapBase */ >
inline int TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::GetHashTableSize( void ) const
{   return mHashTableSize;                                                  }

template < typename KEY, typename VALUE, typename KEY_TYPE /*= KEY*/, typename VALUE_TYPE /*= VALUE */, class Implement /* = CEHashMapBase */ >
inline bool TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::IsEmpty( void ) const
{   return (mElemCount == 0);                                               }

template < typename KEY, typename VALUE, typename KEY_TYPE /*= KEY*/, typename VALUE_TYPE /*= VALUE */, class Implement /* = CEHashMapBase */ >
inline MAPPOS TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::GetStartPosition( void ) const
{   return (mElemCount != 0 ? CETemplateConstants::START_POSITION : NULL);  }

template < typename KEY, typename VALUE, typename KEY_TYPE /*= KEY*/, typename VALUE_TYPE /*= VALUE */, class Implement /* = CEHashMapBase */ >
inline bool TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::Find( KEY_TYPE Key, VALUE & out_Value ) const
{
    bool result     = false;
    CEBlock **block = GetBlockAt(Key);
    if ( (block != NULL) && (*block != NULL) )
    {
        out_Value   = (*block)->mValue;
        result      = true;
    }
    
    return result;
}

template < typename KEY, typename VALUE, typename KEY_TYPE /*= KEY*/, typename VALUE_TYPE /*= VALUE */, class Implement /* = CEHashMapBase */ >
inline MAPPOS TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::Find(KEY_TYPE Key) const
{
    CEBlock** result = GetBlockAt(Key);
    return (result != NULL ? static_cast<MAPPOS>(*result) : static_cast<MAPPOS>(NULL));
}

template < typename KEY, typename VALUE, typename KEY_TYPE /*= KEY*/, typename VALUE_TYPE /*= VALUE */, class Implement /* = CEHashMapBase */ >
MAPPOS TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::SetKey(KEY_TYPE Key, VALUE_TYPE newValue, bool searchBeforeInsert /*= true*/)
{
    ASSERT(mHashTable != (CEBlock**)NULL);

    unsigned int hash = CETemplateConstants::MAP_INVALID_HASH;
    CEBlock* block    = searchBeforeInsert ? GetBlockAt(Key, hash) : NULL;
    hash = (hash == CETemplateConstants::MAP_INVALID_HASH) ? GetHashKey(Key) : hash;
    if (block == static_cast<CEBlock *>(NULL))
    {
        // it doesn't exist, add a new Block
        int idx     = static_cast<int>(hash % mHashTableSize);
        block       = InitNewBlock();
        block->mHash= hash;
        block->mKey	= (KEY)Key;
        // 'block->mValue' is a constructed object, nothing more put into hash table
        block->mNext= mHashTable[idx];
        mHashTable[idx]= block;
    }

    block->mValue = (VALUE)(newValue);
    return static_cast<MAPPOS>(block);
}

template < typename KEY, typename VALUE, typename KEY_TYPE /*= KEY*/, typename VALUE_TYPE /*= VALUE */, class Implement /* = CEHashMapBase */ >
inline MAPPOS TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::SetKey(const TEPair<KEY, VALUE, KEY_TYPE, VALUE_TYPE> &newItem, bool searchBeforeInsert /*= true*/)
{
    return SetKey(newItem.mKey, newItem.mValue, searchBeforeInsert);
}

template < typename KEY, typename VALUE, typename KEY_TYPE /*= KEY*/, typename VALUE_TYPE /*= VALUE */, class Implement /* = CEHashMapBase */ >
MAPPOS TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::UpdateKey(KEY_TYPE Key, VALUE_TYPE newValue)
{
    ASSERT(mHashTable != (CEBlock**)NULL);

    unsigned int hash = CETemplateConstants::MAP_INVALID_HASH;
    CEBlock* block    = GetBlockAt(Key, hash);
    if (block != static_cast<CEBlock *>(NULL))
    {
        // Block exists, update existing key value.
        block->mValue = (VALUE)(newValue);
    }

    return static_cast<MAPPOS>(block);
}

template < typename KEY, typename VALUE, typename KEY_TYPE /*= KEY*/, typename VALUE_TYPE /*= VALUE */, class Implement /* = CEHashMapBase */ >
bool TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::RemoveKey(KEY_TYPE Key)
{
    bool result = false;
    CEBlock** block = GetBlockAt(Key);
    if ((block != NULL) && (*block != NULL))
    {
        result = true;
        RemoveBlock(block);
    }

    return result;
}

template < typename KEY, typename VALUE, typename KEY_TYPE /*= KEY*/, typename VALUE_TYPE /*= VALUE */, class Implement /* = CEHashMapBase */ >
bool TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::RemoveKey(KEY_TYPE Key, VALUE& out_Value)
{
    bool result = false;
    CEBlock** block = GetBlockAt(Key);
    if ((block != NULL) && (*block != NULL))
    {
        result      = true;
        out_Value   = (*block)->mValue;
        RemoveBlock(block);
    }

    return result;
}

template < typename KEY, typename VALUE, typename KEY_TYPE /*= KEY*/, typename VALUE_TYPE /*= VALUE */, class Implement /* = CEHashMapBase */ >
MAPPOS TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::SetAt(MAPPOS atPosition, VALUE_TYPE newValue)
{
    ASSERT(mHashTable   != static_cast<CEBlock **>(NULL));
    ASSERT(atPosition   != static_cast<MAPPOS>(NULL));

    CEBlock* block      = GetBlockAt(atPosition);
    CEBlock* nextBlock  = GetNextValidBlock(block);
    block->mValue       = (VALUE)(newValue);
    return static_cast<MAPPOS>(nextBlock);
}

template < typename KEY, typename VALUE, typename KEY_TYPE /*= KEY*/, typename VALUE_TYPE /*= VALUE */, class Implement /* = CEHashMapBase */ >
MAPPOS TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::RemoveAt(MAPPOS curPos, KEY& out_Key, VALUE& out_Value)
{
    ASSERT(mHashTable   != static_cast<CEBlock **>(NULL));
    ASSERT(curPos       != static_cast<MAPPOS>(NULL));
    MAPPOS   result     = NULL;
    CEBlock* block      = GetBlockAt(curPos);
    CEBlock* nextBlock  = GetNextValidBlock(block);
    result      = static_cast<MAPPOS>(nextBlock);
    out_Key		= block->mKey;
    out_Value	= block->mValue;

    ASSERT(block != NULL);
    RemoveBlock(GetBlockReference(*block));

    return result;
}

template < typename KEY, typename VALUE, typename KEY_TYPE /*= KEY*/, typename VALUE_TYPE /*= VALUE */, class Implement /* = CEHashMapBase */ >
VALUE TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::RemoveAt(MAPPOS atPosition)
{
    ASSERT(mHashTable   != static_cast<CEBlock **>(NULL));
    ASSERT(atPosition	!= static_cast<MAPPOS>(NULL));

    VALUE result;
    CEBlock* block = GetBlockAt(atPosition);
    result = block->mValue;
    
    ASSERT(block != NULL);
    RemoveBlock(GetBlockReference(*block));
    return result;
}

template < typename KEY, typename VALUE, typename KEY_TYPE /*= KEY*/, typename VALUE_TYPE /*= VALUE */, class Implement /* = CEHashMapBase */ >
void TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::RemoveAll( void )
{
    ASSERT(mHashTable   != static_cast<CEBlock **>(NULL));

    CEBlock	*block = NULL;
    for (int idx = 0; idx < mHashTableSize; ++ idx)
    {
        for (block = mHashTable[idx]; block != NULL; block = block->mNext)
        {
            (&block->mKey)->~KEY();
            (&block->mValue)->~VALUE();
        }
    }

    DeleteAllBlocks();
}

template < typename KEY, typename VALUE, typename KEY_TYPE /*= KEY*/, typename VALUE_TYPE /*= VALUE */, class Implement /* = CEHashMapBase */ >
MAPPOS TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::GetNextPosition(MAPPOS atPosition, KEY& out_Key, VALUE& out_Value) const
{
    ASSERT(mHashTable   != static_cast<CEBlock **>(NULL));
    ASSERT(atPosition   != static_cast<MAPPOS>(NULL));

    CEBlock* block      = GetBlockAt(atPosition);
    CEBlock* nextBlock  = GetNextValidBlock(block);
    out_Key		        = block->mKey;
    out_Value	        = block->mValue;

    return static_cast<MAPPOS>(nextBlock);
}

template < typename KEY, typename VALUE, typename KEY_TYPE /*= KEY*/, typename VALUE_TYPE /*= VALUE */, class Implement /* = CEHashMapBase */ >
inline MAPPOS TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::GetNextPosition(MAPPOS atPosition, TEPair<KEY, VALUE, KEY_TYPE, VALUE_TYPE> & out_Element) const
{
    return GetNextPosition(atPosition, out_Element.mKey, out_Element.mValue);
}

template < typename KEY, typename VALUE, typename KEY_TYPE /*= KEY*/, typename VALUE_TYPE /*= VALUE */, class Implement /* = CEHashMapBase */ >
inline MAPPOS TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::GetNextPosition( MAPPOS atPosition ) const
{
    ASSERT(mHashTable   != static_cast<CEBlock **>(NULL));
    ASSERT(atPosition   != static_cast<MAPPOS>(NULL));
    CEBlock* block      = GetBlockAt(atPosition);
    return static_cast<MAPPOS>( GetNextValidBlock(block) );
}

template < typename KEY, typename VALUE, typename KEY_TYPE /*= KEY*/, typename VALUE_TYPE /*= VALUE */, class Implement /* = CEHashMapBase */ >
VALUE_TYPE TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::GetElement( MAPPOS & out_Position ) const
{
    ASSERT( mHashTable   != static_cast<CEBlock **>(NULL) );
    ASSERT( out_Position != static_cast<MAPPOS>(NULL) );

    CEBlock* block      = GetBlockAt( out_Position );
    CEBlock* nextBlock  = GetNextValidBlock( block );
    out_Position        = static_cast<MAPPOS>(nextBlock);
    
    return block->mValue;
}

template < typename KEY, typename VALUE, typename KEY_TYPE /*= KEY*/, typename VALUE_TYPE /*= VALUE */, class Implement /* = CEHashMapBase */ >
inline VALUE& TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::GetAt( const MAPPOS atPosition )
{
    ASSERT( mHashTable != static_cast<CEBlock **>(NULL) );
    ASSERT( atPosition != static_cast<MAPPOS>(NULL) );

    CEBlock* block = GetBlockAt( atPosition );
    return block->mValue;
}

template < typename KEY, typename VALUE, typename KEY_TYPE /*= KEY*/, typename VALUE_TYPE /*= VALUE */, class Implement /* = CEHashMapBase */ >
inline VALUE_TYPE TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::GetAt( const MAPPOS atPosition ) const
{
    ASSERT( mHashTable != static_cast<CEBlock **>(NULL) );
    ASSERT( atPosition != static_cast<MAPPOS>(NULL) );

    CEBlock* block = GetBlockAt( atPosition );
    return block->mValue;
}

template < typename KEY, typename VALUE, typename KEY_TYPE /*= KEY*/, typename VALUE_TYPE /*= VALUE */, class Implement /* = CEHashMapBase */ >
VALUE& TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::GetAt( KEY_TYPE Key )
{
    ASSERT( mHashTable != static_cast<CEBlock **>(NULL) );

    unsigned int hash   = CETemplateConstants::MAP_INVALID_HASH;
    CEBlock *list = GetBlockAt( Key, hash );
    if ( list == static_cast<CEBlock *>(NULL) )
    {
        // add new list
        int idx     = static_cast<int>(hash % mHashTableSize);
        list        = InitNewBlock( );
        list->mHash = hash;
        list->mKey	= (KEY)Key;
        list->mNext	= mHashTable[idx];
        mHashTable[idx] = list;
    }

    return list->mValue;
}

template < typename KEY, typename VALUE, typename KEY_TYPE /*= KEY*/, typename VALUE_TYPE /*= VALUE */, class Implement /* = CEHashMapBase */ >
inline VALUE_TYPE TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::GetAt(KEY_TYPE Key) const
{
    CEBlock ** block  = GetBlockAt(Key);
    ASSERT((block != NULL) && (*block != NULL));
    return (*block)->mValue;
}

template < typename KEY, typename VALUE, typename KEY_TYPE /*= KEY*/, typename VALUE_TYPE /*= VALUE */, class Implement /* = CEHashMapBase */ >
inline void TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::GetAt(MAPPOS atPosition, KEY & out_Key, VALUE & out_Value) const
{
    ASSERT(mHashTable	!= static_cast<CEBlock **>(NULL));
    ASSERT(atPosition   != static_cast<MAPPOS>(NULL));

    CEBlock* block    = GetBlockAt(atPosition);
    out_Key   = block->mKey;
    out_Value= block->mValue;
}

template < typename KEY, typename VALUE, typename KEY_TYPE /*= KEY*/, typename VALUE_TYPE /*= VALUE */, class Implement /* = CEHashMapBase */ >
inline void TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::GetAt(MAPPOS atPosition, TEPair<KEY, VALUE, KEY_TYPE, VALUE_TYPE> & out_Element) const
{
    GetAt(atPosition, out_Element.mKey, out_Element.mValue);
}

template < typename KEY, typename VALUE, typename KEY_TYPE /*= KEY*/, typename VALUE_TYPE /*= VALUE */, class Implement /* = CEHashMapBase */ >
inline VALUE_TYPE TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::GetValueAt( MAPPOS atPosition ) const
{
    ASSERT(mHashTable	!= static_cast<CEBlock **>(NULL));
    ASSERT(atPosition   != static_cast<MAPPOS>(NULL));

    CEBlock* block    = GetBlockAt(atPosition);
    return block->mValue;
}

template < typename KEY, typename VALUE, typename KEY_TYPE /*= KEY*/, typename VALUE_TYPE /*= VALUE */, class Implement /* = CEHashMapBase */ >
inline KEY_TYPE TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::GetKeyAt( MAPPOS atPosition ) const
{
    ASSERT(mHashTable	!= static_cast<CEBlock **>(NULL));
    ASSERT(atPosition   != static_cast<MAPPOS>(NULL));

    CEBlock* block    = GetBlockAt(atPosition);
    return block->mKey;
}

template < typename KEY, typename VALUE, typename KEY_TYPE /*= KEY*/, typename VALUE_TYPE /*= VALUE */, class Implement /* = CEHashMapBase */ >
bool TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::GetNextElem(MAPPOS & in_out_NextPosition, KEY & out_NextKey, VALUE & out_NextValue) const
{
    ASSERT(mHashTable	        != static_cast<CEBlock **>(NULL));
    ASSERT(in_out_NextPosition  != static_cast<MAPPOS>(NULL));

    bool result         = false;
    CEBlock* block      = GetBlockAt(in_out_NextPosition);
    CEBlock* nextBlock  = GetNextValidBlock(block);
    in_out_NextPosition = static_cast<MAPPOS>(nextBlock);
    if ( nextBlock != static_cast<CEBlock *>(NULL) )
    {
        out_NextKey     = nextBlock->mKey;
        out_NextValue   = nextBlock->mValue;
        result          = true;
    }

    return result;
}

template < typename KEY, typename VALUE, typename KEY_TYPE /*= KEY*/, typename VALUE_TYPE /*= VALUE */, class Implement /* = CEHashMapBase */ >
void TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::InitHashTable(int sizeHashTable /*= CETemplateConstants::MAP_DEFAULT_HASH_SIZE*/)
{
    ASSERT(mElemCount   == 0);
    ASSERT(sizeHashTable >  0 && sizeHashTable <= CETemplateConstants::MAP_MAX_TABLE_SIZE);
    if (mHashTable != NULL)
        delete [] mHashTable;

    mHashTable	    = DEBUG_NEW CEBlock *[sizeHashTable];
    mHashTableSize	= mHashTable != NULL ? sizeHashTable : 0;
    NEMemory::ZeroElements<CEBlock *>( mHashTable, mHashTableSize);
}

template < typename KEY, typename VALUE, typename KEY_TYPE /*= KEY*/, typename VALUE_TYPE /*= VALUE */, class Implement /* = CEHashMapBase */ >
void TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::CreateBlockList( void )
{
    // add another block
    // chain them into free list
    int length              = mBlockSize * sizeof(CEBlock) + sizeof(CEBlock *);
    unsigned char* newBlock = DEBUG_NEW unsigned char[length];
    if (newBlock != NULL)
    {
        // link blocks, copy the address of last block into
        // beginning of new block (and skip address part)
        NEMemory::MemCopy(newBlock, length, &mBlockList, sizeof(CEBlock *));
        mBlockList   = reinterpret_cast<CEBlock *>(newBlock);
        newBlock    += sizeof(CEBlock *);
        CEBlock* block = reinterpret_cast<CEBlock *>(newBlock);
        block       += mBlockSize - 1;
        for (int i = mBlockSize; i > 0; -- i, -- block )
        {
            block->mNext= mFreeList;
            mFreeList   = block;
        }
    }
}

template < typename KEY, typename VALUE, typename KEY_TYPE /*= KEY*/, typename VALUE_TYPE /*= VALUE */, class Implement /* = CEHashMapBase */ >
void TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::DeleteBlockList( void )
{
    unsigned char* blocks = reinterpret_cast<unsigned char *>(mBlockList);
    while (blocks != NULL)
    {
        // copy address of next block, placed during creating blocks.
        NEMemory::MemCopy(&mBlockList, sizeof(CEBlock *), blocks, sizeof(CEBlock *));
        delete [] blocks;
        blocks = reinterpret_cast<unsigned char *>(mBlockList);
    }
}

template < typename KEY, typename VALUE, typename KEY_TYPE /*= KEY*/, typename VALUE_TYPE /*= VALUE */, class Implement /* = CEHashMapBase */ >
void TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::FreeBlock(CEBlock* block)
{
    (&block->mKey)->~KEY();
    (&block->mValue)->~VALUE();
    block->mHash = 0;
    block->mNext = mFreeList;
    mFreeList = block;
    mElemCount --;
    ASSERT(mElemCount >= 0);
}

template < typename KEY, typename VALUE, typename KEY_TYPE /*= KEY*/, typename VALUE_TYPE /*= VALUE */, class Implement /* = CEHashMapBase */ >
inline unsigned int TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::GetHashKey( KEY_TYPE Key ) const
{
    return Implement::ImplHashKey(Key);
}

template < typename KEY, typename VALUE, typename KEY_TYPE /*= KEY*/, typename VALUE_TYPE /*= VALUE */, class Implement /* = CEHashMapBase */>
inline bool TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::EqualKeys(KEY_TYPE key1, KEY_TYPE key2) const
{
    return Implement::ImplEqualKeys(key1, key2);
}

template < typename KEY, typename VALUE, typename KEY_TYPE /*= KEY*/, typename VALUE_TYPE /*= VALUE */, class Implement /* = CEHashMapBase */>
inline bool TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::EqualValues(VALUE_TYPE value1, VALUE_TYPE value2) const
{
    return Implement::ImplEqualValues(value1, value2);
}

//////////////////////////////////////////////////////////////////////////
// TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement> class friend methods
//////////////////////////////////////////////////////////////////////////

template < typename KEY, typename VALUE, typename KEY_TYPE /*= KEY*/, typename VALUE_TYPE /*= VALUE */, class Implement /* = CEHashMapBase */ >
const IEInStream & operator >> ( const IEInStream & stream, TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement> & input )
{
    int size = 0;
    stream >> size;
    for (int i = 0; i < size; ++ i)
    {
        TEPair<KEY, VALUE, KEY_TYPE, VALUE_TYPE> mapItem;
        stream >> mapItem;
        input.SetKey(mapItem, false);
    }
    return stream;
}

template < typename KEY, typename VALUE, typename KEY_TYPE /*= KEY*/, typename VALUE_TYPE /*= VALUE */, class Implement /* = CEHashMapBase */ >
IEOutStream & operator << ( IEOutStream & stream, const TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement> & output )
{
    int size = output.GetSize();
    stream << size;
    if ( size != 0 )
    {
        typename TEHashMap<KEY, VALUE, KEY_TYPE, VALUE_TYPE, Implement>::CEBlock * block = output.GetFirstValidBlock();
        for ( ; block != NULL; block = output.GetNextValidBlock(block))
            stream << static_cast<const TEPair<KEY, VALUE, KEY_TYPE, VALUE_TYPE> &>(*block);
    }

    return stream;
}

#endif  // AREG_BASE_TEHASHMAP_HPP
