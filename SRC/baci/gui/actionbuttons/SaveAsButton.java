package baci.gui.actionbuttons;

import baci.gui.*;
import javax.swing.*;
import baci.interpreter.*;
import java.awt.event.*;

/**
 * Action button for Save as operation of editor
 * @author: Moti Ben-Ari
 * @version December 2002
 * See copyright.txt
 */
public class SaveAsButton extends BaciActionButton {

public SaveAsButton() {
	super(Config.SaveAs, Config.SaveAsMN, Config.SaveAsAC);
}

public void actionPerformed(ActionEvent arg1) {
	JFileChooser fileChooser = new JFileChooser(Config.getStringProperty("SOURCE_DIRECTORY"));
	fileChooser.setFileFilter(new SourceFileFilter());
	int val = fileChooser.showSaveDialog(getDebugger().getDebuggerFrame());
	if(val==JFileChooser.APPROVE_OPTION) {
            getDebugger().getDebuggerFrame().getEditor().saveFile(fileChooser.getSelectedFile());
	}
}
}
