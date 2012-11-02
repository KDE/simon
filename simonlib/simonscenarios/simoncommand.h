/*
 *  Copyright (C) 2010 Peter Grasch <peter.grasch@bedahr.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2,
 *  or (at your option) any later version, as published by the Free
 *  Software Foundation
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details
 *
 *  You should have received a copy of the GNU General Public
 *  License along with this program; if not, write to the
 *  Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef SIMON_SIMONCOMMAND_H_15D866BEA2544D34BF320D2D7882E7AF
#define SIMON_SIMONCOMMAND_H_15D866BEA2544D34BF320D2D7882E7AF

/** \file simoncommand.h
 * \brief The file containing the SimonCommand::State enumeration
 */

namespace SimonCommand
{

  /**
   * \enum State
   * \brief This enum describes the current state of the command manager
   */
  enum State
  {
    /** The standard state. This is the initial state of a new CommandManager */
    DefaultState=0,
    /** The greedy state. This state is automatically switched to if startGreedy() is called and the manager was in the DefaultState  */
    GreedyState=0x1000
  };

}
#endif

