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
#if !defined(XALAN_KEYDECLARATION_HEADER_GUARD)
#define XALAN_KEYDECLARATION_HEADER_GUARD



// Base include file.  Must be first.
#include "XSLTDefinitions.hpp"



XALAN_CPP_NAMESPACE_BEGIN



#include <xalanc/PlatformSupport/XalanLocator.hpp>



class XPath;



/**
 * Holds the attribute declarations for the "xsl:key" element.
 */
class KeyDeclaration
{
public:

	/**
	 * Construct an object containing attributes of an "xsl:key" element
	 * 
	 * @param qname name of element
	 * @param matchPattern XPath for "match" attribute
	 * @param use XPath for "use" attribute
	 */
	KeyDeclaration(
			const XalanQName&			qname,
			const XPath&				matchPattern,
			const XPath&				use,
			const XalanDOMString&		uri,
			XalanLocator::size_type		lineNumber,
			XalanLocator::size_type		columnNumber) :
		m_qname(&qname),
		m_match(&matchPattern),
		m_use(&use),
		m_uri(&uri),
		m_lineNumber(lineNumber),
		m_columnNumber(columnNumber)
	{
	}

	explicit
	KeyDeclaration() :
		m_qname(0),
		m_match(0),
		m_use(0),
		m_uri(0),
		m_lineNumber(0),
		m_columnNumber(0)
	{
	}

	KeyDeclaration(const KeyDeclaration&	theSource) :
		m_qname(theSource.m_qname),
		m_match(theSource.m_match),
		m_use(theSource.m_use)
	{
	}

	~KeyDeclaration()
	{
	}

	/**
	 * Retrieves name of element
	 * 
	 * @return name string
	 */
	const XalanQName*
	getQName() const
	{
		return m_qname;
	}

	/**
	 * Retrieves "use" XPath
	 * 
	 * @return XPath for "use" attribute
	 */
	const XPath*
	getUse() const
	{
		return m_use;
	}

	/**
	 * Retrieves "match" XPath
	 * 
	 * @return XPath for "match" attribute
	 */
	const XPath*
	getMatchPattern() const
	{
		return m_match;
	}

	/**
	 * Retrieves the URI.
	 * 
	 * @return A pointer to a URI string.
	 */
	const XalanDOMString*
	getURI() const
	{
		return m_uri;
	}

	/**
	 * Retrieves the line number where the xsl:key element occurred.
	 *
	 * @return The line number
	 */
	XalanLocator::size_type
	getLineNumber() const
	{
		return m_lineNumber;
	}

	/**
	 * Retrieves the column number where the xsl:key element occurred.
	 *
	 * @return The column number
	 */
	XalanLocator::size_type
	getColumnNumber() const
	{
		return m_columnNumber;
	}

private:

	const XalanQName*			m_qname;

	const XPath*				m_match;

	const XPath*				m_use;

	const XalanDOMString*		m_uri;

	XalanLocator::size_type		m_lineNumber;

	XalanLocator::size_type		m_columnNumber;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALAN_KEYDECLARATION_HEADER_GUARD