package baci.gui.actionbuttons;

import baci.gui.*;
import baci.interpreter.*;
import java.awt.event.*;

/**
 * Action button for Cut operation of editor
 * @author: Moti Ben-Ari
 * @version December 2002
 * See copyright.txt
 */
public class CutButton extends BaciActionButton {

public CutButton() {
    super(Config.Cut, Config.CutMN, Config.CutAC);
}

public void actionPerformed(ActionEvent arg1) {
   getDebugger().getDebuggerFrame().getEditor().cut();
}
}
