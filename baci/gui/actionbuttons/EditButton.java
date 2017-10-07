package baci.gui.actionbuttons;

import baci.gui.*;
import baci.interpreter.*;
import java.awt.event.*;

/**
 * Action button to invoke editor
 * @author: Moti Ben-Ari
 * @version December 2002
 * See copyright.txt
 */
public class EditButton extends BaciActionButton {

public EditButton() {
    super(Config.Edit, Config.EditMN, Config.EditAC);
}

public void actionPerformed(ActionEvent arg1) {
    getDebugger().getDebuggerFrame().switchToEditor();
    getDebugger().getInterpreter().openHistoryFile(null);
}
}
