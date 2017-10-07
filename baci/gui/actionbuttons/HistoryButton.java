package baci.gui.actionbuttons;

import baci.gui.*;
import baci.interpreter.*;
import java.awt.event.*;
/**
 * Action button to display history window
 * @author: David Strite
 * @author: Modified by Moti Ben-Ari
 * @version 20 December 2002
 * See copyright.txt.
 */
public class HistoryButton extends BaciActionButton {
/**
 * Constructs a new HistoryButton
 */
public HistoryButton() {
	super(Config.History, Config.HistoryMN);
}
/**
 * Open history window
 * @param arg1
 */
public void actionPerformed(ActionEvent arg1) {
	getDebugger().getDebuggerFrame().getWindowManager().showHistoryWindow(getDebugger(),getDebugger().getInterpreter());
}
}
