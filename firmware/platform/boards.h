/**
 *  boards.h : Board handling
 *  Copyright (C) 2019  Appiko
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef PLATFORM_BOARDS_H_
#define PLATFORM_BOARDS_H_

/**
 * @addtogroup group_platform
 * @{
 *
 * @brief A @ref boards.h file includes the relevant platform based on
 *  macro from the compile time defines
 */

#if defined(BOARD_PCA10040)
  #include "pca10040.h"
#else
#error "Board is not defined"
#endif

#endif /* PLATFORM_BOARDS_H_ */

/** @} */
