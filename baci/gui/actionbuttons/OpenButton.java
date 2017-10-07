package baci.gui.actionbuttons;

import baci.gui.*;
import baci.interpreter.*;
import java.awt.event.*;
import javax.swing.*;

/**
 * Action button for Open operation of editor
 * @author: David Strite
 * @author: Modified by Moti Ben-Ari
 * @version December 2002
 * See copyright.txt
 */
public class OpenButton extends BaciActionButton {
/**
 * Constructs a new OpenButton
 */
public OpenButton() {
    super(Config.Open, Config.OpenMN, Config.OpenAC);
}
/**
 * Open a source file
 */
public void actionPerformed(ActionEvent arg1) {
    JFileChooser fileChooser = new JFileChooser(Config.getStringProperty("SOURCE_DIRECTORY"));
    fileChooser.setFileFilter(new SourceFileFilter());
    int val = fileChooser.showOpenDialog(getDebugger().getDebuggerFrame());
    if(val==JFileChooser.APPROVE_OPTION) {
        getDebugger().getDebuggerFrame().getEditor().openFile(fileChooser.getSelectedFile());
        getDebugger().getDebuggerFrame().switchToEditor();
    }
}
}
