package baci.gui.actionbuttons;

import baci.gui.*;
import baci.interpreter.*;
import java.awt.event.*;

/**
 * Action button for New operation of editor
 * @author: Moti Ben-Ari
 * @version December 2002
 * See copyright.txt
 */
public class NewButton extends BaciActionButton {

public NewButton() {
	super(Config.New, Config.NewMN, Config.NewAC);
}

public void actionPerformed(ActionEvent arg1) {
	getDebugger().getDebuggerFrame().getEditor().newFile();
}
}
