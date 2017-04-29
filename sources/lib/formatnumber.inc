/*
	FormatNumber by Slice.
	-------------------------
	Usage:
		FormatNumber( number, decimals, thousand seperator, decimal point )

		number - This can be any variable type (float, integer, bool, etc.)
			If you want the value to be printed as HEX, tag the variable with hex. Example:
				new myvariable = 5312851;

				FormatNumber( hex:myvariable ); // gives 0x00511153

				Additionaly, you can print out the bits of either the first byte or the whole cell by using the following tags: bit:, bit_byte:

			If you get a type error, tag the variable with _. Example:
				new omgomg:specialVariable = omgomg:5;

				FormatNumber( _:specialVariable ); // gives 5

		decimals - The number of decimals to show
			new mynumber = 50;

			FormatNumber( mynumber, 2 ); // gives 50.00

			new Float:myfloat = 123.456;

			FormatNumber( myfloat, 0 ); // gives 123

		thousand seperator - char between thousands
			new mynumber = 5000000000;

			FormatNumber( myfloat ); // gives 5 000 000 000
			FormatNumber( myfloat, 0, ',' ); // gives 5,000,000,000
			FormatNumber( myfloat, 0, 0 ); // gives 5000000000

		decimal point - duh
			new myfloat = 50.0;

			FormatNumber( myfloat, 2, ' ', ',' ); // gives 50,00
			FormatNumber( myfloat, 2, ' ', '~' ); // gives 50~00

		You can set these constants __before__ including this file:

		HEX_PREFIX - default value "0x". Example:
			#define HEX_PREFIX "#"

			#include <formatnumber.inc>

			FormatNumber( hex:50000 ); // gives #0000C350

		BIT_PREFIX - default value: "0b"

		DEFAULT_DECIMAL_COUNT - default value: 8

		DEFAULT_THOUSAND_SEP - default value: ' '

		DEFAULT_DECIMAL_POINT - default value: '.'
*/

#if defined _formatnumber_included
	#endinput
#else
	#define _formatnumber_included
#endif

#if !defined HEX_PREFIX
	#define HEX_PREFIX  "0x"
#endif

#if !defined BIT_PREFIX
	#define BIT_PREFIX  "0b"
#endif

#if !defined DEFAULT_DECIMAL_COUNT
	#define DEFAULT_DECIMAL_COUNT  8
#endif

#if !defined DEFAULT_THOUSAND_SEP
	#define DEFAULT_THOUSAND_SEP  ' '
#endif

#if !defined DEFAULT_DECIMAL_POINT
	#define DEFAULT_DECIMAL_POINT  '.'
#endif

stock FormatNumber( { _, Float, Text3D, Menu, Text, DB, DBResult, bool, File, hex, Hex, bit, bit_byte, Bit }:xVariable, iDecimals = -1, iThousandSeparator = DEFAULT_THOUSAND_SEP, iDecimalPoint = DEFAULT_DECIMAL_POINT, iTag = tagof( xVariable ) )
{
	static
		s_szReturn[ 32 ],
		s_szThousandSeparator[ 2 ] = { ' ', EOS },
		s_iDecimalPos,
		s_iChar,
		s_iSepPos,
		bool:s_isNegative
	;

	if ( iTag == tagof( bool: ) )
	{
		if ( xVariable )
			memcpy( s_szReturn, "true", 0, 5 * ( cellbits / 8 ) );
		else
			memcpy( s_szReturn, "false", 0, 6 * ( cellbits / 8 ) );

		return s_szReturn;
	}
	else if ( iTag == tagof( Float: ) )
	{
		if ( iDecimals == -1 )
			iDecimals = DEFAULT_DECIMAL_COUNT;

		format( s_szReturn, sizeof( s_szReturn ), "%.*f", iDecimals, xVariable );

		s_iDecimalPos = strfind( s_szReturn, "." );
	}
	else if ( iTag == tagof( hex: ) || iTag == tagof( Hex: ) )
	{
		format( s_szReturn, sizeof( s_szReturn ), HEX_PREFIX "%04h%04h", ( xVariable & ( 0xFFFF0000 ) ) >>> 16, xVariable & ( 0x0000FFFF ) );

		return s_szReturn;
	}
	else if ( iTag == tagof( bit: ) || iTag == tagof( Bit: ) )
	{
		format( s_szReturn, sizeof( s_szReturn ), BIT_PREFIX "%08b%08b%08b%08b", ( xVariable & 0xFF000000 ) >>> 24, ( xVariable & 0x00FF0000 ) >>> 16, ( xVariable & 0x0000FF00 ) >>> 8, ( xVariable & 0x000000FF ) );

		return s_szReturn;
	}
	else if ( iTag == tagof( bit_byte: ) )
	{
		format( s_szReturn, sizeof( s_szReturn ), BIT_PREFIX "%08b", ( xVariable & 0x000000FF ) );

		return s_szReturn;
	}
	else
	{
		format( s_szReturn, sizeof( s_szReturn ), "%d", xVariable );
		if ( iDecimals > 0 )
		{
			strcat( s_szReturn, "." );

			while ( iDecimals-- )
				strcat( s_szReturn, "0" );
		}
		s_iDecimalPos = -1;
	}

	if ( s_iDecimalPos == -1 )
		s_iDecimalPos = strlen( s_szReturn );
	else
		s_szReturn[ s_iDecimalPos ] = iDecimalPoint;

	if ( s_iDecimalPos >= 4 && iThousandSeparator )
	{
		s_szThousandSeparator[ 0 ] = iThousandSeparator;

		s_iChar = s_iDecimalPos;
		s_iSepPos = 0;
		s_isNegative = s_szReturn[0] == '-';

		while ( --s_iChar > _:s_isNegative )
		{
			if ( ++s_iSepPos == 3 )
			{
				strins( s_szReturn, s_szThousandSeparator, s_iChar );

				s_iSepPos = 0;
			}
		}
	}
	return s_szReturn;
}
