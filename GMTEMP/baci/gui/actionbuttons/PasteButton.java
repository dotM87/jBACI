package baci.gui.actionbuttons;

import baci.gui.*;
import baci.interpreter.*;
import java.awt.event.*;

/**
 * Action button for Paste operation of editor
 * @author: Moti Ben-Ari
 * @version December 2002
 * See copyright.txt
 */
public class PasteButton extends BaciActionButton {

public PasteButton() {
	super(Config.Paste, Config.PasteMN, Config.PasteAC);
}

public void actionPerformed(ActionEvent arg1) {
   getDebugger().getDebuggerFrame().getEditor().paste();
}
}
