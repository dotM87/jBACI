package baci.gui.actionbuttons;

import baci.gui.*;
import baci.interpreter.*;
import java.awt.event.*;
import javax.swing.JFileChooser;

/**
 * Action button for Save operation of editor
 * @author: Moti Ben-Ari
 * @version December 2002
 * See copyright.txt
 */
public class SaveButton extends BaciActionButton {

public SaveButton() {
	super(Config.Save, Config.SaveMN, Config.SaveAC);
}

public void actionPerformed(ActionEvent arg1) {
   if (getDebugger().getDebuggerFrame().getEditor().file != null)
      getDebugger().getDebuggerFrame().getEditor().saveFile(null);
   else {
	JFileChooser fileChooser = new JFileChooser(Config.getStringProperty("SOURCE_DIRECTORY"));
	fileChooser.setFileFilter(new SourceFileFilter());
	int val = fileChooser.showSaveDialog(getDebugger().getDebuggerFrame());
	if(val==JFileChooser.APPROVE_OPTION) {
            getDebugger().getDebuggerFrame().getEditor().saveFile(fileChooser.getSelectedFile());
	}
   }
}
}
