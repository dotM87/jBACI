package baci.gui.actionbuttons;

import baci.gui.*;
import baci.interpreter.*;
import java.awt.event.*;
import java.io.*;
import javax.swing.*;

/**
 * Action button to restart interpreter
 * @author: David Strite
 * @author: Modified by Moti Ben-Ari
 * @version 20 December 2002
 * See copyright.txt.
 */
public class RunButton extends BaciActionButton {
/**
 * Construct new RestartButton
 */
public RunButton() {
    super(Config.Run, Config.RunMN, Config.RunAC);
}
/**
 * Restart interpreter
 * @param arg1
 */
public void actionPerformed(ActionEvent arg1) {
    Editor e = getDebugger().getDebuggerFrame().getEditor();
    String fileName = e.fileName;
    if (fileName == null) return;    
    String name = fileName.substring(0, fileName.indexOf('.'));
    File codeFile = new File(e.root + File.separator + name + ".pco");
    if (codeFile.exists()) {
	    getDebugger().startPcodeFile(codeFile);
	    getDebugger().getInterpreter().openHistoryFile(e.root + File.separator + name + ".his");
	    getDebugger().getDebuggerFrame().newInitialize(true);
    }
    else
         JOptionPane.showMessageDialog(getDebugger().getDebuggerFrame(), 
 		(Object) (fileName + " has not been compiled"), "Run error", JOptionPane.ERROR_MESSAGE);
}

}
