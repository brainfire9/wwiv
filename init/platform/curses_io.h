/**************************************************************************/
/*                                                                        */
/*                 WWIV Initialization Utility Version 5.0                */
/*                 Copyright (C)2014, WWIV Software Services              */
/*                                                                        */
/*    Licensed  under the  Apache License, Version  2.0 (the "License");  */
/*    you may not use this  file  except in compliance with the License.  */
/*    You may obtain a copy of the License at                             */
/*                                                                        */
/*                http://www.apache.org/licenses/LICENSE-2.0              */
/*                                                                        */
/*    Unless  required  by  applicable  law  or agreed to  in  writing,   */
/*    software  distributed  under  the  License  is  distributed on an   */
/*    "AS IS"  BASIS, WITHOUT  WARRANTIES  OR  CONDITIONS OF ANY  KIND,   */
/*    either  express  or implied.  See  the  License for  the specific   */
/*    language governing permissions and limitations under the License.   */
/*                                                                        */
/**************************************************************************/
#ifndef __INCLUDED_PLATFORM_CURSESIO_H__
#define __INCLUDED_PLATFORM_CURSESIO_H__

#include <curses.h>

// Curses implementation of the WLocalIO subset required by Init.
class CursesIO {

 public:
  // Constructor/Destructor
  CursesIO();
  CursesIO(const CursesIO& copy);
  virtual ~CursesIO();

  virtual void LocalGotoXY(int x, int y);
  virtual int  WhereX();
  virtual int  WhereY();
  virtual void LocalCls();
  virtual void LocalClrEol();
  virtual void LocalPutch(unsigned char ch);
  virtual void LocalPuts(const char *pszText);
  virtual void LocalXYPuts(int x, int y, const char *pszText);
  virtual int getchd();
  virtual void LocalScrollScreen(int nTop, int nBottom, int nDirection);

 private:
  void SetCursesAttribute();
  int max_x_;
  int max_y_;
  WINDOW *window_;
};

#endif // __INCLUDED_PLATFORM_CURSESIO_H__
