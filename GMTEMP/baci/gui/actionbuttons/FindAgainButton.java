package baci.gui.actionbuttons;

import baci.gui.*;
import baci.interpreter.*;
import java.awt.event.*;

/**
 * Action button for Find Again operation of editor
 * @author: Moti Ben-Ari
 * @version December 2002
 * See copyright.txt
 */
public class FindAgainButton extends BaciActionButton {

public FindAgainButton() {
	super(Config.FindAgain, Config.FindAgainMN, Config.FindAgainAC);
}

public void actionPerformed(ActionEvent arg1) {
   getDebugger().getDebuggerFrame().getEditor().findAgain();
}
}
