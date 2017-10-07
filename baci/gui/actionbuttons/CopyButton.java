package baci.gui.actionbuttons;

import baci.gui.*;
import baci.interpreter.*;
import java.awt.event.*;

/**
 * Action button for Copy operation of editor
 * @author: Moti Ben-Ari
 * @version December 2002
 * See copyright.txt
 */
public class CopyButton extends BaciActionButton {

public CopyButton() {
	super(Config.Copy, Config.CopyMN, Config.CopyAC);
}

public void actionPerformed(ActionEvent arg1) {
   getDebugger().getDebuggerFrame().getEditor().copy();
}
}
