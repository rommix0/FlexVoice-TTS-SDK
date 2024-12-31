/*************************************************************************
*                                                                        *
*  (C) Copyright 2001 Mindmaker Ltd.                                     *
*  All rights reserved.                                                  *
*                                                                        *
*  Developed by Mindmaker Ltd.                                           *
*  Created: 2001.02.07.                                                  *
*                                                                        *
*************************************************************************/

#ifndef TOOLS_H
#define TOOLS_H

template <class T>
inline T * checkedptr(T * ptr)
{
  if (ptr == 0) throw std::bad_alloc();
  return ptr;
}

#endif //TOOLS_H
