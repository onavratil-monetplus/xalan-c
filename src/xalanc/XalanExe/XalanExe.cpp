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

#include <xalanc/Include/PlatformDefinitions.hpp>



#include <cstdlib>
#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <iostream.h>
#else
#include <iostream>
#endif



#if !defined(NDEBUG) && defined(_MSC_VER)
#include <crtdbg.h>
#endif



#include <xercesc/util/PlatformUtils.hpp>



#include <xalanc/Include/XalanAutoPtr.hpp>



#include <xalanc/XalanTransformer/XalanTransformer.hpp>



#if defined(_MSC_VER) && !defined(_WIN64)
#define XALAN_USE_WINDOWS_TIMING
#endif

#if defined(XALAN_USE_WINDOWS_TIMING)
#include "windows.h"
#include "largeint.h"
#else
#include <ctime>
#if defined(XALAN_STRICT_ANSI_HEADERS)
using std::clock;
using std::clock_t;
#endif
#endif



//#define XALAN_VQ_SPECIAL_TRACE
#if defined(XALAN_VQ_SPECIAL_TRACE)
#include "C:/Program Files/Rational/Quantify/pure.h"
#endif



XALAN_USING_STD(cerr)
XALAN_USING_STD(cin)
XALAN_USING_STD(cout)
XALAN_USING_STD(endl)
XALAN_USING_STD(ostream)

#if defined(XALAN_STRICT_ANSI_HEADERS)
using std::atoi;
using std::strcmp;
using std::strlen;
#endif



void
Usage()
{
	cerr << endl
		 << "Xalan version "
		 << XALAN_FULLVERSIONDOT
		 << endl
		 << "Xerces version "
		 << XERCES_FULLVERSIONDOT
		 << endl
		 << "Usage: Xalan [options] source stylesheet"
		 << endl
	     << "Options:"
		 << endl
		 << "  -a                    Use xml-stylesheet PI, not the 'stylesheet' argument"
		 << endl
		 << "  -e encoding           Force the specified encoding for the output."
		 << endl
		 << "  -i integer            Indent the specified amount."
		 << endl
		 << "  -m                    Omit the META tag in HTML output."
		 << endl
		 << "  -o filename           Write output to the specified file."
		 << endl
		 << "  -p name expression    Sets a stylesheet parameter."
		 << endl
		 << "  -t                    Diplay timing information."
		 << endl
		 << "  -u                    Disable escaping of URLs in HTML output."
		 << endl
		 << "  -v                    Validates source documents."
		 << endl
		 << "  -?                    Display this message."
		 << endl
		 << "  -                     A dash as the 'source' argument reads from stdin."
		 << endl
		 << "  -                     A dash as the 'stylesheet' argument reads from stdin."
		 << endl
		 << "                        ('-' cannot be used for both arguments.)"
		 << endl;
}



XALAN_USING_XALAN(XalanTransformer)
XALAN_USING_XALAN(XSLTInputSource)
XALAN_USING_XALAN(XSLTResultTarget)



class Params
{
public:

	Params(unsigned long	maxParams) :
		m_validate(false),
		m_useStylesheetPI(false),
		m_omitMETATag(false),
		m_noURLEscaping(false),
		m_showTiming(false),
		m_indentAmount(-1),
		m_inFileName(0),
		m_xslFileName(0),
		m_outFileName(0),
		m_encoding(0),
		m_params(),
		m_maxParams(maxParams),
		m_currentParam(0)
	{
	}

	bool
	addParam(
			const char*		name,
			const char*		expression)
	{
		if (m_currentParam == m_maxParams)
		{
			return false;
		}
		else
		{
			// Allocate memory if necessary...
			if (m_params.get() == 0)
			{
				m_params.reset(new ParamPair[m_maxParams]);
			}
			assert(m_params.get() != 0);

			m_params[m_currentParam].m_name = name;
			m_params[m_currentParam].m_expression = expression;

			++m_currentParam;

			return true;
		}
	};

	void
	setParams(XalanTransformer&		theTransformer) const
	{
		theTransformer.setUseValidation(m_validate);

		if (m_omitMETATag == true)
		{
			theTransformer.setOmitMETATag(XalanTransformer::eOmitMETATagYes);
		}

		if (m_noURLEscaping == true)
		{
			theTransformer.setEscapeURLs(XalanTransformer::eEscapeURLsNo);
		}

		if (m_indentAmount >= 0)
		{
			theTransformer.setIndent(m_indentAmount);
		}

		for(unsigned long i = 0; i < m_currentParam; ++i)
		{
			theTransformer.setStylesheetParam(
				m_params[i].m_name,
				m_params[i].m_expression);
		}
	}

	bool			m_validate;
	bool			m_useStylesheetPI;
	bool			m_omitMETATag;
	bool			m_noURLEscaping;
	bool			m_showTiming;

	int				m_indentAmount;

	const char*		m_inFileName;
	const char*		m_xslFileName;
	const char*		m_outFileName;

	const char*		m_encoding;

private:

	struct ParamPair
	{
		ParamPair() :
			m_name(0),
			m_expression(0)
		{
		}

		const char*		m_name;
		const char*		m_expression;
	};

	typedef XALAN_CPP_NAMESPACE_QUALIFIER XalanArrayAutoPtr<ParamPair>	ArrayAutoPtrType;

	ArrayAutoPtrType		m_params;

	const unsigned long		m_maxParams;
	unsigned long			m_currentParam;
};



bool
getArgs(
			int			argc,
			char*		argv[],
			Params&		params)
{
	bool fSuccess = true;

	for (int i = 1; i < argc && fSuccess == true; ++i)
	{
		if (argv[i][0] == '-' && argv[i][1] != '\0')
		{
			if (argv[i][2] != '\0')
			{
				fSuccess = false;
			}
			else if (params.m_inFileName != 0 || params.m_xslFileName != 0)
			{
				fSuccess = false;
			}
			else if (argv[i][1] == 'a') 
			{
				params.m_useStylesheetPI = true;
			}
			else if (argv[i][1] == 'e') 
			{
				++i;

				if(i < argc && argv[i][0] != '-' &&
				   strlen(argv[i]) != 0)
				{
					params.m_encoding = argv[i];
				}
				else
				{
					fSuccess = false;
				}
			}
			else if (argv[i][1] == 'i') 
			{
				++i;

				if(i < argc && argv[i][0] != '-' &&
				   strlen(argv[i]) != 0)
				{
					params.m_indentAmount = atoi(argv[i]);
				}
				else
				{
					fSuccess = false;
				}
			}
			else if (argv[i][1] == 'm') 
			{
				params.m_omitMETATag = true;
			}
			else if (argv[i][1] == 'o') 
			{
				++i;

				if(i < argc && argv[i][0] != '-' &&
				   strlen(argv[i]) != 0)
				{
					params.m_outFileName = argv[i];
				}
				else
				{
					fSuccess = false;
				}
			}
			else if (argv[i][1] == 'p') 
			{
				++i;

				if(i >= argc || argv[i][0] == '-')
				{
					fSuccess = false;
				}
				else
				{
					const char* const	name = argv[i];

					++i;

					// Don't check for '-' here, since that might
					// be a valid character in a parameter value.
					if(i >= argc)
					{
						fSuccess = false;
					}
					else
					{
						const char* const	value = argv[i];

						if (params.addParam(name, value) == false)
						{
							cerr << "Maximum numbers of stylesheets params has been exceeded!" << endl;

							fSuccess = false;
						}
					}
				}
			}
			else if (argv[i][1] == 't') 
			{
				params.m_showTiming = true;
			}
			else if (argv[i][1] == 'u') 
			{
				params.m_noURLEscaping = true;
			}
			else if (argv[i][1] == 'v')
			{
				params.m_validate = true;
			}
			else
			{
				fSuccess = false;
			}
		}
		else if (params.m_inFileName == 0 &&
				 strlen(argv[i]) != 0)
		{
			params.m_inFileName = argv[i];

			if (strlen(params.m_inFileName) == 0)
			{
				fSuccess = false;
			}
		}
		else if (params.m_xslFileName == 0 &&
				 strlen(argv[i]) != 0 &&
				 params.m_useStylesheetPI == false)
		{
			params.m_xslFileName = argv[i];

			if (strlen(params.m_xslFileName) == 0)
			{
				fSuccess = false;
			}
		}
		else
		{
			fSuccess = false;
		}
	}

	if (fSuccess == true && params.m_inFileName == 0)
	{
		return false;
	}
	else if (params.m_xslFileName == 0)
	{
		return params.m_useStylesheetPI;
	}
	else if (strcmp(params.m_xslFileName, params.m_inFileName) == 0)
	{
		return false;
	}
	else
	{
		return fSuccess;
	}
}



#if defined(XALAN_USE_WINDOWS_TIMING)
typedef LARGE_INTEGER	ClockType;
#else
#if defined(XALAN_STRICT_ANSI_HEADERS)
typedef std::clock_t	ClockType;
#else
typedef clock_t			ClockType;
#endif
#endif

typedef ostream			OstreamType;


inline ClockType
getClock()
{
#if defined(XALAN_USE_WINDOWS_TIMING)
	ClockType	theResult;

	QueryPerformanceCounter(&theResult);

	return theResult;
#else
#if defined(XALAN_STRICT_ANSI_HEADERS)
	return std::clock();
#else
	return clock();
#endif
#endif
}



#if defined(XALAN_USE_WINDOWS_TIMING)
inline ClockType
getPerformanceFrequencyInMilliseconds()
{
	ClockType	theInterval;

	ULONG		theDummy;

	QueryPerformanceFrequency(&theInterval);

	return ExtendedLargeIntegerDivide(theInterval, 1000UL, &theDummy);
}
#endif



void
writeElapsedMilliseconds(
			ClockType		theStartClock,
			ClockType		theEndClock,
			OstreamType&	theStream)
{
#if defined(XALAN_USE_WINDOWS_TIMING)
	static const ClockType	theInterval = getPerformanceFrequencyInMilliseconds();

	char		theBuffer[1000];

	const ClockType		theDiff = LargeIntegerSubtract(theEndClock, theStartClock);

	ClockType	theRemainder;

	const ClockType		theResult = LargeIntegerDivide(theDiff, theInterval, &theRemainder);

	sprintf(theBuffer, "%I64d.%I64d", theResult, theRemainder);

	theStream << theBuffer;
#else
	theStream << (double(theEndClock - theStartClock) / CLOCKS_PER_SEC) * 1000.0;
#endif
}



inline void
reportElapsedMilliseconds(
			const char*		theString,
			ClockType		theStartClock,
			ClockType		theEndClock,
			OstreamType&	theStream)
{
	theStream << theString;

	writeElapsedMilliseconds(theStartClock, theEndClock, theStream);

	theStream << " milliseconds.\n";
}



inline int
transform(
			XalanTransformer&			theTransformer,
			const Params&				theParams,
			const XSLTInputSource&		theSource,
			const XSLTResultTarget&		theTarget)
{
	if (theParams.m_showTiming == false)
	{
		return theTransformer.transform(
					theSource,
					theTarget);
	}
	else
	{
		XALAN_USING_XALAN(XalanParsedSource)

		ClockType	theStartClock = getClock();

		const XalanParsedSource*	theParsedSource = 0;

		int	theResult = theTransformer.parseSource(theSource, theParsedSource);

		if (theResult == 0)
		{
			ClockType		theEndClock = getClock();

			reportElapsedMilliseconds(
				"Source tree parsing time: ",
				theStartClock,
				theEndClock,
				cerr);

			const XalanTransformer::EnsureDestroyParsedSource	theGuard(theTransformer, theParsedSource);

			theStartClock = getClock();

			theResult = theTransformer.transform(*theParsedSource, theTarget);

			theEndClock = getClock();

			reportElapsedMilliseconds(
				"Transformation time, including stylesheet compilation: ",
				theStartClock,
				theEndClock,
				cerr);
		}

		return theResult;
	}
}



inline int
transform(
			XalanTransformer&			theTransformer,
			const Params&				theParams,
			const XSLTInputSource&		theSource,
			const XSLTInputSource&		theStylesheetSource,
			const XSLTResultTarget&		theTarget)
{
	if (theParams.m_showTiming == false)
	{
		return theTransformer.transform(
				theSource,
				theStylesheetSource,
				theTarget);
	}
	else
	{
		XALAN_USING_XALAN(XalanParsedSource)

		ClockType	theStartClock = getClock();

		const XalanParsedSource*	theParsedSource = 0;

		int		theResult = theTransformer.parseSource(theSource, theParsedSource);

		if (theResult == 0)
		{
			ClockType	theEndClock = getClock();

			reportElapsedMilliseconds(
				"Source tree parsing time: ",
				theStartClock,
				theEndClock,
				cerr);

			const XalanTransformer::EnsureDestroyParsedSource	theSourceGuard(theTransformer, theParsedSource);

			XALAN_USING_XALAN(XalanCompiledStylesheet)

			const XalanCompiledStylesheet*	theCompiledStylesheet = 0;

			theStartClock = getClock();

			theResult = theTransformer.compileStylesheet(theStylesheetSource, theCompiledStylesheet);

			if (theResult == 0)
			{
				theEndClock = getClock();

				reportElapsedMilliseconds(
					"Stylesheet compilation time: ",
					theStartClock,
					theEndClock,
					cerr);

				assert(theCompiledStylesheet != 0);

				const XalanTransformer::EnsureDestroyCompiledStylesheet		theStylesheetGuard(theTransformer, theCompiledStylesheet);

				theStartClock = getClock();

				theResult = theTransformer.transform(*theParsedSource, theCompiledStylesheet, theTarget);

				theEndClock = getClock();

				reportElapsedMilliseconds(
					"Transformation time: ",
					theStartClock,
					theEndClock,
					cerr);
			}
		}

		return theResult;
	}
}



inline int
transform(
			XalanTransformer&		theTransformer,
			const Params&			theParams,
			const XSLTInputSource&	theSource,
			const XSLTInputSource&	theStylesheetSource)
{
	XALAN_USING_XALAN(XalanDOMString)
	XALAN_USING_XALAN(XSLTResultTarget)

	XSLTResultTarget	theTarget;

	if (theParams.m_encoding != 0)
	{
		theTarget.setEncoding(XalanDOMString(theParams.m_encoding));
	}

	if (theParams.m_outFileName != 0)
	{
		theTarget.setFileName(theParams.m_outFileName);
	}
	else
	{
		theTarget.setByteStream(&cout);
	}

	if (theParams.m_useStylesheetPI == true)
	{
		return transform(theTransformer, theParams, theSource, theTarget);
	}
	else
	{
		return transform(theTransformer, theParams, theSource, theStylesheetSource, theTarget);
	}
}



inline int
transform(
			XalanTransformer&		theTransformer,
			const Params&			theParams,
			const XSLTInputSource&	theSource)
{
	assert(theParams.m_useStylesheetPI == true || theParams.m_xslFileName != 0);

	if (theParams.m_useStylesheetPI == true ||
	    (theParams.m_xslFileName[0] == '-' &&
	     theParams.m_xslFileName[1] == '\0'))
	{
		return transform(
				theTransformer,
				theParams,
				theSource,
				&cin);
	}
	else
	{
		return transform(
				theTransformer,
				theParams,
				theSource,
				XSLTInputSource(theParams.m_xslFileName));
	}
}



inline int
transform(
			XalanTransformer&	theTransformer,
			const Params&		theParams)
{
	assert(theParams.m_inFileName != 0);

	if (theParams.m_inFileName[0] == '-' &&
		theParams.m_inFileName[1] == '\0')
	{
		return transform(theTransformer, theParams, &cin);
	}
	else
	{
		return transform(theTransformer, theParams, theParams.m_inFileName);
	}
}



int
xsltMain(
			int		argc,
			char*	argv[])

{
	int	theResult = -1;

	// Set the maximum number of params as half of argc - 1.
	// It's actually argc - 2, but that could get us into negative
	// numbers, so don't bother.  Also, always add 1, in case
	// (argc - 1) / 2 is 0.
	Params	theParams((argc - 1) / 2 + 1);

	if (getArgs(argc, argv, theParams) == false)
	{
		Usage();
	}
	else
	{
		XALAN_USING_XERCES(XMLPlatformUtils)

		// Call the static initializer for Xerces...
		XMLPlatformUtils::Initialize();

		// Initialize Xalan...
		XalanTransformer::initialize();

		{
			// Create a XalanTransformer instance...
			XalanTransformer	theTransformer;

			// Set any options...
			theParams.setParams(theTransformer);

			theResult = transform(theTransformer, theParams);

			if (theResult != 0)
			{
				cerr << theTransformer.getLastError() << endl;
			}
		}

		// Terminate Xalan...
		XalanTransformer::terminate();

		// Terminate Xerces...
		XMLPlatformUtils::Terminate();

		// Clean up the ICU, if it's integrated...
		XalanTransformer::ICUCleanUp();
	}

	return theResult;
}



int
main(
			int		argc,
			char*	argv[])
{
#if !defined(NDEBUG) && defined(_MSC_VER)
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);

	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
#endif

#if defined(XALAN_VQ_SPECIAL_TRACE)
	QuantifyStopRecordingData();
	QuantifyClearData();
#endif

    if (argc < 2)
	{
		Usage();

		return -1;
	}
	else
	{
		return xsltMain(argc, argv);
	}
}