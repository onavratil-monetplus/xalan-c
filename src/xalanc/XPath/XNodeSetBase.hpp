/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2003 The Apache Software Foundation.  All rights 
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
#if !defined(XNODESETBASE_HEADER_GUARD_1357924680)
#define XNODESETBASE_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



// Base class header file.
#include <xalanc/XPath/XObject.hpp>



#include <xalanc/XPath/XNodeSetResultTreeFragProxy.hpp>



XALAN_CPP_NAMESPACE_BEGIN



/**
 * Class to hold XPath return types.
 */
class XALAN_XPATH_EXPORT XNodeSetBase : public XObject
{
public:

	typedef size_t	size_type;

	virtual
	~XNodeSetBase();

	// These methods are inherited from XObject ...

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual XObject*
#else
	virtual XNodeSetBase*
#endif
	clone(void*		theAddress = 0) const = 0;

	virtual XalanDOMString
	getTypeString() const;

	virtual double
	num() const;

	virtual bool
	boolean() const;

	virtual const XalanDOMString&
	str() const;

	virtual void
	str(
			FormatterListener&	formatterListener,
			MemberFunctionPtr	function) const;

	virtual void
	str(XalanDOMString&	theBuffer) const;

	virtual double
	stringLength() const;

	virtual const XalanDocumentFragment&
	rtree() const;

	virtual const NodeRefListBase&
	nodeset() const = 0;

	virtual void
	ProcessXObjectTypeCallback(XObjectTypeCallback&		theCallbackObject);

	virtual void
	ProcessXObjectTypeCallback(XObjectTypeCallback&		theCallbackObject) const;

	virtual XalanNode*
	item(size_type	index) const = 0;

	virtual size_type
	getLength() const = 0;

protected:

	/**
	 * Create an XNodeSetBase
	 */
	XNodeSetBase();

	/**
	 * Create an XNodeSetBase from another.
	 *
	 * @param source    object to copy
	 */
	XNodeSetBase(const XNodeSetBase&	source);

	void
	clearCachedValues();

private:

	// Not implemented...
	XNodeSetBase&
	operator=(const XNodeSetBase&);

	// Data members...
	XNodeSetResultTreeFragProxy		m_proxy;

	mutable XalanDOMString			m_cachedStringValue;

	mutable double					m_cachedNumberValue;
};



XALAN_CPP_NAMESPACE_END



#endif	// XNODESETBASE_HEADER_GUARD_1357924680