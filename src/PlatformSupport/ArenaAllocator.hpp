/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 2000 The Apache Software Foundation.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:  
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written 
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

#if !defined(ARENAALLOCATOR_INCLUDE_GUARD_1357924680)
#define ARENAALLOCATOR_INCLUDE_GUARD_1357924680



#include <algorithm>
#include <vector>



#include "ArenaBlock.hpp"



template<class Type>
class ArenaDeleteFunctor
{
public:

	void
	operator()(const Type*	theType) const
	{
#if defined(XALAN_CANNOT_DELETE_CONST)
		delete (Type*)theType;
#else
		delete theType;
#endif
	}
};



template<class ObjectType,
#if defined(XALAN_NO_DEFAULT_TEMPLATE_ARGUMENTS)
		 class ArenaBlockType>
#else
		 class ArenaBlockType = ArenaBlock<ObjectType> >
#endif
class ArenaAllocator
{
public:

	typedef typename ArenaBlockType::size_type	size_type;

	/*
	 * Construct an instance that will allocate blocks of the specified size.
	 *
	 * @param theBlockSize The block size.
	 */
	ArenaAllocator(size_type	theBlockSize) :
		m_blockSize(theBlockSize),
		m_blocks()
	{
	}

	virtual
	~ArenaAllocator()
	{
		reset();
	}

	/*
	 * Get size of an ArenaBlock, that is, the number
	 * of objects in each block.
	 *
	 * @return The size of the block
	 */
	size_type
	getBlockSize() const
	{
		return m_blockSize;
	}

	/*
	 * Set size of an ArenaBlock, that is, the number
	 * of objects in each block.  Only affects blocks
	 * allocated after the call.
	 *
	 * @param theSize The size of the block
	 */
	void
	setBlockSize(size_type	theSize)
	{
		m_blockSize = theSize;
	}

	/*
	 * Get the number of ArenaBlocks currently allocated.
	 *
	 * @return The number of blocks.
	 */
	size_type
	getBlockCount() const
	{
		return m_blocks.size();
	}

	/*
	 * Allocate a block of the appropriate size for an
	 * object.  Call commitAllocation() when after
	 * the object is successfully constructed.
	 *
	 * @return A pointer to a block of memory
	 */
	virtual ObjectType*
	allocateBlock()
	{
		if (m_blocks.empty() == true ||
			m_blocks.back()->blockAvailable() == false)
		{
			m_blocks.push_back(new ArenaBlockType(m_blockSize));
		}
		assert(m_blocks.empty() == false && m_blocks.back() != 0 && m_blocks.back()->blockAvailable() == true);

		return m_blocks.back()->allocateBlock();
	}

	/*
	 * Commits the allocation of the previous
	 * allocateBlock() call.
	 *
	 * @param theObject A pointer to a block of memory
	 */
	virtual void
	commitAllocation(ObjectType*	theObject)
	{
		assert(m_blocks.empty() == false && m_blocks.back()->ownsBlock(theObject) == true);

		m_blocks.back()->commitAllocation(theObject);
		assert(m_blocks.back()->ownsObject(theObject) == true);
	}

	virtual bool
	ownsObject(const ObjectType*	theObject) const
	{
		bool	fResult = false;

		// Search back for a block that may have allocated the object...
		// Note that this-> is required by template lookup rules.
		const typename ArenaBlockListType::const_reverse_iterator	theEnd = this->m_blocks.rend();

		typename ArenaBlockListType::const_reverse_iterator	i = this->m_blocks.rbegin();

		while(i != theEnd)
		{
			assert(*i != 0);

			if ((*i)->ownsObject(theObject) == true)
			{
				fResult = true;

				break;
			}
			else
			{
				++i;
			}
		}

		return fResult;
	}

	virtual void
	reset()
	{
#if !defined(XALAN_NO_NAMESPACES)
		using std::for_each;
#endif

		for_each(m_blocks.begin(),
				 m_blocks.end(),
				 ArenaDeleteFunctor<ArenaBlockType>());

		m_blocks.clear();
	}

protected:

	// data members...
#if defined(XALAN_NO_NAMESPACES)
	typedef vector<ArenaBlockType*>			ArenaBlockListType;
#else
	typedef std::vector<ArenaBlockType*>	ArenaBlockListType;
#endif

	size_type			m_blockSize;

	ArenaBlockListType	m_blocks;

private:

	// Not defined...
	ArenaAllocator(const ArenaAllocator<ObjectType, ArenaBlockType>&);

	ArenaAllocator<ObjectType, ArenaBlockType>&
	operator=(const ArenaAllocator<ObjectType, ArenaBlockType>&);
};



#endif	// !defined(ARENAALLOCATOR_INCLUDE_GUARD_1357924680)