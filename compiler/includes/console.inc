/* String functions
 *
 * (c) Copyright 1998-2011, ITB CompuPhase
 * This file is provided as is (no warranties).
 */
#if defined _console_included
  #endinput
#endif
#define _console_included
#pragma library Console

// Don't define "printf" twice.
#tryinclude <a_samp>
#if defined _samp_included
	#endinput
#endif

/// <summary>Prints a string to the server console (not in-game chat) and logs (server_log.txt).</summary>
/// <param name="string">The string to print</param>
/// <seealso name="printf"/>
native print(const string[]);

/// <summary>Outputs a formatted string on the console (the server window, not the in-game chat).</summary>
/// <param name="format">The format string</param>
/// <param name="">Indefinite number of arguments of any tag</param>
/// <seealso name="print"/>
/// <seealso name="format"/>
/// <remarks>The format string or its output should not exceed 1024 characters. Anything beyond that length can lead to a server to crash.</remarks>
/// <remarks>This function doesn't support <a href="#strpack">packed</a> strings.</remarks>
/// <remarks>
///   <b>Format Specifiers:</b><p/>
///   <ul>
///     <li><b><c>%i</c></b> - integer (whole number)</li>
///     <li><b><c>%d</c></b> - integer (whole number).</li>
///     <li><b><c>%s</c></b> - string</li>
///     <li><b><c>%f</c></b> - floating-point number (Float: tag)</li>
///     <li><b><c>%c</c></b> - ASCII character</li>
///     <li><b><c>%x</c></b> - hexadecimal number</li>
///     <li><b><c>%b</c></b> - binary number</li>
///     <li><b><c>%%</c></b> - literal <b><c>%</c></b></li>
///     <li><b><c>%q</c></b> - escape a text for SQLite. (Added in <b>0.3.7 R2</b>)</li>
///   </ul>
/// </remarks>
/// <remarks>The values for the placeholders follow in the exact same order as parameters in the call. For example, <b><c>"I am %i years old"</c></b> - the <b><c>%i</c></b> will be replaced with an Integer variable, which is the person's age.</remarks>
/// <remarks>You may optionally put a number between the <b><c>%</c></b> and the letter of the placeholder code. This number indicates the field width; if the size of the parameter to print at the position of the placeholder is smaller than the field width, the field is expanded with spaces. To cut the number of decimal places beeing shown of a float, you can add <b><c>.&lt;max number&gt;</c></b> between the <b><c>%</c></b> and the <b><c>f</c></b>. (example: <b><c>%.2f</c></b>)</remarks>
native printf(const format[], {Float,_}:...);
