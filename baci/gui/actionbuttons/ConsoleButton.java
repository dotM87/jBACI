package baci.gui.actionbuttons;

import baci.gui.*;
import baci.interpreter.*;
import java.awt.event.*;
/**
 * Action button to display console window
 * @author: David Strite
 * @author: Modified by Moti Ben-Ari
 * @version December 2002
 * See copyright.txt
 */
public class ConsoleButton extends BaciActionButton {
/**
 * Constructs new ConsoleButton
 */
public ConsoleButton() {
	super(Config.Console, Config.ConsoleMN);
}
/**
 * Open console window
 * @param arg1
 */
public void actionPerformed(ActionEvent arg1) {
	Interpreter interpreter = getDebugger().getInterpreter();
	getDebugger().getDebuggerFrame().getWindowManager().showConsoleWindow(getDebugger(),interpreter.getConsole());
}
}
