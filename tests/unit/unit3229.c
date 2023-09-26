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

/* iso_8859_1_to_utf_8 unit tests */

UNITTEST_START

  char *iso;
  char *utf;
  char *output;
  CURLcode result;

  /* 7-bit ASCII */
  iso = (char *)"Hi mom!";
  utf = (char *)"Hi mom!";
  result = iso_8859_1_to_utf_8(iso, 0, &output);
  fail_unless(result == CURLE_OK, "bad result code");
  fail_unless(0 == strcmp(utf, output), "no match");

  /* 0x80 to 0xFF */
  iso = (char *)"\xA1Hola Jos\xE9!";
  utf = (char *)"\xC2\xA1Hola Jos\xC3\xA9!";
  result = iso_8859_1_to_utf_8(iso, 0, &output);
  fail_unless(result == CURLE_OK, "bad result code");
  fail_unless(0 == strcmp(utf, output), "no match");

  /* bouncing the UTF-8 output to ISO-8859-1 and back changes nothing */
  result = utf_8_to_iso_8859_1(output, 0, &iso);
  fail_unless(result == CURLE_OK, "bad result code");
  result = iso_8859_1_to_utf_8(iso, 0, &output);
  fail_unless(result == CURLE_OK, "bad result code");
  fail_unless(0 == strcmp(utf, output), "no match");

UNITTEST_STOP
