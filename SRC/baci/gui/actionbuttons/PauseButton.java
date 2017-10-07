package baci.gui.actionbuttons;

import baci.gui.*;
import baci.interpreter.*;
import java.awt.event.*;
/**
 * Action button to pause interpreter
 * @author: David Strite
 * @author: Modified by Moti Ben-Ari
 * @version 20 December 2002
 * See copyright.txt.
 */
public class PauseButton extends BaciActionButton {
/**
 * Construct a new PauseButton
 */
public PauseButton() {
	super(Config.Pause, Config.PauseMN, Config.PauseAC);
}
/**
 * Pause interpreter
 * @param arg1
 */
public void actionPerformed(ActionEvent arg1) {
	Interpreter interpreter = getDebugger().getInterpreter();
	interpreter.getDebugger().getDebuggerFrame().resetShowActiveWindow();
	interpreter.setRunning(false);
}
}
