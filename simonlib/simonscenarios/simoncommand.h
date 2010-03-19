#ifndef SIMON_SIMONCOMMAND_H_15D866BEA2544D34BF320D2D7882E7AF
#define SIMON_SIMONCOMMAND_H_15D866BEA2544D34BF320D2D7882E7AF

/** \file simoncommand.h
 * \brief The file containing the SimonCommand::State enumeration
 */

namespace SimonCommand {

/** 
 * \enum State
 * \brief This enum describes the current state of the command manager
 */
enum State {
	/** The standard state. This is the initial state of a new CommandManager */
	DefaultState=0,
	/** The greedy state. This state is automatically switched to if startGreedy() is called and the manager was in the DefaultState  */
	GreedyState=0x1000
};

}
#endif
