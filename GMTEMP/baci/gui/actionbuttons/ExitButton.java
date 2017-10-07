package baci.gui.actionbuttons;

import baci.gui.*;
import baci.interpreter.*;
import java.awt.event.*;
/**
 * Action button to exit debugger
 * @author: David Strite
 * @author: Modified by Moti Ben-Ari
 * @version 20 December 2002
 * See copyright.txt.
 */
public class ExitButton extends BaciActionButton {
/**
 * Constructs a new ExitButton
 */
public ExitButton() {
    super(Config.Exit, Config.ExitMN, Config.ExitAC);
    setEnabled(true);
}
/**
 * Exit debugger
 * @param arg1
 */
public void actionPerformed(ActionEvent arg1) {
    getDebugger().getInterpreter().openHistoryFile(null);
    getDebugger().stop();
}
}
