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
#include "DoubleSupport.hpp"



#if !defined(XALAN_NO_STD_NUMERIC_LIMITS)
#include <limits>
#endif

#include <clocale>
#include <cmath>



#include "DOMStringHelper.hpp"
#include "XalanUnicode.hpp"



XALAN_CPP_NAMESPACE_BEGIN



#if defined(XALAN_NO_STD_NUMERIC_LIMITS)
// To circumvent an OS/390 problem
#if !defined(OS390)
#define XALAN_POSITIVE_INFINITY HUGE_VAL
#else

static const union
{
	unsigned char	c[8];
	double			d;
} theHugeVal = { { 0x7f, 0xf0, 0, 0, 0, 0, 0, 0 } };

#define XALAN_POSITIVE_INFINITY (theHugeVal.d)

#endif

#if defined(_AIX)
const DoubleSupport::NumberUnion	DoubleSupport::s_NaN = { DBL_QNAN };
#else
const DoubleSupport::NumberUnion	DoubleSupport::s_NaN = { sqrt(-2.01) };
#endif
const double	DoubleSupport::s_positiveInfinity = XALAN_POSITIVE_INFINITY;

#else

#if defined(__SGI_STL_PORT)
const DoubleSupport::NumberUnion	DoubleSupport::s_NaN = { sqrt(-2.01) };
#else
const DoubleSupport::NumberUnion	DoubleSupport::s_NaN = { std::numeric_limits<double>::quiet_NaN() };
#endif

const double	DoubleSupport::s_positiveInfinity = std::numeric_limits<double>::infinity();

#endif

const double	DoubleSupport::s_negativeInfinity = -DoubleSupport::s_positiveInfinity;

const DoubleSupport::NumberUnion	DoubleSupport::s_positiveZero = { 0.0 };

#if !defined(_AIX)
const DoubleSupport::NumberUnion	DoubleSupport::s_negativeZero = { -DoubleSupport::s_positiveZero.d };
#else
// Some compiler are overly aggressive and think that there is no such thing as -0,
// so we have to get it in a very sneaky way.
double	theDummy;

const DoubleSupport::NumberUnion	DoubleSupport::s_negativeZero = { modf(-7.0, &theDummy) };
#endif



bool
DoubleSupport::equal(
			double	theLHS,
			double	theRHS)
{
	if (isNaN(theLHS) == true || isNaN(theRHS) == true)
	{
		return false;
	}
	else
	{
		return theLHS == theRHS;
	}
}



bool
DoubleSupport::lessThan(
			double	theLHS,
			double	theRHS)
{
	if (isNaN(theLHS) == true || isNaN(theRHS) == true)
	{
		return false;
	}
	else
	{
		return theLHS < theRHS;
	}
}



bool
DoubleSupport::lessThanOrEqual(
			double	theLHS,
			double	theRHS)
{
	if (isNaN(theLHS) == true || isNaN(theRHS) == true)
	{
		return false;
	}
	else
	{
		return theLHS <= theRHS;
	}
}



bool
DoubleSupport::greaterThan(
			double	theLHS,
			double	theRHS)
{
	if (isNaN(theLHS) == true || isNaN(theRHS) == true)
	{
		return false;
	}
	else
	{
		return theLHS > theRHS;
	}
}



bool
DoubleSupport::greaterThanOrEqual(
			double	theLHS,
			double	theRHS)
{
	if (isNaN(theLHS) == true || isNaN(theRHS) == true)
	{
		return false;
	}
	else
	{
		return theLHS >= theRHS;
	}
}



double
DoubleSupport::add(
			double	theLHS,
			double	theRHS)
{
	if (isNaN(theLHS) == true)
	{
		return theLHS;
	}
	else if (isNaN(theRHS) == true)
	{
		return theRHS;
	}
	else
	{
		return theLHS + theRHS;
	}
}



double
DoubleSupport::subtract(
			double	theLHS,
			double	theRHS)
{
	if (isNaN(theLHS) == true)
	{
		return theLHS;
	}
	else if (isNaN(theRHS) == true)
	{
		return theRHS;
	}
	else
	{
		return theLHS - theRHS;
	}
}



double
DoubleSupport::multiply(
			double	theLHS,
			double	theRHS)
{
	if (isNaN(theLHS) == true)
	{
		return theLHS;
	}
	else if (isNaN(theRHS) == true)
	{
		return theRHS;
	}
	else
	{
		return theLHS * theRHS;
	}
}



double
DoubleSupport::divide(
			double	theLHS,
			double	theRHS)
{
	if (isNaN(theLHS) == true)
	{
		return theLHS;
	}
	else if (isNaN(theRHS) == true)
	{
		return theRHS;
	}
	else if (theRHS != 0.0L)
	{
		return theLHS / theRHS;
	}
	else if (theLHS == 0.0L)
	{
		// This is NaN...
		return DoubleSupport::getNaN();
	}
	else if (theLHS > 0.0L && isPositiveZero(theRHS) == true)
	{
		// This is positive infinity...
		return DoubleSupport::getPositiveInfinity();
	}
	else
	{
		// This is negative infinity...
		return DoubleSupport::getNegativeInfinity();
	}
}



double
DoubleSupport::modulus(
			double	theLHS,
			double	theRHS)
{
	if (isNaN(theLHS) == true)
	{
		return theLHS;
	}
	else if (isNaN(theRHS) == true)
	{
		return theRHS;
	}
	else if (theRHS == 0.0)
	{
		return getNaN();
	}
	else if (long(theLHS) == theLHS && long(theRHS) == theRHS)
	{
		return long(theLHS) % long(theRHS);
	}
	else
	{
		double	theDummy;

		double	theResult = divide(theLHS, theRHS);

#if defined(XALAN_STRICT_ANSI_HEADERS)
		return std::modf(theResult, &theDummy) * theRHS;
#else
		return modf(theResult, &theDummy) * theRHS;
#endif
	}
}



double
DoubleSupport::negative(double	theDouble)
{
	if (isNaN(theDouble) == true)
	{
		return getNaN();
	}
	else
	{
		return -theDouble;
	}
}



double
DoubleSupport::toDouble(const XalanDOMString&	theString)
{
	return toDouble(c_wstr(theString));
}



inline void
consumeWhitespace(const XalanDOMChar*&	theString)
{
	while(*theString != 0 &&
		  isXMLWhitespace(*theString))
	{
		++theString;
	}
}



inline void
consumeWhitespace(
			const XalanDOMChar*&		theString,
			XalanDOMString::size_type&	theLength)
{
	while(*theString != 0 &&
		  isXMLWhitespace(*theString))
	{
		++theString;
		--theLength;
	}
}



inline static void
consumeNumbers(const XalanDOMChar*&	theString)
{
	while(*theString &&
		  *theString >= XalanUnicode::charDigit_0 &&
		  *theString <= XalanUnicode::charDigit_9)
	{
			++theString;
	}
}



static bool
doValidate(
			const XalanDOMChar*		theString,
			bool&					fGotDecimalPoint)
{
	assert(theString != 0);

	bool	fError = false;
	bool	fGotDigit = false;
	bool	fGotMinus = false;
	bool	fGotWhitespace = false;

	const XalanDOMChar*		theCurrent = theString;

	// trim any whitespace
	consumeWhitespace(theCurrent);

	while(*theCurrent != 0 && fError == false)
	{
		switch(*theCurrent)
		{
		case XalanUnicode::charFullStop:
			if (fGotDecimalPoint == true ||	// can't have more than one...
				fGotWhitespace == true)	// can't have one after whitespace...
			{
				fError = true;
			}
			else
			{
				fGotDecimalPoint = true;

				++theCurrent;
			}
			break;

		case XalanUnicode::charHyphenMinus:
			if (fGotDecimalPoint == true ||
				fGotMinus == true ||
				fGotDigit == true ||
				fGotWhitespace == true)
			{
				// Error -- more than one, or in bad position.
				fError = true;
			}
			else
			{
				fGotMinus = true;

				++theCurrent;
			}
			break;

		case XalanUnicode::charDigit_0:
		case XalanUnicode::charDigit_1:
		case XalanUnicode::charDigit_2:
		case XalanUnicode::charDigit_3:
		case XalanUnicode::charDigit_4:
		case XalanUnicode::charDigit_5:
		case XalanUnicode::charDigit_6:
		case XalanUnicode::charDigit_7:
		case XalanUnicode::charDigit_8:
		case XalanUnicode::charDigit_9:
			if (fGotWhitespace == true)
			{
				fError = true;
			}
			else
			{
				fGotDigit = true;

				consumeNumbers(theCurrent);
			}
			break;

		case XalanUnicode::charSpace:
		case XalanUnicode::charCR:
		case XalanUnicode::charHTab:
		case XalanUnicode::charLF:
			if (fGotWhitespace == true)
			{
				fError = true;
			}
			else
			{
				fGotWhitespace = true;

				consumeWhitespace(theCurrent);
			}
			break;

		default:
			fError = true;
			break;
		}
	}

	// If there was no error, check to see that we got
	// at least one digit.  Otherwise, return false if
	// there was an error.
	return fError == false ? fGotDigit : false;
}



static bool
doValidate(const XalanDOMChar*	theString)
{
	bool	fDummy = false;

	return doValidate(theString, fDummy);
}



#if defined(XALAN_NON_ASCII_PLATFORM)
void
translateWideString(
			const XalanDOMChar*			theWideString,
			char*						theNarrowString,
			XalanDOMString::size_type	theStringLength,
			char						theDecimalPointCharacter)
{
	for(XalanDOMString::size_type i = 0; i < theStringLength; ++i)
	{
		switch(theWideString[i])
		{
		case XalanUnicode::charHyphenMinus:
			theNarrowString[i] = '-';
			break;

		case XalanUnicode::charFullStop:
			theNarrowString[i] = theDecimalPointCharacter;
			break;

		case XalanUnicode::charDigit_0:
			theNarrowString[i] = '0';
			break;

		case XalanUnicode::charDigit_1:
			theNarrowString[i] = '1';
			break;

		case XalanUnicode::charDigit_2:
			theNarrowString[i] = '2';
			break;

		case XalanUnicode::charDigit_3:
			theNarrowString[i] = '3';
			break;

		case XalanUnicode::charDigit_4:
			theNarrowString[i] = '4';
			break;

		case XalanUnicode::charDigit_5:
			theNarrowString[i] = '5';
			break;

		case XalanUnicode::charDigit_6:
			theNarrowString[i] = '6';
			break;

		case XalanUnicode::charDigit_7:
			theNarrowString[i] = '7';
			break;

		case XalanUnicode::charDigit_8:
			theNarrowString[i] = '8';
			break;

		case XalanUnicode::charDigit_9:
			theNarrowString[i] = '9';
			break;

		default:
			theNarrowString[i] = char(0);
			break;
		}
	}
	
	theNarrowString[theStringLength] = char(0);
}
#endif



inline double
convertHelper(
			const XalanDOMChar*		theString,
			bool					fGotDecimalPoint)
{
	// This is a big hack.  If the length of the
	// string is less than n characters, we'll convert
	// it as a long and coerce that to a double.  This
	// is _much_ cheaper...
	const XalanDOMString::size_type		theLongHackThreshold = 10;

	XalanDOMString::size_type	theLength = length(theString);

	if (fGotDecimalPoint == false && theLength < theLongHackThreshold)
	{
		return double(WideStringToLong(theString));
	}
	else
	{
#if defined(XALAN_STRICT_ANSI_HEADERS)
		const char	theDecimalPointChar = std::localeconv()->decimal_point[0];
#else
		const char	theDecimalPointChar = localeconv()->decimal_point[0];
#endif

		// trim any whitespace
		consumeWhitespace(theString, theLength);

		// Use a stack-based buffer, when possible...
		const XalanDOMString::size_type		theBufferSize = 200u;

		if (theLength < theBufferSize)
		{
			char	theBuffer[theBufferSize];

#if defined(XALAN_NON_ASCII_PLATFORM)
			translateWideString(theString, theBuffer, theLength, theDecimalPointChar);
#else
			for(XalanDOMString::size_type i = 0; i < theLength; ++i)
			{
				if (theString[i] == XalanUnicode::charFullStop)
				{
					theBuffer[i] = theDecimalPointChar;
				}
				else
				{
					theBuffer[i] = char(theString[i]);
				}
			}

			theBuffer[theLength] = '\0';
#endif

#if defined(XALAN_STRICT_ANSI_HEADERS)
			return std::atof(theBuffer);
#else
			return atof(theBuffer);
#endif
		}
		else
		{
			CharVectorType	theVector;

#if !defined(XALAN_NON_ASCII_PLATFORM)
			theVector.reserve(theLength + 1);

			CopyWideStringToVector(theString, theVector);
#else
			theVector.resize(theLength + 1, CharVectorType::value_type(0));

			translateWideString(theString, &*theVector.begin(), theLength, theDecimalPointChar);
#endif

#if defined(XALAN_STRICT_ANSI_HEADERS)
			return std::atof(&*theVector.begin());
#else
			return atof(&*theVector.begin());
#endif
		}
	}
}



double
doConvert(const XalanDOMChar*	theString)
{
	assert(theString != 0);
	assert(*theString != 0);

	bool	fGotDecimalPoint = false;

	if (doValidate(theString, fGotDecimalPoint) == false)
	{
		return DoubleSupport::getNaN();
	}
	else
	{
		return convertHelper(theString, fGotDecimalPoint);
	}
}



double
DoubleSupport::toDouble(const XalanDOMChar*		theString)
{
	if (theString == 0 ||
		*theString == 0)
	{
		return getNaN();
	}
	else
	{
		return doConvert(theString);
	}
}



bool
DoubleSupport::isValid(const XalanDOMString&	theString)
{
	return isValid(c_wstr(theString));
}



bool
DoubleSupport::isValid(const XalanDOMChar*		theString)
{
	return doValidate(theString);
}



double
DoubleSupport::round(double		theValue)
{
	if (isNaN(theValue))
	{
		return getNaN();
	}
	else if (isPositiveInfinity(theValue))
	{
		return getPositiveInfinity();
	}
	if (isNegativeInfinity(theValue))
	{
		return getNegativeInfinity();
	}
	else if (theValue == 0)
	{
		return 0.0;
	}
	else if (theValue > 0)
	{
		return long(theValue + 0.5);
	}
	else
	{
		// Negative numbers are a special case.  Any time we
		// have -0.5 as the fractional part, we have to
		// round up (toward 0), rather than down.
		double			intPart = 0;

#if defined(XALAN_STRICT_ANSI_HEADERS)
		const double	fracPart = std::modf(theValue, &intPart);
#else
		const double	fracPart = modf(theValue, &intPart);
#endif

		if (fracPart == -0.5)
		{
			// special case -- we have have to round toward 0...
			return long(theValue + 0.5);
		}
		else
		{
			return long(theValue - 0.5);
		}
	}
}



XALAN_CPP_NAMESPACE_END