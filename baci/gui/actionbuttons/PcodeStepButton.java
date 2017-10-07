package baci.gui.actionbuttons;

import baci.gui.*;
import baci.interpreter.*;
import java.awt.event.*;
import javax.swing.*;
/**
 * Action button to step one pcode instruction
 * @author: David Strite
 * @author: Modified by Moti Ben-Ari
 * @version 20 December 2002
 * See copyright.txt.
 */
public class PcodeStepButton extends BaciActionButton {
/**
 * RunButton constructor comment.
 */
public PcodeStepButton() {
	super(Config.StepPcode, Config.StepPcodeMN, Config.StepPcodeAC);
}
/**
 * Step one pcode instruction
 * @param arg1
 */
public void actionPerformed(ActionEvent arg1) {
	SwingUtilities.invokeLater(getDebugger().getInterpreter());
}
}
