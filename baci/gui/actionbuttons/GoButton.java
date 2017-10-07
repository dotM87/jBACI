package baci.gui.actionbuttons;

import baci.gui.*;
import baci.interpreter.*;
import java.awt.event.*;
/**
 * Action button to run interpreter
 * @author: David Strite
 * @author: Modified by Moti Ben-Ari
 * @version 20 December 2002
 * See copyright.txt.
 */
public class GoButton extends BaciActionButton {
/**
 * Construct new GoButton
 */
public GoButton() {
	super(Config.Go, Config.GoMN, Config.GoAC);
}
/**
 * run interpreter
 * @param arg1
 */
public void actionPerformed(ActionEvent arg1) {
	Interpreter interpreter = getDebugger().getInterpreter();
	interpreter.getDebugger().getDebuggerFrame().setStopOnSwap(); // Reset to option
	interpreter.getDebugger().getDebuggerFrame().setShowActiveWindow(false);
	interpreter.setRunning(true);
}
}
