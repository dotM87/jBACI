package baci.gui.actionbuttons;

import baci.gui.*;
import baci.interpreter.*;
import java.awt.event.*;

/**
 * Action button for Find operation of editor
 * @author: Moti Ben-Ari
 * @version December 2002
 * See copyright.txt
 */
public class FindButton extends BaciActionButton {

public FindButton() {
	super(Config.Find, Config.FindMN, Config.FindAC);
}

public void actionPerformed(ActionEvent arg1) {
   getDebugger().getDebuggerFrame().getEditor().find();
}
}
