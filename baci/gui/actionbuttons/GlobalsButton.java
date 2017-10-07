package baci.gui.actionbuttons;

import baci.gui.*;
import baci.interpreter.*;
import java.awt.event.*;
/**
 * Action button to display globals window
 * @author: David Strite
 * @author: Modified by Moti Ben-Ari
 * @version 20 December 2002
 * See copyright.txt.
 */
public class GlobalsButton extends BaciActionButton {
/**
 * Construct new GlobalsButton
 */
public GlobalsButton() {
	super(Config.Globals, Config.GlobalsMN);
}
/**
 * Opens globals window
 * @param arg1
 */
public void actionPerformed(ActionEvent arg1) {
	Interpreter interpreter = getDebugger().getInterpreter();
	getDebugger().getDebuggerFrame().getWindowManager().showGlobalsWindow(getDebugger(),interpreter);
}
}
