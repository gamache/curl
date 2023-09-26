/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://curl.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 * SPDX-License-Identifier: curl
 *
 ***************************************************************************/
#include "curlcheck.h"

#include "tool_operhlp.h"

static CURLcode unit_setup(void)
{
  return CURLE_OK;
}

static void unit_stop(void)
{
}

/* utf_8_to_iso_8859_1 unit tests */

UNITTEST_START

  char *utf;
  char *iso;
  char *output;
  CURLcode result;

  /* 7-bit ascii */
  utf = (char *)"Hi mom!";
  iso = (char *)"Hi mom!";
  result = utf_8_to_iso_8859_1(utf, 0, &output);
  fail_unless(result == CURLE_OK, "bad result code");
  fail_unless(0 == strcmp(iso, output), "no match");

  /* codepoints U+0080 to U+00FF are present in ISO-8859-1 */
  utf = (char *)"\xC2\xA1Hola Jos\xC3\xA9!";
  iso = (char *)"\xA1Hola Jos\xE9!";
  result = utf_8_to_iso_8859_1(utf, 0, &output);
  fail_unless(result == CURLE_OK, "bad result code");
  fail_unless(0 == strcmp(iso, output), "no match");

  /* two-byte codepoints above U+00FF are ignored */
  utf = (char *)"LATIN CAPITAL LETTER A WITH MACRON \xC4\x80";
  iso = (char *)"LATIN CAPITAL LETTER A WITH MACRON ";
  result = utf_8_to_iso_8859_1(utf, 0, &output);
  fail_unless(result == CURLE_OK, "bad result code");
  fail_unless(0 == strcmp(iso, output), "no match");

  /* three-byte codepoints are ignored */
  utf = (char *)"Korean giyeok (g/k): \xE1\x84\x80";
  iso = (char *)"Korean giyeok (g/k): ";
  result = utf_8_to_iso_8859_1(utf, 0, &output);
  fail_unless(result == CURLE_OK, "bad result code");
  fail_unless(0 == strcmp(iso, output), "no match");

  /* four-byte codepoints are ignored */
  utf = (char *)"Double rainbow! \xF0\x9F\x8C\x88\xF0\x9F\x8C\x88";
  iso = (char *)"Double rainbow! ";
  result = utf_8_to_iso_8859_1(utf, 0, &output);
  fail_unless(result == CURLE_OK, "bad result code");
  fail_unless(0 == strcmp(iso, output), "no match");

  /* everything at once */
  utf = (char *)"\xC3\xA9\xE1\x84\x80\xF0\x9F\x8C\x88\xC4\x80 whoa";
  iso = (char *)"\xE9 whoa";
  result = utf_8_to_iso_8859_1(utf, 0, &output);
  fail_unless(result == CURLE_OK, "bad result code");
  fail_unless(0 == strcmp(iso, output), "no match");

  /* invalid UTF-8, missing continuation byte(s) */
  utf = (char *)"Eat this! \xF0";
  result = utf_8_to_iso_8859_1(utf, 0, &output);
  fail_unless(result == CURLE_BAD_CONTENT_ENCODING, "bad result code");

  /* invalid UTF-8, surprise continuation byte */
  utf = (char *)"Eat this \x80!";
  result = utf_8_to_iso_8859_1(utf, 0, &output);
  fail_unless(result == CURLE_BAD_CONTENT_ENCODING, "bad result code");

  /* invalid UTF-8 in the middle of a codepoint */
  utf = (char *)"Bad rainbow \xF0\x9F\x20\x88";
  result = utf_8_to_iso_8859_1(utf, 0, &output);
  fail_unless(result == CURLE_BAD_CONTENT_ENCODING, "bad result code");

UNITTEST_STOP
