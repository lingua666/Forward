/*******************************************************************************
*               Copyright 2009, Marvell Technology Group Ltd.
*
* THIS CODE CONTAINS CONFIDENTIAL INFORMATION OF MARVELL. NO RIGHTS ARE GRANTED
* HEREIN UNDER ANY PATENT, MASK WORK RIGHT OR COPYRIGHT OF MARVELL OR ANY THIRD
* PARTY. MARVELL RESERVES THE RIGHT AT ITS SOLE DISCRETION TO REQUEST THAT THIS
* CODE BE IMMEDIATELY RETURNED TO MARVELL. THIS CODE IS PROVIDED "AS IS".
* MARVELL MAKES NO WARRANTIES, EXPRESS, IMPLIED OR OTHERWISE, REGARDING ITS
* ACCURACY, COMPLETENESS OR PERFORMANCE. MARVELL COMPRISES MARVELL TECHNOLOGY
* GROUP LTD. (MTGL) AND ITS SUBSIDIARIES, MARVELL INTERNATIONAL LTD. (MIL),
* MARVELL TECHNOLOGY, INC. (MTI), MARVELL SEMICONDUCTOR, INC. (MSI), MARVELL
* ASIA PTE LTD. (MAPL), MARVELL JAPAN K.K. (MJKK), GALILEO TECHNOLOGY LTD. (GTL)
* GALILEO TECHNOLOGY, INC. (GTI) AND RADLAN Computer Communications, LTD.
********************************************************************************
*/

	
/*
 ****************************************************************************
 * Include only once
 ****************************************************************************
 */
#ifndef MrvXML_Included
#define MrvXML_Included

/*
 ****************************************************************************
 * Include all necessary include files
 ****************************************************************************
 */


/*
 ****************************************************************************
 * Some common types for char set portable code
 ****************************************************************************
 */
#ifndef MRV_LPCTSTR
	#define MRV_LPCTSTR const char *
#endif /* MRV_LPCTSTR */

#ifndef MRV_LPTSTR
	#define MRV_LPTSTR char *
#endif /* MRV_LPTSTR */

#ifndef MRV_TCHAR
	#define MRV_TCHAR char
#endif /* MRV_TCHAR */

/*
 ****************************************************************************
 * List macros.
 ****************************************************************************
 */

#define ListEntry(ptr,type,member)\
	((type*)((char*)ptr-(unsigned long)(&((type*)0)->member)))

/*
 ****************************************************************************
 * Enumeration used in union to decipher what type a node is.
 ****************************************************************************
 */
typedef enum MrvXMLNodeType
{
	eNodeEmpty = 0,
	eNodeAttribute,	
	eNodeElement,
	eNodeText,
	eNodeClear

} MrvXMLNodeType;

#define  MrvSTOREOFFSETS

/*
 ****************************************************************************
 * Structure used for a node
 ****************************************************************************
 */
typedef struct MrvXMLNode
{
   /*
    *************************************************************************
    * This dictates what the node is (and consequently which pointer should
	* be used to obtain the appropriate node).
    *************************************************************************
    */
	enum MrvXMLNodeType type;

   /*
    *************************************************************************
    * Union to access the appropriate node.
    *************************************************************************
    */
	union
	{
		struct MrvXMLAttribute	*pAttrib;
		struct MrvXMLElement		*pElement;
		struct MrvXMLText		*pText;
		struct MrvXMLClear		*pClear;

	} node;

#ifdef MrvSTOREOFFSETS
	int nStringOffset;
#endif /* MrvSTOREOFFSETS */

} MrvXMLNode;

/*
 ****************************************************************************
 * Structure used to manage list of nodes
 ****************************************************************************
 */
typedef struct MrvXMLElement
{
	MRV_LPTSTR					lpszName;		/* Element name				 */
	int						nSize;			/* Num of child nodes		 */
	int						nMax;			/* Num of allocated nodes	 */
	int						nIsDeclaration;	/* Whether node is an XML	 */
											/* declaration - '<?xml ?>'	 */
	struct MrvXMLNode		*pEntries;		/* Array of child nodes		 */
	struct MrvXMLElement		*pParent;		/* Pointer to parent element */
} MrvXMLElement;

/*
 ****************************************************************************
 * Structure for XML text
 ****************************************************************************
 */
typedef struct MrvXMLText
{
	MRV_LPTSTR lpszValue;

} MrvXMLText;

/*
 ****************************************************************************
 * Structure for XML clear (unformatted) node
 ****************************************************************************
 */
typedef struct MrvXMLClear
{
	MRV_LPTSTR lpszOpenTag;
	MRV_LPTSTR lpszValue;
	MRV_LPTSTR lpszCloseTag;

} MrvXMLClear;

/*
 ****************************************************************************
 * Structure for XML attribute.
 ****************************************************************************
 */
typedef struct MrvXMLAttribute
{
	MRV_LPTSTR lpszName;
	MRV_LPTSTR lpszValue;

} MrvXMLAttribute;

/*
 ****************************************************************************
 * Enumeration for XML parse errors.
 ****************************************************************************
 */
typedef enum MrvXMLError
{
	eXMLErrorNone = 0,
	eXMLErrorEmpty,
	eXMLErrorFirstNotStartTag,
	eXMLErrorMissingTagName,
	eXMLErrorMissingEndTagName,
	eXMLErrorNoMatchingQuote,
	eXMLErrorUnmatchedEndTag,
	eXMLErrorUnexpectedToken,
	eXMLErrorInvalidTag,
	eXMLErrorNoElements

} MrvXMLError;

/*
 ****************************************************************************
 * Structure used to obtain error details if the parse fails.
 ****************************************************************************
 */
typedef struct MrvXMLResults
{
	enum MrvXMLError error;
	int				nLine;
	int				nColumn;

} MrvXMLResults;

/*
 ****************************************************************************
 * Construct/delete root element
 ****************************************************************************
 */
MrvXMLElement * MrvCreateRoot();
void MrvDeleteRoot(MrvXMLElement * pElement);

/*
 ****************************************************************************
 * Obtain error information in a string.
 ****************************************************************************
 */
MRV_LPCTSTR MrvGetError(MrvXMLError error);

/*
 ****************************************************************************
 * Parse XML string into elements.  This returns a pointer to the first 
 * element (created on the heap) if successful.  This must be deleted by 
 * first calling MrvDeleteElement() to recursively delete child nodes then 
 * calling free on the element to cleanup the heap.
 * If the function fails then 0 will be returned.  If the results pointer
 * given to the function was not 0 the error, line and column can be 
 * obtained.
 ****************************************************************************
 */
MrvXMLElement * MrvParseXML(MRV_LPCTSTR lpszXML, MrvXMLResults *pResults);

/*
 ****************************************************************************
 * Clears an element (deletes its children and the memory which belongs to 
 * it).
 ****************************************************************************
 */
void MrvDeleteElement(MrvXMLElement *pEntry);

/*
 ****************************************************************************
 * Enumerate nodes in the list returning a pointer to a node.  The index 
 * pointer should be initialised to zero initially - this will be incremented 
 * for each subsequent node that is obtained.
 * 0 will be returned when all nodes have been obtained.
 ****************************************************************************
 */
MrvXMLNode * MrvEnumNodes(MrvXMLElement *pEntry, int *pnIndex);

/*
 ****************************************************************************
 * Recursively search the tree for the required element based on the given
 * path.
 ****************************************************************************
 */
MrvXMLElement * MrvFindElement(MrvXMLElement *pHead, MRV_LPCTSTR lpszName);

/*
 ****************************************************************************
 * Search the given element for an attribute.
 ****************************************************************************
 */
MrvXMLAttribute * MrvFindAttribute(MrvXMLElement *pEntry, MRV_LPCTSTR lpszAttrib);

/*
 ****************************************************************************
 * Enumerate elements on the given element.
 ****************************************************************************
 */
MrvXMLElement * MrvEnumElements(MrvXMLElement *pEntry, int *pnIndex);

/*
 ****************************************************************************
 * Add an attribute to the element
 ****************************************************************************
 */
MrvXMLAttribute * MrvAddAttribute(MrvXMLElement *pEntry, MRV_LPTSTR lpszName, 
	MRV_LPTSTR lpszValue, int nGrowBy);

MrvXMLText * MrvAddText(MrvXMLElement *pEntry, MRV_LPTSTR lpszValue, int nGrowBy);

/*
 ****************************************************************************
 * Enumerate attributes on the given element.
 ****************************************************************************
 */
MrvXMLAttribute * MrvEnumAttributes(MrvXMLElement *pEntry, int *pnIndex);

/*
 ****************************************************************************
 * Create elements in the list based on the path, returning the final node.
 ****************************************************************************
 */
MrvXMLElement * MrvCreateElements(MrvXMLElement *pEntry, MRV_LPCTSTR lpszPath);

/*
 ****************************************************************************
 * Duplicate a string.
 ****************************************************************************
 */
MRV_LPTSTR MrvStrdup(MRV_LPCTSTR lpszData, int cbData);

/*
 ****************************************************************************
 * Create an XML string from the head element.
 * If successful this returns the XML string representation of the specified 
 * XML element node and its subnodes.  This string must be deleted with 
 * free().
 *
 * If the size pointer is not 0 then the total size of the returned string
 * NOT INCLUDING THE NULL TERMINATOR will be updated.
 ****************************************************************************
 */
MRV_LPTSTR MrvCreateXMLString(MrvXMLElement * pHead, int nFormat, int *pnSize);

int MrvCreateXMLStringR(MrvXMLElement * pEntry, MRV_LPTSTR lpszMarker, 
	int nFormat);

/*
 ****************************************************************************
 * Add clear unformatted data to the node.
 ****************************************************************************
 */
MrvXMLClear * MrvAddCData(MrvXMLElement *pEntry, MRV_LPTSTR lpszValue, int nGrowBy);


/*
 ****************************************************************************
 * Toby add new functions.
 ****************************************************************************
 */
MrvXMLElement* AddElementToXML(MrvXMLElement *pParent,MRV_LPTSTR lpszText);
MrvXMLAttribute* AddAttrToXML(MrvXMLElement *pEntry,MRV_LPTSTR lpszName,MRV_LPTSTR lpszValue);
void DeleteElementFromXML(MrvXMLElement *pEntry);
void DeleteAttrFromXML(MrvXMLAttribute *pEntry);
int MrvCountElement(MrvXMLElement *pEntry);
MrvXMLText *MrvFindOnlyText(MrvXMLElement * pEntry);
MrvXMLNode * MrvEnumPrintf(MrvXMLElement *pEntry,int *nrow);
MrvXMLElement * MrvAddElement(MrvXMLElement *pEntry, MRV_LPTSTR lpszName, 
							 int nIsDeclaration, int nGrowBy);
void MrvDeleteNode(MrvXMLNode *pEntry);



//void clearList()

#endif /* MrvXML_Included */
