package baci.gui.actionbuttons;

import baci.gui.*;
import baci.interpreter.*;
import java.awt.event.*;
/**
 * Action button to display Linda window
 * @author: Modified by Moti Ben-Ari
 * See copyright.txt
 */
public class LindaButton extends BaciActionButton {
/**
 * Constructs new LindaButton
 */
public LindaButton() {
	super(Config.LindaWindow, Config.LindaMN);
}
/**
 * Open Linda window
 * @param arg1
 */
public void actionPerformed(ActionEvent arg1) {
	Interpreter interpreter = getDebugger().getInterpreter();
	getDebugger().getDebuggerFrame().getWindowManager().showLindaWindow(getDebugger(),interpreter.getLinda());
}
}
